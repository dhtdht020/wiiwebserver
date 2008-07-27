#include "request.h"
#include "Client.h"

#include <boost/lexical_cast.hpp>

#include "globals.h"

extern "C" {
#include <network.h>
}

#include "InternalFile.h"

extern InternalFile epicFail;

Request::Request(Client *c) : client(c),cacheControll() {//the constructor for the class
};

void Request::doRequest() {
	try {
		readRequest();

		if(requestMethod!="GET" && requestMethod!="HEAD" && requestMethod!="POST") {
			loadErrorReply(501);
		} else if(serverOffline) {
			loadErrorReply(503);
		} else {

			try {
				try {
					resource=Resource::load(resourceName);



				} catch (const NonExistantResource &) {
					loadErrorReply(404);
				} catch (const InvalidRequestMethod &) {
					loadErrorReply(405);
					resource->setAllowHeader(*this);
				} catch (const AuthorizationRequired &) {
					loadErrorReply(401);
				} catch (const ForbiddenResource &) {
					loadErrorReply(403);
				}
			} catch (const NonExistantResource &) {//in case the error page couldn't be found
				resource = new InternalFile(InternalFile::epicFail);
			}

		}

	} catch (const InvalidRequest &) {
		loadErrorReply(400);
	}

#if 0

	enum { NO_RANGE, SINGLE_RANGE, MULTIPLE_RANGES } rangeMode=NO_RANGE;

	map<string,string>::iterator rangeIter;
	rangeIter=requestHeaders.find("Range");

	if(rangeIter!=requestHeaders.end() && replyNumber<300) {
		string &headerValue= rangeIter->second;

		unsigned int firstByte=0;

		//range parsing goes here

		if(firstByte>replyBody.length()) {
			loadErrorReply(416);
		} else {
			rangeMode=SINGLE_RANGE;
		}
	}

	if(rangeMode==SINGLE_RANGE) {
		resource->loadBuffer(replyBody,0,0);//todo replace zeros with proper variables
	} else {
		resource->loadBuffer(replyBody);
	}
#else
	replyHeaders["Accept-Ranges"]="none";
#endif


	//lastminute header work, stuff that directly relates to the final request body
	replyHeaders["Content-lenght"]=boost::lexical_cast<std::string>(replyBody.length());
	replyHeaders["Server"]=serverName;

		map<string,string>::iterator requestHeaderIter;

	bool mustClose= httpVersion.minor<1;

	if(!mustClose) {
		//note this example on how to read from a map
		//this is done because it is possible that the key doesn't have an element
		//operator[] would have created the element in that case, not what we wanted

		requestHeaderIter=requestHeaders.find("Connection");
		if(requestHeaderIter!=requestHeaders.end() && requestHeaderIter->second=="close") {
			mustClose=true;
		}
	}

	if(mustClose) {
		replyHeaders["Connection"]="close";
	}


	sendReply();

	if(mustClose) {
		client->close();
	}

};

Request::CacheControll::CacheControll() : cacheMode(PUBLIC),mustRevalidate(false),noTransform(false) {
};

void Request::readRequestHeaders() {
};

void Request::loadErrorReply(const unsigned int replyNumber_p) {
	replyNumber=replyNumber_p;
	if(resource) {
		delete resource;
	}

	resource=Resource::load("error:/"+boost::lexical_cast<std::string>(replyNumber_p));
}

void Request::sendReply() {
	sendReplyHeaders();

	if(requestMethod!="HEAD") {


	}
};

void Request::sendReplyHeaders() {
	map<string,string>::const_iterator endItr=replyHeaders.end();
	for(map<string,string>::const_iterator itr=replyHeaders.begin();itr!=endItr;itr++) {
		string outLine=itr->first + ": " +itr->second;
		net_send(client->socket,outLine.c_str(),outLine.length(),0);
	}
};

Request::~Request() {
	if(resource) {
		delete resource;
	}
}

void Request::readRequest() {
	//read the status line here
	readRequestHeaders();

	if(requestMethod=="POST") {
		//read request body
	}

}
