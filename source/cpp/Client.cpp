#include "Client.h"

Client::Client(SOCKET s) : numRequests(0), lastRequest(0), socket(s) {
}

Client::~Client(void) {
}
