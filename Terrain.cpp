#include "Fear.h"
#include "Console.h"
#include "OpenGL.h"
#include "Hash.h"
#include "Texture.h"
#include "List.h"
#include "Patch.h"
#include "Replacer.h"
#include "Callback.h"
#include <stdio.h>

#define OPENGL_FLUSH_TEXTURE_CACHE_VFT 0x0072A738

namespace Replacer {
	extern bool prefShowMatchedTextures;
}

namespace Terrain {
	#define GridBlock_Material_Plain  ( 0 )
	#define GridBlock_Material_Rotate ( 1 )
	#define GridBlock_Material_FlipX  ( 2 )
	#define GridBlock_Material_FlipY  ( 4 )

	#define GridSquare_Pinned         ( 4 )

	struct HiddenTexture {
		TextureWithMips *replacement, *originalUpsampled;
		u32 stamp;
		Fear::GFXBitmap *parent;
	};

	BuiltInVariable("pref::memstarTerrain", bool, prefMemstarTerrain, true)

	RGBA canvas_1x1    [   1 *   1 ];
	RGBA canvas_2x2    [   2 *   2 ];
	RGBA canvas_4x4    [   4 *   4 ];
	RGBA canvas_8x8    [   8 *   8 ];
	RGBA canvas_16x16  [  16 *  16 ];
	RGBA canvas_32x32  [  32 *  32 ];
	RGBA canvas_64x64  [  64 *  64 ];
	RGBA canvas_128x128[ 128 * 128 ];
	RGBA canvas_256x256[ 256 * 256 ];

	RGBA canvasDefault[ 128 * 128 ];
	RGBA *canvasMipList[] = {
		canvas_256x256,
		canvas_128x128,
		canvas_64x64,
		canvas_32x32,
		canvas_16x16,
		canvas_8x8,
		canvas_4x4,
		canvas_2x2,
		canvas_1x1,
		NULL,
	};

	// gathered data
	u32 Xd, Yd, RotateFlag, TileWidth, MipWidth;
	void *FixedUpMatMap, *PrimaryMatMap, *SecondaryMatMap;
	void *BaseAddr = NULL;
	Fear::GFXBitmap *SrcBM;
	RGBA **targetCanvas;

	/*
		List will not realloc down unless we call Delete, Release, or Pop.
		Clear()'ing the List will retain the allocated memory, so we can
		assume the List memory still stay constant. We should never need
		more than ~200 tiles.
	*/
	typedef List< HiddenTexture >::Iterator TileIterator;
	List< HiddenTexture > tiles( 512 );
	u32 uniqueStamp = 0;
	bool texImageMatch = false, texSubImageMatch = false;

	// terrain fixes
	u32 fnSetBool = 0x005E6B80;
	u32 fnMipBlt = 0x005FBED8;

	// redirect for MipBlt
	CodePatch patchMipBlt = {
		0x005F692D, 
		"\xE8\xA6\x55\x00\x00", 
		"\xe9mipb", 
		5, 
		false
	};

	// check to bump block offset up to the next 256x256 grid
	CodePatch patchCreateFileFromGridFile = {
		0x00605F26,
		"\x8B\xC8\x8B\x44\x24",
		"\xe9GFFC", 
		5, 
		false
	};

	// fix subdivide test
	CodePatch patchSubdivideTest = {
		0x00601A9B,
		"\x66\x8B\x0B\x8B\x15",
		"\xe9subd",
		5,
		false
	};

	CodePatch patchForceTerrainRecache = {
		0x006037D3,
		"\x0f\x84\x89\x01\x00\x00", 
		"\x90\x90\x90\x90\x90\x90", 
		6, 
		false
	};

	CodePatch patchLeaveTerrainRenderLevelNonZero = {
		0x006039BF,
		"\x83\xC4\x34\x5F\x5E",
		"\xE9LTRN",
		5,
		false
	};

	CodePatch patchLeaveTerrainRenderLevelNonZeroLoop = {
		0x00603829,
		"\x81\xE2\xFF\x00\x00",
		"\xE9LTR2",
		5,
		false
	};

	CodePatch patchTerrainRenderLevelZeroLoop = {
		0x006032E2,
		"\x81\xE2\xFF\x00\x00",
		"\xE9RLZL",
		5,
		false
	};

	struct TerrainBlock {
	};

	struct TerrainFile {
		u32 squareSize;
		f32 visibleDistance, hazeDistance, screenSize;
		TerrainBlock *blockMap[3][3];
	};

	struct TerrainRenderState {
		PAD(0x4064 - (0x0000 + 0x000)); u32 squareSize;
		PAD(0x4078 - (0x4064 + 0x004)); f32 growFactor;
		PAD(0x4108 - (0x4078 + 0x004)); Fear::OpenGLSurface *gfxSurface;
	};

	#define PTR_TERRAIN_RENDER_STATE 0x00725B2C

	TerrainRenderState *getTerrainRenderState() {
		__asm {
			mov eax, ds:[PTR_TERRAIN_RENDER_STATE]
		}
	}

	void forceTerrainRefresh() {
		patchForceTerrainRecache.Apply(true);
	}

	void revertTerrainRefresh() {
		patchForceTerrainRecache.Apply(false);
	}

	NAKED void OnLeaveTerrainRenderLevelNonZero() {
		__asm {
			call revertTerrainRefresh
			add esp, 0x34
			pop edi
			pop esi
			pop ebx
			retn
		}
	}

	u32 fnFlushTextureCache;

	NAKED void OnFlushTextureCache() {
		__asm {
			pushad
			call forceTerrainRefresh
			popad
			jmp [fnFlushTextureCache]
		}
	}

	void MipBlt(u32 rotate_flag, RGBA *src_start, s32 src_inc, RGBA *dst_start, s32 tile_width, s32 src_adjust, s32 dst_adjust) {
		switch ( rotate_flag ) {
			case GridBlock_Material_Plain:
				break;

			case GridBlock_Material_Rotate:
				src_start += tile_width*tile_width-tile_width;
				src_inc = -tile_width;
				src_adjust = tile_width*tile_width+1;
				break;

			case GridBlock_Material_FlipX:
				src_start += tile_width-1;
				src_inc = -1;
				src_adjust = 2*tile_width;
				break;

			case GridBlock_Material_FlipY:
				src_start += tile_width*tile_width-tile_width;
				src_adjust = -2*tile_width;
				break;

			case GridBlock_Material_FlipX|GridBlock_Material_FlipY:
				src_start += tile_width*tile_width-1;
				src_inc = -1;
				break;

			case GridBlock_Material_FlipX|GridBlock_Material_Rotate:
				src_start += tile_width*tile_width-1;
				src_inc = -tile_width;
				src_adjust = tile_width*tile_width-1;
				break;

			case GridBlock_Material_FlipY|GridBlock_Material_Rotate:
				src_inc = tile_width;
				src_adjust = -(tile_width*tile_width-1);
				break;

			case GridBlock_Material_FlipX|GridBlock_Material_FlipY|GridBlock_Material_Rotate:
				src_start += tile_width-1;
				src_inc = tile_width;
				src_adjust = -((tile_width*tile_width)+1);
				break;
		}


		// use the simple version for tiles 2x2 or smaller
		if ( tile_width <= 2 ) {
			RGBA *src = ( src_start ), *dst = ( dst_start );

			for (s32 y = 0; y < tile_width; y++, src += src_adjust, dst += dst_adjust)  {
				for (s32 x = 0; x < tile_width; x++, src += src_inc, dst++)
					*dst = *src;
			}

			return;
		}

		// RGBA is 4 bytes wide
		src_inc *= 4;
		src_adjust *= 4;
		dst_adjust *= 4;

		// do 4 texels at once
		__asm {
			mov esi, [ src_start ]
			mov edi, [ dst_start ]
			mov ebx, [ src_inc ]
			lea eax, [ ebx * 2 ]
			add eax, ebx // src_inc * 3

			mov ecx, [ tile_width ]
	__height_loop:
			mov edx, [ tile_width ]
			shr edx, 2
			
			align 16
	__width_loop:		
			movd mm0, [ esi + ebx ]		// src + ( src_inc * 1 )
			movd mm2, [ esi + eax ]		// src + ( src_inc * 3 )
			movd mm3, [ esi + ebx * 2 ]	// src + ( src_inc * 2 )
			movd mm1, [ esi ]			// src		
			psllq mm0, 32
			psllq mm2, 32
			por mm0, mm1
			por mm2, mm3

			movq [ edi ], mm0
			movq [ edi + 8 ], mm2
			
			lea esi, [ esi + ebx * 4 ]
			add edi, 16
			dec edx
			jnz __width_loop

			add esi, [ src_adjust ]
			add edi, [ dst_adjust ]
			dec ecx
			jnz __height_loop

			emms
		}
	}

	bool MipBlt_Examine() {
		if (!OpenGL::IsActive() || !prefMemstarTerrain)
			return false;

		// sanity check the replacement anchor
		HiddenTexture *hidden = (HiddenTexture *)SrcBM->hidden;
		TextureWithMips *terrain = NULL;
		if (hidden) {
			if (hidden < &tiles[0] || hidden > &tiles[511]) {
				hidden = NULL;
			} else if (hidden >= &tiles[tiles.Count()]) {
				hidden = NULL;
			} else if (hidden->stamp != uniqueStamp) {
				hidden = NULL;
			} else if (hidden->parent != SrcBM) {
				hidden = NULL;
			}
		}

		if (hidden) {
			terrain = (hidden->replacement) ? 
				hidden->replacement : hidden->originalUpsampled;
		} else {	
			hidden = tiles.New();
			hidden->stamp = uniqueStamp;
			hidden->parent = SrcBM;
			SrcBM->hidden = hidden;

			// hash the 16x16 mip
			const String *name = Replacer::FindOriginalName(HashBytes(SrcBM->bitmapMips[3], 16 * 16));
			TextureWithMips *replacement = Replacer::FindReplacement(name);

			if (replacement) {
				if ((replacement->mWidth != replacement->mHeight) || (replacement->mWidth < 128) || !ISPOWOF2(replacement->mWidth)) {
					Console::echo("Terrain: %s has bad dimensions! Req: Width = Height, Width >= 128, Width is a power of 2", name->c_str());
					replacement = NULL;
				}
			}

			if (replacement) {
				terrain = replacement;
				
				hidden->replacement = replacement;
				hidden->originalUpsampled = NULL;			
			} else {
				// generate a texture from the default terrain tile
				terrain = new TextureWithMips();
				terrain->New(SrcBM->width, SrcBM->height);
				getTerrainRenderState()->gfxSurface->textureCache->pbmpToRGBA(SrcBM->bitmapData, (RGBA *)terrain->mData, SrcBM->width * SrcBM->height, SrcBM->paletteIdx);

				hidden->replacement = NULL;
				hidden->originalUpsampled = terrain;
			}

			// will only do this once at most for any texture
			terrain->GenerateMipMaps(); 
		}
		
		switch ( TileWidth ) {
			case 256: targetCanvas = &canvasMipList[0]; break;
			case 128: targetCanvas = &canvasMipList[1]; break;
			case  64: targetCanvas = &canvasMipList[2]; break;
			case  32: targetCanvas = &canvasMipList[3]; break;
			case  16: targetCanvas = &canvasMipList[4]; break;
			case   8: targetCanvas = &canvasMipList[5]; break;
			case   4: targetCanvas = &canvasMipList[6]; break;
			case   2: targetCanvas = &canvasMipList[7]; break;
			case   1: targetCanvas = &canvasMipList[8]; break;
		}

		RGBA *sourceCanvas;	

		if (Replacer::prefShowMatchedTextures) {
			int color;

			switch ( MipWidth ) {
				case 64: color = 0x20; break;
				case 32: color = 0x40; break;
				case 16: color = 0x60; break;
				case  8: color = 0x80; break;
				case  4: color = 0xa0; break;
				case  2: color = 0xc0; break;
				case  1: color = 0xff; break;
				default: color = 0x80; break;
			}

			memset( canvasDefault, color, MipWidth * MipWidth * 4 );		
			sourceCanvas = canvasDefault;
		} else {
			u32 mipLevel = 0;
			u32 w = (terrain->mWidth >> 1);
			
			while (w > MipWidth) {
				w >>= 1;
				mipLevel++;
			}

			sourceCanvas = (RGBA *)terrain->mMipMaps[mipLevel]->mData;
		}

		s32 src_inc = 1;
		s32 src_adj = 0;
		RGBA *src_start = sourceCanvas;
		RGBA *dst_start = (*targetCanvas) + (Yd * TileWidth + Xd);
		s32 dst_adj = (TileWidth - MipWidth);
		s32 dst_width = MipWidth;

		MipBlt(RotateFlag, src_start, src_inc, dst_start, MipWidth, src_adj, dst_adj);
		return true;
	}

	u32 fnOnMipBltResume = 0x005F6935;

	NAKED void OnMipBlt( ) {
		__asm {
			call OpenGL::IsActive
			and al, al
			jz gcMM_software

			// _baseAddr
			mov ecx, [ ebp + 0xc ]
			mov [ BaseAddr ], ecx

			// _stride
			mov ecx, [ ebp + 0x10 ]
			mov [ TileWidth ], ecx

			// _matMap[mapI].flags & rotateMask
			mov edx, [ ebp - 0x20 ] // yo + xo
			add edx, [ ebp - 0x2c ] // mapI
			// mov ebx, [ ebp + 0x18 ] // _matMap
			mov ebx, [ebp + 0x18]
			sub ebx, [PrimaryMatMap]
			cmp ebx, 256 * 256 * 2
			jb __found_offset
			mov ebx, [ebp + 0x18]
			sub ebx, [SecondaryMatMap]
		__found_offset:
			add ebx, [FixedUpMatMap]
			mov ax, word ptr [ ebx + edx * 2 ]
			and eax, 7
			mov [ RotateFlag ], eax

			// pSrcBM = (*_matList)[ _matMap[mapI].index ].getTextureMap()
			movzx ecx, byte ptr [ ebx + edx * 2 + 1 ]
			mov eax, [ ebp + 0x14 ]
			mov ebx, [ ebp + 0x14 ]
			mov eax, [ eax + 0x1c ]
			mov ebx, [ ebx + 0x8 ]
			add ecx, ebx
			mov ebx, ecx
			lea ecx, [ ebx + ecx * 8 ]
			lea ecx, [ ebx + ecx * 2 ]
			shl ecx, 2
			add eax, ecx
			mov eax, [ eax ]
			mov eax, [ eax + 0x14 ]
			mov [ SrcBM ], eax
			mov ebx, eax

			// mipWidth
			mov eax, [ ebx + 0x10 ]
			mov ecx, [ ebp - 0x04 ] // mipSize
			sar eax, cl
			mov [ MipWidth ], eax

			// xd + yd
			mov eax, [ ebp - 0x1c ]
			mov [ Yd ], eax
			mov eax, [ ebp - 0x28 ]
			mov [ Xd ], eax

			call MipBlt_Examine
			and al, al
			jnz gcMM_end

			// fall through to software 
	gcMM_software:
			call [ fnMipBlt ]

	gcMM_end:
			add esp, 0x1c // adjust for the call to mipBlt
			jmp [fnOnMipBltResume]
		}
	}

	u32 fnOnCreateFileFromGridFileResume = 0x00605F2C;
	NAKED void OnCreateFileFromGridFile() {
		__asm {
			push eax
			mov eax, [eax]
			mov [SecondaryMatMap], eax
			mov eax, [esp + 0xc]
			mov eax, [eax]
			mov [PrimaryMatMap], eax
			mov eax, [esp + 0x8]
			pop ecx
			jmp [fnOnCreateFileFromGridFileResume]
		}
	}

	void SubdivideTest_Examine(u8 *subdivideFlag, u8 *material, f32 distanceScale, f32 squareDistance, u32 nshift) {
		*subdivideFlag = 0;

		TerrainRenderState *trs = getTerrainRenderState();
		trs->growFactor = 0;
		if ((squareDistance < 1) || (*material & GridSquare_Pinned)) {
			*subdivideFlag = 1;
		} else {
			f32 squareSize = (f32)(trs->squareSize << nshift);
			f32 detailDistance = (distanceScale * squareSize) - squareSize;
			if (squareDistance < detailDistance) {
				*subdivideFlag = 1;
				f32 clampDistance = 0.75f * detailDistance;
				if (squareDistance > clampDistance)
					trs->growFactor = (squareDistance - clampDistance) / (0.25f * detailDistance);
			}
		}
	}

	u32 fnResumeProcessCurrentBlock = 0x00601B8E;
	NAKED void OnSubdivideTest() {
		__asm {
			pushad
			xor ecx, ecx
			mov cx, word ptr [ebx]
			push ecx // nshift
			push dword ptr [ebp - 0x14] // f32:squareDistance
			push dword ptr [ebp - 0x1c] // f32:distanceScale
			lea eax, [esi + 0x6]
			push eax // material
			lea eax, [ebp - 0x59] // bool:subdivideSquare
			push eax // subdivide flag
			call SubdivideTest_Examine
			add esp, 4*5
			popad
			jmp [fnResumeProcessCurrentBlock]
		}
	}

	u32 fnResumeRenderLevelZeroLoop = 0x006032FC;
	NAKED void OnRenderLevelZeroLoop() {
		__asm {
			cmp eax, 256
			jb __use_secondary
			cmp eax, 256+256
			jae __use_secondary
			cmp edx, 256
			jb __use_secondary
			cmp edx, 256+256
			jae __use_secondary

			mov ebx, [PrimaryMatMap]
			jmp __continue
		__use_secondary:
			mov ebx, [SecondaryMatMap]
		__continue:

			and edx, 0xff
			and eax, 0xff
			add eax, eax
			mov esi, [ecx + 0x4060]
			shl edx, 8
			xor ecx, ecx
			jmp [fnResumeRenderLevelZeroLoop]
		}
	}

	u32 fnResumeOnTerrainRenderLevelNonZeroLoop = 0x0060382F;
	NAKED void OnTerrainRenderLevelNonZeroLoop() {
		__asm {
			movzx ebx, word ptr [ecx + 0x8]
			cmp ebx, 256
			jl __use_secondary
			cmp ebx, 256+256
			jae __use_secondary
			movzx ebx, word ptr [ecx + 0xa]
			cmp ebx, 256
			jl __use_secondary
			cmp ebx, 256+256
			jae __use_secondary
			mov ebx, [PrimaryMatMap]
			jmp __continue
		__use_secondary:
			mov ebx, [SecondaryMatMap]
		__continue:
			mov [FixedUpMatMap], ebx

			and edx, 0xff
			jmp [fnResumeOnTerrainRenderLevelNonZeroLoop]
		}
	}

	RGBA **CheckForTerrainTile() {
		if (!prefMemstarTerrain || (!texImageMatch && !texSubImageMatch))
			return NULL;

		if (tiles.Count() <= 1)
			forceTerrainRefresh();

		// generate mips from the generated tile
		u32 width = TileWidth;
		RGBA **src = targetCanvas, **dst = src + 1;
		while (width > 1) {
			u32 halfWidth = (width >> 1);
			Texture::HalveTexture(*src++, width, width, *dst++, halfWidth, halfWidth);	
			width = halfWidth;
		}

		// reset the match variables
		BaseAddr = NULL;
		texImageMatch = false;
		texSubImageMatch = false;

		return targetCanvas;
	}

	// check for the original creation of a grid tile
	bool TexImageCheck(void *bmp) {
		texImageMatch = (BaseAddr == bmp);
		return texImageMatch;
	}

	bool TexSubImageCheck(void *bmp) {
		texSubImageMatch = (BaseAddr == bmp);
		return texSubImageMatch;
	}

	void OnOpenGL(bool isActive) {
		if (isActive)
			forceTerrainRefresh();
	}

	void Reset() {
		// replacement textures will be deleted by the replacing engine
		TileIterator iter = tiles.Begin(), end = tiles.End();
		while (iter != end) {
			delete iter.value().originalUpsampled;
			++iter;
		}

		tiles.Clear();
		uniqueStamp = GetTickCount();
		BaseAddr = NULL;
		texImageMatch = false;
		texSubImageMatch = false;
	}

	void Open() {
		Callback::attach(Callback::OnOpenGL, OnOpenGL);

		patchMipBlt.DoctorRelative((u32)OnMipBlt, 1).Apply(true);
		patchCreateFileFromGridFile.DoctorRelative((u32)OnCreateFileFromGridFile, 1).Apply(true);
		patchSubdivideTest.DoctorRelative((u32)OnSubdivideTest, 1).Apply(true);
		patchLeaveTerrainRenderLevelNonZero.DoctorRelative((u32)OnLeaveTerrainRenderLevelNonZero, 1).Apply(true);
		patchLeaveTerrainRenderLevelNonZeroLoop.DoctorRelative((u32)OnTerrainRenderLevelNonZeroLoop, 1).Apply(true);
		patchTerrainRenderLevelZeroLoop.DoctorRelative((u32)OnRenderLevelZeroLoop, 1).Apply(true);
		fnFlushTextureCache = Patch::ReplaceHook((void *)OPENGL_FLUSH_TEXTURE_CACHE_VFT, OnFlushTextureCache);
		

		Reset();
	}

	/*
	void funset(u16 *in, u16 val) {
		for (u32 i = 0; i < 256 * 256; i++)
			*in++ = val;
	}

	BuiltInFunction("tester", tester) {
		Fear::SimTerrain *ter = Fear::Sim::Client()->findObject<Fear::SimTerrain>(8);
		if (ter) {
			u32 *tf = (u32 *)ter->terrainFile;
			if (tf) {
				funset(*(u16 **)tf[4], fromstring<u32>(argv[0]) << 8);
				funset(*(u16 **)tf[8], fromstring<u32>(argv[1]) << 8);
				Console::echo("%x %x %x", tf[4], tf[5], tf[6]);
				Console::echo("%x %x %x", tf[7], tf[8], tf[9]);
				Console::echo("%x %x %x", tf[10], tf[11], tf[12]);
			}
		}
		return "true";
	}
	*/

	struct Init {
		Init() {
			Open();
		}

		~Init() {
			Reset();
		}
	} init;
}; // namespace Terrain