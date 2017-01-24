#include "file.h"
#include "main.h"

static int file_Init( char *filename );


file_t g_txt={
			NULL,
			0,
			0,
			0,
			0,
			NULL,
			&file_Init,
		};

static int file_Init( char *filename )
{
	FILE *fd;
	int iFd;
	struct stat tStat;

	g_txt.filename = filename;
	fd = fopen(filename, "r+");
	if (fd == NULL)
	{
		DEBUG_PRINT("can't open %s\n", filename);
		return -1;
	}

	iFd = fileno(fd);
	fstat(iFd, &tStat);
	g_txt.size = tStat.st_size;
	g_txt.fmem = (unsigned char *)mmap(NULL , tStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, iFd, 0);
	if (g_txt.fmem == (unsigned char *)-1)
	{
		DEBUG_PRINT("mmap error!\n");
		return -1;
	}
	if( g_txt.fmem[0]==0xff && g_txt.fmem[1]==0xfe )
	{
		g_txt.type = 1;		/* unicode */
		g_txt.head = 2;
	}
	if( g_txt.fmem[0]==0xfe && g_txt.fmem[1]==0xff )
	{
		g_txt.type = 2;		/* unicode big*/
		g_txt.head = 2;
	}
	else if( g_txt.fmem[0]==0xef && g_txt.fmem[1]==0xbb && g_txt.fmem[1]==0xbf )
	{
		g_txt.type = 11;		/* utf-8 */
		g_txt.head = 3;
	}
	return 0;
}

