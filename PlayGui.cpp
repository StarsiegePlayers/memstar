#include "Callback.h"
#include "Patch.h"
#include "Fear.h"
#include "OpenGL.h"
#include "Console.h"

namespace GUI {

#define SIMCANVASSET_ONRENDER_VFT 0x00714AE0
#define SIMCANVAS_RENDER 0x005CD2F4
#define SIMCANVAS_RENDERGUI_CALL 0x005D9658

	CodePatch cpDrawGUI = { SIMCANVAS_RENDERGUI_CALL, "\xe8\x97\x3c\xff\xff", "\xe8\xfb\xe2\x0b\x00", 5, false };

	u32 fnOnRender, fnRenderGui = SIMCANVAS_RENDER;
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
			cpDrawGUI.DoctorRelative((u32)OnPlayGUI, 1);
			cpDrawGUI.Apply(true);

			fnOnRender = Patch::ReplaceHook((void*)SIMCANVASSET_ONRENDER_VFT, OnRender);
		}
	} init;

}; // namespace GUI