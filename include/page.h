#ifndef _PAGE_H__
#define _PAGE_H__
#include <stdio.h>

struct  Link_Page
{
	char* fp;
	int num;
	struct  Link_Page *next;
	struct  Link_Page *pre;
};

typedef struct
{
	int num;
	int screen_size;
	char* fp;
	char* nfp;
	struct  Link_Page* page;
	void (*Init)( char *, int );
	void (*Show)( void );
	void (*Show_next)( void );
	void (*Show_pre)( void );
}page_t;


extern page_t g_page;

#endif