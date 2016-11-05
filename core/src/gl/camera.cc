#include<GL/glew.h>
#include"camera.h"

using namespace core;

// init all matrix
camera::camera() {
	mat.identity();
	mod.identity();
	prj.identity();
}

void camera::lookat(const core::vec3 & eye, const core::vec3 & center, const core::vec3 & up) {
	core::vec3 forward{
		center.x - eye.x,
		center.y - eye.y,
		center.z - eye.z,
	};
	auto _up = up;
	forward.normalize();
	auto side = forward ^ up;
	side.normalize();
	_up = side ^ forward;
	core::matrix m{
		side.x, _up.x, -forward.x, 0.0f,
		side.y, _up.y, -forward.y, 0.0f,
		side.z, _up.z, -forward.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	core::matrix mv{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-eye.x, -eye.y, -eye.z, 1.0f
	};
	mod = m * mv;
}

void camera::ortho(f32 l, f32 r, f32 t, f32 b, f32 n, f32 f) {
	core::matrix m{
		2.0f / (r-l), 0, 0, (r+l) / (r-l),
		0, 2.0f / (t-b), 0, (t+b) / (t-b),
		0, 0, -2.0f / (f-n), (f+n) / (f-n) ,
		0, 0, 0, 1
	};
	prj = m;

	ortho_x = (r - l) / 2.0f;
	ortho_y = (t - b) / 2.0f;
}

void camera::perspective(f32 fv, f32 a, f32 n, f32 f) {
	float radian = 2.0f * core::literal::pi * fv / 360.0f;
	float t = 1.0f / tanf(radian / 2.0f);
	core::matrix m{
		1.0f / a, 0, 0, 0,
		0, t, 0, 0,
		0, 0, (f + n) / (n - f) , -1.0f,
		0, 0, (2.0f * f * n) / (n - f), 0
	};
	prj = m;
}

const f32 camera::x_unit_size_on_ortho() const {
	return ortho_x;
}

const f32 camera::y_unit_size_on_ortho() const {
	return ortho_y;
}

void camera::update_pos() {
	mat.rot(rot, angle);
	mat[0] *= scale.x;
	mat[1] *= scale.y;
	mat[2] *= scale.z;
	mat[3][0] = trans.x;
	mat[3][1] = trans.y;
	mat[3][2] = trans.z;
}

void camera::update() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(&prj[0][0]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(&mod[0][0]);
	update_pos();
	glMultMatrixf(&mat[0][0]);
}

void camera::move() {
	update_pos();
	glMultMatrixf(&mat[0][0]);
}

void camera::set_viewport(f32 x, f32 y, f32 w, f32 h) {
	glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
	vp[0] = x;
	vp[1] = y;
	vp[2] = w;
	vp[3] = h;
}

void camera::set_viewport_gl_only(f32 x, f32 y, f32 w, f32 h) {
	glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
}

core::vec3 camera::calc_world_pos_to_screen(const core::vec3 & v) {
	core::vector4<f32> t{ v.x, v.y, v.z, 1 };
	core::vec3 ret{ 0,0,0 };
	t = t * mod * prj;
	t.w = 1.0f / t.w;
	t.x *= t.w;
	t.y *= t.w;
	t.z *= t.w;
	ret.x = (t.x * 0.5f + 0.5f) * vp[2] + vp[0];
	ret.y = (t.y * 0.5f + 0.5f) * vp[3] + vp[1];
	ret.z = (1.0f + t.z) * 0.5f;
	return ret;
}

core::vec3 camera::calc_screen_pos_to_world(const core::vec3 & v) {
	core::vector4<f32> in;
	core::vec3 ret{ 0,0,0 };
	auto a = (prj * mod).inverse();
	in[0] = (v.x - vp[0]) / vp[2] * 2.0f - 1.0f;
	in[1] = (v.y - vp[1]) / vp[3] * 2.0f - 1.0f;
	in[2] = 2.0f * v.z - 1.0f;
	in[3] = 1.0f;
	auto out = in * a;
	ret.x = out[0];
	ret.y = out[1];
	ret.z = out[2];
	return ret;
}

void camera::clear_buffer() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

const core::vector4<f32> & camera::get_viewport() const { 
	return vp;
}
