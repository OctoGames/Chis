#ifndef _AUDIO_SYSTEM_H_
#define _AUDIO_SYSTEM_H_

#include <stdio.h>
#include <string>
#include <map>

#include <fmod.hpp>
#include <fmod_errors.h>

class AudioSystem 
{
private:
	AudioSystem();

	void ERRCHECK(FMOD_RESULT result);

	static AudioSystem* instance_;
	const char* PATH_ = "Assets/Sounds/";
	std::map<std::string, FMOD::Sound*> sounds_;

	FMOD_RESULT result_;
	FMOD::System* system_;

public:
	static AudioSystem* Instance();

	void addSound(const std::string& audioId, const std::string& filename);
	void update();

	void play(std::string file, FMOD::Channel* channel, float volume = 1.0, float pitch = 1.0);
	void pause(FMOD::Channel* channel);
	void stop(FMOD::Channel* channel);

	void release(std::string audioId);
};

#endif 