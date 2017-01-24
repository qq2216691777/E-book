#include "freetype.h"
#include "fb.h"
#include <freetype/ftglyph.h>

static int freetype_Init( char *font_file );
static void freetype_put_char( int x, int y, int *c );
static int freetype_put_string( int x, int y, int* c );
static int* freetype_show_page( short* pf );

ft_t g_freetype={
			"12",
			0.0,
			30,
			&freetype_Init,
      &freetype_put_char,
      &freetype_put_string,
      &freetype_show_page,
};



FT_Library    library;
FT_Face face;
FT_Vector  pen;

static int freetype_Init( char *font_file )
{
	FT_Error	  error;

  g_freetype.angle = 0;
  g_freetype.size = 50;

	error = FT_Init_FreeType( &library );

	error = FT_New_Face( library, font_file, 0, &face ); /* create face object */

	return 0;
}



static void freetype_put_char( int x, int y, int* c )
{
  FT_Error	    error;
  FT_Matrix     matrix;
  FT_Vector     pen;
  FT_GlyphSlot  slot;

  float angle;

  error = FT_Set_Pixel_Sizes( face, g_freetype.size, 0 );    /* set character size */
  angle = ( g_freetype.angle / 360 ) * 3.14159 * 2;          /* use 25 degrees     */
  /* set up matrix */
  matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

  pen.x = ( x) * 64;
  pen.y = ( g_fb.yres-y) * 64;

  FT_Set_Transform( face, &matrix, &pen );
  /* load glyph image into the slot (erase previous one) */
  error = FT_Load_Char( face, c[0], FT_LOAD_RENDER );
  slot = face->glyph;

  g_fb.draw_font(slot->bitmap_left,g_fb.yres-slot->bitmap_top,slot->bitmap.width,\
    slot->bitmap.rows,slot->bitmap.buffer);
}

static int freetype_put_string( int x, int y, int* c )
{
  FT_Error      error;
  FT_Matrix     matrix;
  FT_Vector     pen;
  FT_GlyphSlot  slot;

  int font_num=0;
  float angle;

  error = FT_Set_Pixel_Sizes( face, g_freetype.size, 0 );    /* set character size */
  angle = ( g_freetype.angle / 360 ) * 3.14159 * 2;          /* use 25 degrees     */
  /* set up matrix */
  matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

  pen.x = ( x) * 64;
  pen.y = ( g_fb.yres-y) * 64;
  while( *c )
  {
    FT_Set_Transform( face, &matrix, &pen );
    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Char( face, c[0], FT_LOAD_RENDER );
    slot = face->glyph;

    g_fb.draw_font(slot->bitmap_left,g_fb.yres-slot->bitmap_top,slot->bitmap.width,\
      slot->bitmap.rows,slot->bitmap.buffer);
    pen.x += slot->advance.x;
    c++;
    font_num++;
  }

  return font_num;
}

static int *freetype_show_page( short* pf )
{

  FT_Error      error;
  FT_Matrix     matrix;
  FT_Vector     pen;
  FT_GlyphSlot  slot;

  int font_width=0;
  int font_wi=32;

  float angle;
  short *p=pf;

  error = FT_Set_Pixel_Sizes( face, g_freetype.size, 0 );    /* set character size */
  angle = ( g_freetype.angle / 360 ) * 3.14159 * 2;          /* use 25 degrees     */
  /* set up matrix */
  matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

  pen.x = ( 0) * 64;
  pen.y = ( g_fb.yres-g_freetype.size) * 64;

  g_fb.cleanscreen(0xffffff);

  while( (pen.y>10)&&(p[0]) )
  {
    if( p[0]<128 )
      font_wi=32;
    else
      font_wi=64;
    if( pen.x > g_fb.xres*64-font_width*font_wi )
    {
      pen.x = 0;
      pen.y -= g_freetype.size*64;
      continue;
    }

    if( p[0]==0x0d)   // [enter]
    {
      p+=2;
      pen.x = 0;
      pen.y -= g_freetype.size*64;
      continue;
    }
    else if( p[0]==0x14 )   // [tab]
      p[0] = 0x20;

    FT_Set_Transform( face, &matrix, &pen );
    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Char( face, p[0]&0xffff, FT_LOAD_RENDER );
    slot = face->glyph;

    g_fb.draw_font(slot->bitmap_left,g_fb.yres-slot->bitmap_top,slot->bitmap.width,\
      slot->bitmap.rows,slot->bitmap.buffer);
    if( slot->bitmap.width>font_width )
    {
      font_width = slot->bitmap.width;
    }

    pen.x += slot->advance.x;
    p++;

  }
  return p;
}


static void freetype_destroy(void)
{
    FT_Done_Face( face );
    FT_Done_FreeType( library );
}
