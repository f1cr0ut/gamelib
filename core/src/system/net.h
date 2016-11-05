#pragma once

#include<string>
#include<memory>
#include<global_config.h>

namespace core {
	class APICALL net final {
		struct Impl;
		std::unique_ptr<Impl> impl;

		std::string response;
		std::string cookie;

	public:
		net();
		~net();

		// simple get 
		// this won't check CA files
		void https_get(const char * url);

		// same as above
		void https_post(const char * url, const char * data);

		// set raw string cookie
		void set_cookie(const char * str);
		
		// get or post response
		const std::string & get_response() const;

		// get cookie from response
		const std::string & response_cookies();

		// delete memory
		void clear_cookie();

		// delete response
		void clear_response();
	};
}


