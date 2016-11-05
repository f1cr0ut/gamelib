#include<fstream>
#include<vector>
#include<iostream>
#include<GL/glew.h>
#include"gltex.h"
#include"../asset/texture.h"

using namespace core;

namespace {

	struct vbo_vc {
		f32 vertex[3];
		f32 color[4];
	};

	struct vbo_vcu {
		f32 vertex[3];
		f32 color[4];
		f32 uv[2];
	};
}

//========================================================================
//
//========================================================================

gl2D::gl2D() {
	glGenBuffers(1, &vertex_handle);
	mat.identity();
}

gl2D::~gl2D() {
	glDeleteBuffers(1, &vertex_handle);
}

void gl2D::update() {

	mat.rot(rot, angle);

	mat[0] *= scale.x;
	mat[1] *= scale.y;
	mat[2] *= scale.z;
	mat[3][0] = trans.x;
	mat[3][1] = trans.y;
	mat[3][2] = trans.z;

	glMultMatrixf(&mat[0][0]);
}

//========================================================================
//
//========================================================================

void plane::assignVBO() {
	vbo_vc points[4];

	points[0].vertex[0] = -1;
	points[0].vertex[1] = -1;
	points[0].vertex[2] = 0;

	points[1].vertex[0] = 1;
	points[1].vertex[1] = -1;
	points[1].vertex[2] = 0;

	points[2].vertex[0] = 1;
	points[2].vertex[1] = 1;
	points[2].vertex[2] = 0;

	points[3].vertex[0] = -1;
	points[3].vertex[1] = 1;
	points[3].vertex[2] = 0;

	for(auto & i : points) {
		i.color[0] = 1;
		i.color[1] = 1;
		i.color[2] = 1;
		i.color[3] = 1;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertex_handle);
	glBufferData(GL_ARRAY_BUFFER, 112, points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void plane::draw() {
	glPushMatrix();
	update();
	glBindBuffer(GL_ARRAY_BUFFER, vertex_handle);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 28, (void*)0);
	glColorPointer(4, GL_FLOAT, 28, (void*)12);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPopMatrix();
}

//========================================================================
//
//========================================================================

texture::texture() {
	glGenTextures(1, &texture_handle);
}

texture::~texture() {
	glDeleteTextures(1, &texture_handle);
}

void texture::assign_texture(const raw_image & data) {
	core::vector4<f32> c[4];
	for(auto & i : c) {
		i.r = i.g = i.b = i.a = 1;
	}
	assign_texture(data, c);
}

void texture::assign_texture(const raw_image & data, const vector4<f32> * colors) {
	if( ! colors) {
		return;
	}
	vbo_vcu points[4];

	points[0].vertex[0] = -1;
	points[0].vertex[1] = -1;
	points[0].vertex[2] = 0;

	points[1].vertex[0] = 1;
	points[1].vertex[1] = -1;
	points[1].vertex[2] = 0;

	points[2].vertex[0] = 1;
	points[2].vertex[1] = 1;
	points[2].vertex[2] = 0;

	points[3].vertex[0] = -1;
	points[3].vertex[1] = 1;
	points[3].vertex[2] = 0;

	points[0].uv[0] = 0;
	points[0].uv[1] = 1;

	points[1].uv[0] = 1;
	points[1].uv[1] = 1;

	points[2].uv[0] = 1;
	points[2].uv[1] = 0;

	points[3].uv[0] = 0;
	points[3].uv[1] = 0;

	for(int i = 0; i < 4; ++i) {
		points[i].color[0] = colors[i].r;
		points[i].color[1] = colors[i].g;
		points[i].color[2] = colors[i].b;
		points[i].color[3] = colors[i].a;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertex_handle);
	glBufferData(GL_ARRAY_BUFFER, 144, points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// texture settings
	glBindTexture(GL_TEXTURE_2D, texture_handle);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// filtering
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// repeat uv
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_BLEND);
	
	int color = GL_RGB;
	int format = GL_RGB;
	switch(data.mode) {
		case raw_image::rgb: 
			color = GL_RGB8; // 24bit(r:8, g:8, b:8)
			format = GL_RGB; // three colors
			break;
		case raw_image::rgba: 
			color = GL_RGBA8; // 32bit(r:8, g:8, b:8, a:8)
			format = GL_RGBA; // four clolors
			break;
		case raw_image::grayscale: 
			color = GL_ALPHA; // 8bit(transparency only)
			format = GL_ALPHA; // only luminance
			break;
		case raw_image::alpha_grayscale:
			color = GL_RGBA2;
			format = GL_ALPHA;
			break;
		default:
			break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, color, data.w, data.h, 0, format, GL_UNSIGNED_BYTE, &data.data[0]);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::set_original_size(const raw_image & data, f32 unit_x, f32 screen_x, f32 screen_y) {
	scale.x *= unit_x;
	scale.x *= data.w / screen_x;
	scale.y *= data.h / screen_y;
	img_x = (f32)data.w;
	img_y = (f32)data.h;
}

bool texture::load(const char * file, const core::camera * c, const core::vec4 * colors) {
	if( ! file || ! c) {
		return false;
	}
	auto & v = c->get_viewport();
	if(strstr(file, "jpg")) {
		core::jpg f;
		if( ! f.load(file)) {
			return false;
		}
		if(colors) {
			assign_texture(f, colors);
		}
		else {
			assign_texture(f);
		}
		set_original_size(f, c->x_unit_size_on_ortho(), v[2], v[3]);
	}
	else if (strstr(file, "png")) {
		core::png f;
		if( ! f.load(file)) {
			return false;
		}
		if(colors) {
			assign_texture(f, colors);
		}
		else {
			assign_texture(f);
		}
		set_original_size(f, c->x_unit_size_on_ortho(), v[2], v[3]);
	}
	else {
		return false;
	}
	return true;
}

void texture::set_no_repeat() {
	glBindTexture(GL_TEXTURE_2D, texture_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::draw() {
	glPushMatrix();
	update();
	glBindBuffer(GL_ARRAY_BUFFER, vertex_handle);
	glBindTexture(GL_TEXTURE_2D, texture_handle);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 36, (void*)0);
	glColorPointer(4, GL_FLOAT, 36, (void*)12);
	glTexCoordPointer(2, GL_FLOAT, 36, (void*)28);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPopMatrix();
}

//========================================================================
//
//========================================================================

shader::shader() {
}

shader::~shader() {
	clear();
}

void shader::start_link() {
	if(program == 0) {
		program = glCreateProgram();
	}
}

void shader::load_vertex(const char * fname) {
	if( ! fname) {
		return;
	}
	if(vertex_shader > 0) {
		return;
	}
	std::ifstream ifs(fname, std::ios::in);
	if( ! ifs.is_open()) {
		return;
	}
	u32 len = (u32)ifs.seekg(0, std::ios::end).tellg();
	ifs.seekg(std::ios::beg);
	std::vector<char> source(len + 1);
	ifs.read(&source[0], len);
	attach_vertex(&source[0], len + 1);
}

void shader::load_fragment(const char * fname) {
	if( ! fname) {
		return;
	}
	if(fragment_shader > 0) {
		return;
	}
	std::ifstream ifs(fname, std::ios::in);
	if( ! ifs.is_open()) {
		return;
	}
	u32 len = (u32)ifs.seekg(0, std::ios::end).tellg();
	ifs.seekg(std::ios::beg);
	std::vector<char> source(len + 1);
	ifs.read(&source[0], len);
	attach_fragment(&source[0], len + 1);
}

void shader::attach_vertex(const char * source, u32 len) {
	if( ! source) {
		return;
	}
	if(len == 0) {
		return;
	}
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSourceARB(vertex_shader, 1, &source, (int*)&len);
	glCompileShaderARB(vertex_shader);
	glAttachObjectARB(program, vertex_shader); 
	GLint compiled = 0;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled); 
	if(compiled == GL_FALSE) {
#if ! defined(RELEASE_BUILD)
		std::cout << "vertex shader error" << std::endl;
#endif
		GLint size = 0;
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &size);
		if(size > 0) {
			std::vector<GLchar> err(size);
			GLsizei l = 0;
			glGetShaderInfoLog(vertex_shader, size, &l, &err[0]);
#if ! defined(RELEASE_BUILD)
			std::cout << &err[0] << std::endl;
#endif
		}
		clear();
	}
}

void shader::attach_fragment(const char * source, u32 len) {
	if( ! source) {
		return;
	}
	if(len == 0) {
		return;
	}
	fragment_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	glShaderSourceARB(fragment_shader, 1, &source, (int*)&len);
	glCompileShaderARB(fragment_shader);
	glAttachObjectARB(program, fragment_shader);
	GLint compiled = 0;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
#if ! defined(RELEASE_BUILD)
		std::cout << "fragment shader error" << std::endl;
#endif
		GLint size = 0;
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &size);
		if(size > 0) {
			std::vector<GLchar> err(size);
			GLsizei l = 0;
			glGetShaderInfoLog(fragment_shader, size, &l, &err[0]);
#if ! defined(RELEASE_BUILD)
			std::cout << &err[0] << std::endl;
#endif
		}
		clear();
	}
}

void shader::end_link() {
	glLinkProgramARB(program);
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);	
	if(linked == GL_FALSE) {
#if ! defined(RELEASE_BUILD)
		std::cout << "shader link error" << std::endl;
#endif
		GLint size = 0;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &size);
		if(size > 0) {
			std::vector<GLchar> err(size);
			GLsizei l = 0;
			glGetShaderInfoLog(program, size, &l, &err[0]);
#if ! defined(RELEASE_BUILD)
			std::cout << &err[0] << std::endl;
#endif
		}
		clear();
	}
	else {
		glDetachShader(program, vertex_shader);
		glDetachShader(program, fragment_shader);
	}

	glUseProgram(program);
	texture_id = glGetUniformLocation(program, "texture");
	glUseProgram(0);
}

void shader::clear() {
	if(program > 0) {
		glDeleteProgram(program);
		program = 0;
	}
	if(vertex_shader > 0) {
		glDeleteShader(vertex_shader);
		vertex_shader = 0;
	}
	if(fragment_shader > 0) {
		glDeleteShader(fragment_shader);
		fragment_shader = 0;
	}
}

void shader::begin() {
	glUseProgram(program);
	glUniform1i(texture_id, 0);
}

void shader::end() {
	glUseProgram(0);
}

//========================================================================
//
//========================================================================

void sprite_base_shader::init_shader_id() {
	shader::begin();
	id[0] = glGetUniformLocation(program, "alpha");
	id[1] = glGetUniformLocation(program, "uv_u");
	id[2] = glGetUniformLocation(program, "uv_v");
	shader::end();
}

void sprite_base_shader::assign_a(f32 v) {
	glUniform1f(id[0], v);
}

void sprite_base_shader::assign_u(f32 u) {
	glUniform1f(id[1], u);
}

void sprite_base_shader::assign_v(f32 v) {
	glUniform1f(id[2], v);
}