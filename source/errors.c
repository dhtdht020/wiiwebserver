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
#include "inbuilt.h"

void headersend(client_t *client);

s32 send_exact(s32 s, char *buf, s32 length);

void senderror(client_t *client, int errorcode)
{
	char* buffer = "";
	int i;
	
	for (i = 0; errorcodes[i].errornumber != NULL; i++) 
	{
		if (errorcodes[i].errornumber==errorcode)
		{
			client->response.status=errorcodes[i].errormessage;
			client->response.statuscode=errorcodes[i].errorcode;
		}
	}
	
	//sprintf(buffer,"Error %s\r\nOccured for the reason of %s\r\n", client->response.statuscode, client->response.status);
	
	char path[1024] = "sd://data/web/error/";
	
	bool custom = true;
	
	strcat(path,client->response.statuscode);
	
	strcat(path,".html");
	
	FILE* pFile;
	long lSize = 0;
	size_t result;
	pFile = fopen (path, "r");
	if (pFile!=NULL) 
	{   
		fseek (pFile , 0 , SEEK_END);
		lSize = ftell (pFile);
		rewind (pFile);
		buffer = (char*) malloc (sizeof(char)*lSize);
		if (buffer == NULL) {custom=false;}
		result = fread (buffer,1,lSize,pFile);
		if (result != lSize) {custom=false;}
		fclose (pFile); 
	}
	else
	{
		custom = false;
	}
	if((custom==false))
	{
		client->response.contlength=3+30+6+28+2;
	}
	else
	{
		client->response.contlength=lSize;
	}
	printf("Sending error page\n");
	strcpy(client->response.mimetype,"text/html");
	headersend(client);
	if((custom==false))
	{
		net_write(client->socket,"Error ",6);
		net_write(client->socket,client->response.statuscode,3);
		net_write(client->socket,"\r\nOccured for the reason of ",28);
		net_write(client->socket,client->response.status,30);
		net_write(client->socket,"\r\n",2);
	}
	else
	{
		send_exact(client->socket,buffer,client->response.contlength);
	}
	printf("Sent\n");
	sleep(2);
	VIDEO_WaitVSync();
	return;
}
