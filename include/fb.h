#ifndef _FB_H__
#define _FB_H__

#define DEVICE_NAME	"/dev/fb0"

typedef struct
{
	int fd;
	int xres;
	int yres;
	int pixel_bits;
	int pixel_chars;
	int screen_size;
	unsigned char *fbmem;
	void (*Devie_Init)(void);
	void (*put_pixel)(int x, int y, unsigned int color );
	void (*cleanscreen)( unsigned int color );
	void (*Devie_Destory)(void);
	void (*draw_font)( int x, int y, int width, int hight, unsigned char *buffer );

}fb_t;

extern fb_t g_fb;


#endif