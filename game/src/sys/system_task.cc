#include"system_task.h"

using namespace game;

bool system_task::update() {
	if(app->key_mouse.button(core::INPUT_KEY_F5)) {
		app->screen.set_screen_size(app->get_original_window_w(), app->get_original_window_h());
		app->main_camera.set_viewport(0, 0, app->get_original_window_w(), app->get_original_window_h());
		auto resolution = app->get_original_window_w() / app->get_original_window_h();
		app->main_camera.ortho(-1.0f * resolution, 1.0f * resolution, 1.0f, -1.0f, -1.0f, 1.0f);
		app->main_camera.update();
		app->settings.window_w = app->get_original_window_w();
		app->settings.window_h = app->get_original_window_h();
		app->main_camera.set_viewport_gl_only(0, 0, (f32)app->settings.window_w, (f32)app->settings.window_h);
		app->screen.centering(app->get_original_window_w(), app->get_original_window_h());
	}
	return true;
}