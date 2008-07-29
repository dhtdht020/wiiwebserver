#include <string>

extern "C" {
#include <ogcsys.h>
#include <gccore.h>

#include <wiiuse/wpad.h>
}

#include <list>
using std::list;

using std::string;

extern bool volatile serverOffline;
extern bool volatile listenForConnections;
extern const string serverName;

extern lwp_t displayThread;
extern lwp_t listenThread;

class Client;

extern list<Client *> clients;

enum ListenStatus { UNINITALIZED, OK, ERROR };

extern volatile ListenStatus listenstatus;


