/* ioapi.c -- IO base function header for compress/uncompress .zip
   files using zlib + zip or unzip API

   Version 1.01e, February 12th, 2005

   Copyright (C) 1998-2005 Gilles Vollant
*/

#include "zlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ioapi.h"



/* I've found an old Unix (a SunOS 4.1.3_U1) without all SEEK_* defined.... */

#ifndef SEEK_CUR
#define SEEK_CUR    1
#endif

#ifndef SEEK_END
#define SEEK_END    2
#endif

#ifndef SEEK_SET
#define SEEK_SET    0
#endif

void fill_fopen_filefunc (pzlib_filefunc_def)
  zlib_filefunc_def* pzlib_filefunc_def;
{

}
