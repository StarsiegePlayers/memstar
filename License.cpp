#include "Console.h"
#include "Callback.h"
#include "Strings.h"
#include "version.h"
#include "resource.h"

namespace Licence {

	BuiltInFunction("Memstar::version", version) {
		Console::echo("%s", VER_PRODUCTNAME_STR);
		Console::echo("------------------");
		Console::echo("Version: %s", VER_FILEVERSION_STR);
#ifdef VER_SPECIAL_STR
		Console::dbecho("Special Build Note: %s", VER_SPECIAL_STR);
#endif
#ifdef VER_PRIVATE_STR
		Console::dbecho("Private Build Note: %s", VER_PRIVATE_STR);
#endif
#ifdef VER_COMMENT_STR
		Console::echo("Build Note: %s", VER_COMMENT_STR);
#endif
		Console::echo("------------------");
		Console::echo("%s", VER_LEGALCOPYRIGHT_STR);
		Console::echo("Initial versions written and published by memstar and floodyberry.");

		return "true";
	}

	BuiltInFunction("Memstar::license", license) {
		//HRSRC licenseResource = FindResourceA(NULL, NULL, MAKEINTRESOURCEA(IDR_LICENSE1));
		//HGLOBAL licenseLoad = LoadResource(NULL, licenseResource);

		MessageBoxW(NULL, L"This is from mem.dll!", L"Test!", MB_YESNOCANCEL | MB_ICONQUESTION | MB_SETFOREGROUND);

		return "true";
	}

#if 0
	class LicenseHandler {
	public:
		static PHKEY key;

		static void OpenRegKey() {
			LSTATUS status = RegOpenCurrentUser(KEY_READ | KEY_WRITE, key);
			if (status != ERROR_SUCCESS) {
				// open file here
			}
		}

		static void WriteRegKey() {
			LSTATUS status = RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\Dynamix\Starsiege\mem.dll", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, key, NULL);
			RegSetValueEx()
		}

	};

	INT_PTR CALLBACK LicenseDialog(HWND hwind, UINT uMsg, WPARAM wParam, LPARAM lParam) {

		switch (uMsg) {
		case WM_INITDIALOG:


		}

		SetWindowLongA(hwind, DWL_MSGRESULT, IDCONTINUE);
		return TRUE;
	}

	void LicenseAgreement() {
		int choice = MessageBoxW(NULL, L"In order to use this copy of mem.dll, you must view and accept the license agreements", L"License Agreement Required", MB_OKCANCEL | MB_ICONASTERISK | MB_SETFOREGROUND);
		if (choice == IDCANCEL) {
			exit(EXIT_SUCCESS);
		}
		//DialogBoxA(NULL, MAKEINTRESOURCEA(IDD_PROPPAGE_SMALL), NULL, LicenseDialog);
	}

	struct Init {
		Init() {
			LicenseAgreement();
		}
	} init;
#endif

}

