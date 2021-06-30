#ifndef __VERSIONSNOOP_H__
#define __VERSIONSNOOP_H__

#include "Memstar.h"

const enum VERSION {
	vNotGame = -2,
	vUnknown = -1,
	v001000 = 0,
	v001002 = 1,
	v001003 = 2,
	v001004 = 3
};

static wchar_t* VersionStrings[5] = {
	L"V001.000R",
	L"V001.002R",
	L"V001.003R",
	L"V001.004R",
	L"Unknown",
};

namespace VersionSnoop {

	struct VersionInfo {
		u32 PTR;
		DWORD Value;
		VERSION Version;
	};

	VERSION GetVersion();
	VERSION versionSnoop();
	wchar_t* GetVersionString(VERSION in);

	static DWORD* versionPtr;
	static VERSION Version = VERSION::vUnknown;

};

#endif
