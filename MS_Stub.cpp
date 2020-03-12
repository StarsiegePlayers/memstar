#include "Memstar.h"

extern "C" DLLAPI void *MS_Calloc(size_t amt, size_t zero) {
	return calloc(amt, zero);
}

extern "C" DLLAPI void *MS_Malloc(size_t amt) {
	return malloc(amt);
}

extern "C" DLLAPI void MS_Free(void *block) {
	free(block);
}

extern "C" DLLAPI void *MS_Realloc(void *block, size_t amt) {
	return realloc(block, amt);
}
