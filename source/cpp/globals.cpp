#include "globals.h"

string serverName("New Wii httpd");

lwp_t displayThread;
lwp_t listenThread;

bool volatile listenForConnections=false;
bool volatile serverOffline=false;
