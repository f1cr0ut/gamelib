#pragma once

#include<global_config.h>

#if defined(OS_WIN)
	#include<windows.h>
#endif

namespace core {

	class APICALL shared_object {
		#if defined(OS_WIN)
			HMODULE handle { NULL };
		#elif defined(OS_LINUX)
			void * handle { nullptr };
		#else
			#error shared object cannot handle function
		#endif
	public:

		shared_object() = default;
		shared_object(const char * library); 
		virtual ~shared_object(); 
		bool open(const char * library); 
		void * get_func_address(const char * name); 
		void close();
	};
}
