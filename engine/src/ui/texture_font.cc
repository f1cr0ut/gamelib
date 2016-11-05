#include"texture_font.h"

using namespace engine;

texture_font::~texture_font() {
	close();
}

bool texture_font::init(const char * file, u32 dpi, u32 point, const core::camera * c) {
	return handle.open(file, dpi, point, c);
}

bool texture_font::create(const char * str, f32 space, const core::vector4<f32> * colors) {
	auto num = (u32)strlen(str);
	characters.resize(num);
	if( ! handle.build_string((u8*)str, &characters[0], colors)) {
		return false;
	}
	handle.adjust_font_coordinate(&characters[0], num, 0, 0, space);
	handle.centering(&characters[0], num);
	return true;
}

void texture_font::draw() {
	for(auto & i : characters) {
		i.draw();
	}
}

void texture_font::pos(f32 x, f32 y, f32 z) {
	auto num = (u32)characters.size();
	handle.centering(&characters[0], num);
	for(auto & i : characters) {
		i.trans.x += x;
		i.trans.y += y;
		i.trans.z += z;
	}
}

void texture_font::close() {
	handle.close();
}