#ifndef __AUDIO_SOURCE_H__
#define __AUDIO_SOURCE_H__

#include <Engine.h>

class AudioSource : public Component
{
public:
	AudioSource();
	virtual ~AudioSource();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

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

class AudioSourceFactory : public BaseFactory
{
public:
	AudioSourceFactory() {}
	virtual ~AudioSourceFactory() {}

	virtual Component* create() { return new AudioSource(); }
};

#endif // !__AUDIO_SOURCE_H__