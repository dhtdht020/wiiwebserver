#include "InternalFile.h"

InternalFile::InternalFile(const string &name_p) : name(name_p) {
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
