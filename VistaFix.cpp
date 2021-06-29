#include "Memstar.h"

namespace VistaFix {

	NAKED void SetWindowsHookExAStub() {
		__asm {
			call ds : [GetCurrentThreadId]
			push eax
			push[esp + 0x10]
			push[esp + 0x10]
			push[esp + 0x10]
			call ds : [SetWindowsHookExA]
			retn 0x10
		}
	}

	struct Init {
		Init() {
			u32 old_protect, hook_proc;
			for (hook_proc = 0x84ca28; hook_proc <= 0x885A48; hook_proc += 4)
				if (*(u32*)hook_proc == (u32)&SetWindowsHookExA)
					break;
			if (*(u32*)hook_proc == (u32)&SetWindowsHookExA) {
				VirtualProtect((LPVOID)hook_proc, 256, PAGE_READWRITE, (PDWORD)&old_protect);
				*(u32*)hook_proc = (u32)&SetWindowsHookExAStub;
			}
		}
	} VistaFixInit;

}; // namespace VistaFix