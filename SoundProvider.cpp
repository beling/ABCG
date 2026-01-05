/*
 * SoundProvider.cpp
 *
 *  Created on: 28-04-2011
 *      Author: qwak
 */

#include "SoundProvider.h"

#include <stdexcept>
#include <vector>

#include <AL/al.h>
#include <AL/alure.h>

#include <iostream>

#include <cstdlib>

#define SND_DESTROY0 0
#define SND_DESTROY1 1
#define SND_DESTROY2 2
#define SND_DESTROY3 3
#define SND_DESTROY_NUM 4

#define SND_NUM 4

const ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 0.0, 0.0 };

struct OpenALSoundProvider: public SoundProvider {
	// A vector list of sources for multiple emissions.
	std::vector<ALuint> sound_events_sources;

	ALuint buffers[SND_NUM];

	ALuint loadALBuffer(const char* filename) {
	    ALuint result = alureCreateBufferFromFile(filename);
	    if (result == AL_NONE)
	    	std::cerr << "Error while loading sound sample " << filename << ": " << alureGetErrorString() << std::endl;
	    return result;
	}

	void throwOnAlError(const char* error_str = "OpenAL error") noexcept(false) {
		if (alGetError() != AL_NO_ERROR) {
			alureShutdownDevice();
			throw std::runtime_error(error_str);
		}
	}

	OpenALSoundProvider() {
		if (!alureInitDevice(0, 0)) throw std::runtime_error(std::string("Can't initialize ALURE: ") + alureGetErrorString());
		buffers[SND_DESTROY0] = loadALBuffer("/home/qwak/prog/cpp/ABCG/res/sounds/boom0.wav");
		buffers[SND_DESTROY1] = loadALBuffer("/home/qwak/prog/cpp/ABCG/res/sounds/boom1.wav");
		buffers[SND_DESTROY2] = loadALBuffer("/home/qwak/prog/cpp/ABCG/res/sounds/boom2.wav");
		buffers[SND_DESTROY3] = loadALBuffer("/home/qwak/prog/cpp/ABCG/res/sounds/boom3.wav");

		alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		throwOnAlError("Error while setup listener velocity.");
		alListenerfv(AL_ORIENTATION, ListenerOri);
		throwOnAlError("Error while setup listener orientation.");
	}

	~OpenALSoundProvider() {
		for (std::vector<ALuint>::iterator iter = sound_events_sources.begin(); iter != sound_events_sources.end(); ++iter)
			alDeleteSources(1, &*iter);
		alDeleteBuffers(SND_NUM, buffers);
		alureShutdownDevice();
	}

	void playEvent(ALuint event_num, const vec2d<double>& where) {
	    ALuint source;
	    alGenSources(1, &source);
	    if (alGetError() != AL_NO_ERROR) {
	        std::cerr << "Error generating OpenAL audio source (in playEvent)." << std::endl;
	        return;
	    }
	    alSourcei (source, AL_BUFFER,	buffers[event_num]);
	    alSourcef (source, AL_PITCH,    1.0);
	    alSourcef (source, AL_GAIN,     1.0);
	    alSource3f(source, AL_POSITION, where.x, where.y, 0.0f);
	    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	    alSourcei (source, AL_LOOPING,  AL_FALSE);

	    if (alGetError() != AL_NO_ERROR) {
	        std::cerr << "Error setup OpenAL audio source (in playEvent)." << std::endl;
	        return;
	    }

	    alSourcePlay(source);
	    //TODO use alurePlaySource with call-back in alure 1.1

	    if (alGetError() != AL_NO_ERROR) {
	        std::cerr << "Error play OpenAL audio source (in playEvent)." << std::endl;
	        return;
	    }

	    sound_events_sources.push_back(source);
	}

	virtual void playDestroy(const vec2d<double>& where) { playEvent(SND_DESTROY0 + rand() % SND_DESTROY_NUM, where); }

	virtual void timerEvent(double elasped_time) {
		for (std::vector<ALuint>::iterator iter = sound_events_sources.begin(); iter != sound_events_sources.end(); ) {
		    ALenum state;
		    alGetSourcei(*iter, AL_SOURCE_STATE, &state);
			if (state != AL_PLAYING) {
				alDeleteSources(1, &*iter);
				sound_events_sources.erase(iter);
			} else
				++iter;
		}

	}

	virtual void updateListenerPosition(const vec2d<double>& new_listener_pos) {
		alListener3f(AL_POSITION, new_listener_pos.x, new_listener_pos.y, 0.0f);
	}

};

SoundProvider* _soundProvider = 0;

void initSoundProvider() {
	freeSoundProvider();
	try {
		_soundProvider = new OpenALSoundProvider();
	} catch (std::exception& e) {
		std::cerr << "Sound system error: " << e.what() << std::endl << "(sounds will not be not played)";
		_soundProvider = new SoundProvider();
	}
}

void freeSoundProvider() {
	delete _soundProvider;
	_soundProvider = 0;
}

SoundProvider& soundProvider() {
	return *_soundProvider;
}
