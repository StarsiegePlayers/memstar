#ifndef __FEAR_H__
#define __FEAR_H__

#include "Memstar.h"
#include <math.h>
#include <string.h>

#define PAD_PREFIX() p
#define PAD( size )  char PAD_PREFIX()__LINE__[(size)];

template< class type >
class Vector {
public:
	class Iterator {
	public:
		Iterator( type *item ) : mElement(item) {}
		type &value() { return ( *mElement ); }
		type &operator* () { return ( *mElement ); }
		Iterator &operator++ () { mElement++; return ( *this ); }
		bool operator== ( const Iterator &b ) { return ( mElement == b.mElement ); }
		bool operator!= ( const Iterator &b ) { return ( mElement != b.mElement ); }
	private:
		type *mElement;
	};

	Iterator Begin( ) { return Iterator( mElements ); }
	Iterator End( ) { return Iterator( mElements + mItems ); }

	int mItems, mSize, mFlags;
	type *mElements;
};


namespace Fear {
	struct Object {
										u32         vft;                     // 0x0000
										const char *name;                    // 0x0004
		PAD(0x003c - (0x0004 + 0x004)); u32         id;                      // 0x003c
		PAD(0x004c - (0x003c + 0x004));
	}; /* 0x4c */

	struct Sim : public Object {
		PAD(0x0080 - (0x004c + 0x000)); f32         Time;                    // 0x0080

		template<class T> T *findObject(u32 id) {
			if (!this)
				return NULL;

			__asm {
				mov eax, this
				push eax
				mov edx, [id]
				mov ecx, [eax]
				call [ecx+0x68]
				pop this
			}
		}

		template<class T> T *findObject(const char *nameOrId);
		f32 getTime();
		static Sim *Client();
	};

	struct SimSet : public Object {
										Vector<Object *> mSet;               // 0x0050

		typedef Vector<Object *>::Iterator Iterator;

		Iterator Begin() { return( mSet.Begin( ) ); }
		Iterator End() { return( mSet.End( ) ); }
	};

	struct Sky : public Object {
		PAD(0x00b8 - (0x004c + 0x000)); Vector3f  skyColor;                 //0x00b8
										Vector3f  hazeColor;                //0x00c4
		PAD(0x011c - (0x00c4 + 0x00c)); f32       rotation;                 //0x011c
		PAD(0x0124 - (0x011c + 0x004)); f32       size;                     //0x0124

		void calcPoints();
	};

	struct GFXBitmap {
		PAD(0x0010 - (0x0000 + 0x000)); u32       width;                    //0x0010
		                                u32       height;                   //0x0014
		PAD(0x0030 - (0x0014 + 0x004)); u32       flags;                    //0x0030
		                                u32       paletteIdx;               //0x0034
		                                u8        *bitmapData;              //0x0038
		                                u8        *bitmapMips[7];           //0x0058
		                                void      *hidden;                  //0x005c
										u32       mipLevels;                //0x0060
	};


	Sky *findSky();


	struct OpenGLState {
		void *_glMultiTexCoord2fvARB;   // 0x00
		void *_glMultiTexCoord4fvARB;   // 0x04 
		void *_glActiveTextureARB;      // 0x08
		u32 a;                          // 0x0c
		u32 b;                          // 0x10
		u32 TEXTURE_2D0_ARB;            // 0x14
		u32 TEXTURE_2D1_ARB;            // 0x18
		u32 TEXTURE_ENV0_ARB;           // 0x1c
		u32 TEXTURE_ENV1_ARB;           // 0x20
		u32 BLEND_SRC;                  // 0x24
		u32 BLEND_DST;                  // 0x28
		u32 ALPHA_TEST;                 // 0x2c
		u32 DEPTH_TEST;                 // 0x30
		u32 e;                          // 0x34
		u32 DEPTH_FUNC;                 // 0x38
		u32 f;                          // 0x3c
		u32 TEXTURE_UNIT_ACTIVE;        // 0x40
		u32 GL_BOUNDTEXTURE0;           // 0x44
		u32 GL_BOUNDTEXTURE1;           // 0x48
	};

	struct GraphicsAdapter {
	};

	struct GridFile {
		PAD(0x0058 - (0x0000 + 0x000));   void *gridBlockArray;             // 0x0058
		PAD(0x0064 - (0x0058 + 0x004));   const char *dtfName;              // 0x0064

		inline u16 *getMaterialBlock(u32 block) {
			__asm {
				mov eax, [this]
				mov eax, [eax + 0x58]
				mov edx, [block]
				lea eax, [eax + edx * 4]
				mov eax, [eax]
				mov eax, [eax]
				and eax, eax
				jz __not_loaded
				mov eax, [eax+0x14]
				mov eax, [eax+0x40]
			__not_loaded:
			}
		}
	};

	struct SimTerrain : public Object {
		PAD(0x057c - (0x004c + 0x000));   GridFile *gf;                     // 0x057c
		PAD(0x0584 - (0x057c + 0x004));   void *terrainFile;                // 0x0584
	};

	struct OpenGLTextureCache {
		struct MPCache {
			u8 data[256*4];
			u8 dataTransparent[256*4];
			u16 dataPacked[256];
			u16 dataPackedTransparent[256];
			u32 packedType;
			u32 packedTransparentType;
			s32 paletteIndex;
		};

		PAD(0x0230 - (0x0000 + 0x000)); MPCache multiPalettes[16];

		MPCache &getPalette(s32 index) {
			for (u32 i = 0; i < 16; i++)
				if (multiPalettes[i].paletteIndex == index)
					return multiPalettes[i];
			return multiPalettes[0];
		}

		void pbmpToRGBA(u8 *in, RGBA *out, size_t bytes, s32 index) {
			RGBA *pal = (RGBA *)getPalette(index).data;
			for (size_t i = 0; i < bytes; i++, in++, out++ )
				*out = pal[*in];
		}
	};

	struct OpenGLSurface : public Object {
		PAD(0x0158 - (0x004c + 0x000)); OpenGLTextureCache *textureCache;   // 0x0158
	};

	bool getScreenDimensions(Vector2i *dim);

	struct SimGuiCtrl : public Object {
		PAD(0x019c - (0x004c + 0x000)); Vector2i    pos;                    //0x019c
		                                Vector2i    dimensions;             //0x01a4
	};

	struct Herc : public Object {
	};

	struct PlayerPSC : public Object {
		PAD(0x005c - (0x004c + 0x000)); Object *orbitCamera;                //0x005c
		                                Object *flyCamera;                  //0x0060
										Object *easyCamera;                 //0x0064
										Object *editCamera;                 //0x0068
										Object *splineCamera;               //0x006c
										Herc *player;                       //0x0070
	}

	PlayerPSC *findPSC();
};

#endif // __FEAR_H__