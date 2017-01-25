#ifndef _INPUT_H__
#define _INPUT_H__

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>

#include "tslib.h"

typedef struct
{
	int type;
	int ix;
	int iy;
	int ipress;
	int lpress;
	int (*Input)(void);
	int (*get_value)( void );
}input_t;

extern input_t g_input;

#endif