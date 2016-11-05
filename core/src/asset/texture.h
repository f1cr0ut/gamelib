#pragma once 

#include"asset.h"

namespace core {

	class APICALL jpg final : public raw_image {
	public:
		bool load(const char * file);
	};

	class APICALL png final : public raw_image {
	public:
		bool load(const char * file);
	};
}
