#include "globals.h"

string serverName("New Wii httpd");

lwp_t displayThread;
lwp_t listenThread;

bool volatile listenForConnections=true;
bool volatile serverOffline=false;
