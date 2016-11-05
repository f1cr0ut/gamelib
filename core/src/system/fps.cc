#include<thread>
#include<GLFW/glfw3.h>
#include"fps.h"

using namespace core;

fps::fps() {
	prev = std::chrono::high_resolution_clock::now();
}

void fps::vsync(bool sync) {
	if(sync) {
		glfwSwapInterval(1);
	}
	else {
		glfwSwapInterval(0);
	}
}

void fps::set_wait_time(u32 microsec) {
	std::chrono::microseconds w(microsec);
	std::swap(wait_time, w);
}

void fps::wait() {
	if(have_to_wait) {
		std::this_thread::sleep_for(wait_time - (std::chrono::high_resolution_clock::now() - prev));
		prev = std::chrono::high_resolution_clock::now();
	}
}

void fps::force_wait(bool waiting) {
	have_to_wait = waiting;
}
