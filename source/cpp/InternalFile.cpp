#include "InternalFile.h"

InternalFile::InternalFile(const string &contents_p, const string &mimeType_p) : contents(contents_p) {
	mimeType=mimeType_p;
};

InternalFile::~InternalFile(void) {
};

void InternalFile::setAllowHeader(Request&) {
	//todo
};

void InternalFile::loadBuffer(string &outBuffer) {
	outBuffer=contents;
};

void InternalFile::loadBuffer(string &outBuffer,unsigned int minPos,unsigned int maxPos) {
	outBuffer=contents.substr(minPos,maxPos-minPos+1);
};

InternalFile *InternalFile::load(const std::string &name) {
	map<string,InternalFile>::const_iterator itr=files.find(name);
	if(itr!=files.end()) {
		return new InternalFile(itr->second);
	} else {
		throw NonExistantResource();
	}
};

InternalFile::InternalFile(const InternalFile &c) : contents(c.contents) {
	mimeType=c.mimeType;
};

map<string,InternalFile> InternalFile::files;
