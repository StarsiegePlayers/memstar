#include "OpenGL.h"
#include "Texture.h"
#include "Console.h"
#include "Callback.h"

unsigned int Texture::mMulTab[256 * 256];
bool Texture::mMulTabCalculated = (false);
Texture::Hash Texture::mTexturesLoadedToOpenGL(128);

/*
	Targa
*/

void TargaHeader::LoadPalette(unsigned char*& input, RGBA* pal) {
	for (int i = 0; i < colormap_length; i++) {
		RGBA& entry = (pal[i + colormap_index]);

		if (colormap_size == 16) {
			unsigned char c1 = (*input++), c2 = (*input++);
			entry.b = (c1 & 0x1f);
			entry.g = (c2 & 0x03) | ((c1 & 0xd0) >> 5);
			entry.r = (c2 & 0x7c) >> 2;
			entry.a = (c2 & 0x80) >> 7;
		}
		else {
			entry.b = (*input++);
			entry.g = (*input++);
			entry.r = (*input++);
			entry.a = (colormap_size == 32) ? (*input++) : 255;
		}
	}
}

void TargaHeader::LoadIndexed(unsigned char* input, RGBA* pixels, RGBA* pal) {
	pixels = (pixels + (height - 1) * width);
	int count = (width * height);
	int x = (width);

	if (image_type == TGA_INDEXED) {
		while (count) {
			x = (min(x, count));
			count -= x;

			while (x--)
				*pixels++ = pal[*input++];

			pixels -= (width * 2);
			x = (width);
		}
	}
	else if (image_type == TGA_INDEXED_RLE) {
		while (count) {
			unsigned char header = (*input++);
			bool packed = ((header & 0x80) == 0x80);
			int length = (header & 0x7f) + 1;

			length = (min(length, count));
			count -= length;

			while (length) {
				int unpack = (min(x, length));
				length -= (unpack);
				x -= (unpack);

				if (packed) {
					RGBA quad = (pal[*input++]);
					while (unpack--)
						*pixels++ = (quad);
				}
				else {
					while (unpack--)
						*pixels++ = (pal[*input++]);
				}

				if (x == 0) {
					pixels -= (width * 2);
					x = (width);
				}
			}
		}
	}
}

void TargaHeader::LoadRGB(unsigned char* input, RGBA* pixels) {
	pixels = (pixels + (height - 1) * width);
	int count = (width * height);
	int x = (width);

	if (image_type == TGA_RGB) {
		while (count) {
			x = (min(x, count));
			count -= x;

			while (x--) {
				pixels->b = (*input++);
				pixels->g = (*input++);
				pixels->r = (*input++);
				pixels->a = (pixel_size == 32) ? (*input++) : 255;
				pixels++;
			}

			pixels -= (width * 2);
			x = (width);
		}
	}
	else if (image_type == TGA_RGB_RLE) {
		while (count) {
			unsigned char header = (*input++);
			bool packed = ((header & 0x80) == 0x80);
			int length = (header & 0x7f) + 1;

			length = (min(length, count));
			count -= length;

			while (length) {
				int unpack = (min(x, length));
				length -= (unpack);
				x -= (unpack);

				if (packed) {
					RGBA quad;
					quad.b = (*input++);
					quad.g = (*input++);
					quad.r = (*input++);
					quad.a = (pixel_size == 32) ? (*input++) : 255;

					while (unpack--)
						*pixels++ = (quad);
				}
				else {
					while (unpack--) {
						pixels->b = (*input++);
						pixels->g = (*input++);
						pixels->r = (*input++);
						pixels->a = (pixel_size == 32) ? (*input++) : 255;
						pixels++;
					}
				}

				if (x == 0) {
					pixels -= (width * 2);
					x = (width);
				}
			}
		}
	}
}

/*
	Texture
*/


/*
	Gaussian blurs the alpha channel and adds it back to the
	original image. About as fast as it's going to get..
*/

void Texture::AlphaBloom(int blur_radius, int actual_width, int actual_height) {
	if (mWidth > 2048 || mHeight > 2048)
		return;

	// limit the width/height coverage if you like
	actual_width += (blur_radius * 2);
	actual_height += (blur_radius * 2);
	if (actual_width > mWidth)
		actual_width = (mWidth);
	if (actual_height > mHeight)
		actual_height = (mHeight);

	Texture::BuildMulTab();
	int kernel_size = (blur_radius * 2 + 1);
	int half = (kernel_size / 2);
	float sigma = (kernel_size - 1) / 6.0f;

	// create the gaussian kernel
	int* kernel = new int[kernel_size];
	int* k = (kernel + half);
	for (int i = -half; i <= half; i++) {
		float height = (1 / sqrtf(PI2 * sigma * sigma)) * (powf(E, -((i * i) / (2 * sigma * sigma))));
		k[i] = (int)(height * 256) << 8; // offset for indexing into multab
	}

	// canvas to hold horiztonal blur
	unsigned char* canvas = new unsigned char[mWidth * mHeight];
	memset(canvas, 0, mWidth * mHeight);

	// min/max/total weight tables for edges
	int _min[2048], _max[2048], _weight[2048];

	// precompute spans & weight
	for (int x = 0; x < mWidth; x++) {
		_min[x] = (max(x - half, 0) - x);
		_max[x] = (min(x + half, mWidth - 1) - x);

		_weight[x] = 0;
		for (int i = _min[x]; i <= _max[x]; i++)
			_weight[x] += (k[i] >> 8);
		if (_weight[x] >= 253)
			_weight[x] = 256;
	}

	// horizontal pass
	for (int y = 0; y < actual_height; y++) {
		RGBA* in = (mData + (y * mWidth));
		unsigned char* out = (canvas + (y * mWidth));

		for (int x = 0; x < actual_width; x++, in++, out++) {
			int accum = (0), low = _min[x], high = _max[x];
			int* weight = (&k[low]);
			RGBA* pixels = (&in[low]);

			// convolve the 1d filter
			for (; low <= high; low++, weight++, pixels++)
				accum += (mMulTab[*weight + pixels->a]);

			// assume weight is 1, proper sigma handles this but edges may fade a little
			*out = (accum >> 8);
		}
	}

	// precompute spans & weight
	for (int y = 0; y < mHeight; y++) {
		_min[y] = (max(y - half, 0) - y);
		_max[y] = (min(y + half, mHeight - 1) - y);

		_weight[y] = 0;
		for (int i = _min[y]; i <= _max[y]; i++)
			_weight[y] += (k[i] >> 8);
	}

	// vertical pass
	for (int y = 0; y < actual_height; y++) {
		unsigned char* in = (canvas + (y * mWidth));
		RGBA* out = (mData + (y * mWidth));

		for (int x = 0; x < actual_width; x++, in++, out++) {
			int accum = 0, low = _min[y], high = _max[y];
			int* weight = (&k[low]);
			unsigned char* bytes = (&in[low * mWidth]);

			// convolve the 1d filter
			for (; low <= high; low++, weight++, bytes += mWidth)
				accum += (mMulTab[*weight + *bytes]);

			int final = (out->a + (accum >> 8));
			out->a = (final > 255) ? 255 : final;
		}
	}

	delete[] canvas;
	delete[] kernel;
}

void Texture::Draw(float x, float y) {
	if (!BindToGraphicsCard())
		return;

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(x, y);
	glTexCoord2f(1, 0); glVertex2f(x + mWidth, y);
	glTexCoord2f(1, 1); glVertex2f(x + mWidth, y + mHeight);
	glTexCoord2f(0, 1); glVertex2f(x, y + mHeight);
	glEnd();
}

void Texture::DrawCentered(float x, float y) {
	Draw(x - (mWidth / 2.0f), y - (mHeight / 2.0f));
}

void Texture::DrawTo(float x, float y, float width, float height) {
	if (!BindToGraphicsCard())
		return;

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(x, y);
	glTexCoord2f(1, 0); glVertex2f(x + width, y);
	glTexCoord2f(1, 1); glVertex2f(x + width, y + height);
	glTexCoord2f(0, 1); glVertex2f(x, y + height);
	glEnd();
}

void Texture::DrawPartial(float x, float y, float percent, int flags) {
	if (!BindToGraphicsCard())
		return;

	float x1 = x, y1 = y, x2 = (x + mWidth), y2 = (y + mHeight);
	float tx1 = 0, ty1 = 0, tx2 = 1, ty2 = 1;

	float* screen, * tex, * src, dim;

	if (flags & TEXTURE_WIDTH) {
		dim = ((float)mWidth);
		src = (&x);

		if (flags & TEXTURE_INVERSE) {
			screen = (&x1);
			tex = (&tx1);
			//percent = ( 1.0f - percent );
		}
		else {
			screen = (&x2);
			tex = (&tx2);
		}
	}
	else if (flags & TEXTURE_HEIGHT) {
		dim = ((float)mHeight);
		src = (&y);

		if (flags & TEXTURE_INVERSE) {
			screen = (&y2);
			tex = (&ty2);
		}
		else {
			screen = (&y1);
			tex = (&ty1);
		}
	}

	*tex = (percent);
	*screen = (*src + dim * percent);

	glBegin(GL_QUADS);
	glTexCoord2f(tx1, ty1); glVertex2f(x1, y1);
	glTexCoord2f(tx2, ty1); glVertex2f(x2, y1);
	glTexCoord2f(tx2, ty2); glVertex2f(x2, y2);
	glTexCoord2f(tx1, ty2); glVertex2f(x1, y2);
	glEnd();


	/*
	glDisable( GL_TEXTURE_2D );
	if ( flags & TEXTURE_INVERSE )
		glColor4ub( 0, 255, 0, 128 );
	else
		glColor4ub( 255, 0, 0, 128 );
	glBegin( GL_QUADS );
		glVertex2f( x1, y1 );
		glVertex2f( x2, y1 );
		glVertex2f( x2, y2 );
		glVertex2f( x1, y2 );
	glEnd( );
	*/
}


void Texture::DrawRotated(float x, float y, float scale_x, float scale_y, float rotation) {
	if (!BindToGraphicsCard())
		return;

	float halfx = (scale_x * (float)mWidth / 2.0f);
	float halfy = (scale_y * (float)mHeight / 2.0f);

	Vector2f texturecoords[4] = {
		Vector2f(0, 0),
		Vector2f(1, 0),
		Vector2f(1, 1),
		Vector2f(0, 1)
	};

	Vector2f corners[4] = {
		Vector2f(-halfx, -halfy),
		Vector2f(halfx, -halfy),
		Vector2f(halfx,  halfy),
		Vector2f(-halfx,  halfy),
	};

	Vector2f::Rotate(corners, 4, rotation);

	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) {
		glTexCoord2f(texturecoords[i].x, texturecoords[i].y);
		glVertex2f(x + corners[i].x, y + corners[i].y);
	}
	glEnd();
}

void Texture::DrawScaled(float x, float y, float scale_x, float scale_y) {
	if (!BindToGraphicsCard())
		return;

	float width = (mWidth * scale_x);
	float height = (mHeight * scale_y);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(x, y);
	glTexCoord2f(1, 0); glVertex2f(x + width, y);
	glTexCoord2f(1, 1); glVertex2f(x + width, y + height);
	glTexCoord2f(0, 1); glVertex2f(x, y + height);
	glEnd();
}

Texture* Texture::Duplicate() {
	Texture* dup = new Texture();
	if (!dup)
		return (NULL);

	if (!Duplicate(*dup)) {
		delete dup;
		dup = NULL;
	}

	return (dup);
}

bool Texture::Duplicate(Texture& b) const {
	b.Free();

	if (!mData)
		return (false);

	b.mWidth = mWidth;
	b.mHeight = mHeight;
	b.mData = (RGBA*)malloc(mWidth * mHeight * sizeof(RGBA));
	memcpy(b.mData, mData, mWidth * mHeight * 4);

	return (true);
}

bool Texture::LoadTGA(unsigned char* input) {
	Free();

	TargaHeader tga = *(TargaHeader*)input;
	input += (sizeof(tga));
	input += (tga.id_length);

	if (tga.image_type == TGA_RGB || tga.image_type == TGA_RGB_RLE) {
		if (tga.pixel_size != 32 && tga.pixel_size != 24 || tga.colormap_type != 0) {
			Console::echo("TGA: Invalid pixel size");
			return (false);
		}
	}
	else if (tga.image_type == TGA_INDEXED || tga.image_type == TGA_INDEXED_RLE) {
		if (tga.pixel_size != 8 || tga.colormap_type != 1) {
			Console::echo("TGA: Invalid pixel size");
			return (false);
		}
	}
	else if (tga.image_type == TGA_GRAY) {
		Console::echo("TGA: Grayscale not supported");
		return (false);
	}
	else {
		Console::echo("TGA: Unsupported image type");
		return (false);
	}

	RGBA pal[256];
	if (tga.colormap_type)
		tga.LoadPalette(input, pal);

	mWidth = (tga.width);
	mHeight = (tga.height);
	mData = (RGBA*)malloc(mWidth * mHeight * sizeof(RGBA));

	switch (tga.image_type) {
	case TGA_INDEXED:
	case TGA_INDEXED_RLE:
		tga.LoadIndexed(input, mData, pal);
		break;

	case TGA_RGB:
	case TGA_RGB_RLE:
		tga.LoadRGB(input, mData);
		break;
	}

	if (tga.attributes & 0x20) {
		RGBA* flip = (RGBA*)malloc(mWidth * sizeof(RGBA));

		for (int y = 0; y < mHeight / 2; y++) {
			RGBA* src = (mData + (y * mWidth));
			RGBA* dst = (mData + (mHeight - y - 1) * mWidth);

			memcpy(flip, src, mWidth * sizeof(RGBA));
			memcpy(src, dst, mWidth * sizeof(RGBA));
			memcpy(dst, flip, mWidth * sizeof(RGBA));
		}

		free(flip);
	}

	return (true);
}

bool Texture::New(int width, int height) {
	Free();

	mData = (RGBA*)malloc(width * height * sizeof(RGBA));
	if (!mData)
		return (false);

	memset(mData, 0, height * width * 4);
	mWidth = (width);
	mHeight = (height);

	return (true);
}

/*
	Virtuals
*/

void Texture::Free() {
	// HashTables mItemCount check should keep this from crashing ...
	if (mHandle) {
		// don't delete in UnloadFromGraphicsCard
		mTexturesLoadedToOpenGL.Delete(mHandle);
		UnloadFromGraphicsCard();
	}

	free(mData);
	mHandle = mWidth = mHeight = 0;
	mData = NULL;
}

bool Texture::BindToGraphicsCard() {
	if (mHandle) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mHandle);
		return (true);
	}

	if (!mData)
		return (false);

	glGenTextures(1, &(GLuint)mHandle);
	if (!mHandle)
		return (false);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	mTexturesLoadedToOpenGL.InsertUnique(mHandle, this);
	return (true);
}

void Texture::UnloadFromGraphicsCard() {
	if (mHandle)
		glDeleteTextures(1, &(GLuint)mHandle);

	mHandle = 0;
}






/*
	TextureWithMips
*/

void TextureWithMips::Free() {
	Texture::Free();
	FreeMipMaps();
}

void TextureWithMips::GenerateMipMaps() {
	RGBA* mipSrc = (mData);
	int width, height;
	int level = 0;

	if (!mipSrc || mMipMaps[0])
		return;

	width = (mWidth);
	height = (mHeight);

	if (!ISPOWOF2(width) || !ISPOWOF2(height))
		return;

	while ((width > 1) || (height > 1)) {
		int newWidth = (width >> 1);
		int newHeight = (height >> 1);

		if (newWidth <= 0)
			newWidth = 1;
		if (newHeight <= 0)
			newHeight = 1;

		mMipMaps[level] = new Texture();
		mMipMaps[level]->New(newWidth, newHeight);

		Texture::HalveTexture(mipSrc, width, height, (RGBA*)mMipMaps[level]->mData, newWidth, newHeight);

		width = newWidth;
		height = newHeight;
		mipSrc = (mMipMaps[level++]->mData);
	}
}

void TextureWithMips::FreeMipMaps() {
	for (int i = 0; mMipMaps[i]; i++) {
		mMipMaps[i]->Free();
		delete mMipMaps[i];
		mMipMaps[i] = NULL;
	}
}

bool TextureWithMips::BindToGraphicsCard() {
	if (mHandle) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mHandle);
		return (true);
	}

	if (!mData)
		return (false);

	glGenTextures(1, &(GLuint)mHandle);
	if (!mHandle)
		return (false);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mData);

	GenerateMipMaps();
	for (int i = 0; mMipMaps[i]; i++)
		glTexImage2D(GL_TEXTURE_2D, i + 1, GL_RGBA8, mMipMaps[i]->mWidth, mMipMaps[i]->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mMipMaps[i]->mData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	mTexturesLoadedToOpenGL.InsertUnique(mHandle, this);
	return (true);
}


/*
	Statics
*/

// Table of multiplications for up to 256x256
void Texture::BuildMulTab() {
	if (mMulTabCalculated)
		return;

	unsigned int i, j;
	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			mMulTab[(i << 8) + j] = (i * j);

	mMulTabCalculated = true;
}

// Not quite proper but this needs to be fast
void Texture::HalveTexture(RGBA* src, int srcX, int srcY, RGBA* dst, int dstX, int dstY) {
	RGBA* in = (src), * out = (dst);

	if (!ISPOWOF2(srcX) || !ISPOWOF2(srcY))
		return;

	if (srcY > 1 && srcX > 1) {
		__asm {
			mov esi, [dst]
			mov eax, [src]

			mov edx, [dstY]
			test edx, edx
			jle __done

			mov ecx, [srcX]
			shl ecx, 2

			__yloop:
			mov	edi, [dstX]
				pxor mm7, mm7

				align 16
				__xloop :
				movd mm0, [ecx + eax]
				movd mm1, [ecx + eax + 4]
				punpcklbw mm0, mm7
				movd mm2, [eax + 4]
				punpcklbw mm1, mm7
				movd mm3, [eax]
				punpcklbw mm2, mm7
				punpcklbw mm3, mm7
				paddw mm0, mm1
				paddw mm2, mm3
				paddw mm0, mm2
				psrlw mm0, 2
				packuswb mm0, mm7

				add esi, 4
				add	eax, 8
				dec	edi
				movd[esi - 4], mm0
				jne __xloop

				add	eax, ecx
				dec	edx
				jne	__yloop

				__done :
			emms
		}
	}
	else {
		int until = (max(dstX, dstY));

		for (int x = 0; x < until; x++, in += 2, out += 1) {
			out->r = (in->r + (in + 1)->r) >> 1;
			out->g = (in->g + (in + 1)->g) >> 1;
			out->b = (in->b + (in + 1)->b) >> 1;
			out->a = (in->a + (in + 1)->a) >> 1;
		}
	}
}

void Texture::UnloadAllFromGraphicsCard() {
	Hash::Iterator iter = (mTexturesLoadedToOpenGL.Begin()), end = (mTexturesLoadedToOpenGL.End());
	while (iter != end) {
		iter.value()->UnloadFromGraphicsCard();
		++iter;
	}

	mTexturesLoadedToOpenGL.Clear();
}

namespace TexturePurger {
	void OnOpenGL(bool state) {
		if (!state)
			Texture::UnloadAllFromGraphicsCard();
	}

	struct Init {
		Init() {
			Callback::attach(Callback::OnOpenGL, OnOpenGL);
		}
	} init;
};
