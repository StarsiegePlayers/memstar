#ifndef __HASH_H__
#define __HASH_H__

#include "Memstar.h"

u32 HashString(const char *str);
u32 HashBytes(const unsigned char *key, size_t len);

#endif // __HASH_H__

