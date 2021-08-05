#include "Replacer.h"
#include "HashTable.h"
#include "FileSystem.h"
#include "Console.h"
#include "OpenGL.h"
#include "Patch.h"
#include "Callback.h"
#include "Terrain.h"
#include "OpenGL_Pointers.h"

namespace Replacer {

#include "Originals.h"

	MultiPointer(ptr_OPENGL_FLUSH_TEXTURE_VFT, 0, 0, 0x0071a15c, 0x0072A748);

	BuiltInVariable("pref::ShowMatchedTextures", bool, prefShowMatchedTextures, false);

	typedef HashTable< String, TextureWithMips*, IKeyCmp, ValueDeleter<String, TextureWithMips*> > ReplacementHash;
	typedef HashTable< u32, String > OriginalHash;

	bool lastScanMatched = false;
	bool foundLastCRC = false;
	bool ignoreTexImageMips = false;
	bool ignoreTexSubImageMips = false;
	bool lastNonMipMatched = false;

	u32 lastFoundCRC = 0;
	TextureWithMips* lastMatchedTexture = NULL, * lastUsedTexture = NULL;
	FileSystem mFiles(128);
	ReplacementHash mTextures(128);
	OriginalHash mOriginals(4096);

	void Forget() {
		lastScanMatched = false;
		lastMatchedTexture = NULL;
		foundLastCRC = false;
	}

	TextureWithMips* LastMatchedTexture() {
		return (lastScanMatched) ? lastMatchedTexture : NULL;
	}

	bool LastScanWasReplaceable() {
		return foundLastCRC;
	}

	TextureWithMips* FindReplacement(const String* name) {
		// allowed to pass null
		if (!name)
			return (NULL);

		TextureWithMips** find = mTextures.Find(*name);
		if (find)
			return *find;

		TextureWithMips* texture = new TextureWithMips();
		if (!texture || !mFiles.ReadTGA(*name, *texture) || !mTextures.Insert(*name, texture)) {
			delete texture;
			texture = NULL;
		}

		return texture;
	}

	const String* FindOriginalName(u32 texture_crc) {
		return mOriginals.Find(texture_crc);
	}

	void Open() {
		mFiles.Clear();
		mTextures.Clear();
		mFiles.Grok("Memstar/Replacements");
	}

	void Scan(Fear::GFXBitmap* bmp) {
		if (Terrain::TexImageCheck(bmp->bitmapData))
			return;

		lastFoundCRC = HashBytes(bmp->bitmapData, bmp->width * bmp->height);
		const String* file = FindOriginalName(lastFoundCRC);
		if (prefShowMatchedTextures != true) {
			Console::echo("Matched texture %s", file->c_str());
		}
		foundLastCRC = (file != NULL);
		lastMatchedTexture = FindReplacement(file);
		lastScanMatched = (lastMatchedTexture != NULL);
	}

	BuiltInFunction("Memstar::AddReplacement", AddReplacement) {
		if (argc != 2) {
			Console::echo("%s( <texture crc>, <texture name> );", self);
		}
		else {
			u32 crc;
			if (atohhl(argv[0], &crc)) {
				const String* find = FindOriginalName(crc);
				if (find) {
					Console::echo("Replacer: Duplicate CRC! Already bound to %s", find->c_str());
				}
				else {
					Console::echo("Replacer: Adding %s(hash:%x)", argv[1], crc);
					mOriginals.Insert(crc, String2(argv[1]));
				}
			}
		}

		return "true";
	}


	u32 fnglTexImage2D, fnglTexSubImage2D, fnFlushTexture;

	MultiPointer(fnResumeCacheBitmap, 0, 0, 0x0063e71a, 0x0064D822);
	MultiPointer(fnIsFromCurrentRound, 0, 0, 0x0063dedc, 0x0064CFAC);

	MultiPointer(ptr_cacheBitmapPatch, 0, 0, 0x0063e714, 0x0064D81C);
	CodePatch cacheBitmapPatch = {
		ptr_cacheBitmapPatch,
		"",
		"\xE9tccb",
		5,
		false
	};

	NAKED void OnCacheBitmap() {
		__asm {
			pushad

			mov edi, ecx
			mov esi, edx
			mov ebx, eax
			mov edx, edi
			mov eax, ebx
			call[fnIsFromCurrentRound]
			test al, al
			jnz __ignore

			push esi
			call Scan
			add esp, 4

			__ignore:
			popad

				push ebp
				mov ebp, esp
				add esp, -0x18
				jmp[fnResumeCacheBitmap]
		}
	}

	NAKED void OnFlushTexture() {
		__asm {
			pushad

			mov edi, edx
			mov ebx, eax
			mov edx, edi
			mov eax, [ebx + 0x158]
			call[fnIsFromCurrentRound]
			test al, al
			jz __ignore

			push dword ptr[edi + 0x38]
			call Terrain::TexSubImageCheck
			add esp, 4

			__ignore:
			popad
				jmp[fnFlushTexture]
		}
	}

	void GLAPIENTRY OnGlTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, GLvoid* pixels) {
		typedef void (GLAPIENTRY* fn)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);

		bool isMip = (level != 0), needDefault = true;
		if (ignoreTexImageMips && isMip)
			return;

		lastNonMipMatched &= isMip;
		ignoreTexImageMips = false;

		if (!isMip) {
			if (prefShowMatchedTextures && foundLastCRC) {
				static const RGBA colors[16] = {
					RGBA(204,204,204,64),RGBA(255,102,  0,64),RGBA(255, 51,  0,64),RGBA(153,204,  0,64),
					RGBA(255,204,102,64),RGBA(153,153,  0,64),RGBA(255,  0,  0,64),RGBA(153,153, 51,64),
					RGBA(255, 51, 51,64),RGBA(102,153,  0,64),RGBA(153,  0, 51,64),RGBA(102,255, 51,64),
					RGBA(204,153,102,64),RGBA(153,204,102,64),RGBA(102,204,102,64),RGBA(255,153,255,64),
				};
				((fn)fnglTexImage2D)(target, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colors[lastFoundCRC & 0x0f]);
				ignoreTexImageMips = true;
				needDefault = false;
			}
			else if (lastMatchedTexture) {
				((fn)fnglTexImage2D)(target, level, GL_RGBA8, lastMatchedTexture->mWidth, lastMatchedTexture->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, lastMatchedTexture->mData);
				lastUsedTexture = lastMatchedTexture;
				lastNonMipMatched = true;
				needDefault = false;
			}
			else {
				RGBA** terrain = Terrain::CheckForTerrainTile();
				if (terrain) {
					u32 mipWidth = width, mipLevel = 0;
					while (mipWidth > 0) {
						if (!*terrain)
							break;
						((fn)fnglTexImage2D)(target, mipLevel, GL_RGBA8, mipWidth, mipWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, *terrain++);
						mipLevel++;
						mipWidth >>= 1;
					}
					ignoreTexImageMips = true;
					needDefault = false;
				}
			}
		}
		else if (lastNonMipMatched && (level == 1)) {
			lastUsedTexture->GenerateMipMaps();
			for (u32 i = 0; lastUsedTexture->mMipMaps[i]; i++) {
				Texture* mip = lastUsedTexture->mMipMaps[i];
				((fn)fnglTexImage2D)(target, i + 1, GL_RGBA8, mip->mWidth, mip->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mip->mData);
			}
			ignoreTexImageMips = true;
			needDefault = false;
		}

		if (needDefault)
			((fn)fnglTexImage2D)(target, level, internalformat, width, height, border, format, type, pixels);
		Forget();
	}

	void GLAPIENTRY OnGlTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels) {
		typedef void (GLAPIENTRY* fn)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);

		if (ignoreTexSubImageMips) {
			if (level != 0)
				return;
			ignoreTexSubImageMips = false;
		}

		bool needDefault = true;
		RGBA** terrain = Terrain::CheckForTerrainTile();
		if (terrain) {
			u32 mipWidth = width, mipLevel = 0;
			while (mipWidth > 0) {
				if (!*terrain)
					break;
				((fn)fnglTexSubImage2D)(target, mipLevel, xoffset, yoffset, mipWidth, mipWidth, GL_RGBA, GL_UNSIGNED_BYTE, *terrain++);
				mipLevel++;
				mipWidth >>= 1;
			}
			ignoreTexSubImageMips = true;
			needDefault = false;
		}

		if (needDefault)
			((fn)fnglTexSubImage2D)(target, level, xoffset, yoffset, width, height, format, type, pixels);
	}

	void OnStarted(bool state) {
		Open();

		for (int i = 0; i < __ORIGINAL_COUNT__; i++) {
			mOriginals.Insert(mOriginalsTable[i].mCrc, String2(mOriginalsTable[i].mName));
		}

		fnglTexImage2D = Patch::ReplaceHook((void*)OpenGLPtrs::ptr_OPENGL32_GLTEXIMAGE2D, OnGlTexImage2D);
		fnglTexSubImage2D = Patch::ReplaceHook((void*)OpenGLPtrs::ptr_OPENGL32_GLTEXSUBIMAGE2D, OnGlTexSubImage2D);
		fnFlushTexture = Patch::ReplaceHook((void*)ptr_OPENGL_FLUSH_TEXTURE_VFT, OnFlushTexture);

		cacheBitmapPatch.DoctorRelative((u32)&OnCacheBitmap, 1).Apply(true);
	}

	struct Init {
		Init() {
			if (VersionSnoop::GetVersion() == VERSION::vNotGame) {
				return;
			}
			if (VersionSnoop::GetVersion() < VERSION::v001003) {
				return;
			}
			Callback::attach(Callback::OnStarted, OnStarted);
		}
	} init;

}; // namespace Replacer