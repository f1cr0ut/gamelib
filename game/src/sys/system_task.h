#pragma once

#include<engine_header.h>

namespace game {
	class APICALL system_task : public engine::scene {

		engine::mainloop * app{ nullptr };

	public:
		DISALLOW_COPY_AND_ASSIGN(system_task);
		system_task() = delete;
		system_task(engine::mainloop * app_core) : app(app_core) {}
		virtual ~system_task() {}

		virtual bool update() override;
	};
}