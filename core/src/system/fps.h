#pragma once

#include<chrono>
#include<global_config.h>

namespace core {
	class APICALL fps final {
		std::chrono::microseconds wait_time;
		std::chrono::time_point<std::chrono::steady_clock> prev;
		bool have_to_wait { true };
	public:
		fps();
		void vsync(bool sync);
		void set_wait_time(u32 microsec);
		void wait();
		void force_wait(bool waiting);
	};
}


