#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <gccore.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>

static const char  table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const int   BASE64_INPUT_SIZE = 57;

bool isbase64(char c)
{
	return c && strchr(table, c) != NULL;
}

int UnBase64(unsigned char *dest, const unsigned char *src, int srclen)
{
	*dest = 0;
	if(*src == 0) 
	{
		return 0;
	}	
	unsigned char *p = dest;
	do
	{

		char a = (int)src[0];
		char b = (int)src[1];
		char c = (int)src[2];
		char d = (int)src[3];
		*p++ = (a << 2) | (b >> 4);
		*p++ = (b << 4) | (c >> 2);
		*p++ = (c << 6) | d;
		if(!isbase64(src[1])) 
		{
			p -= 2;
			break;
		} 
		else if(!isbase64(src[2])) 
		{
			p -= 2;
			break;
		} 
		else if(!isbase64(src[3])) 
		{
			p--;
			break;
		}
		src += 4;
		while(*src && (*src == 13 || *src == 10)) src++;
	}
	while(srclen-= 4);
	*p = 0;
	return p-dest;
}
