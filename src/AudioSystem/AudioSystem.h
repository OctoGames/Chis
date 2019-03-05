#ifndef _AUDIO_SYSTEM_H_
#define _AUDIO_SYSTEM_H_

#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>
#include <iostream>
#include <map>

class AudioSystem {
private:
	const char* PATH_ = "Assets/Sounds/";

	FMOD_RESULT result;
	FMOD::System *system = NULL;
	
	std::map<std::string, FMOD::Sound*> sounds_;
	
	static AudioSystem* instance_;

	void ERRCHECK(FMOD_RESULT result);
	void init();
	
	AudioSystem();
	~AudioSystem();

public:
	static AudioSystem* Instance();
	void addSound(const std::string& audioId, const std::string& filename);
	void update();
	void play(std::string file, FMOD::Channel* channel, float volume, float pitch);
	void pause(FMOD::Channel* channel);
	void stop(FMOD::Channel* channel);
	void release(std::string audioId);
	
};

#endif 