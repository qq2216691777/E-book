#include "input.h"

static int Input_Init(void);
static int Input_get_value( void );

input_t g_input = {
		0,
		0,
		0,
		0,
		0,
		&Input_Init,
		&Input_get_value,
};

struct tsdev *ts;
static int Input_Init(void)
{
	char *tsdevice=NULL;

        if( (tsdevice = getenv("TSLIB_TSDEVICE")) != NULL ) {
                ts = ts_open(tsdevice,0);
        } else {
#ifdef USE_INPUT_API
            ts = ts_open("/dev/input/event0", 0);
#else
            ts = ts_open("/dev/touchscreen/ucb1x00", 0);
#endif /* USE_INPUT_API */
        }

	if (!ts) {
		perror("ts_open");
		return -1;
	}

	if (ts_config(ts)) {
		perror("ts_config");
		return -1;
	}

	g_input.type = 1;

	return 0;
}

static int Input_get_value( void )
{
	struct ts_sample samp;
	int ret;

	ret = ts_read(ts, &samp, 1);

	if (ret < 0) {
		perror("ts_read");
		return -1;
	}

	if (ret == 1)
	{
		g_input.ix = samp.x;
		g_input.iy = samp.y;
		g_input.lpress = g_input.ipress;
		g_input.ipress = samp.pressure;
	}

	return 0;
}