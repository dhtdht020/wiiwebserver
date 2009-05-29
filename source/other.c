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
#include <sys/dir.h>

#include "wiiwebserver.h"
#define NOTVER
#include "libhttp.h"

/*s32 set_blocking(s32 s, bool blocking) 
{
    s32 flags;
    flags = net_fcntl(s, F_GETFL, 0);
    if (flags >= 0) flags = net_fcntl(s, F_SETFL, blocking ? (flags&~4) : (flags|4));
    return flags;
}*/


bool csocket(void)
{
	int port = 80;

	int Connect = net_init();
	if(Connect != 0)
	{
		return false;
	}
   
	netSocket = net_socket(AF_INET, SOCK_STREAM, 0);

	if(netSocket == INVALID_SOCKET)
	{
		return false;
	}

	memset(&sa, 0, sizeof(struct sockaddr_in)); /* clear our address */
	sa.sin_family= AF_INET;              /* this is our host address */
	sa.sin_port= htons(port);                /* this is our port number */
    sa.sin_addr.s_addr = INADDR_ANY;
	
	if(net_bind(netSocket,(struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		return false;
	}
	
	while (net_listen(netSocket, 1) == SOCKET_ERROR);
	
	return true;
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
	{
		return br;
	}
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
	//printf("Sending %s",buf);
	write_data(socket, buf, retval);
	free(buf);
	return retval;
}
