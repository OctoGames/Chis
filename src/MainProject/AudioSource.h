#ifndef __AUDIO_SOURCE_H__
#define __AUDIO_SOURCE_H__

#include "Component.h"
#include "AudioSystem.h"

class AudioSource : public Component
{
public:
	AudioSource(GameObject* container, const std::string& audioId, const std::string& filename, bool enabled = true);
	virtual ~AudioSource();

	virtual std::string getName() const { return name_; }

	void play();
	void pause();
	void stop();

	inline float getVolume() const { return volume_; }
	inline float getPitch() const { return pitch_; }
	inline void setVolume(float volume) { volume_ = volume; }
	inline void setPitch(float pitch) { pitch_ = pitch; }

private:
	static std::string name_;

	FMOD::Channel* channel_;
	std::string filename_;
	std::string audioId_;
	float volume_;
	float pitch_;

};

#endif // !__AUDIO_SOURCE_H__