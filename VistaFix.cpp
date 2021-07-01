#include "Memstar.h"
#include "Patch.h"
#include "MultiPointer.h"

namespace VistaFix {

	MultiPointer(ptrWindowsHookExA, 0x0084ca28, 0x0084ea30, 0x00864a48, 0x00885a48);
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
			if (VersionSnoop::GetVersion() == VERSION::vNotGame) {
				return;
			}
			Patch::Protect((u32*)ptrWindowsHookExA, 256, PAGE_READWRITE);
			Patch::ReplaceHook((u32*)ptrWindowsHookExA, &SetWindowsHookExAStub);
		}
	} VistaFixInit;



}; // namespace VistaFix
