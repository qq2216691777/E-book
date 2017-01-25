#include "main.h"
#include "fb.h"
#include "freetype.h"
#include "file.h"
#include "page.h"
#include "input.h"

/***********************************************
*	function name: judge_file_type
*	function destion: Determinie whether the file suffix is the same
*	function parameter: filename: the file type  cmp_name: destion type
***********************************************/
bool judge_file_type( char const *filename, char *cmp_name )
{
	char *p=calloc(32,1);
	int i;

	memcpy(p,filename,strlen(filename));

	for( i=0;p[i];i++)
		if(p[i]>96)
			p[i] -= 0x20;

	p = strstr(p,cmp_name);

	if( (p==NULL)||(p[strlen(cmp_name)]!=0) )
		return false;
	return true;
}

int main(int argc, char const *argv[])
{

	char ch;

	if( argc != 3 )
	{
		DEBUG_PRINT("Usaged:%s font_file string\n", argv[0]);
		return -1;
	}
	if( !judge_file_type(argv[1],".TTF") )
	{
		DEBUG_PRINT("error: file type is wrong\n");
		return -1;
	}
	if(access(argv[1],0))
	{
		DEBUG_PRINT("error:%s is not exist\n",argv[1]);
		return -1;
	}

	if(access(argv[2],0))
	{
		DEBUG_PRINT("error:%s is not exist\n",argv[2]);
		return -1;
	}


	g_fb.Devie_Init();

	if(g_txt.Init( argv[2]) )
	{
		DEBUG_PRINT("error:TXT init failed\n");
		return -1;
	}

	g_freetype.Init( argv[1] );


	if( g_txt.type == 1 )
		//g_freetype.show_page( g_txt.fmem+g_txt.head );
		g_page.Init(g_txt.fmem+g_txt.head,g_fb.screen_size );

	g_page.Show();

	if(g_input.Input())
	{
		DEBUG_PRINT("error:input init failed\n");
		return -1;
	}
	while(1)
	{
		g_input.get_value();
		if( g_input.ipress && (!g_input.lpress) )
		{
			if( g_input.iy < g_fb.yres/2 )
			{
				g_page.Show_pre();
			}
			else
			{
				g_page.Show_next();
			}

		}
		usleep(1000);
	}

	return 0;
}