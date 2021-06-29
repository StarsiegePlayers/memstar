#ifndef __TERRAIN_H__
#define __TERRAIN_H__


namespace Terrain {
	RGBA** CheckForTerrainTile();
	bool TexImageCheck(void* bmp);
	bool TexSubImageCheck(void* bmp);
}; // namespace Terrain


#endif // __TERRAIN_H__