#include "Console.h"
#include "Patch.h"
#include "Fear.h"
#include "Callback.h"

namespace Loader {

namespace Test {

BuiltInFunction("Sky::setRotation", skySetRot) {
	Fear::Sky *sky = Fear::findSky();
	if (sky) {
		sky->rotation = (f32)atof(argv[0]);
		sky->calcPoints();
	}
	return "true";
}

}; // namespace Test


void Startup() {
	Callback::trigger(Callback::OnStarted, true);
}

#define FEARINSTANCE_INIT_VFT 0x006D802C
#define FEARINSTANCE_UEHANDLER 0x0087F16C
#define SOMETHING_ENDFRAME_VFT 0x006D7FA0

u32 fnFearInstanceInit, fnUEHandler, fnEndFrame;
u32 Crashed = 0;

NAKED void UEHandler( ) {
	__asm {
		mov dword ptr [Crashed], 1
		jmp [fnUEHandler]
	}
}

NAKED void StartupStub() {
	__asm {
		push dword ptr [fnFearInstanceInit]
		pop dword ptr ds:[FEARINSTANCE_INIT_VFT]
		call [fnFearInstanceInit]
		call Startup
		retn
	}
}

void OnEndFrameManaged() {
	Callback::trigger(Callback::OnEndframe, true);
}

NAKED void OnEndFrame() {
	__asm {
		push eax
		call OnEndFrameManaged
		pop eax
		jmp [fnEndFrame]
	}
}

struct Init {
	Init() {
		fnUEHandler = Patch::ReplaceHook((void *)FEARINSTANCE_UEHANDLER, &UEHandler);
		fnFearInstanceInit = Patch::ReplaceHook((void *)FEARINSTANCE_INIT_VFT, &StartupStub);
		fnEndFrame = Patch::ReplaceHook((void *)SOMETHING_ENDFRAME_VFT, &OnEndFrame);
	}
} init;




}; // namespace Loader