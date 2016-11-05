#include<cstdlib>
#include<cstring>
#include<iostream>
#include<engine_header.h>
#include"game/src/scene/title/title.h"
#include"game/src/sys/system_task.h"
#include"boot.h"

using namespace game;

namespace {
	void parse_args(int argc, char ** argv) {
		for(int i = 0; i < argc; ++i) {
			auto arg = argv[i];
			DBGPRINT(arg);
			if(strlen(arg) <= 1) {
				continue;
			}
			if(arg[0] == '-') {
				if(strcmp(&arg[1], "clear-conf") == 0) {
					char buf[512] = { 0 };
					core::get_exe_pass(buf);
					std::string conf_file = buf;
					conf_file += "boot.config";
					core::replace_path_slash(&conf_file[0]);
					core::delete_file(conf_file.c_str());
				}
				if(strcmp(&arg[1], "clear-log") == 0) {
					char buf[512] = { 0 };
					core::get_exe_pass(buf);
					std::string log_file;
					std::string log_dir = buf;
					log_dir += "log";
					log_file = log_dir + "\\boot.log";
					core::replace_path_slash(&log_file[0]);
					core::delete_file(log_file.c_str());
					core::delete_dir(log_dir.c_str());
				}
			}
		}
	}
}

int game::boot(int argc, char ** argv) {
	parse_args(argc, argv);	
	auto app = new engine::mainloop("game", 1280.0f, 720.0f);
	app->add(new game::title(app));
	app->run();
	delete app;
	return EXIT_SUCCESS;
}