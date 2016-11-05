#pragma once

#include<core_header.h>

namespace engine {

	class APICALL mouse_event_base {
	protected:
		core::camera * camera{ nullptr };
		core::input * input{ nullptr };
		f32 left{ 0 };
		f32 right{ 0 };
		f32 top{ 0 };
		f32 bottom{ 0 };
	public:
		void init(core::camera * c, core::input * i, f32 l, f32 r, f32 t, f32 b) {
			if( ! c || ! i) {
				return;
			}
			camera = c;
			input = i;
			left = l;
			right = r;
			top = t;
			bottom = b;
		}

		void init(core::camera * c, core::input * i, f32 w, f32 h) {
			if( ! c || ! i) {
				return;
			}
			camera = c;
			input = i;
			left = -w;
			right = w;
			top = h;
			bottom = -h;
		}
	};

	template<class T>
	class APICALL onpush_event final : public mouse_event_base{
		T instance;
		u32 flags{ 0 };
		
		enum {
			none = 0,
			pushed = 1 << 1,
			set_start = 1 << 2,
			released = 1 << 3,
			ready = 1 << 4,
		};

		bool first{ false };
		bool last{ false };

	public:

		void update() {
			if( ! input || ! camera) {
				return;
			}
			core::vec3 p{0, 0, 0};

			bool mouse_flag =
				! input->button(core::INPUT_MOUSE_BUTTON_LEFT) &&
				! input->button(core::INPUT_MOUSE_BUTTON_RIGHT) &&
				! input->button(core::INPUT_MOUSE_BUTTON_MIDDLE);

			if(mouse_flag) {
				if(flags & pushed) {
					flags |= released;
					flags &= ~pushed;
				}
			}
			else {
				flags |= pushed;
				flags &= ~released;
			}

			input->get_mouse(p.x, p.y);
			auto ret = camera->calc_screen_pos_to_world(p);

			// screen origin is left top, so bottom is grater than 0 and y on top of the window will be 0.
			// but world coordinate origin is center; top of the world coordinate on window is 1 and bottom is -1.
			// we have to set inverse value of y-coordinate.
			ret.y *= -1.0f;

			if(flags & pushed) {
				if((flags & set_start) == 0) {
					if(
						left <= ret.x && ret.x <= right &&
						bottom <= ret.y && ret.y <= top
					)						
					{
						first = true;
					}
				}
				flags |= set_start;
			}
			if(flags & released) {
				if(
					left <= ret.x && ret.x <= right &&
					bottom <= ret.y && ret.y <= top
				)
				{
					last = true;
				}
				flags &= ~released;
				flags |= ready;
			}

			// invoke callback
			if(first && last && (flags & ready)) {
				instance.click();
				flags = none;
				first = false;
				last = false;
			}
			if(flags & ready) {
				flags = none;
				first = false;
				last = false;
			}
		}
	};

	template<class T>
	class APICALL onmouse_event final : public mouse_event_base {
		T instance;
	public:
		void update() {
			if( ! input || ! camera) {
				return;
			}
			core::vec3 p{0, 0, 0};
			input->get_mouse(p.x, p.y);
			auto ret = camera->calc_screen_pos_to_world(p);
			ret.y *= -1.0f;
			if(
				left <= ret.x && ret.x <= right &&
				bottom <= ret.y && ret.y <= top
			)
			{
				instance.onmouse();
			}
		}
	}; 
}
