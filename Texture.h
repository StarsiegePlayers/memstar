#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Fear.h"
#include "HashTable.h"

#define TGA_INDEXED		(  1 )
#define TGA_RGB			(  2 )
#define TGA_GRAY		(  3 )
#define TGA_INDEXED_RLE	(  9 )
#define TGA_RGB_RLE		( 10 )

#pragma pack(push)
#pragma pack(1)

class TargaHeader {
public:
	void LoadPalette(unsigned char*& input, RGBA* pal);
	void LoadIndexed(unsigned char* input, RGBA* pixels, RGBA* pal);
	void LoadRGB(unsigned char* input, RGBA* pixels);


	unsigned char 	id_length, colormap_type, image_type;
	unsigned short	colormap_index, colormap_length;
	unsigned char	colormap_size;
	unsigned short	x_origin, y_origin, width, height;
	unsigned char	pixel_size, attributes;
};

#pragma pack(pop)

#define TEXTURE_WIDTH   ( 1 << 0 )
#define TEXTURE_HEIGHT  ( 1 << 1 )
#define TEXTURE_INVERSE ( 1 << 2 )

class Texture {
public:
	/*
		FUNCTIONS
	*/
	Texture() : mData(NULL), mHandle(0), mWidth(0), mHeight(0) { }
	~Texture() { Free(); }

	void AlphaBloom(int blur_radius, int actual_width, int actual_height);

	static int CeilPow2(int x) {
		int y = 1;
		while (y < x)
			y <<= 1;

		return (y);
	}

	void Clear(int r, int g, int b, int a) {
		if (!mData)
			return;
		RGBA color = RGBA(r, g, b, a);
		for (int i = 0; i < mWidth * mHeight; i++)
			mData[i] = color;
	}

	void Draw(float x, float y);
	void DrawScaled(float x, float y, float scale_x, float scale_y);
	void DrawCentered(float x, float y);
	void DrawTo(float x, float y, float width, float height);
	void DrawPartial(float x, float y, float percent, int flags);
	void DrawRotated(float x, float y, float scale_x, float scale_y, float rotation);

	Texture* Duplicate();
	bool Duplicate(Texture& b) const;

	bool Loaded() const { return (mData != NULL); }
	bool LoadTGA(unsigned char* input);
	bool New(int width, int height);

	// Virtuals
	virtual void Free();
	virtual bool BindToGraphicsCard();
	virtual void UnloadFromGraphicsCard();

	// Operators
	RGBA& operator()(int x, int y) {
		return (mData[y * mWidth + x]);
	}

	/*
		VARIABLES
	*/
	unsigned int mHandle;
	int mWidth, mHeight;
	RGBA* mData;


	/*
		STATICS
	*/
	typedef HashTable< unsigned int, Texture* > Hash;

	static void BuildMulTab();
	static void HalveTexture(RGBA* src, int srcX, int srcY, RGBA* dst, int dstX, int dstY);
	static void UnloadAllFromGraphicsCard();

	static unsigned int mMulTab[256 * 256];
	static bool mMulTabCalculated;

	static Hash mTexturesLoadedToOpenGL;
};



class TextureWithMips : public Texture {
public:
	TextureWithMips() : Texture() {
		memset(mMipMaps, 0, sizeof(mMipMaps));
	}

	~TextureWithMips() {
		FreeMipMaps();
	}

	virtual void Free();
	void GenerateMipMaps();
	void FreeMipMaps();
	virtual bool BindToGraphicsCard();

	//private:
	Texture* mMipMaps[12];
};

#endif // __TEXTURE_H__