#ifndef __GLOPCODES_H__
#define __GLOPCODES_H__

#include "Fear.h"
#include "FontManager.h"
#include "Strings.h"
#include "ScriptGL.h"
#include "List.h"


/*
	Construct in place: type *item = new( from ) type;
*/
INLINE void* __cdecl operator new(size_t size, void* in_place) {
	size;
	return (in_place);
}

INLINE void __cdecl operator delete(void* block, void* in_place) {
	block;
	in_place;
}

#define GLEX_DRAW ( 0 )
#define GLEX_CENTERED ( 1 )
#define GLEX_SCALED ( 2 )
#define GLEX_ROTATED ( 3 )

#define GLEX_FONT_PIXEL ( Font::Pixel )
#define GLEX_FONT_SMOOTH ( Font::Smooth )

/*
	Base opcode
*/

class GLOp {
public:
	virtual void Execute() {};
	virtual void Free() {};
};

/*
	More an op-stack than a compiler..
*/

class GLCompiler {
public:
	typedef List< char > OpcodeCache;
	typedef List< size_t > OpsIndex;

	GLCompiler::~GLCompiler() { Clear(); }

	void Clear() {
		OpsIndex::Iterator iter = (mOpsIndex.Begin()), end = (mOpsIndex.End());
		while (iter != end) {
			((GLOp*)&mCache[iter.value()])->Free();
			++iter;
		}

		// clear resets the item count without freeing memory
		mCache.Clear();
		mOpsIndex.Clear();
	}

	void Execute() {
		ScriptGL::mBeginCount = (0);

		OpsIndex::Iterator iter = (mOpsIndex.Begin()), end = (mOpsIndex.End());
		while (iter != end) {
			((GLOp*)&mCache[iter.value()])->Execute();
			++iter;
		}

		// an un-closed glBegin will royally screw things up
		if (ScriptGL::mBeginCount > 0)
			glEnd();
	}

	template< class type >
	void Push(const type& op) {
		// abuse the vector of bytes for variable object size storage
		type* item = (type*)(mCache.Allocate(sizeof(op)));
		if (!item) {
			return;
		}

		new(item) type; // construct in place
		*item = (op);

		// need to use offsets instead of pointers because List reallocs can move 
		// base pointer around. this means we need to re-align string2's as well
		mOpsIndex.Push(((char*)item - &mCache[0]));
	}

private:
	OpcodeCache mCache;
	OpsIndex mOpsIndex;
};

/*
	Opcodes
*/




class GLBegin : public GLOp {
public:
	GLBegin() {}
	GLBegin(GLenum cap) : mCap(cap) {}
	virtual void Execute() {
		glBegin(mCap); ScriptGL::mBeginCount++;
	}
private:
	GLenum mCap;
};

class GLBindTexture : public GLOp {
public:
	GLBindTexture() {}
	GLBindTexture(const char* tex) : mTexture(tex) {}
	virtual void Execute() {
		Texture* find = ScriptGL::FindTexture(mTexture.Realign().c_str());
		if (find)
			find->BindToGraphicsCard();
	}
	virtual void Free() { mTexture.Free(); }
private:
	String2 mTexture;
};

class GLBlendFunc : public GLOp {
public:
	GLBlendFunc() {}
	GLBlendFunc(GLenum sfactor, GLenum dfactor) : mSource(sfactor), mDest(dfactor) {}
	virtual void Execute() {
		glBlendFunc(mSource, mDest);
	}
private:
	GLenum mSource, mDest;
};

class GLColor4ub : public GLOp {
public:
	GLColor4ub() {}
	GLColor4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a) : mRGBA(RGBA(r, g, b, a)) {}
	virtual void Execute() { glColor4ub(mRGBA.r, mRGBA.g, mRGBA.b, mRGBA.a); }
private:
	RGBA mRGBA;
};

class GLDisable : public GLOp {
public:
	GLDisable() {}
	GLDisable(GLenum cap) : mCap(cap) {}
	virtual void Execute() { glDisable(mCap); }
private:
	GLenum mCap;
};

class GLEnable : public GLOp {
public:
	GLEnable() {}
	GLEnable(GLenum cap) : mCap(cap) {}
	virtual void Execute() { glEnable(mCap); }
private:
	GLenum mCap;
};

class GLEnd : public GLOp {
public:
	GLEnd() {}
	virtual void Execute() {
		glEnd(); ScriptGL::mBeginCount--;
	}
};

class GLTexCoord2f : public GLOp {
public:
	GLTexCoord2f() {}
	GLTexCoord2f(GLfloat u, GLfloat v) : mU(u), mV(v) {}
	virtual void Execute() { glTexCoord2f(mU, mV); }
private:
	float mU, mV;
};

class GLTexEnvi : public GLOp {
public:
	GLTexEnvi() {}
	GLTexEnvi(GLint param) : mParam(param) {}
	virtual void Execute() { glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mParam); }
private:
	int mParam;
};

class GLVertex2i : public GLOp {
public:
	GLVertex2i() {}
	GLVertex2i(GLint x, GLint y) : mX(x), mY(y) {}
	virtual void Execute() { glVertex2i(mX, mY); }
private:
	GLint mX, mY;
};

class GLDrawString : public GLOp {
public:
	GLDrawString() {}
	GLDrawString(int x, int y, const char* str) : mX(x), mY(y) { mStr = str; }
	virtual void Execute() {
		if (!ScriptGL::mFont)
			return;
		ScriptGL::mFont->Draw(mStr.Realign().c_str(), mX, mY);
	}
	virtual void Free() { mStr.Free(); }
private:
	int mX, mY;
	String2 mStr;
};

class GLDrawTexture : public GLOp {
public:
	GLDrawTexture() { }
	GLDrawTexture(const char* file, int mode, float x, float y, float sx, float sy, float r)
		: mMode(mode), mX(x), mY(y), mSX(sx), mSY(sy), mR(r) {
		mFile = file;
	}

	virtual void Execute() {
		Texture* find = (ScriptGL::FindTexture(mFile.Realign().c_str()));
		if (!find)
			return;

		glEnable(GL_TEXTURE_2D);

		if (mMode == GLEX_DRAW) {
			find->Draw(mX, mY);
		}
		else if (mMode == GLEX_CENTERED) {
			find->DrawCentered(mX, mY);
		}
		else if (mMode == GLEX_SCALED) {
			find->DrawScaled(mX, mY, mSX, mSY);
		}
		else if (mMode == GLEX_ROTATED) {
			find->DrawRotated(mX, mY, mSX, mSY, mR);
		}
	}
	virtual void Free() { mFile.Free(); }

private:
	String2 mFile;
	int mMode;
	float mX, mY, mSX, mSY, mR;
};


class GLRectangle : public GLOp {
public:
	GLRectangle() {}
	GLRectangle(float x, float y, float w, float h) : mX(x), mY(y), mW(w), mH(h) {}
	virtual void Execute() {
		glBegin(GL_QUADS);
		glVertex2f(mX, mY);
		glVertex2f(mX + mW, mY);
		glVertex2f(mX + mW, mY + mH);
		glVertex2f(mX, mY + mH);
		glEnd();
	}
private:
	float mX, mY, mW, mH;
};


class GLSetFont : public GLOp {
public:
	GLSetFont() {}
	GLSetFont(const char* name, int height, Font::Rendering mode, int glow)
		: mHeight(height), mMode(mode), mGlow(glow), mName(name) { }
	virtual void Execute() {
		ScriptGL::mFont = FontManager::LoadFont(mName.Realign().c_str(), mHeight, mMode, mGlow);
	}
private:
	String2 mName;
	int mHeight, mGlow;
	Font::Rendering mMode;
};

#endif // __GLOPCODES_H__
