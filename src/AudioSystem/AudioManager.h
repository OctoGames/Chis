#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include <stdio.h>
#include <string>
#include <map>

#include <fmod.hpp>
#include <fmod_errors.h>

class AudioManager 
{
public:
	static AudioManager* Instance();

	void init();
	void update();
	void close();

	void addSound(const std::string& audioId, const std::string& filename);

	FMOD::Channel* play(std::string file, float volume = 1.0, float pitch = 1.0, bool loop = false);
	void pause(FMOD::Channel* channel);
	void stop(FMOD::Channel* channel);
	void release(std::string audioId);

private:
	AudioManager();
	~AudioManager();

	void ERRCHECK(FMOD_RESULT result);

	static AudioManager* instance_;

	const char* PATH_ = "Assets/Sounds/";

	FMOD_RESULT result_;
	FMOD::System* system_;
	std::map<std::string, FMOD::Sound*> sounds_;
};

#endif	// !__AUDIO_MANAGER_H__