#include<iostream>
#include<core_header.h>
#include<chrono>
#include<thread>

/*
void fonttest() {
	core::screen s;
	core::input ip;
	s.open("fonttest", 1280, 720, false);
	ip.set_target_screen(s.get_window_handle());
	ip.set_keymap(0, core::INPUT_MOUSE_BUTTON_LEFT);
	ip.set_keymap(0, core::INPUT_MOUSE_BUTTON_RIGHT);

	core::gltex p, pic;
	core::png pv;
	core::raw_image aew;
	core::font f;
	core::glshader sh;
	core::fps fpser;
	fpser.vsync(true);

	sh.start_link();
	sh.load_vertex("transparency.vertex_shader");
	sh.load_fragment("transparency.fragment_shader");
	sh.end_link();

	f.load("test.ttf", 340, 20);
	pv.load("test.png");

	std::vector<u16> ft;
	core::utf8_to_utf16(u8"羅", ft);

	f.build(ft[1], aew);
	p.assign_texture(aew);
	pic.assign_texture(pv);

	core::vec3 v(0, 0, 1.0f), q{ 0.5f,0,0 }, z{ 0.99f, 0.99f, 0.99f };

	while (true) {
//		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		s.polling();
		s.swap_buffer();
		pic.draw();

//		p.scale(z);
		p.rot(v, 0.05f);
		p.trans(q);

//		sh.begin();
		p.draw();
//		sh.end();

		if (ip.button(core::INPUT_KEY_ANY)) {
			break;
		}
	}
}

void pngtest() {
	core::screen s;
	s.open("pngtest", 1280, 720, false);
	core::gltex p;
	core::input ip;
	ip.set_target_screen(s.get_window_handle());
	ip.set_keymap(0, core::INPUT_MOUSE_BUTTON_LEFT);
	ip.set_keymap(0, core::INPUT_MOUSE_BUTTON_RIGHT);
	core::png t;
	t.load("test.png");
	p.assign_texture(t);
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		s.polling();
		s.swap_buffer();
		p.draw();
		if (ip.button(core::INPUT_KEY_ESCAPE) || s.should_close()) {
			break;
		}
	}
}

void jpgtest() {
	core::screen s;
	s.open("jpgtest", 1280, 720, false);
	core::gltex p;
	core::input ip;
	ip.set_target_screen(s.get_window_handle());
	ip.set_keymap(0, core::INPUT_MOUSE_BUTTON_LEFT);
	ip.set_keymap(0, core::INPUT_MOUSE_BUTTON_RIGHT);
	core::jpg t;
	t.load("test.jpg");
	p.assign_texture(t);
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		s.polling();
		s.swap_buffer();
		p.draw();
		if (ip.button(core::INPUT_KEY_ESCAPE) || s.should_close()) {
			break;
		}
	}
}

void streamingtest() {
	core::streaming_player streaming;
	streaming.open("test.ogg");
	streaming.play();
	streaming.set_loop(true);
	int count = 0;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(16));

		if (++count == 120) {
			streaming.volume(0.5f);
		}
		if (count == 240) {
			streaming.pitch(1.1f);
		}
		if (count == 360) {
			streaming.pause();
		}
		if (count == 480) {
			streaming.replay();
		}
		if (count == 600) {
			streaming.pause();
		}
		if (count == 720) {
			streaming.replay();
		}
		if (count == 840) {
			streaming.stop();
		}
		if (count == 960) {
			streaming.close();
			break;
		}
	}
}

void sqltest() {
	core::sql s;
	s.open("test.db");
//	s.execute("create table tbl_test(id integer,name text);");
	s.execute("select * from tbl_test;");
	s.next();
	auto a = s.get_int(0);
	auto p = s.get_str(1);
	s.next();
	if (a == 0) {
		a = 1;
	}
	s.finish();
	s.close();
}

void nettest() {
	core::net ne;
	ne.https_get("https://www.yahoo.com");
	const std::string & response = ne.get_response();
	const std::string & c = ne.response_cookies();
	if( ! response.empty()) {

	}
}

void shadertest() {
	core::screen s;
	s.open("shadertest", 1280, 720, false);
	core::gltex p;
	core::input ip;
	ip.set_target_screen(s.get_window_handle());
	ip.set_keymap(0, core::INPUT_MOUSE_BUTTON_LEFT);
	ip.set_keymap(0, core::INPUT_MOUSE_BUTTON_RIGHT);
	core::jpg t;
	t.load("test.jpg");
	p.assign_texture(t);

	core::glshader shader;
	shader.start_link();
	shader.load_vertex("v.shader");
	shader.load_fragment("v.frag");
	shader.end_link();
	core::vec3 vec{ 0,0,1 }, pos{ 0,0,0 }, sc{0.99f, 1, 1};
	float a = 0;
	bool vsync = true;

	core::fps counter;
	counter.set_wait_time(1667);
//	counter.force_wait(false);
	counter.vsync(true);

	while (true) {
		auto start = std::chrono::high_resolution_clock::now();
		s.polling();
		s.swap_buffer();

		// in order of scale->rot->trans
//		p.scale(sc);

		p.rot(vec, 0.01f);

		pos.x = sinf(a);
		a += 0.02f;
		p.trans(pos);

		shader.begin();
		p.draw();
		shader.end();

		if (ip.button(core::INPUT_KEY_ESCAPE) || s.should_close()) {
			break;
		}
		counter.wait();
	}
}

void soundtest() {
	core::sound s;
	s.open_wav("test.wav");
	s.play();
	float a = 1;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}


void cameratest() {
	core::screen s;
	s.open("cameratest", 1280, 720, false);
	core::input ip;
	ip.set_target_screen(s.get_window_handle());
	core::camera c;
	core::jpg t;
	t.load("test.jpg");
	core::gltex p;
	p.assign_texture(t);

	// call set_viewport first
	c.set_viewport(0, 0, 1280, 720);

	auto q = c.calc_world_pos_to_screen({ 0, 0, 0 });
	q.z = 0;
	auto r = c.calc_screen_pos_to_world(q);
	float aa = 3, cc = 0;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		s.polling();
		s.swap_buffer();
		// scale->rot->trans
		c.rot({ 0,0,1 }, 0.01f);
		c.trans({ 3,0,0 });
		// lookat and perspective and ortho are able to call in any order before update
		c.lookat({ 0,0,aa }, { 0,0,0 }, { 0,1,0 });
		c.perspective(100.0f, 1280.0f / 720.0f, 1.0f, 1000.0f);
		c.update();
		p.draw();
		aa += sinf(cc) * 0.1f;
		cc += 0.75f;
	}
}
*/
void memorytest() {
	core::fixed_memory_pool mp;
	mp.init(4, 4);
	for(int i = 0; i < 2; ++i) {
		auto p = mp.pop();
		if(p->id == 3) {
			std::cout << "OK" << std::endl;
		}
		if(p->next->id == -2 && p->prev->id == -2) {
			std::cout << "OK" << std::endl;
		}
		auto pp = mp.pop();
		if(pp->id == 2) {
			std::cout << "OK" << std::endl;
		}
		if(pp->next->id == 3 && pp->prev->id == -2) {
			std::cout << "OK" << std::endl;
		}
		mp.push(pp);
		mp.push(p);
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for(int i = 0; i < 2; ++i) {
		auto p = mp.pop();
		if(i == 0) {
			if(p->id == 3) {
				std::cout << "OK" << std::endl;
			}
			if(p->next->id == -2 && p->prev->id == -2) {
				std::cout << "OK" << std::endl;
			}
		}
		else {
			if(p->id == 2) {
				std::cout << "OK" << std::endl;
			}
			if(p->next->id == -2 && p->prev->id == -2) {
				std::cout << "OK" << std::endl;
			}
		}
		auto pp = mp.pop();
		if(i == 0) {
			if(pp->id == 2) {
				std::cout << "OK" << std::endl;
			}
			if(pp->next->id == 3 && pp->prev->id == -2) {
				std::cout << "OK" << std::endl;
			}
		}
		else {
			if(pp->id == 3) {
				std::cout << "OK" << std::endl;
			}
			if(pp->next->id == 2 && pp->prev->id == -2) {
				std::cout << "OK" << std::endl;
			}
		}
		mp.push(p);
		mp.push(pp);
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for(int i = 0; i < 5; ++i) {
		 mp.pop();
	}
	if(mp.pop() == nullptr) {
		std::cout << "OK" << std::endl;
	}
}

void postest() {
	core::screen s;
	core::input ip;
	s.open("fonttest", 1280, 720, false);
	ip.set_target_screen(&s);

	core::plane pl;
	pl.assignVBO();
	core::camera c;
	c.set_viewport(0, 0, 1280, 720);
	pl.rot.z = 1;
	c.ortho(-1, 1, 1, -1, 1, 100);
	c.update();

	core::vec3 p1{ 1280, 720, -1 };
	auto a = c.calc_screen_pos_to_world(p1);
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		pl.angle += 0.01f;
		pl.trans.x = sinf(pl.angle);
		pl.draw();
		c.trans.z = sinf(pl.angle * 90) * 0.1f;
		s.swap_buffer();
		s.polling();
		c.clear_buffer();
		c.move();
		f32 xx = 0, yy = 0;
		ip.get_mouse_scroll(xx, yy);
		if (yy == -1) {
//			break;
		}
	}
}

int main() {
	postest();
	return 0;
}
