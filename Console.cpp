#include "Console.h"
#include "Callback.h"
#include "Strings.h"

namespace Console {

#define CONSOLE_PTR 0x722FA4

	const u32 fnAddFunction = 0x005E6D80;
	const u32 fnAddVariable = 0x005E6D18;
	const u32 fnEcho = 0x005E6A1C;
	const u32 fnExecFunction = 0x005E6ED0;
	const u32 fnEval = 0x005E6DF0;
	const u32 fnGetVariable = 0x005E6C38;
	const u32 fnSetVariable = 0x005E6B50;
	const u32 fnFunctionExists = 0x005E7120;
	u32 dummy, dummy2;

	VariableConstructor* VariableConstructor::mFirst = NULL;
	ConsoleConstructor* ConsoleConstructor::mFirst = NULL;

	typedef List<const char*> StringList;
	typedef StringList::Iterator StringIter;
	StringList variables, functions;

	void addFunction(const char* name, void* cb) {
		__asm {
			push[cb]
			push 0
			mov ecx, [name]
			xor edx, edx
			mov eax, ds: [CONSOLE_PTR]
			call[fnAddFunction]
		}
	}

	void addVariable(const char* name, const void* address, const VariableType var_type) {
		__asm {
			push[var_type]
			push[address]
			mov ecx, [name]
			xor edx, edx
			mov eax, ds: [CONSOLE_PTR]
			call[fnAddVariable]
		}
	}


	NAKED void echo(const char* fmt, ...) {
		__asm {
			pop[dummy]
			push dword ptr ds : [CONSOLE_PTR]
			call[fnEcho]
			add esp, 4
			jmp[dummy]
		}
	}

	NAKED const char* execFunction(u32 argc, char* function, ...) {
		__asm {
			pop[dummy2]
			push dword ptr ds : [CONSOLE_PTR]
			inc dword ptr[esp + 0x4] // argc
			call[fnExecFunction]
			add esp, 4
			jmp[dummy2]
		}
	}

	void eval(const char* cmd) {
		__asm {
			push 0
			push 0
			mov edx, [cmd]
			xor ecx, ecx
			mov eax, ds: [CONSOLE_PTR]
			call[fnEval]
		}
	}

	bool functionExists(const char* name) {
		__asm {
			mov edx, [name]
			mov eax, ds: [CONSOLE_PTR]
			call[fnFunctionExists]
		}
	}


	const char* getVariable(const char* name) {
		__asm {
			mov eax, ds: [CONSOLE_PTR]
			mov edx, [name]
			call[fnGetVariable]
		}
	}

	void setVariable(const char* name, const char* value) {
		__asm {
			mov eax, ds: [CONSOLE_PTR]
			mov edx, [name]
			mov ecx, [value]
			call[fnSetVariable]
		}
	}

	void OnStarted(bool active) {
		VariableConstructor::Process();
		ConsoleConstructor::Process();
	}

	struct Init {
		Init() {
			Callback::attach(Callback::OnStarted, OnStarted);
		}
	} init;


	struct StringSorter {
		bool operator() (const char*& a, const char*& b) const {
			return _stricmp(a, b) < 0;
		}
	};

	struct FunctionPrinter {
		void operator() (const char*& in) const {
			Console::echo(" %s()", in);
		}
	};

	struct VariablePrinter {
		void operator() (const char*& in) const {
			Console::echo(" $%s = \"%s\";", in, Console::getVariable(in));
		}
	};


	template <typename Printer>
	void DumpStrings(StringList& strings) {
		Printer p;
		strings.Sort(StringSorter());
		for (StringIter iter = strings.Begin(); iter != strings.End(); ++iter)
			p(iter.value());
	}

	BuiltInFunction("Memstar::dumpAddons", dumpAddons) {
		Console::echo("------------------");
		Console::echo("Memstar Functions");
		DumpStrings<FunctionPrinter>(functions);
		Console::echo("------------------");
		Console::echo("Memstar Variables");
		DumpStrings<VariablePrinter>(variables);
		return "true";
	}

}; // namespace Console