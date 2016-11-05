#include<functional>
#include<GLFW/glfw3.h>
#include"screen.h"
#include"input.h"

using namespace core;

namespace {

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		auto p = (screen::user_pointer*)glfwGetWindowUserPointer(window);
		if(p) {
			p->mouse_wheel_x = (f32)xoffset;
			p->mouse_wheel_y = (f32)yoffset;
		}
	}
}

struct input::Impl {
	std::map<u32, std::vector<input_buttons>> keymap;
//	i32 pad_id{ -1 };
	f64 mouse_x{ 0 };
	f64 mouse_y{ 0 };
	screen * monitor{ nullptr };
	GLFWwindow* window_handle{ nullptr };
};

input::input() {
	auto p = std::unique_ptr<Impl>(new Impl);
	std::swap(p, impl);
}

input::~input() {

}

/*

u32 input::detect_numof_gamepad() {
	u32 ct = 0;
	for(int i = 0; i < GLFW_JOYSTICK_LAST; ++i) {
		if(glfwJoystickPresent(i) == GLFW_TRUE) {
			++ct;
		}
	}
	return ct;
}

void input::set_use_gamepad(u32 index) {
	impl->pad_id = index;
}

*/

void input::set_target_screen(core::screen * instance) {
	if( ! instance) {
		return;
	}
	glfwSetScrollCallback((GLFWwindow*)instance->get_window_handle(), scroll_callback);
	impl->monitor = instance;
	impl->window_handle = (GLFWwindow*)instance->get_window_handle();
}

/*

f32 input::axis_x() {
	if(glfwGetKey(impl->user_data.target_screen, GLFW_KEY_LEFT) == GLFW_PRESS) {
		return -1;
	}
	if(glfwGetKey(impl->user_data.target_screen, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		return 1;
	}
	if(impl->pad_id > -1) {
		int count = 0;
		auto ret = glfwGetJoystickAxes(impl->pad_id, &count);
		if(ret) {
			return ret[0];
		}
	}
	return 0;
}

f32 input::axis_y() {
	if(glfwGetKey(impl->user_data.target_screen, GLFW_KEY_UP) == GLFW_PRESS) {
		return 1;
	}
	if(glfwGetKey(impl->user_data.target_screen, GLFW_KEY_DOWN) == GLFW_PRESS) {
		return -1;
	}
	if(impl->pad_id > -1) {
		int count = 0;
		auto ret = glfwGetJoystickAxes(impl->pad_id, &count);
		if(ret) {
			return ret[1];
		}
	}
	return 0;
}

*/

bool input::button(input_buttons id) {
	if(glfwGetMouseButton(impl->window_handle, id - 1000) == GLFW_PRESS) {
		return true;
	}
	if(glfwGetKey(impl->window_handle, id) == GLFW_PRESS) {
		return true;
	}
	/*
	if(impl->pad_id > -1) {
		int count = 0;
		auto ret = glfwGetJoystickButtons(impl->pad_id, &count);
		if(ret) {
			for(int i = 0; i < count; ++i) {
				if(ret[i] == id - 10000) {
					return true;
				}
			}
		}
	}
	*/
	return false;
}

void input::set_keymap(u32 index, input_buttons id) {
	impl->keymap[index].push_back(id);
}

bool input::pushed_keymap(u32 index) {
	for(auto & i : impl->keymap[index]) {
		if(button(i)) {
			return true;
		}
	}
	return false;
}

void input::clear_keymap() {
	std::map<u32, std::vector<input_buttons>> new_one;
	std::swap(impl->keymap, new_one);
}

void input::get_mouse(f32 & x, f32 & y) {
	glfwGetCursorPos(impl->window_handle, &impl->mouse_x, &impl->mouse_y);
	x = (f32)impl->mouse_x;
	y = (f32)impl->mouse_y;
}

void input::get_mouse_scroll(f32 & x, f32 & y) {
	x = impl->monitor->userp_impl->mouse_wheel_x;
	y = impl->monitor->userp_impl->mouse_wheel_y;
}
