#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <gccore.h>
#include <stdarg.h>
#include <unistd.h>
#include <network.h>
#include <fat.h>
#include <wiiuse/wpad.h>

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
	printf("Press A to allow access\n");
	printf("Press B to disallow access\n");
	while(1)
	{
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if ( pressed & WPAD_BUTTON_A ) return true;
		if ( pressed & WPAD_BUTTON_B ) return false;
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
