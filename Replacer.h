#ifndef __TEXTUREREPLACER_H__
#define __TEXTUREREPLACER_H__

#include "Texture.h"

struct Original {
	bool operator< ( const Original &b ) const {
		return ( mCrc < b.mCrc );
	}

	unsigned int mCrc;
	char *mName;
};

namespace Replacer {
	const String *FindOriginalName( unsigned int texture_crc );
	TextureWithMips *FindReplacement( const String *name );
	void Forget( );
	TextureWithMips *LastMatchedTexture( );
	bool LastScanWasReplaceable( );
	void Open( );
};

#endif // __REPLACER_H__