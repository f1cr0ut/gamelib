#pragma once

#include<memory>
#include<global_config.h>

namespace core {

	class APICALL sql final {
		struct Impl;
		std::unique_ptr<Impl> impl;
	public:

		sql();
		~sql();

		bool open(const char * db);
		bool execute(const char * sql);
		void finish();
		void close();

		bool next();
		i32 get_int(u32 i);
		i64 get_i64(u32 i);
		f64 get_f64(u32 i);
		const unsigned char * get_str(u32 i);
		const void * get_data(u32 i);
	};

}
