#pragma once

#include<core_header.h>

namespace engine {

	struct APICALL scene {
		scene() = default;
		virtual ~scene() {}
		virtual bool update() = 0;
	};
}