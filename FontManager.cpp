#include "FontManager.h"

#include "HashTable.h"
#include "FileSystem.h"
#include "OpenGL.h"
#include "Callback.h"
#include "Console.h"

namespace FontManager {

	struct Key {
		Key() { }
		Key(const char* name, int pixel_height, Font::Rendering mode, int glow_radius) :
			mPixelHeight(pixel_height), mGlowRadius(glow_radius), mMode(mode) {

			strncpy(mName, name, 63);
			mName[63] = '\x0';
		}

		bool operator== (const Key& b) const {
			return ((mPixelHeight == b.mPixelHeight) &&
				(mGlowRadius == b.mGlowRadius) &&
				(mMode == b.mMode) &&
				(strcmp(mName, b.mName) == 0));
		}

		char mName[64];
		int mPixelHeight, mGlowRadius;
		Font::Rendering mMode;
	};

	struct KeyUtil {
		unsigned int operator() (const Key& a) const { return (HashBytes((const unsigned char*)&a, sizeof(a))); }
		bool operator() (const Key& a, const Key& b) const { return (a == b); }
	};

	typedef HashTable<
		Key, Font*,
		KeyUtil,
		ValueDeleter<Key, Font*>,
		KeyUtil
	> Hash;

	typedef Hash::Iterator Iterator;


	FileSystem mFiles(32);
	Hash mFonts(32);
	char mBuffer[256];
	int mLastGarbageCollect = (GetTickCount());

	void Close() {
		FileSystem::Iterator iter = mFiles.Begin(), end = mFiles.End();
		while (iter != end) {
			if (stristr(iter.value()->Path(), ".ttf"))
				RemoveFontResource(iter.value()->Path()); // unregister from windows
			++iter;
		}

		mFiles.Clear();
		mFonts.Clear();
	}

	/*
		Check for unused fonts every 5 seconds
	*/
	void GarbageCollect(bool endframe) {
		int ticks = (GetTickCount());
		if (ticks - mLastGarbageCollect < 5000)
			return;
		mLastGarbageCollect = (ticks);

		Iterator iter = mFonts.Begin(), end = mFonts.End();
		while (iter != end) {
			// dont gc the last font that was set
			if (ticks - iter.value()->LastUsed() > 30000) {
				Console::echo("FontManager: Garbage collecting %s", iter.key().mName);

				mFonts.Delete(iter);
			}
			else {
				++iter;
			}
		}
	}

	Font* LoadFont(const char* name, int pixel_height, Font::Rendering mode, int glow_radius) {
		Key id(name, pixel_height, mode, glow_radius);
		Font** find = (mFonts.Find(id)), * font = (NULL);
		if (!find) {
			font = new Font();
			if (!font || !font->Create(name, pixel_height, mode, glow_radius) || !mFonts.Insert(id, font)) {
				delete font;
				font = (NULL);
			}
		}
		else {
			font = (*find);
		}

		if (font)
			font->UpdateLastUsed();

		return (font);
	}

	void Open() {
		Close();

		mFiles.GrokNonZips("memstar/Fonts");

		FileSystem::Iterator iter = mFiles.Begin(), end = mFiles.End();
		while (iter != end) {
			// register the custom fonts with windows
			if (stristr(iter.value()->Path(), ".ttf"))
				AddFontResource(iter.value()->Path());
			++iter;
		}
	}



	struct Init {
		Init() {
			Callback::attach(Callback::OnEndframe, GarbageCollect);
			Open();
		}

		~Init() {
			Close();
		}
	} init;


}; // namespace FontManager