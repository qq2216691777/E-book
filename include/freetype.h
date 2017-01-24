#ifndef _FREETYPE_H__
#define _FREETYPE_H__
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "ft2build.h"
#include FT_FREETYPE_H

typedef struct $
{
	char font_name[32];
	float angle;
	int size;
	int (*Init)( char *);
	void (*put_char)( int ,int ,int *);
	int (*put_string)( int ,int ,int *);
	int* (*show_page)( short* pf );
}ft_t;


extern ft_t g_freetype;

#endif
