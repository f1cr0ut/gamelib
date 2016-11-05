#pragma once

#include<core_header.h>

namespace engine {

	class APICALL fontbuilder final {

		core::font handle;
		const core::camera * camera { nullptr };

	public:

		fontbuilder();
		~fontbuilder();

		bool open(const char * file, u32 dpi, u32 point, const core::camera * c);

		// str must be utf8
		// font color will be white if colors are null
		// whitespace is same size of 'I' and set alpha = 0 in order to display transparently
		bool build_string(const u8 * str, core::texture * dst, const core::vector4<f32> * colors = nullptr);

		// calculate each character position
		// be sure that this coordinate is based on ortho mode
		// so you have to set projection matrix via cmaera::ortho and camera::update
		// NOTE:
		// return value is end of the right on world coordinate.
		// if something is invalid, return same value as start_x.
		f32 adjust_font_coordinate(core::texture * tex, u32 numtex, f32 start_x, f32 start_y, f32 font_spcae);

		// set center of screen
		// z is set to 0
		void centering(core::texture * tex, u32 numtex);

		void close();
	};
}
