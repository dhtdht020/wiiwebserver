#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <gccore.h>
#include <stdarg.h>
#include <unistd.h>
#include <network.h>
#include <fat.h>
#include <malloc.h>
#include <time.h>
#include <wiiuse/wpad.h>
#include <ogc/pad.h>
#include <mxml.h>
#include <ogc/mutex.h>
#include <fcntl.h>

//Built in Files
#include "inbuilt.h"

//Server includes
#include "mimes.h"      //Mime types
#include "header.h"
#include "common.h"

bool scheck(void)
{
	int count=0;
	printf("Permit access to admin pages ?\n");
	printf("Press A or C to allow access\n");
	printf("Press B or Z to disallow access\n");
	while(1)
	{
		WPAD_ScanPads();
		PAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		u32 padpressed = PAD_ButtonsDown(0);
		if ( (pressed & WPAD_BUTTON_A) || (pressed & WPAD_GUITAR_HERO_3_BUTTON_GREEN) || (padpressed & PAD_BUTTON_A) || (pressed & WPAD_NUNCHUK_BUTTON_C) ) return true;
		if ( (pressed & WPAD_BUTTON_B) || (pressed & WPAD_GUITAR_HERO_3_BUTTON_RED) || (padpressed & PAD_BUTTON_B) || (pressed & WPAD_NUNCHUK_BUTTON_Z) ) return false;
		if ( count == 5000000 )
		{
			count=0;
			return false;
		}
		count++;
	}
}

int write_data(int s, char *buf, int n)
{
	int bcount;
	int br;
	bcount = 0;
	br = 0;
	while(bcount < n) 
	{
		if ((br = net_write(s,buf,n-bcount)) > 0) {
		bcount += br;
		buf += br;
    }
    else if (br < 0)
		return br;
	}
	return (bcount);
}

int socket_printf(const s32 socket, const char *format, ...) 
{
	char *buf;
	int retval;
	va_list ap;
	va_start(ap, format);
	retval = vasprintf(&buf, format, ap);
	va_end(ap);
	write_data(socket, buf, retval);
	free(buf);
	return retval;
}

s32 set_blocking(s32 s, bool blocking) {
    s32 flags;
    flags = net_fcntl(s, F_GETFL, 0);
    if (flags >= 0) flags = net_fcntl(s, F_SETFL, blocking ? (flags&~4) : (flags|4));
    return flags;
}

s32 net_close_blocking(s32 s) {
    set_blocking(s, true);
    return net_close(s);
}
