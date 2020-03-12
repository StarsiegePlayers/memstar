#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Memstar.h"
#include "StringConversions.h"
#include "List.h"

enum VariableType {
	VAR_bool = 1,
	VAR_int = 2,
	VAR_float = 3,
	VAR_double = 4,
	/* VAR_zeroToOne = 5, */
	/* VAR_vector3F = 6 */
};

namespace Console {

void addFunction(const char *name, void *cb);
void addVariable(const char *name, const void *address, const VariableType var_type);
void echo(const char *fmt, ...);
const char *execFunction(u32 argc, char *function, ...);
void eval(const char *cmd);
bool functionExists(const char *name);
const char *getVariable(const char *name);
void setVariable(const char *name, const char *value);

extern List<const char *> variables, functions;

struct VariableConstructor {
	VariableConstructor(const char *scripted, void *address, VariableType type) 
		: mName(scripted), mAddress(address), mType(type), mNext(mFirst) {
		
		mFirst = ( this );
	}

	static void Process() {
		VariableConstructor *node = mFirst;
		for (; node; node = node->mNext) {
			addVariable(node->mName, node->mAddress, node->mType);
			variables.Push(node->mName);
		}
	}

protected:
	const char *mName;
	void *mAddress;
	VariableType mType;
	VariableConstructor *mNext;	

	static VariableConstructor *mFirst;
};

struct ConsoleConstructor {
	ConsoleConstructor(const char *scripted, void *cb) 
		: mName(scripted), mCallback(cb), mNext(mFirst) {
		
		mFirst = ( this );
	}

	static void Process() {
		ConsoleConstructor *node = mFirst;
		for (; node; node = node->mNext) {
			addFunction(node->mName, node->mCallback);
			functions.Push(node->mName);
		}
	}
	
protected:
	const char *mName;
	void *mCallback;
	ConsoleConstructor *mNext;
	int mPrivilegeLevel;

	static ConsoleConstructor *mFirst;
};


}; // namespace Console

#define BuiltInVariable( __scripted__, __type__, __name__, __default__ )                 \
__type__ __name__ = __default__;                                                         \
static const Console::VariableConstructor vc##__name__( __scripted__, &__name__, VAR_##__type__ );

#define BuiltInFunction( __scripted__, __name__ )   \
const char * __stdcall __name__##bin(s32 argc, const char *self, const char *argv[]); \
NAKED const char *__name__##stub() {   \
	__asm { mov eax, [esp+4] }         \
	__asm { add eax, 4 }               \
	__asm { push eax }                 \
	__asm { sub eax, 4 }               \
	__asm { push dword ptr [eax] }     \
	__asm { dec ecx }                  \
	__asm { push ecx }                 \
	__asm { call __name__##bin }       \
	__asm { retn 4 }                   \
}                                      \
static const Console::ConsoleConstructor cc##__name__( __scripted__, __name__##stub );  \
const char * __stdcall __name__##bin(s32 argc, const char *self, const char *argv[])

#endif // __CONSOLE_H__
