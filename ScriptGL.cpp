#include "ScriptGL.h"
#include "FileSystem.h"
#include "FontManager.h"
#include "HashTable.h"
#include "OpenGL.h"
#include "Callback.h"
#include "GLOpcodes.h"
#include "Console.h"

class ScriptTexture : public Texture {
public:
	ScriptTexture() : Texture(), mLastUsed(GetTickCount()) { }
	~ScriptTexture() { }

	int LastUsed() const { return (mLastUsed); }
	void UpdateLastUsed() { mLastUsed = GetTickCount(); }

private:
	int mLastUsed;
};


namespace ScriptGL {

	// types
	typedef HashTable<
		String, ScriptTexture*,
		IKeyCmp,
		ValueDeleter<String, ScriptTexture*>
	> ScriptTextureHash;

	typedef ScriptTextureHash::Iterator TextureIterator;

#define __SCRIPTGL_PREDRAW__ ( 0 )
#define __SCRIPTGL_POSTDRAW__ ( 1 )

	// glEnable / Disable
	BuiltInVariable("GL_TEXTURE_2D", int, _GL_TEXTURE_2D, GL_TEXTURE_2D);

	// glTexEnv
	BuiltInVariable("GL_DECAL", int, _GL_DECAL, GL_DECAL);
	BuiltInVariable("GL_MODULATE", int, _GL_MODULATE, GL_MODULATE);
	BuiltInVariable("GL_REPLACE", int, _GL_REPLACE, GL_REPLACE);

	// glBlendFunc
	BuiltInVariable("GL_ZERO", int, _GL_ZERO, GL_ZERO);
	BuiltInVariable("GL_ONE", int, _GL_ONE, GL_ONE);

	BuiltInVariable("GL_SRC_COLOR", int, _GL_SRC_COLOR, GL_SRC_COLOR);
	BuiltInVariable("GL_ONE_MINUS_SRC_COLOR", int, _GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	BuiltInVariable("GL_SRC_ALPHA", int, _GL_SRC_ALPHA, GL_SRC_ALPHA);
	BuiltInVariable("GL_ONE_MINUS_SRC_ALPHA", int, _GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	BuiltInVariable("GL_SRC_ALPHA_SATURATE", int, _GL_SRC_ALPHA_SATURATE, GL_SRC_ALPHA_SATURATE);

	BuiltInVariable("GL_DST_COLOR", int, _GL_DST_COLOR, GL_DST_COLOR);
	BuiltInVariable("GL_ONE_MINUS_DST_COLOR", int, _GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_DST_COLOR);
	BuiltInVariable("GL_DST_ALPHA", int, _GL_DST_ALPHA, GL_DST_ALPHA);
	BuiltInVariable("GL_ONE_MINUS_DST_ALPHA", int, _GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);

	// glBegin
	BuiltInVariable("GL_POINTS", int, _GL_POINTS, GL_POINTS);
	BuiltInVariable("GL_LINES", int, _GL_LINES, GL_LINES);
	BuiltInVariable("GL_LINE_LOOP", int, _GL_LINE_LOOP, GL_LINE_LOOP);
	BuiltInVariable("GL_LINE_STRIP", int, _GL_LINE_STRIP, GL_LINE_STRIP);
	BuiltInVariable("GL_TRIANGLES", int, _GL_TRIANGLES, GL_TRIANGLES);
	BuiltInVariable("GL_TRIANGLE_STRIP", int, _GL_TRIANGLE_STRIP, GL_TRIANGLE_STRIP);
	BuiltInVariable("GL_TRIANGLE_FAN", int, _GL_TRIANGLE_FAN, GL_TRIANGLE_FAN);
	BuiltInVariable("GL_QUADS", int, _GL_QUADS, GL_QUADS);
	BuiltInVariable("GL_QUAD_STRIP", int, _GL_QUAD_STRIP, GL_QUAD_STRIP);
	BuiltInVariable("GL_POLYGON", int, _GL_POLYGON, GL_POLYGON);

	// glDrawTexture
	BuiltInVariable("GLEX_DRAW", int, _GLEX_DRAW, GLEX_DRAW);
	BuiltInVariable("GLEX_CENTERED", int, _GLEX_CENTERED, GLEX_CENTERED);
	BuiltInVariable("GLEX_ROTATED", int, _GLEX_ROTATED, GLEX_ROTATED);
	BuiltInVariable("GLEX_SCALED", int, _GLEX_SCALED, GLEX_SCALED);

	// glSetFont
	BuiltInVariable("GLEX_PIXEL", int, _GLEX_FONT_PIXEL, GLEX_FONT_PIXEL);
	BuiltInVariable("GLEX_SMOOTH", int, _GLEX_FONT_SMOOTH, GLEX_FONT_SMOOTH);


	BuiltInVariable("ScriptGL::Latency", int, mLatency, 100);

	bool mCanDraw = false;
	int mLastGarbageCollect = 0;
	int mBeginCount = 0;
	int mLastGLDraw = 0;
	ScriptTextureHash mScriptTextures(32);
	FileSystem mFiles(32);
	Font* mFont = (NULL);
	GLCompiler mCompilers[2];
	GLCompiler* mCompiler;


	bool Allowed() {
		return (mCanDraw);
	}

	void Close() {
		mScriptTextures.Clear();
		mFiles.Clear();
	}

	void DefaultSettings() {
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture* FindTexture(const char* name) {
		String2 key(name);

		ScriptTexture** find = mScriptTextures.Find(key);
		if (find) {
			(*find)->UpdateLastUsed();
			return (*find);
		}

		ScriptTexture* tmp = new ScriptTexture();
		if (!tmp || !mFiles.ReadTGA(key, *tmp) || !mScriptTextures.Insert(key, tmp)) {
			delete tmp;
			tmp = (NULL);
		}

		return (tmp);
	}

	void GarbageCollect(bool active) {
		mFont = (NULL);

		int ticks = (GetTickCount());
		if (ticks - mLastGarbageCollect < 5000)
			return;
		mLastGarbageCollect = ticks;

		TextureIterator iter = mScriptTextures.Begin(), end = mScriptTextures.End();
		while (iter != end) {
			if (ticks - iter.value()->LastUsed() > 30000) {
				Console::echo("ScriptGL: Garbage collecting %s", iter.key().c_str());
				mScriptTextures.Delete(iter);
			}
			else {
				++iter;
			}
		}
	}

	void Open() {
		Close();
		mFiles.Grok("Memstar/ScriptGL");
	}

	void OnDraw(int pre_or_post) {
		mCanDraw = (OpenGL::IsActive());

		if (!Allowed())
			return;

		mCompiler = (&mCompilers[pre_or_post]);

		int ticks = (GetTickCount());
		if (ticks - mLastGLDraw >= mLatency) {
			char* function = (pre_or_post) ?
				"ScriptGL::playGUI::onPostDraw" : "ScriptGL::playGUI::onPreDraw";

			Vector2i screen;
			Fear::getScreenDimensions(&screen);
			String2 dim;
			dim.Assign("%d %d", screen.x, screen.y);

			mCompiler->Clear();

			if (Console::functionExists(function))
				Console::execFunction(1, function, dim.c_str());

			if (pre_or_post == __SCRIPTGL_POSTDRAW__)
				mLastGLDraw = (ticks);
		}

		DefaultSettings();
		mCompiler->Execute();

		mCanDraw = false;
	}

	BuiltInFunction("glBegin", _glBegin) {
		if (!Allowed())
			return "false";

		if (argc != 1)
			Console::echo("%s( <cap> )", self);
		else
			mCompiler->Push(GLBegin((GLenum)atoi(argv[0])));

		return "true";
	}

	BuiltInFunction("glBindTexture", _glBindTexture) {
		if (!Allowed())
			return "false";

		if (argc != 1)
			Console::echo("%s( <file.tga> )", self);
		else
			mCompiler->Push(GLBindTexture(argv[0]));

		return "true";
	}

	BuiltInFunction("glBlendFunc", _glBlendFunc) {
		if (!Allowed())
			return "false";

		if (argc != 2)
			Console::echo("%s( <sFactor>, <dFactor> )", self);
		else
			mCompiler->Push(GLBlendFunc((GLenum)atoi(argv[0]), (GLenum)atoi(argv[1])));

		return "true";
	}

	BuiltInFunction("glColor4ub", _glColor4ub) {
		if (!Allowed())
			return "false";

		if (argc != 4) {
			Console::echo("%s( <r>, <g>, <b>, <a> ), Values in [0-255]", self);
		}
		else {
			GLubyte c[4];
			for (int i = 0; i < 4; i++)
				c[i] = atoi(argv[i]);
			mCompiler->Push(GLColor4ub(c[0], c[1], c[2], c[3]));
		}

		return "true";
	}

	BuiltInFunction("glDisable", _glDisable) {
		if (!Allowed())
			return "false";

		if (argc < 1)
			Console::echo("%s( <cap> )", self);
		else
			mCompiler->Push(GLDisable((GLenum)atoi(argv[0])));

		return "true";
	}

	BuiltInFunction("glEnable", _glEnable) {
		return "true";
	}

	BuiltInFunction("glEnd", _glEnd) {
		if (!Allowed())
			return "false";

		mCompiler->Push(GLEnd());

		return "true";
	}

	BuiltInFunction("glTexCoord2f", _glTexCoord2f) {
		if (!Allowed())
			return "false";

		if (argc != 2)
			Console::echo("%s( <u>, <v> )", self);
		else
			mCompiler->Push(GLTexCoord2f(fromstring<f32>(argv[0]), fromstring<f32>(argv[1])));

		return "true";
	}

	BuiltInFunction("glTexEnvi", _glTexEnvi) {
		if (!Allowed())
			return "false";

		if (argc != 1)
			Console::echo("%s( <mode> )", self);
		else
			mCompiler->Push(GLTexEnvi(atoi(argv[0])));

		return "true";
	}

	BuiltInFunction("glVertex2i", _glVertex2i) {
		if (!Allowed())
			return "false";

		if (argc != 2)
			Console::echo("%s( <x>, <y> )", self);
		else
			mCompiler->Push(GLVertex2i((GLint)atoi(argv[0]), (GLint)atoi(argv[1])));

		return "true";
	}

	/*
		Pseudo-GL functions
	*/

	BuiltInFunction("glGetTextureDimensions", _glGetTextureDimensions) {
		if (argc != 1) {
			Console::echo("%s( <file.tga> )", self);
		}
		else {
			Texture* find = (FindTexture(argv[0]));
			if (find) {
				static char dim[256];
				Snprintf(dim, 255, "%d %d", find->mWidth, find->mHeight);
				dim[255] = 0;
				return dim;
			}
		}

		return "0 0";
	}

	BuiltInFunction("glDrawString", _glDrawString) {
		if (!Allowed())
			return "false";

		if (argc != 3)
			Console::echo("%s( <x>, <y>, <string> )", self);
		else
			mCompiler->Push(GLDrawString(atoi(argv[0]), atoi(argv[1]), argv[2]));

		return "true";
	}

	BuiltInFunction("glDrawTexture", _glDrawTexture) {
		if (!Allowed())
			return "false";

		if (argc < 4) {
			Console::echo("%s( <file.tga>, $GLEX_(CENTERED/DRAW), <x>, <y> )", self);
			Console::echo("%s( <file.tga>, $GLEX_SCALED, <x>, <y>, <scale_x>, <scale_y> )", self);
			Console::echo("%s( <file.tga>, $GLEX_ROTATED, <x>, <y>, <scale_x>, <scale_y>, <radians> )", self);
		}
		else {
			int mode = (atoi(argv[1]));
			float x = 0, y = 0, sx = 1, sy = 1, r = 0;

			x = fromstring<f32>(argv[2]);
			y = fromstring<f32>(argv[3]);

			if (argc >= 6) {
				sx = fromstring<f32>(argv[4]);
				sy = fromstring<f32>(argv[5]);
			}

			if (argc >= 7)
				r = fromstring<f32>(argv[6]);

			mCompiler->Push(GLDrawTexture(argv[0], mode, x, y, sx, sy, r));
		}

		return "true";
	}

	BuiltInFunction("glGetStringDimensions", _glGetStringDimensions) {
		Vector2i dimensions(0, 0);

		if (argc != 1) {
			Console::echo("%s( <string> )", self);
		}
		else {
			if (mFont)
				dimensions = mFont->StringDimensions(argv[0]);
		}

		return tostring(dimensions);
	}

	BuiltInFunction("glRectangle", _glRectangle) {
		if (!Allowed())
			return "false";

		if (argc != 4)
			Console::echo("%s( <x>, <y>, <width>, <height> )", self);
		else
			mCompiler->Push(GLRectangle(
				fromstring<f32>(argv[0]),
				fromstring<f32>(argv[1]),
				fromstring<f32>(argv[2]),
				fromstring<f32>(argv[3])));

		return "true";
	}

	BuiltInFunction("glRescanTextureDirectory", _glRescanTextureDirectory) {
		Open();
		return "true";
	}

	BuiltInFunction("glSetFont", _glSetFont) {
		if (argc < 1) {
			Console::echo("%s( <font name>, [pixel height], $GLEX_(PIXELS/MOOTH), [glow radius] )", self);
		}
		else {
			Font::Rendering mode = (Font::Smooth);
			int pixel_height = 16;
			int glow_radius = 0;

			if (argc >= 2)
				pixel_height = (atoi(argv[1]));

			if (argc >= 3)
				mode = (Font::Rendering)(atoi(argv[2]));

			if (argc >= 4)
				glow_radius = (atoi(argv[3]));

			pixel_height = (pixel_height <= 6) ? 6 : (pixel_height > 1024) ? 1024 : pixel_height;
			glow_radius = (glow_radius < 2) ? 0 : (glow_radius > 16) ? 16 : glow_radius;
			if (mode != Font::Pixel && mode != Font::Smooth)
				mode = (Font::Pixel);

			// need to execute this now for glGetStringDimension calls
			GLSetFont setfont(argv[0], pixel_height, mode, glow_radius);
			setfont.Execute();

			if (Allowed())
				mCompiler->Push(setfont);
		}

		return "true";
	}

	void OnGuiDraw(bool is_predraw) {
		OnDraw((is_predraw) ? __SCRIPTGL_PREDRAW__ : __SCRIPTGL_POSTDRAW__);
	}

	struct Init {
		Init() {
			Callback::attach(Callback::OnEndframe, GarbageCollect);
			Callback::attach(Callback::OnGuiDraw, OnGuiDraw);
			Open();
		}

		~Init() {
			Close();
			mFont = (NULL);
		}
	} init;

}; // namespace ScriptGL