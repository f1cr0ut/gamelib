#include<thread>
#include<vector>
#include<cstring>
#include<fstream>
#include<AL/al.h>
#include<AL/alc.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include"sound.h"

using namespace core;

namespace {

	struct internal_al final {

		ALCdevice * dev { nullptr };
		ALCcontext * ctx { nullptr };
		bool has_init { false };

		static internal_al & instance() {
			static internal_al a;
			return a;
		}

		~internal_al() {
			alcMakeContextCurrent(NULL);
			alcDestroyContext(ctx);
			alcCloseDevice(dev);
		}

		void init() {
			if( ! has_init) {
				dev = alcOpenDevice(NULL);
				ctx = alcCreateContext(dev, NULL);
				alcMakeContextCurrent(ctx);
				has_init = true;
			}
		}
	};

	constexpr u32 NumberOfStreamingBuffers = 32;
	constexpr u32 BufferSize = 4096;
}

struct streaming_player::Impl {
	u32 buffers[NumberOfStreamingBuffers];
	u32 source;
	char queue_buffer[BufferSize];
	OggVorbis_File vf;
	vorbis_info *vi { nullptr };
	std::thread thread;
	bool continue_play { true };
	bool end_thread { false };
	u32 flag { 0 };
	FILE * fp { nullptr };
	bool loop { false };
	u32 begin_of_sound_area { 0 };
	enum {
		pause,
		stop,
		standby,
	};
};

streaming_player::streaming_player() {
	auto p = std::unique_ptr<Impl>(new Impl);
	std::swap(p, impl);
	internal_al::instance().init();
	alGenBuffers(NumberOfStreamingBuffers, impl->buffers);
	alGenSources(1, &impl->source);
}

streaming_player::~streaming_player() {
	close();
	alDeleteBuffers(NumberOfStreamingBuffers, impl->buffers);
	alDeleteSources(1, &impl->source);
}

bool streaming_player::open(const char * file) { 
	if( ! file) {
		return false;
	}
	impl->fp = fopen(file, "rb");
	return open(impl->fp);
}

bool streaming_player::open(FILE * fp) {
	if( ! fp) {
		return false;
	}
	if(ov_open(fp, &impl->vf, NULL, 0) < 0) {
		fclose(fp);
		fp = nullptr;
		return false;
	}
	impl->vi = ov_info(&impl->vf, -1);
	impl->begin_of_sound_area = ftell(fp);
	i32 current_section;
	i32 size = 0;
	auto type = AL_FORMAT_MONO16;
	if(impl->vi->channels == 2) {
		type = AL_FORMAT_STEREO16;
	}
	for(int i = 0; i < NumberOfStreamingBuffers; ++i) {
		size = ov_read(&impl->vf, &impl->queue_buffer[0], BufferSize, 0, 2, 1, &current_section);
		if(size > 0) {
			alBufferData(impl->buffers[i], type, &impl->queue_buffer[0], size, impl->vi->rate);
		}
	}
	alSourceQueueBuffers(impl->source, NumberOfStreamingBuffers, impl->buffers);
	return true;
}

void streaming_player::play() {
	alSourcePlay(impl->source);
	impl->flag = Impl::standby;
	impl->continue_play = true;
	std::thread th([&] {
		impl->end_thread = false;
		while(impl->continue_play) {
			ALuint buffer;
			ALint val;
			alGetSourcei(impl->source, AL_BUFFERS_PROCESSED, &val);
			if(val <= 0) {
				continue;
			} 
			while(val--) {
				i32 size = 0;
				i32 current_section;
				alSourceUnqueueBuffers(impl->source, 1, &buffer);
				size = ov_read(&impl->vf, &impl->queue_buffer[0], BufferSize, 0, 2, 1, &current_section);
				// reached the end of file
				if(size <= 0 && impl->loop) {
					fseek(impl->fp, impl->begin_of_sound_area, SEEK_SET);
					size = ov_read(&impl->vf, &impl->queue_buffer[0], BufferSize, 0, 2, 1, &current_section);
				}
				auto type = AL_FORMAT_MONO16;
				if(impl->vi->channels == 2) {
					type = AL_FORMAT_STEREO16;
				}
				alBufferData(buffer, type, &impl->queue_buffer[0], size, impl->vi->rate);
				alSourceQueueBuffers(impl->source, 1, &buffer);
			}
			alGetSourcei(impl->source, AL_SOURCE_STATE, &val);
			if(impl->flag == Impl::pause) {
				alSourcePause(impl->source);
			}
			else if(impl->flag == Impl::stop) {
				alSourceStop(impl->source);
			}
			else if(val != AL_PLAYING) {
				alSourcePlay(impl->source);
			}
			impl->flag = Impl::standby;
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
		impl->end_thread = true;
	});
	th.detach();
	std::swap(th, impl->thread);
}

void streaming_player::close() {
	pause();
	impl->continue_play = false;
	while(impl->end_thread != true) {}
	ov_clear(&impl->vf);
	if(impl->fp) {
		fclose(impl->fp);
		impl->fp = nullptr;
	}
	// reset 
	impl->begin_of_sound_area = 0;
	impl->flag = 0;
	impl->loop = false;
}

void streaming_player::pause() {
	impl->flag = Impl::pause;
}

void streaming_player::replay() {
	alSourcePlay(impl->source);
}

void streaming_player::stop() {
	impl->flag = Impl::stop;
	impl->continue_play = false;
}

void streaming_player::volume(f32 v) {
	if(v < 0) v = 0;
	if(v > 1) v = 1;
	alSourcef(impl->source, AL_GAIN, v); 
}

void streaming_player::pitch(f32 p) {
	if(p < 0) p = 0;
	alSourcef(impl->source, AL_PITCH, p);
}

void streaming_player::set_loop(bool loop) {
	impl->loop = loop;
}

//========================================================================
// sound
//========================================================================

struct sound::Impl{
	u32 buffer { 0 };
	u32 source { 0 };
	std::vector<char> sound_buffer;
};

sound::sound() {
	auto p = std::unique_ptr<Impl>(new Impl);
	std::swap(p, impl);
	internal_al::instance().init();
	alGenBuffers(1, &impl->buffer);
	alGenSources(1, &impl->source);
}


sound::~sound() {
	alDeleteBuffers(1, &impl->buffer);
	alDeleteSources(1, &impl->source);
}

bool sound::open_wav(const char * file) {
	if( ! file) {
		return false;
	}
	std::ifstream ifs(file, std::ios::binary);
	if( ! ifs.is_open()) {
		return false;
	}
	char buf[4] = {0};
	ifs.read(buf, 4);
	if(strstr(buf, "RIFF") == NULL) {
		return false;
	}
	u32 size = 0;
	ifs.read((char*)&size, 4);
	ifs.read(buf, 4);
	if(strstr(buf, "WAVE") == NULL) {
		return false;
	}
	ifs.read(buf, 4);
	if(strstr(buf, "fmt") == NULL) {
		return false;
	}
	i32 format_size, sampling, byte, total;
	i16 format, channel, alignment, bit;
	ifs.read((char*)&format_size, 4);
	ifs.read((char*)&format, 2);
	ifs.read((char*)&channel, 2);
	ifs.read((char*)&sampling, 4);
	ifs.read((char*)&byte, 4);
	ifs.read((char*)&alignment, 2);
	ifs.read((char*)&bit, 2);
	ifs.read(buf, 4);
	if(strstr(buf, "data") == NULL) {
		return false;
	}
	ifs.read((char*)&total, 4);
	impl->sound_buffer.resize(total);
	ifs.read(&impl->sound_buffer[0], total);
	auto type = AL_FORMAT_MONO16;
	if(channel == 2) {
		type = AL_FORMAT_STEREO16;
	}
	alBufferData(impl->buffer, type, &impl->sound_buffer[0], total, sampling);
	alSourcei(impl->source, AL_BUFFER, impl->buffer);
	ifs.close();
	return true;
}

bool sound::open_ogg(const char * file) {
	auto fp = fopen(file, "rb");
	if( ! fp) {
		return false;
	}
	OggVorbis_File vf;
	if (ov_open(fp, &vf, NULL, 0) < 0) {
		fclose(fp);
		return false;
	}
	auto vi = ov_info(&vf, -1);
	i32 current_section;
	i32 size = 0;
	u32 bytes = (u32)vi->channels * 2 * (u32)ov_pcm_total(&vf, -1);
	impl->sound_buffer.resize(bytes);
	u32 pos = 0;
	while(true) {
		size = ov_read(&vf, &impl->sound_buffer[pos], BufferSize, 0, 2, 1, &current_section);
		if(size <= 0) break;
		pos += size;
		if(pos >= bytes) break;
	}
	auto type = AL_FORMAT_MONO16;
	if(vi->channels == 2) {
		type = AL_FORMAT_STEREO16;
	}
	alBufferData(impl->buffer, type, &impl->sound_buffer[0], bytes, vi->rate);
	alSourcei(impl->source, AL_BUFFER, impl->buffer);
	fclose(fp);
	ov_clear(&vf);

	return true;
}

void sound::play() {
	alSourcePlay(impl->source);
}

void sound::pause() {
	alSourcePause(impl->source);
}

void sound::replay() {
	alSourcePlay(impl->source);
}

void sound::stop() {
	alSourceStop(impl->source);
}

void sound::volume(f32 v) {
	if (v < 0) v = 0;
	if (v > 1) v = 1;
	alSourcef(impl->source, AL_GAIN, v);
}

void sound::pitch(f32 p) {
	if (p < 0) p = 0;
	alSourcef(impl->source, AL_PITCH, p);
}

void sound::set_loop(bool loop) {
	alSourcei(impl->source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

void sound::position(f32 x, f32 y, f32 z) {
	alSource3f(impl->source, AL_POSITION, x, y, z);
}
