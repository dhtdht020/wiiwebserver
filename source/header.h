struct http10header
{
	char* httpver;
	char* statuscode;
	char* status;
	char* length;
};

struct http11header
{
	char* httpver;
	char* statuscode;
	char* status;
	char* date;
	char* length;
	char* server;
	char* host;
};

struct httprequest
{
	char* method;
	char* path;
	char* httpver;
	char* args;
};

char* httpver;
char* httpverss;
char* mimetype;

struct client_struct {
	s32 socket;
	char* mimetype;
	int contlength;
	struct sockaddr_in address;
	struct httprequest httpreq;
	struct http11header http11;
	struct http10header http10;
};


typedef struct client_struct client_t;
