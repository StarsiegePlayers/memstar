#ifndef __PATCH_H__
#define __PATCH_H__

#include "Memstar.h"

namespace Patch {

INLINE u32 Protect(void *base, u32 size, u32 protection) {
	if ((int)base <= 0) {
		return 0;
	}
	DWORD old_protect;
	VirtualProtect(base, size, protection, &old_protect);
	return (u32)old_protect;
}

INLINE void Write(void *base, void *bytes, u32 size) {
	if ((int)base <= 0) {
		return;
	}
	Protect(base, size, PAGE_EXECUTE_READWRITE);
	memcpy(base, bytes, size);
}

INLINE u32 ReplaceHook(void *address, void *new_hook) {
	if ((int)address <= 0) {
		return 0;
	}
	u32 tmp = *(u32 *)address;
	*(u32 *)address = (u32)new_hook;
	return tmp;
}

}; // namespace Patch


struct CodePatch {
	void Apply(bool state) {
		if ((int)location <= 0) {
			return;
		}
		Patch::Write((void *)location, (state) ? patched_bytes : unpatched_bytes, size);
	}

	// alter an int at patched_byttes[ patch_offset ]
	CodePatch &DoctorInt(u32 value, u32 patch_offset) {
		Patch::Protect(patched_bytes, size, PAGE_EXECUTE_READWRITE);
		memcpy(patched_bytes + patch_offset, &value, 4);
		return *this;
	}

	// alter a value relative to location
	CodePatch &DoctorRelative(u32 value, s32 patch_offset) {
		return DoctorInt(value + -(location + patch_offset + 4), patch_offset);
	}

public:
	s32 location;
	char *unpatched_bytes;
	char *patched_bytes;
	u32 size;
	bool patched;
};

#endif // __PATCH_H__