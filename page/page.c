#include "page.h"
#include "freetype.h"

static void Page_Init( char *fmem, int size );
static void Page_Show( void );
static void Page_Show_next( void );
static void Page_Show_pre( void );


page_t g_page = {
			0,
			0,
			NULL,
			NULL,
			NULL,
			&Page_Init,
			&Page_Show,
			&Page_Show_next,
			&Page_Show_pre,
};
static void Page_Init( char *fmem, int size )
{
	g_page.fp = fmem;
	g_page.nfp = fmem;
	g_page.num = 0;
	g_page.screen_size = size;
	g_page.page = calloc(sizeof(struct  Link_Page),1);
	g_page.page->next = NULL;
	g_page.page->pre = NULL;
	g_page.page->num ++;
	g_page.page->fp = fmem;

}


static void Page_Show( void )
{
	if( g_page.page->next == NULL )
	{
		g_page.page->next = calloc(sizeof(struct  Link_Page),1);
		g_page.page->next->pre = g_page.page;
		g_page.page->next->next = NULL;
		g_page.page->next->num = g_page.page->num+1;
	}
	g_page.page->next->fp = g_freetype.show_page( g_page.page->fp );

}

static void Page_Show_next( void )
{

	g_page.page = g_page.page->next;
	if( g_page.page->next == NULL )
	{
		g_page.page->next = calloc(sizeof(struct  Link_Page),1);
		g_page.page->next->pre = g_page.page;
		g_page.page->next->next = NULL;
		g_page.page->next->num = g_page.page->num+1;
	}
	g_page.page->next->fp = g_freetype.show_page( g_page.page->fp );

}

static void Page_Show_pre( void )
{
	if( g_page.page->pre != NULL )
	{
		g_page.page = g_page.page->pre;
		g_freetype.show_page( g_page.page->fp );
	}

}