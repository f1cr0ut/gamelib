#include<vector>
#include<string>
#include<GL/glew.h>
#include<global_config.h>
#include<GLFW/glfw3.h>
#include"screen.h"

#include"../gl/camera.h"

using namespace core;

struct screen::Impl {
	GLFWwindow* window{ nullptr };
	GLFWmonitor ** monitors{ nullptr };
	i32 numof_monitors{ 0 };
	std::vector<std::string> monitor_names;
	GLFWmonitor * target_monitor{ nullptr };
	i32 w{ 0 };
	i32 h{ 0 };
};

namespace {

	struct glfw_handler final {
		glfw_handler() {
			glfwInit();
		}
		~glfw_handler() {
			glfwTerminate();
		}
	};

	static glfw_handler glfw_handle;

	void callback(GLFWwindow *w, int x, int y) {
		glViewport(0, 0, x, y);
		glfwSwapBuffers(w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		auto p = (screen::user_pointer*)glfwGetWindowUserPointer(w);
		if(p) {
			if(p->callback) {
				p->callback(x, y);
			}
		}
	}
		
}

screen::screen() {
	auto p = std::unique_ptr<Impl>(new Impl);
	std::swap(p, impl);
	auto pp = std::unique_ptr<user_pointer>(new user_pointer);
	std::swap(pp, userp_impl);
	impl->monitors = glfwGetMonitors(&impl->numof_monitors);
	for(int i = 0; i < impl->numof_monitors; ++i) {
		impl->monitor_names.push_back(glfwGetMonitorName(impl->monitors[i]));
	}
	impl->target_monitor = glfwGetPrimaryMonitor();
}

screen::~screen() {
	close();
}

void * screen::get_window_handle() {
	return impl->window;
}

u32 screen::get_numof_monitors() {
	return impl->numof_monitors;
}

const char * screen::get_monitor_name(u32 index) {
	if((i32)index >= impl->numof_monitors) {
		return nullptr;
	}
	return impl->monitor_names[index].c_str();
}

void screen::set_target_monitor(u32 index) {
	if((i32)index >= impl->numof_monitors) {
		return;
	}
	impl->target_monitor = impl->monitors[index];
}

bool screen::internal_open(
	const char * title,
	u32 w, 
	u32 h,
	u32 x,
	u32 y,
	bool fullscreen
)
{
	if(impl->window) {
		return false;
	}
	auto monitor = fullscreen ? impl->target_monitor : nullptr;
	// set fullscreen mode as windowed
	if(fullscreen) {
		auto mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	}
	// enable window resize
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	// disable maximze
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
	// create window
	impl->window = glfwCreateWindow(w, h, title, monitor, NULL);
	if( ! impl->window) {
		return false;
	}
	// enable opengl
	glfwMakeContextCurrent(impl->window);
	glewInit();
	// set callback
	glfwSetWindowSizeCallback(impl->window, callback);
	// set user pointer
	glfwSetWindowUserPointer(impl->window, userp_impl.get());
	return true;
}

bool screen::open(
	const char * title,
	u32 w, 
	u32 h,
	u32 x,
	u32 y,
	bool fullscreen
)
{
	auto ret = internal_open(title, w, h, x, y, fullscreen);
	glfwSetWindowPos(impl->window, x, y);
	return ret;
}

bool screen::open( const char * title, u32 w, u32 h, bool fullscreen) {
	auto mode = glfwGetVideoMode(impl->target_monitor);
	auto ret = internal_open(title, w, h, 0, 0, fullscreen);
	glfwSetWindowPos(impl->window, (mode->width - w) / 2, (mode->height - h) / 2);
	return ret;
}

void screen::set_screen_size(u32 w, u32 h) {
	glfwSetWindowSize(impl->window, w, h);
}

void screen::get_screen_size(u32 & w, u32 & h) {
	glfwGetWindowSize(impl->window, &impl->w, &impl->h);
	w = impl->w;
	h = impl->h;
}

void screen::close() {
	glfwDestroyWindow(impl->window);
}

void screen::polling() {
	glfwPollEvents();

	userp_impl->mouse_wheel_x = 0;
	userp_impl->mouse_wheel_y = 0;
}

void screen::swap_buffer() {
	glfwSwapBuffers(impl->window);
}

bool screen::should_close() {
	return glfwWindowShouldClose(impl->window) != 0;
}

void screen::set_title(const char * title) {
	glfwSetWindowTitle(impl->window, title);
}

void screen::set_icon(u32 w, u32 h, void * data) {
	GLFWimage img;
	img.width = w;
	img.height = h;
	img.pixels = (unsigned char*)data;
	glfwSetWindowIcon(impl->window, 1, &img);
}

void screen::clear_icon() {
	glfwSetWindowIcon(impl->window, 0, NULL);
}

void screen::centering(u32 w, u32 h) {
	auto mode = glfwGetVideoMode(impl->target_monitor);;
	glfwSetWindowPos(impl->window, (mode->width - w) / 2, (mode->height - h) / 2);
}

void screen::set_window_resize_callback(const std::function<void(i32, i32)> & callback) {
	userp_impl->callback = callback;
}