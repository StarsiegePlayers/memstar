#include "Memstar.h"
#include "VersionSnoop.h"
#include "Callback.h"

namespace VersionSnoop {

	const int MIN_VERSIONSNOOP_IMAGE_SIZE = 1400000; // 1.4 Mib

	VersionInfo version[4] = {
	{
		0x006C3BA7,       //PTR
		0x61337365,       //Value - "es3a"
		VERSION::v001000  // Version
	},
	{
		0x006c5fdc,       //PTR
		0x2,              //Value
		VERSION::v001002  // Version
	},
	{
		0x006d726c,       //PTR
		0x3,              //Value
		VERSION::v001003  // Version
	},
	{
		0x006e7384,       //PTR
		0x4,              //Value
		VERSION::v001004  // Version
	}
	};

	MODULEINFO info;

	LPMODULEINFO GetExecutingModuleInfo() {
		if (info.SizeOfImage == NULL) {
			HANDLE handle = GetCurrentProcess();
			HMODULE module = GetModuleHandleW(NULL);
			GetModuleInformation(handle, module, &info, sizeof(info));
		}
		
		return &info;
	}

	wchar_t* GetVersionString(VERSION in) {

		if (in == VERSION::vUnknown) {
			return VersionStrings[4];
		}

		return VersionStrings[in];
	}

	VERSION GetVersion() {
		if (Version == VERSION::vUnknown) {
			return versionSnoop();
		}
		return Version;
	}

	VERSION versionSnoop() {
		GetExecutingModuleInfo();
		if (info.SizeOfImage <= MIN_VERSIONSNOOP_IMAGE_SIZE) {
			return VERSION::vNotGame;
		}

		for (int i = 0; i < 4; i++) {
			versionPtr = (DWORD*)version[i].PTR;
			if (*versionPtr == version[i].Value) {
				Version = version[i].Version;
				return Version;
			}
		}
		return VERSION::vUnknown;
	}

	struct Init {
		Init() {
			VERSION v = versionSnoop();
#ifdef _DEBUG
			wchar_t message[100];
			swprintf_s(message, L"Detected Starsisge exectuable version: %s", GetVersionString(v));
			MessageBoxW(NULL, message, L"Detection info", MB_OK | MB_ICONINFORMATION);
#endif
		}
	} Init;
};