#ifndef __FONTMANAGER_H__
#define __FONTMANAGER_H__

#include "Font.h"

namespace FontManager {
	Font* LoadFont(const char* name, int pixel_height, Font::Rendering mode, int glow_radius);
};

#endif // __FONTMANAGER_H__