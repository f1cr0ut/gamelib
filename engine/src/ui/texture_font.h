#pragma once

#include<core_header.h>
#include"fontbuilder.h"

namespace engine {

	// just for the one line
	class APICALL texture_font final {
		fontbuilder handle;
		std::vector<core::texture> characters;

	public:
		texture_font() = default;
		~texture_font();
		bool init(const char * file, u32 dpi, u32 point, const core::camera * c);
		// str must be utf8
		bool create(const char * str, f32 space, const core::vector4<f32> * colors = nullptr);

		void draw();

		void close();

		// from center + (x,y,z)
		// center of z is set as 0
		void pos(f32 x, f32 y, f32 z);

		const std::vector<core::texture> & get_chara_image() const {
			return characters;
		}
		std::vector<core::texture> & get_chara_image() {
			return characters;
		}
	};
}
