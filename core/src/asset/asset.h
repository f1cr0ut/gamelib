#pragma once

#include<vector>
#include<global_config.h>

namespace core {

	struct APICALL raw_image {
		enum color_type {
			rgb,
			rgba,
			grayscale,
			alpha_grayscale,
		};
		color_type mode { rgb };
		i32 w { 0 };
		i32 h { 0 };
		std::vector<unsigned char> data;

		raw_image & operator=(const raw_image & dst) {
			mode = dst.mode;
			w = dst.w;
			h = dst.h;
			data = dst.data;
			return *this;
		}
	};
}
