#ifndef __SCRIPTGL_H__
#define __SCRIPTGL_H__

#include "Texture.h"
#include "Font.h"

namespace ScriptGL {
	Texture *FindTexture( const char *name );
	void OnPlayGUIPreDraw( );
	void OnPlayGUIPostDraw( );

	extern Font *mFont;
	extern int mBeginCount;
};

#endif // __SCRIPTGL_H__