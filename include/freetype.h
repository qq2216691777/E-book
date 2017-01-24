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
	int (*Init)( const char *);
	void (*put_char)( int ,int ,int *);
	int (*put_string)( int ,int ,int *);
	short* (*show_page)( short* pf );
	void (*destroy)(void);
}ft_t;


extern ft_t g_freetype;

#endif
