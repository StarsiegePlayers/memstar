#include "Callback.h"
#include "Patch.h"
#include "Fear.h"
#include "OpenGL.h"
#include "Console.h"

namespace VersionSnoop {
	extern DWORD* versionPtr;
};

namespace GUI {

MultiPointer(ptr_SIMCANVASSET_ONRENDER_VFT, 0, 0, 0x00704670, 0x00714AE0);
MultiPointer(ptr_SIMCANVAS_RENDER, 0, 0, 0x005c9a50, 0x005CD2F4);
MultiPointer(ptr_SIMCANVAS_RENDERGUI_CALL, 0, 0, 0x005d5db4, 0x005D9658);

	CodePatch cpDrawGUI = { ptr_SIMCANVAS_RENDERGUI_CALL, "\xe8\x97\x3c\xff\xff", "\xe8rgui", 5, false };

	u32 fnOnRender, fnRenderGui = ptr_SIMCANVAS_RENDER;
	Fear::GraphicsAdapter* gfxAdapter;

	void GatherInformation() {
	}

	void OnPlayGUIPreDraw(Fear::GraphicsAdapter* gfx) {
		if (OpenGL::IsActive()) {
			OpenGL::ShutdownTex1ARB();
			glDisable(GL_CULL_FACE);
			Callback::trigger(Callback::OnGuiDraw, true);
		}
	}

	void OnPlayGUIPostDraw(Fear::GraphicsAdapter* gfx) {
		if (OpenGL::IsActive())
			Callback::trigger(Callback::OnGuiDraw, false);
	}

	NAKED void OnRender() {
		__asm {
			pushad
			call GatherInformation
			popad
			jmp[fnOnRender]
		}
	}

	NAKED void OnPlayGUI() {
		__asm {
			mov[gfxAdapter], edx

			pushad
			push[gfxAdapter]
			call OnPlayGUIPreDraw
			pop[gfxAdapter]
			popad

			push[esp + 8]
			push[esp + 8]
			call[fnRenderGui]

			pushad
			push[gfxAdapter]
			call OnPlayGUIPostDraw
			pop[gfxAdapter]
			popad

			retn 8
		}
	}

	struct Init {
		Init() {
			if (VersionSnoop::GetVersion() == VERSION::vNotGame) {
				return;
			}
			if (VersionSnoop::GetVersion() != VERSION::v001004) {
				return;
			}

			cpDrawGUI.DoctorRelative((u32)OnPlayGUI, 1).Apply(true);
			fnOnRender = Patch::ReplaceHook((void*)ptr_SIMCANVASSET_ONRENDER_VFT, OnRender);
		}
	} Init;

}; // namespace GUI