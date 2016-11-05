#include<string>
#include<iostream>
#include<fstream>
#include<cassert>

#include"obj_loader.h"

using namespace core;

namespace {
	i32 count_space(const std::string & s) {
		i32 ret = 0;
		for(auto & c : s) {
			if(c == ' ') ++ret;
		}
		return ret;
	}

	template<int N>
	void get_float_value(const std::string & s, f32 * ret) {
		bool delim = false;
		static char str_float[32] = {0};
		for(auto & k : str_float) k = 0;
		int i = 0, j = 0;
		for(auto & c : s) {
			if(delim) str_float[i++] = c;
			if(c == ' ') {
				if(i != 0) {
					sscanf(str_float, "%f", &ret[j++]);
					for(auto & k : str_float) k = 0;
				}
				delim = true;
				i = 0;
			}
#if ! defined(RELEASE_BUILD)
			assert(j < 3 && i <= 31);
#endif
		}
		sscanf(str_float, "%f", &ret[j]);
	}

	void get_float_vector(const std::string & s, f32 * res) {
		auto num = count_space(s);
		if(num == 2) {
			get_float_value<2>(s, res);
		}
		else if(num == 3) {
			get_float_value<3>(s, res);
		}
		else if(num == 4) {
			get_float_value<4>(s, res);
		}
		else {
			DBGPRINT("error : vertex is over 4");
			return;
		}
	}

	void read_line(const std::string & s) {
		static float retval[4] = {0};

		switch(s[0]) {
			// v, vn, vt, vp
			case 'v':
				for(auto & i : retval) i = 0; // zero clear
				get_float_vector(s, retval);
				break;
			// object
			case 'o':
				break;
			// smooth shading
			case 's':
				break;
			// index	
			case 'f':
				break;
			// group
			case 'g':
				break;
			// mtl file
			case 'm': // mtllib
				break;
			// material name
			case 'u': // usemtl
				break;
			// ignore comment	
			case '#':
			default:
				break;
		}
	}
}

void obj_loader::load(const char * file) {
	std::ifstream ifs(file);
	if( ! ifs.is_open()) {
		return;
	}
	std::string line;
	while(getline(ifs, line)) {
		read_line(line);
	}
}
