#include"shared_object.h"

#if defined(OS_LINUX)
	#include<dlfcn.h>
#endif

using namespace core;

shared_object::shared_object(const char * library) {
	open(library);
}

shared_object::~shared_object() {
	close();
}

bool shared_object::open(const char * library) {
	if( ! library) {
		return false;
	}
	// should close fast
	if(handle) {
		return false;
	}
#if defined(OS_WIN)
	handle = LoadLibrary(library);
#elif defined(OS_LINUX)
	handle = dlopen(library, RTLD_LAZY);
#endif
	// failed to open
	if( ! handle) {
		return false;
	}
	return true;
}

void * shared_object::get_func_address(const char * name) {
	if( ! name) {
		return nullptr;
	}
	if( ! handle) {
		return nullptr;
	}
#if defined(OS_WIN)
	return GetProcAddress(handle, name);
#elif defined(OS_LINUX)
	return dlsym(handle, name);
#endif
}

void shared_object::close() {
	if( ! handle) {
		return;
	}
#if defined(OS_WIN)
	FreeLibrary(handle);
#elif defined(OS_LINUX)
	dlclose(handle);
#endif
}
