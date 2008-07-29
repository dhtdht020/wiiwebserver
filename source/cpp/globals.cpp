#include "globals.h"


const string serverName("New Wii httpd");

lwp_t displayThread=LWP_THREAD_NULL;
lwp_t listenThread=LWP_THREAD_NULL;

bool volatile listenForConnections=true;
bool volatile serverOffline=false;

list<Client *> clients;

volatile ListenStatus listenstatus=UNINITALIZED;

