#include"title.h"

using namespace game;

bool title::load() {
	std::string bg_path = app->get_exe_path() + "res\\title\\title_bg.jpg";
	std::string font_path = app->get_exe_path() + "res\\title\\title_font.otf";

	bg_image.load(bg_path.c_str(), &app->main_camera);
	bg_image.trans.z = -0.99f;


	msg.init(font_path.c_str(), 200, 10, &app->main_camera);
	core::vec4 c[4];
	for(auto & i : c) {
		i.r = i.g = i.b = 0; i.a = 1;
	}
	msg.create("press any key", 0.01f, c);
	msg.pos(0, -0.5f, -0.98f);
	msg.close();
	func = &title::exe;
	return true;
}

bool title::move() {
	return true;
}

void title::draw() {
	bg_image.draw();
	msg.draw();
}

bool title::exe() {
	if( ! move()) {
		return false;
	}
	draw();
	return true;
}

bool title::update() {
	return (this->*func)();
}
