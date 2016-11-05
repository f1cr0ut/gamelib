#pragma once

#include<memory>
#include<cstdio>
#include<global_config.h>

namespace core {

	// for ogg only
	// ogg file must be stereo 16bit
	// monoral is also able to use, but I don't recommend that
	// because of sound quality
	class APICALL streaming_player final {
		struct Impl;
		std::unique_ptr<Impl> impl;
	public:

		streaming_player();
		~streaming_player();

		bool open(const char * file);
		bool open(FILE * fp);
		void close();
		void play();
		void pause();
		void replay();
		void stop();
		void volume(f32 v);
		void pitch(f32 p);
		void set_loop(bool loop);
	};

	// mono,stereo 16bit sounds only.
	// 8bits are not supported.
	class APICALL sound final {
		struct Impl;
		std::unique_ptr<Impl> impl;
	public:

		sound();
		~sound();

		bool open_wav(const char * file);
		bool open_ogg(const char * file);
		void play();
		void pause();
		void replay();
		void stop();
		void volume(f32 v);
		void pitch(f32 p);
		void set_loop(bool loop);
		// sound format must be monoral & 16bit if using position.
		// if you are going to use pannig with stereo & 16bit format,
		// you have to use two sounds and set volume like panning.
		// this is due to openal specification...
		void position(f32 x, f32 y, f32 z);
	};
}
