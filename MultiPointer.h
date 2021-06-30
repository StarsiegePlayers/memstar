#ifndef __MULTIPOINTER_H__
#define __MULTIPOINTER_H__

#include "Memstar.h"
#include "VersionSnoop.h"

struct MultiPtr {

	u32 Pointers[4];

	MultiPtr(u32 v0, u32 v2, u32 v3, u32 v4) {
		Pointers[v001000] = v0;
		Pointers[v001002] = v2;
		Pointers[v001003] = v3;
		Pointers[v001004] = v4;
	}

	u32 Get() {
		if (ResolvedPointer == vUnknown) {
			VERSION ver = VersionSnoop::GetVersion();
			if (ver <= v001004 && ver >= v001000) {
				ResolvedPointer = Pointers[ver];
			}
		}
		return ResolvedPointer;
	}

private:
	u32 ResolvedPointer = vUnknown;

};

#define MultiPointer(__name__, __val1__, __val2__, __val3__, __val4__)           \
MultiPtr mptr_##__name__ = { __val1__, __val2__, __val3__, __val4__ };           \
static const u32 __name__ = mptr_##__name__##.Get()



#endif