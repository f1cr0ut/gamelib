#pragma once

#include<map>
#include<vector>
#include<memory>
#include<global_config.h>

namespace core {

	class screen;

	// from glfw3.2
	enum input_buttons : u32 {
		INPUT_MOUSE_BUTTON_1 = 1000 , // input_buttons - 1000 = mouse button id
		INPUT_MOUSE_BUTTON_2 = 1001 ,
 		INPUT_MOUSE_BUTTON_3 = 1002 ,
 		INPUT_MOUSE_BUTTON_4 = 1003 ,
 		INPUT_MOUSE_BUTTON_5 = 1004 ,
 		INPUT_MOUSE_BUTTON_6 = 1005 ,
 		INPUT_MOUSE_BUTTON_7 = 1006 ,
 		INPUT_MOUSE_BUTTON_8 = 1007 ,
 		INPUT_MOUSE_BUTTON_LEFT = INPUT_MOUSE_BUTTON_1 ,
 		INPUT_MOUSE_BUTTON_RIGHT = INPUT_MOUSE_BUTTON_2 ,
 		INPUT_MOUSE_BUTTON_MIDDLE = INPUT_MOUSE_BUTTON_3 ,
		INPUT_GAMEPAD_BUTTON_1 = 10001 , // input_buttons - 10000 = game pad button id
		INPUT_GAMEPAD_BUTTON_2 = 10002 ,
 		INPUT_GAMEPAD_BUTTON_3 = 10003 ,
 		INPUT_GAMEPAD_BUTTON_4 = 10004 ,
 		INPUT_GAMEPAD_BUTTON_5 = 10005 ,
 		INPUT_GAMEPAD_BUTTON_6 = 10006 ,
 		INPUT_GAMEPAD_BUTTON_7 = 10007 ,
 		INPUT_GAMEPAD_BUTTON_8 = 10008 ,
		INPUT_GAMEPAD_BUTTON_9 = 10009 , 
		INPUT_GAMEPAD_BUTTON_10 = 10010 ,
 		INPUT_GAMEPAD_BUTTON_11 = 10011 ,
 		INPUT_GAMEPAD_BUTTON_12 = 10012 ,
 		INPUT_GAMEPAD_BUTTON_13 = 10013 ,
 		INPUT_GAMEPAD_BUTTON_14 = 10014 ,
 		INPUT_GAMEPAD_BUTTON_15 = 10015 ,
 		INPUT_GAMEPAD_BUTTON_16 = 10016 ,
 		INPUT_GAMEPAD_BUTTON_17 = 10017 ,
 		INPUT_GAMEPAD_BUTTON_18 = 10018 ,
 		INPUT_GAMEPAD_BUTTON_19 = 10019 ,
 		INPUT_GAMEPAD_BUTTON_20 = 10020 ,
 		INPUT_GAMEPAD_BUTTON_21 = 10021 ,
 		INPUT_GAMEPAD_BUTTON_22 = 10022 ,
 		INPUT_GAMEPAD_BUTTON_23 = 10023 ,
 		INPUT_GAMEPAD_BUTTON_24 = 10024 ,
 		INPUT_GAMEPAD_BUTTON_25 = 10025 ,
 		INPUT_GAMEPAD_BUTTON_26 = 10026 ,
 		INPUT_GAMEPAD_BUTTON_27 = 10027 ,
 		INPUT_GAMEPAD_BUTTON_28 = 10028 ,
 		INPUT_GAMEPAD_BUTTON_29 = 10029 ,
 		INPUT_GAMEPAD_BUTTON_30 = 10030 ,
		INPUT_KEY_SPACE = 32,
 		INPUT_KEY_APOSTROPHE = 39,
 		INPUT_KEY_COMMA = 44,
 		INPUT_KEY_MINUS = 45,
 		INPUT_KEY_PERIOD = 46,
 		INPUT_KEY_SLASH = 47,
 		INPUT_KEY_0 = 48,
 		INPUT_KEY_1 = 49,
 		INPUT_KEY_2 = 50,
 		INPUT_KEY_3 = 51,
 		INPUT_KEY_4 = 52,
 		INPUT_KEY_5 = 53,
 		INPUT_KEY_6 = 54,
 		INPUT_KEY_7 = 55,
 		INPUT_KEY_8 = 56,
 		INPUT_KEY_9 = 57,
 		INPUT_KEY_SEMICOLON = 59,
 		INPUT_KEY_EQUAL = 61,
 		INPUT_KEY_A = 65,
 		INPUT_KEY_B = 66,
 		INPUT_KEY_C = 67,
 		INPUT_KEY_D = 68,
 		INPUT_KEY_E = 69,
 		INPUT_KEY_F = 70,
 		INPUT_KEY_G = 71,
 		INPUT_KEY_H = 72,
 		INPUT_KEY_I = 73,
 		INPUT_KEY_J = 74,
 		INPUT_KEY_K = 75,
 		INPUT_KEY_L = 76,
 		INPUT_KEY_M = 77,
 		INPUT_KEY_N = 78,
 		INPUT_KEY_O = 79,
 		INPUT_KEY_P = 80,
 		INPUT_KEY_Q = 81,
 		INPUT_KEY_R = 82,
 		INPUT_KEY_S = 83,
 		INPUT_KEY_T = 84,
 		INPUT_KEY_U = 85,
 		INPUT_KEY_V = 86,
 		INPUT_KEY_W = 87,
 		INPUT_KEY_X = 88,
 		INPUT_KEY_Y = 89,
 		INPUT_KEY_Z = 90,
 		INPUT_KEY_LEFT_BRACKET = 91,
 		INPUT_KEY_BACKSLASH = 92,
 		INPUT_KEY_RIGHT_BRACKET = 93,
 		INPUT_KEY_GRAVE_ACCENT = 96,
 		INPUT_KEY_WORLD_1 = 161, 
 		INPUT_KEY_WORLD_2 = 162,
 		INPUT_KEY_ESCAPE = 256,
 		INPUT_KEY_ENTER = 257,
 		INPUT_KEY_TAB = 258,
 		INPUT_KEY_BACKSPACE = 259,
 		INPUT_KEY_INSERT = 260,
 		INPUT_KEY_DELETE = 261,
 		INPUT_KEY_RIGHT = 262,
 		INPUT_KEY_LEFT = 263,
 		INPUT_KEY_DOWN = 264,
 		INPUT_KEY_UP = 265,
 		INPUT_KEY_PAGE_UP = 266,
 		INPUT_KEY_PAGE_DOWN = 267,
 		INPUT_KEY_HOME = 268,
 		INPUT_KEY_END = 269,
 		INPUT_KEY_CAPS_LOCK = 280,
 		INPUT_KEY_SCROLL_LOCK = 281,
 		INPUT_KEY_NUM_LOCK = 282,
 		INPUT_KEY_PRINT_SCREEN = 283,
 		INPUT_KEY_PAUSE = 284,
 		INPUT_KEY_F1 = 290,
 		INPUT_KEY_F2 = 291,
 		INPUT_KEY_F3 = 292,
 		INPUT_KEY_F4 = 293,
 		INPUT_KEY_F5 = 294,
 		INPUT_KEY_F6 = 295,
 		INPUT_KEY_F7 = 296,
 		INPUT_KEY_F8 = 297,
 		INPUT_KEY_F9 = 298,
 		INPUT_KEY_F10 = 299,
 		INPUT_KEY_F11 = 300,
 		INPUT_KEY_F12 = 301,
 		INPUT_KEY_F13 = 302,
 		INPUT_KEY_F14 = 303,
 		INPUT_KEY_F15 = 304,
 		INPUT_KEY_F16 = 305,
 		INPUT_KEY_F17 = 306,
 		INPUT_KEY_F18 = 307,
 		INPUT_KEY_F19 = 308,
 		INPUT_KEY_F20 = 309,
 		INPUT_KEY_F21 = 310,
 		INPUT_KEY_F22 = 311,
 		INPUT_KEY_F23 = 312,
 		INPUT_KEY_F24 = 313,
 		INPUT_KEY_F25 = 314,
 		INPUT_KEY_KP_0 = 320,
 		INPUT_KEY_KP_1 = 321 ,
 		INPUT_KEY_KP_2 = 322 ,
 		INPUT_KEY_KP_3 = 323 ,
 		INPUT_KEY_KP_4 = 324 ,
 		INPUT_KEY_KP_5 = 325 ,
 		INPUT_KEY_KP_6 = 326 ,
 		INPUT_KEY_KP_7 = 327 ,
 		INPUT_KEY_KP_8 = 328 ,
 		INPUT_KEY_KP_9 = 329 ,
 		INPUT_KEY_KP_DECIMAL = 330, 
 		INPUT_KEY_KP_DIVIDE = 331 ,
 		INPUT_KEY_KP_MULTIPLY = 332 ,
 		INPUT_KEY_KP_SUBTRACT = 333 ,
 		INPUT_KEY_KP_ADD = 334 ,
 		INPUT_KEY_KP_ENTER = 335, 
 		INPUT_KEY_KP_EQUAL = 336 ,
 		INPUT_KEY_LEFT_SHIFT = 340, 
 		INPUT_KEY_LEFT_CONTROL = 341, 
 		INPUT_KEY_LEFT_ALT = 342 ,
 		INPUT_KEY_LEFT_SUPER = 343 ,
 		INPUT_KEY_RIGHT_SHIFT = 344 ,
 		INPUT_KEY_RIGHT_CONTROL = 345 ,
 		INPUT_KEY_RIGHT_ALT = 346 ,
 		INPUT_KEY_RIGHT_SUPER = 347 ,
 		INPUT_KEY_MENU = 348 ,

		// INPUT_KEY_ANY is same as GLFW_KEY_ANY
		// original source code of glfw project doesn't have GLFW_KEY_ANY, so
		// I modified input.c and glfw3.h to able to detect that any key has pressed or not.
		// I tested just on windows but seems to work fine, maybe other OS can do it.
		INPUT_KEY_ANY = 349, 
	};

	class APICALL input final {
		friend class screen; 
		struct Impl;
		std::unique_ptr<Impl> impl;

	public:

		input();
		~input();

		// return 0 if none
//		u32 detect_numof_gamepad();
//		void set_use_gamepad(u32 index);

		// set a screen pointer
		void set_target_screen(core::screen * instance);

		// -1 <= x <= 1
		// keyboard will return just -1 or 1
//		f32 axis_x();

		// -1 <= y <= 1
		// same as above
//		f32 axis_y();

		// get key input
		// just get if button is pressed or not in previous frame
		bool button(input_buttons id);

		// this is for key mapping or key config:
		void set_keymap(u32 index, input_buttons id);

		// return true if attached index button is pressed
		bool pushed_keymap(u32 index);

		// delete memory
		void clear_keymap();

		// get mouse position
		void get_mouse(f32 & x, f32 & y);

		// get mouse wheel
		void get_mouse_scroll(f32 & x, f32 & y);
	};
}


