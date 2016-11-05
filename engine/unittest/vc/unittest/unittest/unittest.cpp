#include<iostream>
#include<thread>
#include<chrono>
#include<engine_header.h>

using namespace std;

namespace {
	struct clicker {
		int n{0};
		void click() {
			cout << n << endl;
			++n;
		}

		void onmouse() {
			cout << n << endl;
		}
	};
}

/*
void fontbuildtest() {

	//---------------------

	core::screen monitor;
	core::input keymouse;
	core::camera maincam;
	core::fps fps_keeper;

	monitor.open("font_building_test", 1280, 720, false);
	keymouse.set_target_screen(&monitor);
	maincam.set_viewport(0, 0, 1280, 720);
	maincam.ortho(-1, 1, 1, -1, -1, 1);
	maincam.update();
	fps_keeper.set_wait_time(16667);
	fps_keeper.vsync(true);
	fps_keeper.force_wait(false);

	//---------------------

	engine::fontbuilder builder;

	const char * strings1 = u8"゜epistnpk!?,.列テ◯１1①Yあいう。お゜；＠：、」aA";
	core::texture splite[33];

	char pass[256]={0};
	core::get_exe_pass(pass);
	sprintf(pass, "%stest.ttf", pass);

	cout << "path is " << pass << endl;
	if( ! builder.open(pass, 200, 10, &maincam)) {
		cout << "fafiled font open" << endl;
	}
	else {
		cout << "font open ok" << endl;
	} 

	builder.build_string((u8*)strings1, splite);
	builder.adjust_font_coordinate(splite, 33, -0.75f, -0.5f, 0.01f);


	//---------------------

	while(true) {
		monitor.swap_buffer();
		monitor.polling();
		maincam.clear_buffer();

		for(auto & i : splite) {
			i.draw();
		}

		fps_keeper.wait();
		if(monitor.should_close() || keymouse.button(core::INPUT_KEY_ESCAPE)) {
			break;
		}
	} 

	//---------------------
}
*/

void spritetest() {

	//---------------------

	core::screen monitor;
	core::input keymouse;
	core::camera maincam;
	core::fps fps_keeper;

	monitor.open("font_building_test", 1280, 720, false);
	keymouse.set_target_screen(&monitor);
	maincam.set_viewport(0, 0, 1280, 720);
	auto res = 1280.0f / 720.0f;
	maincam.ortho(-1 * res, 1 * res, 1, -1, -1, 1);
	maincam.update();
	fps_keeper.set_wait_time(16667);
	fps_keeper.vsync(true);
	fps_keeper.force_wait(false);

	//---------------------


	auto && s = core::get_system_info();

	char pass[256] = { 0 };
	core::get_exe_pass(pass);
	sprintf(pass, "%stest.jpg", pass);

	engine::sprite spr;
	spr.init(pass, &maincam);


	f32 alpha = 1;
	f32 time = 0;
	//---------------------

	while (true) {
		monitor.swap_buffer();
		monitor.polling();
		maincam.clear_buffer();

		//---------------------
		spr.u += 0.001f;
		spr.rot.z = 1;
//		spr.angle += 0.01f;
		spr.draw();
		//---------------------
		fps_keeper.wait();
		if(monitor.should_close()) {
			break;
		}
	}

	//---------------------
}


void savetest(const char * n) {
	engine::savedata s;
	s.open("test.db", n);
	
	engine::savedata::schema a {
		{"id", "integer", "primary key autoincrement" },
		{"name", "text", "" },
		{ "body", "text", "" },
		{ "power", "integer", "default 100" }
	};
	s.create_table(a);

	auto && str = core::val_to_str<u32>(1997);

	engine::savedata::coldata q{ "name", "body", "power"};

	engine::savedata::rowdata b{
		{ "'aaa'", "'body1'" , str.c_str()},
		{ "'bbb'", "'body2'" , str.c_str() }
	};

	s.insert_value(q, b);

	/*
	engine::savedata::coldata qq{ "power" }, qw{ "0" };
	s.update_value(qq, qw);
	*/

	engine::savedata::coldata xq{ "*"};
	s.start_select(xq, nullptr);
	char * p = nullptr, *qa = nullptr;
	i32 pw = 0;
	s.find_value(0, &p);
	s.find_value(1, &qa);
	s.find_value(3, pw);
	s.next_row();
	s.find_value(0, &p);
	s.find_value(1, &qa);
	s.find_value(3, pw);
	s.end_select();

//	s.delete_table();
}

struct APICALL foo : public engine::scene {

	engine::sprite spr;

	virtual ~foo(){}

	foo(engine::mainloop * m) {
		char pass[256] = { 0 };
		core::get_exe_pass(pass);
		sprintf(pass, "%stest.jpg", pass);

		spr.init(pass, &m->main_camera);
	}


	virtual bool update() override {
		spr.u += 0.01f;
		spr.draw();
		return true;
	}
};

struct APICALL main_test : public engine::scene {

	main_test() {}

	engine::sprite spr;

	engine::mainloop * c;

	int time = 0;

	main_test(engine::mainloop * m) {

		char pass[256] = { 0 };
		core::get_exe_pass(pass);
		sprintf(pass, "%stest.png", pass);

		spr.init(pass, &m->main_camera);
		c = m;
	}

	virtual ~main_test() {}

	virtual bool update() override {
//		spr.u += 0.01f;
		spr.draw();
		if(++time > 180) {
//			auto p = std::unique_ptr<foo>(new foo(c));
			c->add(new foo(c));
			return false;
		}
		return true;
	}
};

void main_app_test() {
	auto app = new engine::mainloop("main_test", 1280.0f, 720.0f);
	app->add(new main_test(app));
	app->run();
	delete app;
}

int main() {
	main_app_test();
//	spritetest();
//	savetest("test");
//	savetest("boo");
	return 0;
}
