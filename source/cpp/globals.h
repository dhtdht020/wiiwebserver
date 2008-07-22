#include <string>

extern "C" {
#include <ogcsys.h>
#include <gccore.h>

#include <wiiuse/wpad.h>
}

using std::string;

extern bool volatile serverOffline;
extern bool volatile listenForConnections;
extern string serverName;

extern lwp_t displayThread;
extern lwp_t listenThread;


