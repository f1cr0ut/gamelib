#pragma once

#include<global_config.h>
#include"../asset/asset.h"
#include"../util/core_math.h"
#include"camera.h"

namespace core {

	class APICALL gl2D {
	protected:
		core::matrix mat;
		u32 vertex_handle{ 0 };
		void update();
	public:
		gl2D();
		virtual ~gl2D();
		f32 angle{0};
		core::vec3 rot{0,0,0};
		core::vec3 scale{1,1,1};
		core::vec3 trans{0,0,0};

	};

	class APICALL plane final : public gl2D {
	public:
		plane() {}
		virtual ~plane() {}
		void assignVBO();
		void draw();
	};

	class APICALL texture : public gl2D {

	protected:
		u32 texture_handle{ 0 };
		f32 img_x{ 0 };
		f32 img_y{ 0 };

	public:
		texture();
		virtual ~texture();

		void assign_texture(const raw_image & data);
		void assign_texture(const raw_image & data, const vector4<f32> * colors);

		// this works only ortho mode
		void set_original_size(const raw_image & data, f32 unit_x, f32 screen_x, f32 screen_y);

		// load does assign_texture and set_original_size
		// jpg, png files only.
		bool load(const char * file, const core::camera * c, const core::vec4 * colors = nullptr);

		// default is set as repeat texture.
		// call this after assign_texture if you want to do that.
		void set_no_repeat();

		void draw();
	};

	class APICALL shader {
	protected:
		u32 vertex_shader{ 0 };
		u32 fragment_shader{ 0 };
		u32 program{ 0 };
		i32 texture_id{ -1 };

	public:
		shader();
		virtual ~shader();

		// gen program object of opengl
		// load() and attach() must be in between of start_link() and end_link() functions
		void start_link();

		// load a file as text mode such like fopen(fname, "w")
		void load_vertex(const char * fname);
		void load_fragment(const char * fname);

		// source must be null-terminated
		void attach_vertex(const char * source, u32 len);
		void attach_fragment(const char * source, u32 len);

		// link and detach shader objects if succeeded
		void end_link();

		// draw globject between begin and end
		void begin();
		void end();
		void clear();
	};

	class APICALL sprite_base_shader : public shader {
	protected:
		i32 id[3];
	public:

		sprite_base_shader() {}
		virtual ~sprite_base_shader() {}

		void init_shader_id();

		void assign_a(f32 v); // alpha
		void assign_u(f32 u); // texture_u
		void assign_v(f32 v); // texture_v
	};
}
