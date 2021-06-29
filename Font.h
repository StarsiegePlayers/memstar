#ifndef __FONT_H__
#define __FONT_H__

#include "Texture.h"
#include "OpenGL.h"


class LetterWidth {
public:
	LetterWidth() {}
	LetterWidth(int a, int b, int c) : mA(a), mB(b), mC(c) {}
	int mA, mB, mC;
};


class Letter {
public:
	Letter() { }

	// letter loads itself to opengl on demand
	void Draw(int x, int y, int font_height, int glow_radius) {
		if (!mTexture.Loaded() || !mTexture.BindToGraphicsCard())
			return;

		x -= (glow_radius);
		y -= (glow_radius);

		int glow = (glow_radius * 2);
		int width = (mWidth.mB + glow);
		int height = (font_height + glow);

		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2i(x, y);
		glTexCoord2f(mExtent.x, 0); glVertex2i(x + width, y);
		glTexCoord2f(mExtent.x, mExtent.y); glVertex2i(x + width, y + height);
		glTexCoord2f(0, mExtent.y); glVertex2i(x, y + height);
		glEnd();

		/*
				glDisable( GL_TEXTURE_2D );
				glBegin( GL_LINE_STRIP );
					glVertex2f(         x, y );
					glVertex2f( x + width, y );
					glVertex2f( x + width, y + height );
					glVertex2f(         x, y + height );
					glVertex2f(         x, y );
				glEnd( );
				glEnable( GL_TEXTURE_2D );
		*/
	}

	// size of actual character, not trailing/leading rendering space
	int GlyphWidth() const { return (mWidth.mB); }

	bool CreateTexture(int font_height, int glow_radius) {
		int width = (GlyphWidth() + glow_radius * 2);
		int height = (font_height + glow_radius * 2);

		mTexture.New(Texture::CeilPow2(width), Texture::CeilPow2(height));
		if (!mTexture.Loaded())
			return (false);

		// clear to pure white, no alpha
		mTexture.Clear(255, 255, 255, 0);
		mExtent.x = ((float)width / (float)mTexture.mWidth);
		mExtent.y = ((float)height / (float)mTexture.mHeight);
		return (true);
	}

	Texture mTexture;
	Vector2f mExtent;
	LetterWidth mWidth;
};

class Font {
public:
	enum Rendering {
		Pixel,
		Smooth,
	};

	Font() : mLoaded(false), mLastUsed(GetTickCount()), mCreatedDC(false) { }
	~Font() { Close(); }

	void Close();
	bool Create(const char* name, int pixel_height, Rendering mode, int glow_radius);
	void Draw(const char* str, int x, int y);
	int LastUsed() { return (mLastUsed); }
	Vector2i StringDimensions(const char* str);
	void UpdateLastUsed() { mLastUsed = (GetTickCount()); }

private:
	bool CreateLetter(Letter& l, char ch);
	void RenderCharacter(Letter& l, char ch);
	void RenderCharacterSmooth(Letter& l, char ch);

	// font info
	Letter mLetters[256];
	int mHeight;
	bool mLoaded;
	int mLastUsed;

	// only for rendering
	int mGlowAdd, mBlurRadius;
	Rendering mMode;
	bool mCreatedDC;
	RGBA* mDibPixels;
	HDC mDC;
	HFONT mFont;
	HBITMAP mDib;
	int mDibSize, mAscent, mMaxWidth;
	bool mTrueType;

};




#endif // __FONT_H__
