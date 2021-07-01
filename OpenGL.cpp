#include "OpenGL.h"
#include "OpenGL_Pointers.h"
#include "Callback.h"
#include "Console.h"
#include "Patch.h"

#pragma warning( disable : 4035 ) // suppress "no return value"

namespace Loader {
	extern u32 Crashed;
};

namespace OpenGL {

	BuiltInVariable("pref::forceTrilinear", bool, glForceTrilinear, true);

	// outsourced setting mState to Hooks.cpp
	bool glActive = false;
	void* _wglMakeCurrent,
		* _glMultiTexCoord2fARB,
		* _glTexParameteri,
		* _setAlphaSource;
	u32 _glTexImage2d, _glTexSubImage2d;
	Fear::OpenGLState* mState;

	/*
		Helpers
	*/

	bool CheckBlendFunc(GLenum src, GLenum dst) {
		if (mState->BLEND_SRC == src && mState->BLEND_DST == dst)
			return true;

		mState->BLEND_SRC = src;
		mState->BLEND_DST = dst;
		return false;
	}

	bool CheckCap(GLenum cap, GLboolean state) {
		int want = (state) ? 1 : 0;
		bool already_set = false;

		switch (cap) {
		case GL_TEXTURE_2D: {
			already_set = (mState->TEXTURE_2D0_ARB == want);
			mState->TEXTURE_2D0_ARB = want;
		}  break;

		case GL_ALPHA_TEST: {
			already_set = (mState->ALPHA_TEST == want);
			mState->ALPHA_TEST = want;
		} break;

		case GL_DEPTH_TEST: {
			already_set = (mState->DEPTH_TEST == want);
			mState->DEPTH_TEST = want;
		} break;
		}

		return already_set;
	}

	bool CheckTexEnv(GLint param) {
		if (mState->TEXTURE_ENV0_ARB == param)
			return true;
		mState->TEXTURE_ENV0_ARB = param;
		return false;
	}

	NAKED void setAlphaSource() {
		__asm {
			mov ecx, [eax + 0x158]
			mov[mState], ecx

			cmp dword ptr[esp], 0x47D250 // Planet::onRender
			jne __continue
			mov edx, 4
			__continue:
			jmp[_setAlphaSource]
		}
	}

	NAKED void glTexParamateri() {
		__asm {
			cmp[glForceTrilinear], 0
			je __check_nearest

			mov eax, [esp + 0x8]
			cmp eax, GL_TEXTURE_MAG_FILTER
			je __check_filter
			cmp eax, GL_TEXTURE_MIN_FILTER
			jne __done

			__check_filter :
			// 0x2700 - 0x2703 are ours
			mov eax, [esp + 0xc]
				cmp eax, GL_NEAREST_MIPMAP_NEAREST
				jb __check_nearest
				cmp eax, GL_LINEAR_MIPMAP_LINEAR
				ja __check_nearest

				mov dword ptr[esp + 0xc], GL_LINEAR_MIPMAP_LINEAR

				__check_nearest :
			mov eax, [esp + 0xc]
				cmp eax, GL_NEAREST
				jne __done

				// sky overlays are rendered with GL_NEAREST, let's go linear
				mov dword ptr[esp + 0xc], GL_LINEAR

				__done :
			jmp ds : [_glTexParameteri]
		}
	}

	bool IsActive() {
		return glActive;
	}

	// The second texture unit is left enabled sometimes, this will disable it
	void ShutdownTex1ARB() {
		mState->TEXTURE_2D0_ARB = 0;
		mState->TEXTURE_2D1_ARB = 0;
		mState->TEXTURE_UNIT_ACTIVE = 0;
		if (!mState->_glActiveTextureARB)
			return;

		__asm {
			mov ebx, [mState]
			push dword ptr GL_TEXTURE1_ARB
			call[ebx + 8] // glActiveTextureARB( GL_TEXTURE1_ARB );
			push dword ptr GL_TEXTURE_2D
			mov esi, OpenGLPtrs::ptr_OPENGL32_GLDISABLE
			call ds:[esi] // glDisable( GL_TEXTURE_2D );
			push dword ptr GL_TEXTURE0_ARB
			call[ebx + 8] // glActiveTextureARB( GL_TEXTURE0_ARB );
			push dword ptr GL_TEXTURE_2D
			mov esi, OpenGLPtrs::ptr_OPENGL32_GLDISABLE
			call ds:[esi] // glDisable( GL_TEXTURE_2D );
		}
	}

	BOOL __stdcall wglMakeCurrent(HDC hdc, HGLRC hglrc) {
		glActive = (hglrc != NULL);

		if (!glActive)
			Callback::trigger(Callback::OnOpenGL, false);

		BOOL result;
		__asm {
			push[hglrc]
			push[hdc]
			call[OpenGL::_wglMakeCurrent]
			movzx eax, al
			mov[result], eax
		}

		if (glActive)
			Callback::trigger(Callback::OnOpenGL, true);

		Console::echo("OpenGL is %s", glActive ? "active" : "not active");

		return result; // return must be BOOL(32bit), not GLboolean(8bit)
	}

	void OnStarted(bool active) {
		_wglMakeCurrent = (void*)Patch::ReplaceHook((void*)OpenGLPtrs::ptr_OPENGL32_WGLMAKECURRENT, &wglMakeCurrent);
		_glTexParameteri = (void*)Patch::ReplaceHook((void*)OpenGLPtrs::ptr_OPENGL32_GLTEXPARAMETERI, &glTexParamateri);
		_setAlphaSource = (void*)Patch::ReplaceHook((void*)OpenGLPtrs::ptr_OPENGL_SET_ALPHA_SOURCE, &setAlphaSource);
	}

	struct Init {
		Init() {
			if (VersionSnoop::GetVersion() != VERSION::v001004) {
				return;
			}
			if (VersionSnoop::GetVersion() == VERSION::vNotGame) {
				return;
			}
			Callback::attach(Callback::OnStarted, OnStarted);
		}
	} init;

}; // namespace OpenGL


/*
	OpenGL Interface
*/

void glBegin(GLenum mode) {
	typedef void (GLAPIENTRY* fn)(GLenum mode);

	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLBEGIN)(mode);
}

void glBindTexture(GLenum target, GLuint texture) {
	typedef void (GLAPIENTRY* fn)(GLenum target, GLuint texture);
	int* bound = (int*)(OpenGLPtrs::ptr_OPENGL_BOUND_TEXTURE);
	if (*bound == texture && OpenGL::mState->GL_BOUNDTEXTURE0 == texture)
		return;
	*bound = (texture);
	OpenGL::mState->GL_BOUNDTEXTURE0 = (texture);

	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLBINDTEXTURE)(target, texture);
}

void glBlendFunc(GLenum sfactor, GLenum dfactor) {
	if (OpenGL::CheckBlendFunc(sfactor, dfactor))
		return;

	typedef void (GLAPIENTRY* fn)(GLenum sfactor, GLenum dfactor);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLBLENDFUNC)(sfactor, dfactor);
}

void glCallLists(GLsizei n, GLenum type, const GLvoid* lists) {
	typedef void (GLAPIENTRY* fn)(GLsizei n, GLenum type, const GLvoid* lists);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLCALLLISTS)(n, type, lists);
}

void glCallList(GLuint list) {
	typedef void (GLAPIENTRY* fn)(GLuint a);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLCALLLIST)(list);
}

void glColor3ub(GLubyte red, GLubyte green, GLubyte blue) {
	typedef void (GLAPIENTRY* fn)(GLubyte red, GLubyte green, GLubyte blue);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLCOLOR3UB)(red, green, blue);
}

void glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) {
	typedef void (GLAPIENTRY* fn)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLCOLOR4UB)(red, green, blue, alpha);
}

void glColorMask(GLboolean r, GLboolean g, GLboolean b, GLboolean a) {
	typedef void (GLAPIENTRY* fn)(GLboolean r, GLboolean g, GLboolean b, GLboolean a);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLCOLORMASK)(r, g, b, a);
}

void glDeleteLists(GLuint list, GLsizei range) {
	if (Loader::Crashed)
		return;

	typedef void (GLAPIENTRY* fn)(GLuint list, GLsizei range);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLDELETELISTS)(list, range);
}

void glDeleteTextures(GLsizei n, const GLuint* textures) {
	if (Loader::Crashed)
		return;

	typedef void (GLAPIENTRY* fn)(GLsizei n, const GLuint* textures);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLDELETETEXTURES)(n, textures);
}

void glDepthFunc(GLenum func) {
	typedef void (GLAPIENTRY* fn)(GLenum func);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLDEPTHFUNC)(func);
}

void glDepthMask(GLboolean flag) {
	typedef void (GLAPIENTRY* fn)(GLboolean flag);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLDEPTHMASK)(flag);
}

void glDepthRange(GLclampd znear, GLclampd zfar) {
	typedef void (GLAPIENTRY* fn)(GLclampd znear, GLclampd zfar);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLDEPTHRANGE)(znear, zfar);
}

void glDisable(GLenum cap) {
	if (OpenGL::CheckCap(cap, false))
		return;

	typedef void (GLAPIENTRY* fn)(GLenum cap);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLDISABLE)(cap);
}

void glDrawPixels(GLsizei width, GLsizei height, GLenum format, GLenum pixeltype, const GLvoid* pixels) {
	typedef void (GLAPIENTRY* fn)(GLsizei width, GLsizei height, GLenum format, GLenum pixeltype, const GLvoid* pixels);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLDRAWPIXELS)(width, height, format, pixeltype, pixels);
}

void glEnable(GLenum cap) {
	if (OpenGL::CheckCap(cap, true))
		return;

	typedef void (GLAPIENTRY* fn)(GLenum cap);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLENABLE)(cap);
}

void glEnd() {
	typedef void (GLAPIENTRY* fn)();
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLEND)();
}

void glEndList() {
	typedef void (GLAPIENTRY* fn)();
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLENDLIST)();
}

void glFlush() {
	typedef void (GLAPIENTRY* fn)();
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLFLUSH)();
}

GLenum glGetError() {
	typedef GLenum(GLAPIENTRY* fn)();
	return (*(fn*)OpenGLPtrs::ptr_OPENGL32_GLGETERROR)();
}

GLuint glGenLists(GLsizei range) {
	typedef GLuint(GLAPIENTRY* fn)(GLsizei range);
	return (*(fn*)OpenGLPtrs::ptr_OPENGL32_GLGENLISTS)(range);
}

void glGenTextures(GLsizei n, GLuint* textures) {
	typedef void (GLAPIENTRY* fn)(GLsizei n, GLuint* textures);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLGENTEXTURES)(n, textures);
}

void glGetIntegerv(GLenum pname, GLint* params) {
	typedef void (GLAPIENTRY* fn)(GLenum pname, GLint* params);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLGETINTEGERV)(pname, params);
}

void glGetTexEnviv(GLenum target, GLenum pname, GLint* params) {
	typedef void (GLAPIENTRY* fn)(GLenum target, GLenum pname, GLint* params);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLGETTEXENVIV)(target, pname, params);
}

void glListBase(GLuint base) {
	typedef void (GLAPIENTRY* fn)(GLuint base);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLLISTBASE)(base);
}

void glLoadIdentity() {
	typedef void (GLAPIENTRY* fn)();
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLLOADIDENTITY)();
}

void glPixelMapusv(GLenum map, GLsizei mapsize, const GLushort* values) {
	typedef void (GLAPIENTRY* fn)(GLenum map, GLsizei mapsize, const GLushort* values);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLPIXELMAPUSV)(map, mapsize, values);
}

void glMatrixMode(GLenum mode) {
	typedef void (GLAPIENTRY* fn)(GLenum mode);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLMATRIXMODE)(mode);
}

void glNewList(GLuint list, GLenum mode) {
	typedef void (GLAPIENTRY* fn)(GLuint list, GLenum mode);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLNEWLIST)(list, mode);
}

void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble znear, GLdouble zfar) {
	typedef void (GLAPIENTRY* fn)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble znear, GLdouble zfar);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLORTHO)(left, right, bottom, top, znear, zfar);
}

void glPixelTransferi(GLenum pname, GLint param) {
	typedef void (GLAPIENTRY* fn)(GLenum pname, GLint param);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLPIXELTRANSFERI)(pname, param);
}

void glPopMatrix() {
	typedef void (GLAPIENTRY* fn)();
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLPOPMATRIX)();
}

void glPopAttrib() {
	typedef void (GLAPIENTRY* fn)();
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLPOPATTRIB)();
}

void glPushAttrib(GLenum attrib) {
	typedef void (GLAPIENTRY* fn)(GLenum attrib);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLPUSHATTRIB)(attrib);
}

void glPushMatrix() {
	typedef void (GLAPIENTRY* fn)();
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLPUSHMATRIX)();
}

void glRasterPos2i(GLint x, GLint y) {
	typedef void (GLAPIENTRY* fn)(GLint x, GLint y);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLRASTERPOS2I)(x, y);
}

void glReadBuffer(GLenum mode) {
	typedef void (GLAPIENTRY* fn)(GLenum mode);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLREADBUFFER)(mode);
}

void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum pixeltype, const GLvoid* pixels) {
	typedef void (GLAPIENTRY* fn)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum pixeltype, const GLvoid* pixels);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLREADPIXELS)(x, y, width, height, format, pixeltype, pixels);
}

void glScalef(GLfloat x, GLfloat y, GLfloat z) {
	typedef void (GLAPIENTRY* fn)(GLfloat x, GLfloat y, GLfloat z);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLSCALEF)(x, y, z);
}

void glScissor(GLint x, GLint y, GLsizei width, GLsizei height) {
	typedef void (GLAPIENTRY* fn)(GLint x, GLint y, GLsizei width, GLsizei height);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLSCISSOR)(x, y, width, height);
}

void glTexCoord2f(GLfloat s, GLfloat t) {
	typedef void (GLAPIENTRY* fn)(GLfloat s, GLfloat t);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLTEXCOORD2F)(s, t);
}

void glTexCoord2fv(Vector2f* v) {
	typedef void (GLAPIENTRY* fn)(Vector2f* v);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLTEXCOORD2FV)(v);
}

void glTexEnvi(GLenum target, GLenum pname, GLint param) {
	if (OpenGL::CheckTexEnv(param))
		return;

	typedef void (GLAPIENTRY* fn)(GLenum target, GLenum pname, GLint param);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLTEXENVI)(target, pname, param);
}

void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels) {
	typedef void (GLAPIENTRY* fn)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLTEXIMAGE2D)(target, level, internalformat, width, height, border, format, type, pixels);
}

void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels) {
	typedef void (GLAPIENTRY* fn)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLTEXSUBIMAGE2D)(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

void glTexParameteri(GLenum target, GLenum pname, GLint param) {
	typedef void (GLAPIENTRY* fn)(GLenum target, GLenum pname, GLint param);
	//(*( fn *)OPENGL32_GLTEXPARAMETERI)( target, pname, param );
	((fn)OpenGL::_glTexParameteri)(target, pname, param); // call it directly so we bypass our hook
}

void glTranslatef(GLfloat x, GLfloat y, GLfloat z) {
	typedef void (GLAPIENTRY* fn)(GLfloat x, GLfloat y, GLfloat z);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLTRANSLATEF)(x, y, z);
}

void glVertex2f(GLfloat x, GLfloat y) {
	typedef void (GLAPIENTRY* fn)(GLfloat x, GLfloat y);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLVERTEX2F)(x, y);
}

void glVertex2fv(Vector2f* v) {
	typedef void (GLAPIENTRY* fn)(Vector2f* v);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLVERTEX2FV)(v);
}

void glVertex2i(GLint x, GLint y) {
	typedef void (GLAPIENTRY* fn)(GLint x, GLint y);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLVERTEX2I)(x, y);
}

void glVertex3f(GLfloat x, GLfloat y, GLfloat z) {
	typedef void (GLAPIENTRY* fn)(GLfloat x, GLfloat y, GLfloat z);
	(*(fn*)OpenGLPtrs::ptr_OPENGL32_GLVERTEX3F)(x, y, z);
}

#pragma warning( default : 4035 )
