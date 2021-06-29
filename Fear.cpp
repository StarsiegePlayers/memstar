#include "Fear.h"
#include "Strings.h"

namespace Fear {
#define SIM_PTR 0x0071CF5C
#define SIMSKY_VFT 0x0071265C
#define SIMSET_VFT 0x0071492C
#define SIMCANVAS_PTR 0x0077FAE8

	Sim* Sim::Client() {
		return *(Sim**)(SIM_PTR);
	}

	template<class T>
	T* Sim::findObject(const char* nameOrId) {
		if (!this)
			return NULL;

		__asm {
			mov eax, this
			push eax
			mov edx, [nameOrId]
			mov ecx, [eax]
			call[ecx + 0x6c]
			pop this
		}
	}

	Sky* findSky() {
		SimSet* render_set = Sim::Client()->findObject<SimSet>(13);
		if (render_set->vft == SIMSET_VFT) {
			for (SimSet::Iterator iter = render_set->Begin(); iter != render_set->End(); ++iter)
				if ((*iter)->vft == SIMSKY_VFT)
					return (Sky*)*iter;
		}
		return NULL;
	}

	PlayerPSC* findPSC() {
		return Sim::Client()->findObject<PlayerPSC>(636);
	}

	f32 Sim::getTime() {
		return (this) ? Time : 0;
	}

	static const u32 fnSkyCalcPoints = 0x005855E0;

	void Sky::calcPoints() {
		__asm {
			mov eax, this
			push eax
			call[fnSkyCalcPoints]
			pop this
		}
	}

	bool getScreenDimensions(Vector2i* dim) {
		__asm {
			// SimGui::Canvas
			mov eax, ds: [SIMCANVAS_PTR]
			and eax, eax
			jz done

			// GFXDevice::?
			mov ebx, [eax + 0x144]
			xor eax, eax
			and ebx, ebx
			jz done

			// GFX::Surface
			mov eax, [ebx + 0x3c]

			mov edx, [dim]
			mov ecx, [eax + 0x3c]
			mov[edx + 0], ecx
			mov ecx, [eax + 0x40]
			mov[edx + 4], ecx
			mov eax, 1
			done:
		}
	}

}; // namespace Fear