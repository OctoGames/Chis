#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include <stdio.h>
#include <string>
#include <map>

#include <fmod.hpp>
#include <fmod_errors.h>

class AudioManager 
{
private:
	AudioManager();

	void ERRCHECK(FMOD_RESULT result);

	static AudioManager* instance_;
	const char* PATH_ = "Assets/Sounds/";
	std::map<std::string, FMOD::Sound*> sounds_;

	FMOD_RESULT result_;
	FMOD::System* system_;

public:
	static AudioManager* Instance();

	void addSound(const std::string& audioId, const std::string& filename);
	void update();

	void play(std::string file, FMOD::Channel* channel, float volume = 1.0, float pitch = 1.0);
	void pause(FMOD::Channel* channel);
	void stop(FMOD::Channel* channel);

	void release(std::string audioId);
};

#endif	// !__AUDIO_MANAGER_H__