#pragma once

#include<engine_header.h>

namespace game {
	class APICALL title : public engine::scene {

		core::texture bg_image;
		engine::texture_font msg;

		engine::mainloop * app{ nullptr };
		bool(title::*func)();
		bool load();
		bool exe();

		bool move();
		void draw();

	public:
		DISALLOW_COPY_AND_ASSIGN(title);
		title() = delete;
		title(engine::mainloop * app_core) : app(app_core) , func(&title::load){}
		virtual ~title() {}

		virtual bool update() override;
	};
}