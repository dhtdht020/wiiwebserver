/**
Wii Web server

sorce code copyright to Luke Bridges
except for any code that is credited to other
developers, this goes for all wii web server 
source files

joedj - sections of ftpii are used and unchanged in common.c and common.h
			unwanted code has been removed from these files, the only code left
			should be write_exact and relating functions
		  
teknecal - 	sections of wiihttpd are used, these include converting %20 to spaces
				and checking for ../ and ./
			the "HEAD" handling routine is used as 'inspiration' and is not exactly
				the same but credit is still given here
			time code for the header is also used

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from
the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1.The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software in a
product, an acknowledgment in the product documentation would be
appreciated but is not required.

2.Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3.This notice may not be removed or altered from any source distribution.

The best effort has been made to keep to the  copyright rights, if there
are any faults please contact me and i'll be glad to make any necessary corrections.
**/

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
//#include <kbd.h>

//Built in Files
#include "inbuilt.h"

//Server includes
#include "mimes.h"      //Mime types
#include "header.h"

#include "common.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

static u32 aMutex = 0;
lwp_t serverT;
#define THREADSTACK 8192
u8 threadonestack[THREADSTACK];
lwpq_t ThreadOneQ;

//int client;             //ID for connected client, much like a socket number
s32 netSocket;

// Current server version
char* servver="0.91";

// Settings file path
//char* settingsfile = "sd:/data/settings/wiiweb.xml";
// Inits 404
char* c_404="nofile";    // Custom 404 file path
// Inits 500
char* c_500="nofile";    // Custom 500 file path
// Inits denied
char* c_denied="nofile"; // Custom denied file path
// RAM based error 500 page
//char* ram_500;
//int ram_500_size;
void sdpage(client_t *client);
void _500(client_t *client,char* forcd, bool rampage);
char* password;
// Timezone for Date: header response
int server_timezone = 0; // Set your timezone in relation to GMT time (in this case GMT + 5)

extern char* readfile(char* filename);
struct sockaddr_in sa;

// How many clients are connected
int clients=0;
// The max number of clients
int maxclients=10;

// If the socket has been opened or not
bool socket_ready;
int four04ercount=0;

//Files
FILE* pFile;
long lSize;
char * buffer;
size_t result;

int headersend(client_t *client)
{
	if ((strcmp(client->http10.statuscode, " "))==0)
	{
		client->http10.statuscode="200";
	}
	
	if ((strcmp(client->http11.statuscode, " "))==0)
	{
		client->http11.statuscode="200";
	}
	
	if ((strcmp(client->http10.status, " "))==0)
	{
		client->http10.status="OK";
	}
	
	if ((strcmp(client->http11.status, " "))==0)
	{
		client->http11.status="OK";
	}
	
	// Date/time
	long the_time = time(0) - (server_timezone * 3600);
	
	char* date_time[5];
	char* tok;
	int x = 0;
	
	tok = strtok (ctime (&the_time)," ");
	while (tok != NULL) {
		date_time[x] = tok;
		tok = strtok (NULL, " ");
		x++;
	}
	
	char year[6] = "";
	strncpy (year,date_time[4], 4);
	year[5]='\0';

	if ((strcmp(client->httpreq.httpver,"HTTP/1.1"))==0)
	{
		socket_printf(client->socket, "%s %s %s\r\n", client->httpreq.httpver, client->http11.statuscode, client->http11.status);
		printf("%s %s %s\r\n", client->httpreq.httpver, client->http11.statuscode, client->http11.status);
		socket_printf(client->socket, "Date: %s, %s %s %s %s GMT\r\n", date_time[0], date_time[1], date_time[2], year, date_time[3]);
		socket_printf(client->socket, "Server: WiiWebServer %s\r\n", servver);
		socket_printf(client->socket, "Host: 192.168.0.3\r\n");
		socket_printf(client->socket, "Content-Type: %s\r\n", client->mimetype);
		socket_printf(client->socket, "Content-Length: %d\r\n",client->contlength);
		socket_printf(client->socket, "Connection: close\r\n");
		socket_printf(client->socket, "\r\n");
	}
	else
	{	
		socket_printf(client->socket, "%s %s %s\r\n", client->httpreq.httpver, client->http10.statuscode, client->http10.status);
		printf("%s %s %s\r\n", client->httpreq.httpver, client->http11.statuscode, client->http11.status);
		socket_printf(client->socket, "Date: %s, %s %s %s %s GMT\r\n", date_time[0], date_time[1], date_time[2], year, date_time[3]);
		socket_printf(client->socket, "Server: WiiWebServer %s\r\n", servver);
		socket_printf(client->socket, "Content-Type: %s\r\n", client->mimetype);
		socket_printf(client->socket, "Content-Length: %d\r\n",client->contlength);
		socket_printf(client->socket, "\r\n");
	}
	
	client->http10.statuscode=" ";
	client->http11.statuscode=" ";
	client->http10.status=" ";
	client->http11.status=" ";
	return 1;
}

void favicon(client_t *client)
{
	set_blocking(client->socket,true);
	client->contlength=favicon_ico_size;
	printf("Sending favicon\n");
	client->mimetype="icon/x-icon";
	headersend(client);
	net_write(client->socket,favicon_ico,client->contlength);
	printf("Sent\n");
	usleep(200000);
	VIDEO_WaitVSync();
	set_blocking(client->socket,false);
	net_close(client->socket);
	return;
}

void _500(client_t *client,char* forcd, bool rampage)
{
	set_blocking(client->socket,true);
	client->http10.statuscode="500";
	client->http11.statuscode="500";
	client->http10.status="Internal Server Error";
	client->http11.status="Internal server Error";
	if ((rampage==1))
	{
		client->contlength=_500_html_size;
		client->mimetype="text/html";
		headersend(client);
		net_write(client->socket, _500_html, _500_html_size);
		printf("Sent\n");
		usleep(200000);
		VIDEO_WaitVSync();
		set_blocking(client->socket,false);
		net_close(client->socket);
	}
	else if ((strcmp(forcd,"yes")==0))
	{
		client->contlength=_500_html_size;
		client->mimetype="text/html";
		headersend(client);
		net_write(client->socket, _500_html, _500_html_size);
		printf("Sent\n");
		usleep(200000);
		VIDEO_WaitVSync();
		set_blocking(client->socket,false);
		net_close(client->socket);
	}
	else
	{
		if ((strcmp(c_500,"nofile")==0))
		{
			client->contlength=_500_html_size;
			client->mimetype="text/html";
			headersend(client);
			net_write(client->socket, _500_html, _500_html_size);
			printf("Sent\n");
			usleep(200000);
			VIDEO_WaitVSync();
			set_blocking(client->socket,false);
			net_close(client->socket);
		}
		else
		{
			char path[100] = "sd:/data/web";
			strcat(path, c_500);
			
			while (LWP_MutexLock (aMutex) != 0);
	
			pFile = fopen (path, "r");
	
			if (pFile!=NULL) 
			{   
				fseek (pFile , 0 , SEEK_END);
				lSize = ftell (pFile);
				rewind (pFile);
				buffer = (char*) malloc (sizeof(char)*lSize);
				if (buffer == NULL) {fputs ("   Memory error",stderr); fclose(pFile); _500(client, "no", 1); return;}
				result = fread (buffer,1,lSize,pFile);
				if (result != lSize) {fputs ("   Reading error",stderr); fclose(pFile); _500(client, "no", 1); return;}
				fclose (pFile); 
				client->mimetype="text/html";
				char* filecontent=buffer;
				buffer=NULL;
				client->contlength=lSize;
				printf("Sending\n");
				headersend(client);
				if ((strcmp(client->httpreq.method, "HEAD")==0))
				{
				}
				else
				{
					write_exact(client->socket, filecontent, client->contlength);
				}
				printf("Sent\n");
				sleep(1);
				VIDEO_WaitVSync();
				LWP_MutexUnlock (aMutex);
				set_blocking(client->socket,false);
				net_close(client->socket);
				return;
			}
			else
			{
				client->contlength=_500_html_size;
				client->mimetype="text/html";
				headersend(client);
				net_write(client->socket, _500_html, _500_html_size);
				printf("Sent\n");
				usleep(200000);
				VIDEO_WaitVSync();
				set_blocking(client->socket,false);
				net_close(client->socket);
			}
		}
	}
}

void _404(client_t *client, char* forcd)
{
	set_blocking(client->socket,true);
	client->http10.statuscode="404";
	client->http11.statuscode="404";
	client->http10.status="Not Found";
	client->http11.status="Not Found";
	
	if ((strcmp(forcd,"yes")==0))
	{
		client->contlength=_404_html_size;
		client->mimetype="text/html";
		headersend(client);
		net_write(client->socket, _404_html, _404_html_size);
		printf("Sent\n");
		usleep(200000);
		VIDEO_WaitVSync();
		set_blocking(client->socket,false);
		net_close(client->socket);
	}
	else
	{
		if ((strcmp(c_404,"nofile")==0))
		{
			client->contlength=_404_html_size;
			client->mimetype="text/html";
			headersend(client);
			net_write(client->socket, _404_html, _404_html_size);
			printf("Sent\n");
			usleep(200000);
			VIDEO_WaitVSync();
			set_blocking(client->socket,false);
			net_close(client->socket);
		}
		else
		{
			char path[100] = "sd:/data/web";
			strcat(path, c_404);
			
			while (LWP_MutexLock (aMutex) != 0);
	
			pFile = fopen (path, "r");
	
			if (pFile!=NULL) 
			{   
				fseek (pFile , 0 , SEEK_END);
				lSize = ftell (pFile);
				rewind (pFile);
				buffer = (char*) malloc (sizeof(char)*lSize);
				if (buffer == NULL) 
				{
					fputs ("   Memory error",stderr); 
					fclose(pFile); 
					_500(client, "no", 1); 
					set_blocking(client->socket,false);
					return;
				}
				result = fread (buffer,1,lSize,pFile);
				if (result != lSize) 
				{
					fputs ("   Reading error",stderr); 
					fclose(pFile); 
					_500(client, "no", 1); 
					set_blocking(client->socket,false);
					return;
				}
				fclose (pFile); 
				client->mimetype="text/html";
				char* filecontent=buffer;
				buffer=NULL;
				client->contlength=lSize;
				printf("Sending\n");
				headersend(client);
				if ((strcmp(client->httpreq.method, "HEAD")==0))
				{
				}
				else
				{
					write_exact(client->socket, filecontent, client->contlength);
				}
				printf("Sent\n");
				sleep(1);
				VIDEO_WaitVSync();
				LWP_MutexUnlock (aMutex);
				set_blocking(client->socket,false);
				net_close(client->socket);
			}
			else
			{
				client->contlength=_404_html_size;
				client->mimetype="text/html";
				headersend(client);
				net_write(client->socket, _404_html, _404_html_size);
				printf("Sent\n");
				usleep(200000);
				VIDEO_WaitVSync();
				set_blocking(client->socket,false);
				net_close(client->socket);
			}
		}
	}
	return;
}

void sdpage(client_t *client)
{
	char* filecontent = "TEST";
	char* origpath = client->httpreq.path;
	int pathlen = strlen(origpath);
	
	printf("\nLength : %d\n",pathlen);
	
	if ((strcmp(client->httpreq.path,"/")==0))
	{
		client->httpreq.path="/index.html";
	}
	
	if ((strcmp(client->httpreq.path, "/index")==0))
	{
		strcat(client->httpreq.path, ".html");
	}
	
	if (strstr(client->httpreq.path, "./") || strstr(client->httpreq.path, "../")) 
	{
		_404(client,"no");
		set_blocking(client->socket,false);
		LWP_MutexUnlock (aMutex);
		net_close(client->socket);
		return;
	}
	
	if (strstr(client->httpreq.path, "wiiweb")) 
	{
		_404(client,"no");
		set_blocking(client->socket,false);
		LWP_MutexUnlock (aMutex);
		net_close(client->socket);
		return;
	}

	char* space_tok= "";
	char* space_replace = "";
	
	space_tok = strtok (client->httpreq.path,"%20");
	space_replace = space_tok;
	while (space_tok != NULL) 
	{
		space_tok = strtok (NULL, "%20"); 
		if (space_tok != NULL) 
		{
			strcat(space_replace, " ");
			strcat(space_replace, space_tok);
		}
	}
	client->httpreq.path = space_replace;
	
	char path[100] = "sd:/data/web";
	strcat(path,client->httpreq.path);

	strtok(path, "?");

	printf("File : %s\n", path);
	
	while (LWP_MutexLock (aMutex) != 0);

	pFile = fopen (path, "r");

	if (pFile!=NULL) 
	{   
		fseek (pFile , 0 , SEEK_END);
		lSize = ftell (pFile);
		rewind (pFile);
		buffer = (char*) malloc (sizeof(char)*lSize);
		if (buffer == NULL) 
		{
			fputs ("Memory error",stderr); 
			fclose(pFile); 
			_500(client, "no", 1); 
			set_blocking(client->socket,false);
			LWP_MutexUnlock (aMutex);
			net_close(client->socket);
			return;
		}
		result = fread (buffer,1,lSize,pFile);
		if (result != lSize) 
		{
			fputs ("Reading error\n",stderr); 
			fclose(pFile); 
			_500(client, "no", 1); 
			set_blocking(client->socket,false);
			LWP_MutexUnlock (aMutex);
			net_close(client->socket);
			return;
		}
		fclose (pFile); 
	}
	else if (pFile==NULL) 
	{
		if ((strcmp(client->httpreq.method, "HEAD")==0))
		{
		}
		else
		{
			fclose (pFile);
			_404(client,"no");
			set_blocking(client->socket,false);
			LWP_MutexUnlock (aMutex);
			net_close(client->socket);
			return;
		}
	}
	
	char* fileext = strtok(client->httpreq.path,".");
	fileext = strtok(NULL,".");
	int i;
	
	for (i = 0; builtin_mime_types[i].extension != NULL; i++) 
	{
		if ((strcmp(fileext,builtin_mime_types[i].extension))==0)
		{
			client->mimetype=builtin_mime_types[i].mime_type;
		}
	}
	
	if ((strcmp(client->mimetype, "")==0))
	{
		if ((strcmp(client->httpreq.method, "HEAD")==0))
		{
		}
		else
		{
			fclose (pFile);
			_500(client,"no", 1);
			set_blocking(client->socket,false);
			LWP_MutexUnlock (aMutex);
			net_close(client->socket);
			return;
		}
	}
	
	if (client->mimetype == NULL)
	{
		if ((strcmp(client->httpreq.method, "HEAD")==0))
		{
		}
		else
		{
			fclose (pFile);
			_500(client,"no", 1);
			set_blocking(client->socket,false);
			LWP_MutexUnlock (aMutex);
			net_close(client->socket);
			return;
		}
	}
	
	filecontent=buffer;
	buffer=NULL;
	
	client->contlength=lSize;
	printf("Sending\n");
	
	client->httpreq.path = origpath;
	
	headersend(client);
	
	s32 sent = 0;
	
	//printf(filecontent);
	//printf("\n");
	
	if ((strcmp(client->httpreq.method, "HEAD")==0))
	{
	}
	else
	{
		sent = write_exact(client->socket, filecontent, client->contlength);
	}

	sleep(2);
	printf("Sent\n");
	LWP_MutexUnlock (aMutex);
	set_blocking(client->socket,false);
	net_close(client->socket);
	return;
}

static void* reqhandle(void *arg)
{	
	char received[256];
	client_t *client = arg;
	net_recv(client->socket, received, sizeof(received), 0);
	client->httpreq.method=strtok(received, " ");
	client->httpreq.path=strtok(NULL, " ");
	client->httpreq.httpver=strtok(NULL, " ");

	if ((strcmp(client->httpreq.method,"POST")==0))
	{
		if ((strcmp(client->httpreq.httpver, "HTTP/1.1")==0))
		{
			client->http11.statuscode = "501";
			client->http11.status = "Not Implemented";
			socket_printf(client->socket, "%s %s %s\r\n", client->httpreq.httpver, client->http10.statuscode, client->http10.status);
			printf("Client requested with an unsupported HTTP/1.1 method\n");
			usleep(200000);
			net_close(client->socket);
			return NULL;
		}
		else
		{
			client->http11.statuscode = "100";
			client->http11.status = "Not Implemented";
			socket_printf(client->socket, "%s %s %s\r\n", client->httpreq.httpver, client->http10.statuscode, client->http10.status);
			printf("Client requested with an unsupported method\n");
			usleep(200000);
			net_close(client->socket);
			return NULL;
		}
	}
	
	char* newline = strstr(client->httpreq.httpver, "\r\n"); 
	if (newline) *newline = '\0';
	
	printf("File: %s \n",client->httpreq.path);
	
	if ((strcmp(client->httpreq.path,"/favicon.ico"))==0)
	{
		favicon(client);
		free(client);
	}
	else
	{
		sdpage(client);
		free(client);
	}

	return NULL;
}	

void csocket(void)
{
	int port = 80;

	int Connect = net_init();
	if(Connect != 0)
	{
		return;
	}
   
	netSocket = net_socket(AF_INET, SOCK_STREAM, 0);

	if(netSocket == INVALID_SOCKET)
	{
		return;
	}

	memset(&sa, 0, sizeof(struct sockaddr_in)); /* clear our address */
	sa.sin_family= AF_INET;              /* this is our host address */
	sa.sin_port= htons(port);                /* this is our port number */
    sa.sin_addr.s_addr = INADDR_ANY;
	
	if(net_bind(netSocket,(struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		return;
	}
	
	while (net_listen(netSocket, 1) == SOCKET_ERROR);
	
	socket_ready=true;
}

static void* waitforhome()
{
	while(1) 
	{
		WPAD_ScanPads();
		PAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		u32 padpressed = PAD_ButtonsDown(0);
		if ( (pressed & WPAD_BUTTON_HOME) || (pressed & WPAD_GUITAR_HERO_3_BUTTON_YELLOW) || (padpressed & PAD_BUTTON_MENU) || (pressed & WPAD_NUNCHUK_BUTTON_C) )
		{
			printf("ByeBye ...");
			sleep(1);			
			exit(0);
		}
		VIDEO_WaitVSync();
	}
}

int main(int argc, char **argv) 
{
	VIDEO_Init();
	PAD_Init();
	WPAD_Init();
	fatInitDefault();
	
	switch(VIDEO_GetCurrentTvMode()) 
	{
		case VI_NTSC:
			rmode = &TVNtsc480IntDf;
			break;
		case VI_PAL:
			rmode = &TVPal528IntDf;
			break;
		case VI_MPAL:
			rmode = &TVMpal480IntDf;
			break;
		default:
			rmode = &TVNtsc480IntDf;
			break;
	}
	
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
		
	printf("\n");
	printf("Wii Web Server v%s\n", servver);
	printf("\n");
	printf("Starting ...\n");
	printf("\n");
	password = " ";
	
	LWP_MutexInit (&aMutex, false);

	socket_ready=false;
	while(socket_ready==false)
	{
		csocket();
	}
		
	int client_s;
	u32 len = sizeof(sa);
	
	LWP_CreateThread (&serverT, waitforhome, NULL, NULL, 0, 80);
	
	printf("Ready ...\n\n");
	
	while(1)
	{	
		client_s=net_accept(netSocket, (struct sockaddr*)&sa, &len);
		client_t *client = malloc(sizeof(client_t));
		printf("Connection\n");
		client->http11.httpver="1.1";
		client->http10.httpver="1.0";
		client->http11.statuscode=" ";
		client->http10.statuscode=" ";
		client->http11.status=" ";
		client->http10.status=" ";
		client->socket = client_s;
		LWP_CreateThread (&serverT, reqhandle, client, NULL, 0, 80);
		//reqhandle(client, message);
	}

    exit(0);
}
