#include "Fear.h"
#include "Font.h"
#include "OpenGL.h"

#pragma warning( disable : 4244 ) // conversion from 'double' to 'float', possible loss of data

void Font::Close() {
	if (!mCreatedDC)
		return;
	DeleteObject(mFont);
	DeleteObject(mDib);
	DeleteDC(mDC);

	mDC = (NULL);
	mFont = (NULL);
	mDib = (NULL);
	mLoaded = (false);
}

// get the font information and create windows handles
bool Font::Create(const char* name, int pixel_height, Rendering mode, int glow_radius) {
	Close();

	// rendering
	mMode = (mode);
	mGlowAdd = (glow_radius > 16) ? 16 : glow_radius;
	mBlurRadius = (glow_radius);

	mDC = (CreateCompatibleDC(NULL));
	if (!mDC)
		return (false);
	SetMapMode(mDC, MM_TEXT);

	mFont = CreateFont(
		-pixel_height,
		0, /* width */
		0, /* escapement */
		0, /* orientation */
		FW_NORMAL, /* weight */
		0, /* italic */
		0, /* underline */
		0, /* strikeout */
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		name);

	if (!mFont) {
		DeleteDC(mDC);
		mDC = (NULL);
		return (false);
	}
	else {
		SelectObject(mDC, mFont);
	}

	// Font height
	TEXTMETRIC metric;
	GetTextMetrics(mDC, &metric);
	mHeight = (metric.tmHeight);
	mAscent = (metric.tmAscent);
	mMaxWidth = (metric.tmMaxCharWidth);
	mTrueType = ((metric.tmPitchAndFamily & TMPF_TRUETYPE) != 0);

	// Char widths
	ABC widths[256];

	if (!mTrueType || !GetCharABCWidths(mDC, 0, 255, widths)) {
		int widths2[256];
		GetCharWidth(mDC, 0, 255, widths2);
		for (int i = 0; i < 256; i++) {
			widths[i].abcA = (0);
			widths[i].abcB = (widths2[i]);
			widths[i].abcC = (0);
		}
	}

	// DibSection
	mDibSize = Texture::CeilPow2(max(mMaxWidth, mHeight));

	BITMAPINFO header;
	memset(&header, 0, sizeof(header));
	header.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	header.bmiHeader.biWidth = (mDibSize);
	header.bmiHeader.biHeight = -(mDibSize); // negative means top down
	header.bmiHeader.biPlanes = (1);
	header.bmiHeader.biBitCount = (32);
	header.bmiHeader.biCompression = (BI_RGB);
	mDib = CreateDIBSection(mDC, &header, DIB_RGB_COLORS, (void**)&mDibPixels, 0, 0);
	SelectObject(mDC, mDib);

	// Color
	SetTextColor(mDC, (COLORREF)0x00FFFFFF);
	SetBkColor(mDC, (COLORREF)0x00000000);
	SetBkMode(mDC, TRANSPARENT);

	// Init character widths
	for (int i = 0, x = 0, y = 0; i < 256; i++) {
		Letter& l = (mLetters[i]);
		ABC& abc = (widths[i]);
		l.mWidth = LetterWidth(abc.abcA, abc.abcB, abc.abcC);
	}

	mCreatedDC = (true);
	mLoaded = (true);
	return (true);
}

// render a letter for use
bool Font::CreateLetter(Letter& l, char ch) {
	if (l.mTexture.Loaded())
		return (true);

	// dont draw these
	if (ch < 31 || ch > 126 || (l.GlyphWidth() <= 0))
		return (false);

	if (mMode == Smooth)
		RenderCharacterSmooth(l, ch);
	else
		RenderCharacter(l, ch);

	if (mGlowAdd)
		l.mTexture.AlphaBloom(mBlurRadius, l.mWidth.mB, mHeight);

	return (true);
}

void Font::Draw(const char* str, int x, int y) {
	if (!mLoaded)
		return;

	glDisable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	Vector2i start(x, y);

	RGBA color(255, 255, 255, 255);
	while (*str) {
		unsigned char ch = (*str++);

		if (ch == '\n') {
			x = (start.x);
			y += (mHeight);
			continue;
		}
		else if (ch == '<') {
			if (atoh(str, (char*)&color, 4) == 4) {
				str += (8);
				if (*str)
					str++;
				glColor4ub(color.r, color.g, color.b, color.a);
				continue;
			}
		}

		Letter& l = (mLetters[ch]);
		if (CreateLetter(l, ch)) {
			x += (l.mWidth.mA);
			l.Draw(x, y, mHeight, mGlowAdd);
			x += (l.mWidth.mB + l.mWidth.mC);
		}
	}
}

/*
	Renders a character with no smoothing
*/
void Font::RenderCharacter(Letter& l, char ch) {
	if (!l.CreateTexture(mHeight, mGlowAdd))
		return;

	memset(mDibPixels, 0, mDibSize * mDibSize * 4);
	TextOut(mDC, -l.mWidth.mA, 0, (char*)&ch, 1);
	for (int y = 0; y < mHeight; y++) {
		for (int x = 0; x < l.GlyphWidth(); x++) {
			RGBA& quad = (mDibPixels[x + y * mDibSize]);
			if (quad.r)
				quad.a = 255;
			else
				quad = (RGBA(255, 255, 255, 0));
			l.mTexture(x + mGlowAdd, y + mGlowAdd) = (quad);
		}
	}
}


/*
	Attempts to create an AA character with GetGlyphOutline
*/
void Font::RenderCharacterSmooth(Letter& l, char ch) {
	GLYPHMETRICS metrics;
	MAT2 mat2 = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };

	int bytes = (GetGlyphOutline(mDC, ch, GGO_GRAY8_BITMAP, &metrics, 0, NULL, &mat2));
	if (bytes == 0) {
		// fallback to non-aa
		RenderCharacter(l, ch);
		return;
	}

	unsigned char* gray = new unsigned char[bytes];
	GetGlyphOutline(mDC, ch, GGO_GRAY8_BITMAP, &metrics, bytes, gray, &mat2);

	// character dimensions may change with anti-aliasing
	int pad = (metrics.gmBlackBoxX - l.mWidth.mB), half = (pad >> 1);

	l.mWidth.mA -= (pad - half);
	l.mWidth.mB = (metrics.gmBlackBoxX);
	l.mWidth.mC -= (half);

	// resize the texture, it wont fail!
	l.CreateTexture(mHeight, mGlowAdd);

	int width = (metrics.gmBlackBoxX + 3) & ~3; // dword aligned	
	int start_y = (mAscent - metrics.gmptGlyphOrigin.y);
	int start_x = 0;

	for (unsigned int j = 0; j < metrics.gmBlackBoxY; j++) {
		for (unsigned int i = start_x; i < metrics.gmBlackBoxX; i++) {
			int x = (i - start_x) + mGlowAdd;
			int y = (j + start_y) + mGlowAdd;

			if ((x >= l.mTexture.mWidth) || (y >= l.mTexture.mHeight))
				continue;

			// format is 64 shades of gray
			l.mTexture(x, y) = RGBA(255, 255, 255, gray[(j * width) + i] * 255 / 64);
		}
	}

	delete[] gray;
}

Vector2i Font::StringDimensions(const char* str) {
	Vector2i dimensions(0, mHeight);

	if (!mLoaded)
		return (dimensions);

	int x = 0;

	while (*str) {
		unsigned char ch = (*str++);

		if (ch == '\n') {
			dimensions.y += (mHeight);
			continue;
		}
		else if (ch == '<') {
			RGBA color;
			if (atoh(str, (char*)&color, 4) == 4) {
				str += (8);
				if (*str)
					str++;
				continue;
			}
		}

		Letter& l = (mLetters[ch]);
		x += (l.mWidth.mA + l.mWidth.mB + l.mWidth.mC);
		dimensions.x = (max(dimensions.x, x));
	}

	return (dimensions);
}
