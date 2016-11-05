#include"fontbuilder.h"

using namespace engine;

fontbuilder::fontbuilder() {
}

fontbuilder::~fontbuilder() {
	close();
}

bool fontbuilder::open(const char * file, u32 dpi, u32 point, const core::camera * c) {
	if( ! file || ! c) {
		return false;
	}
	camera = c;
	return handle.load(file, dpi, point);
}

bool fontbuilder::build_string(const u8 * str, core::texture * dst, const core::vector4<f32> * colors) {
	if( ! dst || ! str || ! camera) {
		return false;
	}
	std::vector<u16> utf16str;
	core::utf8_to_utf16((char*)str, utf16str);
	if(utf16str.empty() || utf16str.size() <= 1) {
		return false;
	}
	u32 num = 0;
	while(utf16str[num] != 0) {
		++num;
	}
	// there is utf16 bom in the head so remove that
	--num; 

	// gen image
	for(u32 i = 0, j = 1; i < num; ++i, ++j) {
		core::raw_image img;
		i32 baseline;
		handle.build(utf16str[j], img, baseline);
		// most of fonts does not have whitespace 
		bool white_space = false;
		if(img.w == 0 || img.h == 0 || img.data.empty()) {
			white_space = true;
		}

		// use 'I' for the whitespace size
		if(white_space) {
			std::vector<u16> ft;
			core::utf8_to_utf16(u8"I", ft);
			handle.build(ft[1], img, baseline);
			core::vec4 cc[4];
			memset(&cc, 0, sizeof(cc));
			dst[i].assign_texture(img, cc);
		}
		else {
			// set default(white) color if colors are null
			if( ! colors) {
				dst[i].assign_texture(img);
			}
			else {
				// use font color
				dst[i].assign_texture(img, colors);
			}
		}
		// set no uv repeat
		dst[i].set_no_repeat();

		auto & vp = camera->get_viewport();
		dst[i].set_original_size(img, camera->x_unit_size_on_ortho(), vp[2], vp[3]);

		//set position so as to each character's bottoms are the same line
		dst[i].trans.y = dst[i].scale.y;

		// adjust using baseline
		if(img.h > baseline) {
			dst[i].trans.y -= ((((f32)img.h - (f32)baseline) * 2.0f) / vp[3]);
		}		
		if(img.h < baseline) {
			dst[i].trans.y += ((((f32)baseline - (f32)img.h) * 2.0f) / vp[3]);
		}
	}
	return true;
}

void fontbuilder::close() {
	handle.close();
}

f32 fontbuilder::adjust_font_coordinate(core::texture * tex, u32 numtex, f32 start_x, f32 start_y, f32 font_space) {
	if( ! tex || numtex == 0) {
		return start_x;
	}
	for(u32 i = 0; i < numtex; ++i) {
		tex[i].trans.x += start_x;
		tex[i].trans.y += start_y;
	}
	for(u32 i = 0; i < numtex; ++i) {
		if(i > 0) {
			start_x += tex[i].scale.x;
		}
		tex[i].trans.x = start_x;
		start_x += tex[i].scale.x + font_space;
	}
	return start_x;
}

void fontbuilder::centering(core::texture * tex, u32 numtex) {
	f32 right = tex[numtex - 1].trans.x + tex[numtex - 1].scale.x;
	f32 left = tex[0].trans.x - tex[0].scale.x;
	f32 center = (right + left) / 2.0f;
	for(u32 i = 0; i < numtex; ++i) {
		tex[i].trans.x -= center;
		tex[i].trans.z = 0;
	}
}