#pragma once

#include<global_config.h>
#include"../util/core_math.h"

namespace core {

	class APICALL camera final {

		// camera matrix
		core::matrix mat;

		// projection matrix
		core::matrix prj;

		// modelview matrix
		core::matrix mod;

		// viewport size;[0]=x, [1]=y, [2]=w, [3]=h
		core::vector4<f32> vp{ 0,0,0,0 };
		
		f32 ortho_x{ 0 };
		f32 ortho_y{ 0 };

		void update_pos();

	public:

		camera();

		f32 angle{0};
		core::vec3 rot{0,0,0};
		core::vec3 scale{1,1,1};
		core::vec3 trans{0,0,0};

		// update modelview matrix
		void lookat(const core::vec3 & eye, const core::vec3 & center, const core::vec3 & up);

		// update projection matrix
		void ortho(f32 l, f32 r, f32 t, f32 b, f32 n, f32 f);
		void perspective(f32 fv, f32 a, f32 n, f32 f);

		// for UI stuff
		const f32 x_unit_size_on_ortho() const;
		const f32 y_unit_size_on_ortho() const;

		// update all
		// call this when updated some matrix(s)
		void update();

		// update only camera positon, rotate, and scale
		void move();

		// first of all, we have to set viewport when using camera class, or
		// calc_world_pos_to_screen() and calc_screen_pos_to_world() will failure
		// set original resolution
		void set_viewport(f32 x, f32 y, f32 w, f32 h);

		// set screen resolution
		void set_viewport_gl_only(f32 x, f32 y, f32 w, f32 h);

		// world->screen
		core::vec3 calc_world_pos_to_screen(const core::vec3 & v);

		// screen->world
		core::vec3 calc_screen_pos_to_world(const core::vec3 & v);

		// clear back buffer like color buffer bit, depth buffer bit, and so on
		void clear_buffer();

		const core::vector4<f32> & get_viewport() const;
	};
}
