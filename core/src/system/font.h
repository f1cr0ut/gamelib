#pragma once

#include<memory>
#include<global_config.h>

namespace core {
	class APICALL font final {
		struct Impl;
		std::unique_ptr<Impl> impl;

	public:
		font();
		~font();
		// load font from file
		// currently i tested just one of the .ttf file
		// otf or some has not tested yet
		bool load(const char * file, u32 dpi, u32 point);

		// generate grayscale(8bit) image of black and white
		void build(const u32 str, raw_image & res, i32 & baseline);

		// release memory
		void close();
	};
}


