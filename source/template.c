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
#include <mxml.h>

//Built in Files
#include "inbuilt.h"

//Server includes
#include "mimes.h"      //Mime types
#include "header.h"

#include "common.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

int client;             //ID for connected client, much like a socket number
s32 netSocket;

//! Current server version
char* servver="0.77";

//! Settings file path
char* settingsfile = "fat0:/data/settings/wiiweb.xml";
//! Inits 404
char* c_404="/four04.html";    // Custom 404 file path
//! Inits 500
char* c_500="/five00.html";    // Custom 500 file path
//! Inits denied
char* c_denied="/denied.html"; // Custom denied file path

void sdpage(void);

//! Timezone for Date: header response
int server_timezone = 0; // Set your timezone in relation to GMT time (in this case GMT + 5)

extern char* readfile(char* filename);
struct sockaddr_in sa;

//! How many clients are connected
int clients=0;
//! The max number of clients
int maxclients=10;

//! If the socket has been opened or not
bool socket_ready;
int four04ercount=0;

//Files
FILE* pFile;
long lSize;
char * buffer;
size_t result;

//! Sends headers
void headersend(void)
{
	if ((strcmp(http10.statuscode, " "))==0)
	{
		http10.statuscode="200";
	}
	if ((strcmp(http11.statuscode, " "))==0)
	{
		http11.statuscode="200";
	}
	if ((strcmp(http10.status, " "))==0)
	{
		http10.status="OK";
	}
	if ((strcmp(http11.status, " "))==0)
	{
		http11.status="OK";
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

	if ((strcmp(httpreq.httpver,"HTTP/1.1"))==0)
	{
		socket_printf(client, "%s %s %s\r\n", httpreq.httpver, http11.statuscode, http11.status);
		socket_printf(client, "Date: %s, %s %s %s %s GMT\r\n", date_time[0], date_time[1], date_time[2], year, date_time[3]);
		socket_printf(client, "Server: WiiWebServer %s\r\n", servver);
		socket_printf(client, "Host: 192.168.0.3\r\n");
		socket_printf(client, "Content-Type: %s\r\n", mimetype);
		socket_printf(client, "Content-Length: %d\r\n",contlength);
		socket_printf(client, "\r\n");
	}
	if ((strcmp(httpreq.httpver,"HTTP/1.0"))==0)
	{
		socket_printf(client, "%s %s %s\r\n", httpreq.httpver, http10.statuscode, http10.status);
		socket_printf(client, "Date: %s, %s %s %s %s GMT\r\n", date_time[0], date_time[1], date_time[2], year, date_time[3]);
		socket_printf(client, "Server: WiiWebServer %s\r\n", servver);
		socket_printf(client, "Content-Type: %s\r\n", mimetype);
		socket_printf(client, "Content-Length: %d\r\n",contlength);
		socket_printf(client, "\r\n");
	}
	
	http10.statuscode=" ";
	http11.statuscode=" ";
	http10.status=" ";
	http11.status=" ";
}

void server(void);

//! Loads stuff from wiiweb.xml
char* LoadXmlFile(char* tag,char* node)
{
	FILE *fp;
	mxml_node_t *tree;
	mxml_node_t *data;
	//mxml_node_t *group;

	char* filename = settingsfile;

	fp = fopen(filename, "r");
   
	if (fp==NULL)
	{
		fclose(fp);
		return "nofile";
	}
   
	tree = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK);
	fclose(fp);
	data = mxmlFindElement(tree, tree, tag, NULL, NULL, MXML_DESCEND);
  
	char* setting = (char*)mxmlElementGetAttr(data,node);
	//mxmlDelete(group);
	mxmlDelete(data);
	mxmlDelete(tree);
   
	return setting;
}

//! Deals with //quit
void ret2loader(void)
{
	bool allowed = false;;
	char* password = LoadXmlFile("settings","password");
	if ((strcmp(password, "nofile")==0) || (strcmp(httpreq.path, "//quit")==0) || !strstr(httpreq.path, "?"))
	{
		allowed=scheck();
	}
	else
	{
		char* backuppath = httpreq.path;
		char* enteredpass = strtok(httpreq.path, "?");
		enteredpass = strtok(NULL, "?");
		httpreq.path = backuppath;
		if ((strcmp(password, enteredpass)==0))
		{
			allowed=true;
		}
		else
		{
			allowed=false;
		}
	}
	if (allowed==true)
	{
		contlength=quit_html_size;
		printf("Sending\n");
		mimetype="text/html";
		headersend();
		net_write(client,quit_html,contlength);
		printf("Sent\n");
		usleep(200000);
		VIDEO_WaitVSync();
		net_close(netSocket);
		net_close(client);
		allowed=false;
		exit(0);
	}
	else
	{
		contlength=denied_html_size;
		printf("Sending\n");
		mimetype="text/html";
		headersend();
		net_write(client,denied_html,contlength);
		printf("Sent\n");
		usleep(200000);
		VIDEO_WaitVSync();
		net_close(client);
		return;
	}
}

//! Deals with favicon
void favicon(void)
{
	contlength=favicon_ico_size;
	printf("Sending favicon\n");
	mimetype="icon/x-icon";
	headersend();
	net_write(client,favicon_ico,contlength);
	printf("Sent\n");
	usleep(200000);
	VIDEO_WaitVSync();
	net_close(client);
	return;
}

//! Sends error 500 pages
void _500(char* forcd)
{
	http10.statuscode="500";
	http11.statuscode="500";
	http10.status="Internal Server Error";
	http11.status="Internal server Error";
	if ((strcmp(forcd,"yes")==0))
	{
		contlength=_500_html_size;
		mimetype="text/html";
		headersend();
		net_write(client, _500_html, _500_html_size);
		printf("Sent\n");
		usleep(200000);
		VIDEO_WaitVSync();
		net_close(client);
	}
	else
	{
		if ((strcmp(c_500,"nofile")==0))
		{
			contlength=_500_html_size;
			mimetype="text/html";
			headersend();
			net_write(client, _500_html, _500_html_size);
			printf("Sent\n");
			usleep(200000);
			VIDEO_WaitVSync();
			net_close(client);
		}
		else
		{
			char path[100] = "fat0:/data/web/five00.html";
	
			pFile = fopen (path, "r");
	
			if (pFile!=NULL) 
			{   
				fseek (pFile , 0 , SEEK_END);
				lSize = ftell (pFile);
				rewind (pFile);
				buffer = (char*) malloc (sizeof(char)*lSize);
				if (buffer == NULL) {fputs ("   Memory error",stderr); exit (2);}
				result = fread (buffer,1,lSize,pFile);
				if (result != lSize) {fputs ("   Reading error",stderr); exit (3);}
				fclose (pFile); 
				mimetype="text/html";
				char* filecontent=buffer;
				buffer=NULL;
				contlength=lSize;
				printf("Sending\n");
				headersend();
				if ((strcmp(httpreq.method, "HEAD")==0))
				{
				}
				else
				{
					write_exact(client, filecontent, contlength);
				}
				printf("Sent\n");
				usleep(1000000);
				VIDEO_WaitVSync();
				net_close(client);
				return;
			}
			else
			{
				contlength=_500_html_size;
				mimetype="text/html";
				headersend();
				net_write(client, _500_html, _500_html_size);
				printf("Sent\n");
				usleep(200000);
				VIDEO_WaitVSync();
				net_close(client);
			}
		}
	}
}

//! Sends 404 error pages
void _404(char* forcd)
{
	http10.statuscode="404";
	http11.statuscode="404";
	http10.status="Not Found";
	http11.status="Not Found";
	
	if ((strcmp(forcd,"yes")==0))
	{
		contlength=_404_html_size;
		mimetype="text/html";
		headersend();
		net_write(client, _404_html, _404_html_size);
		printf("Sent\n");
		usleep(200000);
		VIDEO_WaitVSync();
		net_close(client);
	}
	else
	{
		if ((strcmp(c_404,"nofile")==0))
		{
			contlength=_404_html_size;
			mimetype="text/html";
			headersend();
			net_write(client, _404_html, _404_html_size);
			printf("Sent\n");
			usleep(200000);
			VIDEO_WaitVSync();
			net_close(client);
		}
		else
		{
			char path[100] = "fat0:/data/web/four04.html";
	
			pFile = fopen (path, "r");
	
			if (pFile!=NULL) 
			{   
				fseek (pFile , 0 , SEEK_END);
				lSize = ftell (pFile);
				rewind (pFile);
				buffer = (char*) malloc (sizeof(char)*lSize);
				if (buffer == NULL) {fputs ("   Memory error",stderr); exit (2);}
				result = fread (buffer,1,lSize,pFile);
				if (result != lSize) {fputs ("   Reading error",stderr); exit (3);}
				fclose (pFile); 
				mimetype="text/html";
				char* filecontent=buffer;
				buffer=NULL;
				contlength=lSize;
				printf("Sending\n");
				headersend();
				if ((strcmp(httpreq.method, "HEAD")==0))
				{
				}
				else
				{
					write_exact(client, filecontent, contlength);
				}
				printf("Sent\n");
				usleep(1000000);
				VIDEO_WaitVSync();
				net_close(client);
				return;
			}
			else
			{
				contlength=_404_html_size;
				mimetype="text/html";
				headersend();
				net_write(client, _404_html, _404_html_size);
				printf("Sent\n");
				usleep(200000);
				VIDEO_WaitVSync();
				net_close(client);
			}
		}
	}
	return;
}


//! Sends pages and files located on the front SD
void sdpage()
{
	char* filecontent = "TEST";
	
	if ((strcmp(httpreq.path,"/")==0))
	{
		httpreq.path="/index.html";
	}
	
	if ((strcmp(httpreq.path, "/index")==0))
	{
		strcat(httpreq.path, ".html");
	}
	
	if (strstr(httpreq.path, "./") || strstr(httpreq.path, "../")) 
	{
		_404("no");
		return;
	}
	
	if (strstr(httpreq.path, "wiiweb")) {
		_404("no");
		return;
	}
	
	char* space_tok= "";
	char* space_replace = "";
	
	space_tok = strtok (httpreq.path,"%20");
	space_replace = space_tok;
	while (space_tok != NULL) {
		space_tok = strtok (NULL, "%20"); 
		if (space_tok != NULL) {
			strcat(space_replace, " ");
			strcat(space_replace, space_tok);
		}
	}
	httpreq.path = space_replace;
	
	char path[100] = "fat0:/data/web";
	strcat(path,httpreq.path);
	
	pFile = fopen (path, "r");
	
	if (pFile!=NULL) 
	{   
		fseek (pFile , 0 , SEEK_END);
		lSize = ftell (pFile);
		rewind (pFile);
		buffer = (char*) malloc (sizeof(char)*lSize);
		if (buffer == NULL) {fputs ("   Memory error",stderr); exit (2);}
		result = fread (buffer,1,lSize,pFile);
		if (result != lSize) {fputs ("   Reading error",stderr); exit (3);}
		fclose (pFile); 
	}
	else if (pFile==NULL) 
	{
		if ((strcmp(httpreq.method, "HEAD")==0))
		{
		}
		else
		{
			fclose (pFile);
			_404("no");
			return;
		}
	}
	
	char* fileext = strtok(httpreq.path,".");
	fileext = strtok(NULL,".");
	int i;
	
	for (i = 0; builtin_mime_types[i].extension != NULL; i++) 
	{
		if ((strcmp(fileext,builtin_mime_types[i].extension))==0)
		{
			mimetype=builtin_mime_types[i].mime_type;
		}
	}
	
	if ((strcmp(mimetype, "")==0))
	{
		if ((strcmp(httpreq.method, "HEAD")==0))
		{
		}
		else
		{
			fclose (pFile);
			_500("no");
			return;
		}
	}
	
	if (mimetype == NULL)
	{
		if ((strcmp(httpreq.method, "HEAD")==0))
		{
		}
		else
		{
			fclose (pFile);
			_500("no");
			return;
		}
	}
	
	filecontent=buffer;
	buffer=NULL;
	
	contlength=lSize;
	printf("Sending\n");
	headersend();
	if ((strcmp(httpreq.method, "HEAD")==0))
	{
	}
	else
	{
		write_exact(client, filecontent, contlength);
	}
	printf("Sent\n");
	usleep(2000000);
	VIDEO_WaitVSync();
	net_close(client);
	return;
}

//! A very poor request handler
void reqhandle(void)
{	
	httpreq.method=strtok(message, " ");
	httpreq.path=strtok(NULL, " ");
	httpreq.httpver=strtok(NULL, " ");
	
	if ((strcmp(httpreq.method,"POST")==0))
	{
		if ((strcmp(httpreq.httpver, "HTTP/1.1")==0))
		{
			http11.statuscode = "501";
			http11.status = "Not Implemented";
			socket_printf(client, "%s %s %s\r\n", httpreq.httpver, http10.statuscode, http10.status);
			printf("Client requested with an unsupported HTTP/1.1 method\n");
			usleep(200000);
			net_close(client);
			return;
		}
	}
	
	char* newline = strstr(httpreq.httpver, "\r\n"); 
	if (newline) *newline = '\0';
	
	printf("File: %s \n",httpreq.path);
	
	if (strstr(httpreq.path, "//quit")) 
	{
		ret2loader();
	}
	else if ((strcmp(httpreq.path,"/favicon.ico"))==0)
	{
		favicon();
	}
	else
	{
		sdpage();
	}

	return;
}	

//!  Listens on socket
void server(void)
{
	printf("Ready\n");
	u32 len = sizeof(sa);
	client=net_accept(netSocket, (struct sockaddr*)&sa, &len);
	printf("Connection\n");
	net_recv(client, message, sizeof(message), 0);
	reqhandle();
}

//! Opens the socket onto port 80
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

//! Main
int main(int argc, char **argv) 
{

	VIDEO_Init();
	PAD_Init();
	WPAD_Init();
	fatInitDefault();
	
	http10.httpver="1.0";
	http11.httpver="1.1";
	http10.statuscode=" ";
	http11.statuscode=" ";
	http10.status=" ";
	http11.status=" ";
	
	switch(VIDEO_GetCurrentTvMode()) {
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
	char* password = LoadXmlFile("settings","password");
	char* c_404=LoadXmlFile("pages","f04");
	char* c_500=LoadXmlFile("pages","f00");
	char* c_denied=LoadXmlFile("pages","denied");
	printf("%s\n",password);
	printf("%s\n",c_404);
	printf("%s\n",c_500);
	printf("%s\n",c_denied);
	printf("\n");
	printf("Starting ...\n");
	printf("\n");
	
	socket_ready=false;
	while(socket_ready==false)
	{
		csocket();
	}
	
	while(1)
    {
		server();
    }

    exit(0);
}
