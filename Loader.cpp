#include "Console.h"
#include "Patch.h"
#include "Fear.h"
#include "Callback.h"
#include "MultiPointer.h"

namespace Loader {

	namespace Test {

		BuiltInFunction("Sky::setRotation", skySetRot) {
			Fear::Sky* sky = Fear::findSky();
			if (sky) {
				sky->rotation = (f32)atof(argv[0]);
				sky->calcPoints();
			}
			return "true";
		}

	}; // namespace Test

	u32 fnFearInstanceInit, fnUEHandler, fnEndFrame;
	u32 Crashed = 0;

	MultiPointer(ptrFearInstanceUEHandler, 0, 0, 0x0085e084, 0x0087F16C);
	NAKED void UEHandler() {
		__asm {
			mov dword ptr[Crashed], 1
			jmp[fnUEHandler]
		}
	}

	MultiPointer(ptrFearInstanceInitVFT, 0, 0, 0x006c8000, 0x006D802C);
	void Startup() {
		Callback::trigger(Callback::OnStarted, true);
	}
	NAKED void StartupStub() {
		__asm {
			push dword ptr[fnFearInstanceInit]
			mov esi, ptrFearInstanceInitVFT
			pop dword ptr ds:[esi]
			call[fnFearInstanceInit]
			call Startup
			retn
		}
	}

	MultiPointer(ptrEndFrameVFT, 0, 0, 0x006c7f74, 0x006D7FA0);
	void OnEndFrameManaged() {
		Callback::trigger(Callback::OnEndframe, true);
	}
	NAKED void OnEndFrame() {
		__asm {
			push eax
			call OnEndFrameManaged
			pop eax
			jmp[fnEndFrame]
		}
	}

	struct Init {
		Init() {
			if (VersionSnoop::GetVersion() == VERSION::vNotGame) {
				return;
			}
			if (VersionSnoop::GetVersion() < VERSION::v001003) {
				return;
			}
			fnUEHandler = Patch::ReplaceHook((void*)ptrFearInstanceUEHandler, &UEHandler);
			fnFearInstanceInit = Patch::ReplaceHook((void*)ptrFearInstanceInitVFT, &StartupStub);
			fnEndFrame = Patch::ReplaceHook((void*)ptrEndFrameVFT, &OnEndFrame);
		}
	} Init;




}; // namespace Loader