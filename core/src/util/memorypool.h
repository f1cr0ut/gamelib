#pragma once

#include<vector>
#include<global_config.h>

namespace core {
	class APICALL fixed_memory_pool {

	public:

		struct elem {
			i32 id;
			elem * prev { nullptr };
			elem * next { nullptr };
			void * work { nullptr };
		};

		void init(u32 area_size, u32 num);

		elem * pop();
		void push(elem * p);
		void clear();
		u32 availables() const;
	private:
		std::vector<u8> memory;
		struct elem; 
		elem * wait { nullptr };
		elem * active { nullptr };
		u32 num_wait { 0 };
	};
}
