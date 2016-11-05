#include<fstream>
#include<string>
#include<chrono>
#include <ctime>
#if defined(OS_WIN)
	#include<Windows.h>
#endif
#include"mainloop.h"

using namespace engine;

mainloop::mainloop(const char * title, f32 w, f32 h) {
	original_window_w = w;
	original_window_h = h;

	char buf[512] = { 0 };
	core::get_exe_pass(buf);

	exe_path = buf;
	config_file += buf;
	config_file += "boot.config";

	std::string log_path = buf;
	log_path += "log";
	core::mkdir(log_path.c_str());

	boot_log_file += buf;
	boot_log_file += "log\\boot.log";

	std::ifstream ifs(config_file, std::ios::binary);
	// if failed to read, use default value
	if(ifs.is_open()) {
		ifs.read((char*)&settings, sizeof(settings));
		// check version and ifs.read works fine
		if(ifs.fail() || memcmp(settings.version, GAME_VERSION_STRING, strlen(GAME_VERSION_STRING)) != 0) {
			// set default value
			mainloop::config_struct c;
			memcpy(&settings, &c, sizeof(c));
		}
	}

	// create log file
	std::ofstream ofs(boot_log_file, std::ios::out);
	if( ! ofs.is_open()) {
#if defined(OS_WIN)
		MessageBox(NULL, "[error] cannot create log file!\nplease check directory permission", "init error", 0);
#else
		std::cerr << "[error] cannot create log file!\nplease check directory permission" << std::endl;
#endif
	}

	if(screen.open(title, settings.window_w, settings.window_h, false)) {
		key_mouse.set_target_screen(&screen);
		main_camera.set_viewport(0, 0, w, h);
		auto resolution = (f32)settings.window_w / (f32)settings.window_h;
		main_camera.ortho(-1.0f * resolution, 1.0f * resolution, 1.0f, -1.0f, -1.0f, 1.0f);
		main_camera.update();
		main_camera.set_viewport_gl_only(0, 0, (f32)settings.window_w, (f32)settings.window_h);
		frame_counter.set_wait_time(settings.wait_micro_time);
		frame_counter.vsync(settings.vsync);
		frame_counter.force_wait(settings.force_wait);
		screen.set_window_resize_callback([&](i32 w, i32 h) -> void {
			settings.window_h = (u32)h;
			settings.window_w = (u32)w;
		});
		if(ofs.is_open()) {
			auto now = std::chrono::system_clock::now();
			auto time = std::chrono::system_clock::to_time_t(now);
			std::string now_str = std::ctime(&time);
			now_str.pop_back(); // delete end of \n
			ofs << "--- " + now_str + " ---\n";
			ofs << "[sys] location mode " + core::val_to_str(settings.location) + "\n";
			ofs << "[sys] machine " + core::get_system_info() + "\n";
			ofs << "[sys] window size(" + core::val_to_str(w) + ", " + core::val_to_str(h) + ")\n";
			ofs << "[sys] vsync " + core::val_to_str(settings.vsync) + "\n";
			ofs << "[sys] force wait " + core::val_to_str(settings.force_wait) + "\n";
			if(settings.force_wait) {
				ofs << "[sys] wait time per sec" + core::val_to_str(settings.wait_micro_time) + "\n";
			}
		}
	}
	else {
		if(ofs.is_open()) {
			ofs << "[err] failed to open window";
		}
	}
}

mainloop::~mainloop() {
	// deallocate memory manually
	for(auto it = scene_list.begin(); it != scene_list.end();) {
		auto p = (*it);
		it = scene_list.erase(it);
		delete p;
	}
	// save config file
	std::ofstream ofs(config_file, std::ios::binary);
	if(ofs.is_open()) {
		ofs.write((char*)&settings, sizeof(settings));
	}
}

void mainloop::run() {
	while(continue_game) {
		screen.swap_buffer();
		screen.polling();
		main_camera.clear_buffer();
		for(auto it = scene_list.begin(); it != scene_list.end();) {
			if( ! (*it)->update()) {
				auto p = (*it);
				it = scene_list.erase(it);
				delete p;
				continue;
			}
			++it;
		}
		frame_counter.wait();
		if(screen.should_close()) {
			continue_game = false;
			break;
		}
		++elapsed; // count frame
	}
}

void mainloop::add(scene * task) {
	if( ! task) {
		return;
	}
	scene_list.push_back(task);
}