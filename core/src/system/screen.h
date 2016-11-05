#pragma once

#include<memory>
#include<functional>
#include<global_config.h>

namespace core {

	class input;

	class APICALL screen final {
		friend class input;

	public:
		screen();
		~screen();

		u32 get_numof_monitors();
		const char * get_monitor_name(u32 index);
		void set_target_monitor(u32 index);
		bool open(
			const char * title,
			u32 w,
			u32 h,
			u32 x,
			u32 y,
			bool fullscreen
		);
		bool open(const char * title, u32 w, u32 h, bool fullscreen);
		void set_screen_size(u32 w, u32 h);
		void get_screen_size(u32 & w, u32 & h);
		void close();
		void polling();
		void swap_buffer();
		bool should_close();
		void set_title(const char * title);
		void set_icon(u32 w, u32 h, void * data);
		void clear_icon();
		void centering(u32 w, u32 h);
		void set_window_resize_callback(const std::function<void(i32, i32)> & callback);


		struct user_pointer {
			f32 mouse_wheel_y{ 0 };
			f32 mouse_wheel_x{ 0 };
			std::function<void(i32, i32)> callback;
		};

		struct Impl;

	private:

		std::unique_ptr<Impl> impl;
		std::unique_ptr<user_pointer> userp_impl;

		bool internal_open(
			const char * title,
			u32 w,
			u32 h,
			u32 x,
			u32 y,
			bool fullscreen
		);

		void * get_window_handle();
	};
}


