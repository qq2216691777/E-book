#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdlib.h>


#include "fb.h"
#include "main.h"

static int Device_FB_Init(void);
static void lcd_put_pixel(int x, int y, unsigned int color );
static void lcd_clean( unsigned int color );
static void Device_FB_Destory(void);
static void lcd_draw_font_bitmap( int x, int y, int width, int hight, unsigned char *buffer );

fb_t g_fb={
			0,
			0,
			0,
			0,
			0,
			0,
			NULL,
			&Device_FB_Init,
			&lcd_put_pixel,
			&lcd_clean,
			&Device_FB_Destory,
			&lcd_draw_font_bitmap,
};

static void lcd_put_pixel(int x, int y, unsigned int color )
{
	unsigned char *pen_8 = g_fb.fbmem + y*g_fb.xres*g_fb.pixel_chars+x*g_fb.pixel_chars;
	unsigned short *pen_16 = (unsigned short *)pen_8;
	unsigned short *pen_32 = (unsigned short *)pen_8;
	unsigned char red,blue,green;
	switch(g_fb.pixel_bits)
	{
		case 8:
			*pen_8 = color;
			break;
		case 16:
			/* 565 */
        	red = (color>>16) & 0xff;
        	green = (color>>8) & 0xff;
        	blue = (color>>0) & 0xff;
        	color = ((red>>3)<<11) | ((green>>2)<<5) |((blue>>3));
        	*pen_16 = color;
       		break;
       	case 32:
       		*pen_32 = color;
       		break;
       	default:
       		DEBUG_PRINT("ERROR: can't support %ddpp\n",g_fb.pixel_bits);
       		break;
	}
}

static void lcd_clean( unsigned int color )
{
	unsigned char *pen_8 = g_fb.fbmem;
	unsigned short *pen_16 = (unsigned short *)pen_8;
	unsigned short *pen_32 = (unsigned short *)pen_8;
	unsigned char red,blue,green;
	int i = g_fb.screen_size;
	switch(g_fb.pixel_bits)
	{
		case 8:
			while(i)
			{
				*pen_8 = color;
				pen_8++;
				i--;
			}
			break;
		case 16:
			/* 565 */
        	red = (color>>16) & 0xff;
        	green = (color>>8) & 0xff;
        	blue = (color>>0) & 0xff;
        	color = ((red>>3)<<11) | ((green>>2)<<5) |((blue>>3));
        	while(i)
			{
				*pen_16 = color;
				pen_16++;
				i-=2;
			}
       		break;
       	case 32:
       		while(i)
			{
				*pen_32 = color;
				pen_32++;
				i-=4;
			}
       		break;
       	default:
       		DEBUG_PRINT("ERROR: can't support %ddpp\n",g_fb.pixel_bits);
       		return;
	}

}

static void lcd_draw_font_bitmap( int x, int y, int width, int hight, unsigned char *buffer )
{
	unsigned char *pen_8 = g_fb.fbmem;
	unsigned short *pen_16 = (unsigned short *)pen_8;
	unsigned short *pen_32 = (unsigned short *)pen_8;
	unsigned char red,blue,green;
	unsigned int color;
	unsigned char *buffer8 = (unsigned char *)buffer;

	int  i, j,q,p;
	int  x_max = x + width;
  	int  y_max = y + hight;

  	red = 0xff;
	green = 0;
	blue = 0;

	for ( j = y,q=0; j < y_max; j++,q++ )
	{
		for ( i = x,p=0 ; i < x_max; i++,p++ )
		{
			if ( i < 0 || j < 0 || i >= g_fb.xres || j >= g_fb.yres )
				continue;
			color = buffer8[q * width + p];
			if( color<10)
				continue;
			if( g_fb.pixel_bits==16 )
			{
				color = 256-color;
        		color = (((color)>>3)<<11) | (((color)>>2)<<5) |(((color)>>3));

        		*(pen_16+i+j*g_fb.xres) = color;
			}
			else if( g_fb.pixel_bits==8 )
			{

			}
			else if( g_fb.pixel_bits==32 )
			{

			}
		}
	}
}

static int Device_FB_Init(void)
{
	int fb_fd;
	struct fb_fix_screeninfo fix;
	struct fb_var_screeninfo var;
	unsigned char *fbmem;


	fb_fd = open(DEVICE_NAME, O_RDWR);
	if (fb_fd<0)
	{
		DEBUG_PRINT("open device fb failed\n");
		return -1;
	}
	if( ioctl( fb_fd,  FBIOGET_VSCREENINFO, &var ) )
    {
        DEBUG_PRINT("can't get var\n");
        return -1;
    }

    if( ioctl( fb_fd,  FBIOGET_FSCREENINFO, &fix ) )
    {
        DEBUG_PRINT("can't get fix\n");
        return -1;
    }


    g_fb.fd = fb_fd;
    g_fb.xres = var.xres;
	g_fb.yres = var.yres;
	g_fb.pixel_bits = var.bits_per_pixel;
	g_fb.pixel_chars = var.bits_per_pixel/8;
	g_fb.screen_size = var.xres*var.yres*var.bits_per_pixel/8;

	fbmem = (unsigned char *)mmap( NULL, g_fb.screen_size,  PROT_READ | PROT_WRITE, MAP_SHARED,fb_fd,0 );
    if( fbmem==(unsigned char *)-1 )
    {
    	DEBUG_PRINT("mmap is failed\n");
        return -1;
    }

	g_fb.fbmem = fbmem;

	printf("xres:%d\n", g_fb.xres);
	printf("yres:%d\n", g_fb.yres);
	printf("pixel bit:%d\n", g_fb.pixel_bits);
	printf("pixel char:%d\n", g_fb.pixel_chars);
	return 0;


}

static void Device_FB_Destory(void)
{
	free(g_fb.fbmem);
}