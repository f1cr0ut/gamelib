#pragma once

#include<vector>
#include<core_header.h>

namespace engine {

	class APICALL sprite : public core::texture, public core::sprite_base_shader {
	public:

		f32 u{ 0 };
		f32 v{ 0 };
		f32 alpha{ 1 };

		bool init(
			const char * img,
			const core::vector4<f32> & color,
			const core::camera * c
		);

		bool init(const char * img, const core::camera * c);

		void draw();
	};
}
