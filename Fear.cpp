#include "Fear.h"
#include "Strings.h"
#include "MultiPointer.h"

namespace Fear {

	MultiPointer(ptr_SIM_PTR, 0, 0, 0x0070caec, 0x0071CF5C);
	MultiPointer(ptr_SIMSKY_VFT, 0, 0, 0x007021ec, 0x0071265C);
	MultiPointer(ptr_SIMSET_VFT, 0, 0, 0x0070452c, 0x0071492C);
	MultiPointer(ptr_SIMCANVAS_PTR, 0, 0, 0x0076f4d0, 0x0077FAE8);

	Sim* Sim::Client() {
		return *(Sim**)(ptr_SIM_PTR);
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
		if (render_set->vft == ptr_SIMSET_VFT) {
			for (SimSet::Iterator iter = render_set->Begin(); iter != render_set->End(); ++iter)
				if ((*iter)->vft == ptr_SIMSKY_VFT)
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
			mov esi, ptr_SIMCANVAS_PTR
			mov eax, ds:[esi]
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