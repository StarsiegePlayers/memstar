#include "Console.h"
#include "Patch.h"
#include "VersionSnoop.h"
#include "MultiPointer.h"

namespace ExeFixes {

	MultiPointer(ptrWideScreen, 0, 0, 0x0063c807, 0x0064B747);
	CodePatch widescreen = {
		ptrWideScreen,
		"",
		"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90",
		58,
		false
	};

	MultiPointer(ptrDoSFix, 0, 0, 0x0067c7e6, 0x0068C6B2);
	//                                                 |.  8DBD 38FFFFFF     LEA EDI,DWORD PTR SS:[EBP-C8]
	CodePatch dosfix = {
		ptrDoSFix,
		"",
		"\xE9OSFX",
		5,
		false
	};


	MultiPointer(fnBitStreamReadInt, 0, 0, 0, 0x0056D4A0);
	MultiPointer(fnReadPacketAcksResume, 0, 0, 0, 0x0068C6E9);
	static const char* crashAttempt = "DoSFiX: Crash Attempt by %s";
	NAKED void DosFix() {
		__asm {
			push ebx
			lea edi, [ebp - 0xc8]
			lea ebx, [edi + (0x1a * 0x8)]
			jmp __primed_jump
			__read_ack_loop :
			lea eax, [ebp - 0xf0]
				mov edx, 0x5
				call[fnBitStreamReadInt]
				mov[edi - 0x4], eax
				inc dword ptr[ebp - 0x28]
				add edi, 0x8
				__primed_jump:
			lea eax, [ebp - 0xf0]
				mov edx, 0x3
				call[fnBitStreamReadInt]
				mov[edi], eax
				cmp edi, ebx
				jae __crash_attempt
				test eax, eax
				jnz __read_ack_loop
				__leave_loop :
			pop ebx
				jmp[fnReadPacketAcksResume]

				__crash_attempt :
				lea eax, [ebp + 0x28]
				push eax
				mov eax, [crashAttempt]
				push eax
				call Console::echo
				add esp, 0x8
				jmp __leave_loop
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

			widescreen.Apply(true);
			dosfix.DoctorRelative((u32)DosFix, 1).Apply(true);
		}
	} init;
}; // namespace ExeFixes