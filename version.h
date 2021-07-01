#pragma once
#ifndef __VERSION_H__
#define __VERSION_H__

#include <winver.h>

/**
* Windows Version Info Flags
*/
#ifndef VER_PRERELEASE_BUILD
#define VER_PRERELEASE_BUILD        0
#endif

#ifndef VER_PATCHED_BUILD
#define VER_PATCHED_BUILD           0
#endif

#ifndef VER_PRIVATE_BUILD
#define VER_PRIVATE_BUILD           0
#endif

#ifndef VER_SPECIAL_BUILD
#define VER_SPECIAL_BUILD           0
#endif

//#define VER_SPECIAL_STR             ""
//#define VER_PRIVATE_STR             ""
#define VER_COMMENT_STR             "StarsiegePlayers Build on 2021-06-30"

// Required Information
#define VER_FILEVERSION             1,1,0,0
#define VER_FILEVERSIONSTR          1.1.0.0

#define VER_PRODUCTVERSION          1,1,0,0
#define VER_PRODUCTVERSIONSTR       1.1.0.0

#define VER_COMPANYNAME_STR         "Starsiege Players Community"
#define VER_PRODUCTNAME_STR         "mem.dll replacement for Starsiege"
#define VER_FILEDESCRIPTION_STR     "Neo's replacment memstar dll"
#define VER_INTERNALNAME_STR        "memstar.dll"
#define VER_LEGALCOPYRIGHT_STR      "Copyright (C) 2021 Starsiege Players Community, et al."
#define VER_ORIGINALFILENAME_STR    "mem.dll"

#define JOIN(a, b, c, d, e, f, g) JOIN_AGAIN(a, b, c, d, e, f, g)
#define JOIN_AGAIN(a, b, c, d, e, f, g) a ## b ## c ## d ## e ## f ## g

#define Q(x) #x
#define QUOTE(x) Q(x)

#ifdef _DEBUG
#define VER_DEBUG_BUILD VS_FF_DEBUG
#define VER_DEBUGBUILD_STR , Debug
#else
#define VER_DEBUG_BUILD 0
#define VER_DEBUGBUILD_STR , Release
#endif

#if VER_PRERELEASE_BUILD != 0
#undef VER_PRERELEASE_BUILD
#define VER_PRERELEASE_BUILD VS_FF_PRERELEASE
#define VER_PRERELEASEBUILD_STR , Pre-Release

#ifdef VER_DEBUGBUILD_STR
#undef VER_DEBUGBUILD_STR
#define VER_DEBUGBUILD_STR
#endif
#else
#define VER_PRERELEASEBUILD_STR
#endif

#if VER_PATCHED_BUILD != 0
#undef VER_PATCHED_BUILD
#define VER_PATCHED_BUILD VS_FF_PATCHED
#define VER_PATCHEDBUILD_STR , Patched
#else
#define VER_PATCHEDBUILD_STR
#endif

#if VER_PRIVATE_BUILD != 0
#undef VER_PRIVATE_BUILD
#define VER_PRIVATE_BUILD VS_FF_PRIVATEBUILD
#define VER_PRIVATEBUILD_STR , Private
#else
#define VER_PRIVATEBUILD_STR
#endif

#if VER_SPECIAL_BUILD != 0
#undef VER_SPECIAL_BUILD
#define VER_SPECIAL_BUILD VS_FF_SPECIALBUILD
#define VER_SPECIALBUILD_STR , Special
#else
#define VER_SPECIALBUILD_STR
#endif

#define VER_FILEVERSION_STR QUOTE(JOIN(VER_FILEVERSIONSTR, VER_DEBUGBUILD_STR, VER_PRERELEASEBUILD_STR, VER_PATCHEDBUILD_STR, VER_PRIVATEBUILD_STR, VER_SPECIALBUILD_STR, \0))
#define VER_PRODUCTVERSION_STR QUOTE(JOIN(VER_PRODUCTVERSIONSTR, VER_DEBUGBUILD_STR, VER_PRERELEASEBUILD_STR, VER_PATCHEDBUILD_STR, VER_PRIVATEBUILD_STR, VER_SPECIALBUILD_STR, \0))

#endif
