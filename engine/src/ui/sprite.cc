#include<cstring>
#include"sprite.h"

using namespace engine;

namespace {

	// we are using opengl 3.0, glsl version is 1.3.
	// this is very old version on 2016, but we will able to support most of PC.
	const char * frag =
		"#version 130\n\
		uniform sampler2D texture;\n\
		uniform float alpha;\n\
		void main(void) {\n\
			vec4 color = texture2DProj(texture, gl_TexCoord[0]);\n\
			if(color.a > 0.0) {\n\
				color.a = alpha;\n\
			}\n\
			gl_FragColor = color;\n\
		}\n";

	const char * vert =
		"#version 130\n\
		uniform float uv_u;\n\
		uniform float uv_v;\n\
		vec4 uv; \n\
		void main(void) {\n\
			uv = gl_MultiTexCoord0; \n\
			uv.x -= uv_u;\n\
			uv.y += uv_v;\n\
			gl_TexCoord[0] = gl_TextureMatrix[0] * uv;\n\
			gl_Position = ftransform(); \n\
		}\n";
}

bool sprite::init(
	const char * img,
	const core::vector4<f32> & color,
	const core::camera * c
)
{
	if( ! img || ! c) {
		return false;
	}
	core::vector4<f32> colors[4];
	for(auto & i : colors) {
		i.r = color.r;
		i.g = color.g;
		i.b = color.b;
		i.a = color.a;
	}
	auto & vp = c->get_viewport();
	if(strstr(img, "jpg") != nullptr) {
		core::jpg j;
		if( ! j.load(img)) {
			return false;
		}
		texture::assign_texture(j, colors);
		texture::set_original_size(j, c->x_unit_size_on_ortho(), vp[2], vp[3]);
	}
	else if(strstr(img, "png") != nullptr) {
		core::png p;
		if( ! p.load(img)) {
			return false;
		}
		texture::assign_texture(p, colors);
		texture::set_original_size(p, c->x_unit_size_on_ortho(), vp[2], vp[3]);
	}
	else {
		return false;
	}
	sprite_base_shader::start_link();
	sprite_base_shader::attach_vertex(vert, (u32)strlen(vert));
	sprite_base_shader::attach_fragment(frag, (u32)strlen(frag));
	sprite_base_shader::end_link();
	sprite_base_shader::init_shader_id();

	return true;
}


bool sprite::init(const char * img,	const core::camera * c) {
	core::vector4<f32> cl{ 1,1,1,1 };
	return init(img, cl, c);
}

void sprite::draw() {
	sprite_base_shader::begin();
	sprite_base_shader::assign_a(alpha);
	sprite_base_shader::assign_u(u);
	sprite_base_shader::assign_v(v);
	texture::draw();
	sprite_base_shader::end();
}
