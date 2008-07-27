#include "Resource.h"

#include "InternalFile.h"

Resource::Resource(void) {
};

Resource::~Resource(void) {
};

Resource *Resource::load(std::string name) {

	string realLocation;

	string errorPrefix("error:");
	size_t errorLocation=name.find(errorPrefix);
	if(errorLocation==0) {
		//todo, add user customable error pages
		realLocation="builtin:/errors/"+name.substr(errorPrefix.length());
	}

	string builtinPrefix("builtin:");
	size_t builtinLocation=realLocation.find(builtinPrefix);
	if(builtinLocation==0) {
		string internalFile=realLocation.substr(builtinPrefix.length());

		return InternalFile::load(internalFile);
	}

	//ops, we ran out of resource locations, time to throw!
	throw NonExistantResource();
};

ForbiddenResource::~ForbiddenResource() {};
