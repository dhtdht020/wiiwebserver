struct http10header
{
	char* httpver;
	char* statuscode;
	char* status;
	char* mimetype;
	char* length;
};

struct http11header
{
	char* httpver;
	char* statuscode;
	char* status;
	char* date;
	char* mimetype;
	char* length;
	char* server;
	char* host;
};

struct httprequest //Very basic
{
	char* method;
	char* path;
	char* httpver;
};

char* httpver;
char* httpverss;
char* mimetype;
int contlength;

char message[256];

struct http10header http10;
struct http11header http11;
struct httprequest httpreq;

struct client_struct {
	s32 socket;
	struct sockaddr_in address;
	struct httprequest http_request;
	struct http11header http11_header;
	struct http10header http10_header;
};


typedef struct client_struct client_t;
