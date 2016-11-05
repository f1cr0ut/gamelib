#pragma once

#include<list>
#include<cstring>
#include<core_header.h>
#include"scene.h"

namespace engine {

	class APICALL mainloop {
		std::string config_file;
		std::string boot_log_file;
		std::string exe_path;
		u32 elapsed{ 0 };
		f32 original_window_w{ 0 };
		f32 original_window_h{ 0 };
	public:
		mainloop() = delete;
		DISALLOW_COPY_AND_ASSIGN(mainloop);

		mainloop(const char * title, f32 w, f32 h);
		virtual ~mainloop();

		// app configuration
		struct config_struct {
			enum locate_id : u32 {
				jpn = 1,
				us,
				uk,
				china,
				korea,
			};
			char version[64];
			locate_id location{ jpn };
			bool vsync{ true };
			bool force_wait{ false };
			u32 wait_micro_time{ 16667 };
			u32 window_w{ 1280 };
			u32 window_h{ 720 };

			config_struct() {
				memset(version, 0, sizeof(version));
				strncpy(version, GAME_VERSION_STRING, strlen(GAME_VERSION_STRING));
			}
		};

		config_struct settings;

		core::screen screen;
		core::camera main_camera;
		core::input key_mouse;
		core::fps frame_counter;
		std::list<scene*> scene_list;
		bool continue_game{ true };

		void run();

		void add(scene* task);

		const std::string & get_exe_path() const { return exe_path; }
		const f32 get_original_window_w() const { return original_window_w; }
		const f32 get_original_window_h() const { return original_window_h; }
	};
}