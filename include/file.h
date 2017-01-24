#ifndef _FILE_H__
#define _FILE_H__
#include <file.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

typedef struct
{
	char *filename;
	int fd;
	int size;
	int type;
	int head;
	unsigned char *fmem;
	int (*Init)( char *filename );
}file_t;

extern file_t g_txt;

#endif