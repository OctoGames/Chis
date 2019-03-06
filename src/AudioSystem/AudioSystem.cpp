#include "AudioSystem.h"

AudioSystem* AudioSystem::instance_ = nullptr;

AudioSystem::AudioSystem()
{
	result_ = FMOD::System_Create(&system_);
	ERRCHECK(result_);

	result_ = system_->init(128, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result_);
}

AudioSystem* AudioSystem::Instance() 
{
	if (instance_ == nullptr) {
		instance_ = new AudioSystem();
	}
	return instance_;
}

void AudioSystem::ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK) 
	{
		printf("FMOD error %d - %s", result, FMOD_ErrorString(result));
	}
}

void AudioSystem::addSound(const std::string& audioId, const std::string& filename) 
{
	std::string filepath = PATH_ + filename;
	result_ = system_->createSound(filepath.c_str(), FMOD_DEFAULT, 0, &sounds_[audioId]);
	ERRCHECK(result_);
}

void AudioSystem::update() 
{
	result_ = system_->update();
	ERRCHECK(result_);
}

void AudioSystem::play(std::string audioId, FMOD::Channel* channel, float volume, float pitch) 
{
	result_ = system_->playSound(sounds_[audioId], 0, false, &channel);
	ERRCHECK(result_);

	result_ = channel->setVolume(volume);
	ERRCHECK(result_);

	result_ = channel->setPitch(pitch);
	ERRCHECK(result_);
}
void AudioSystem::pause(FMOD::Channel* channel) 
{
	bool paused;

	result_ = channel->getPaused(&paused);
	ERRCHECK(result_);

	result_ = channel->setPaused(!paused);
	ERRCHECK(result_);
}

void AudioSystem::stop(FMOD::Channel* channel) 
{
	result_ = channel->stop();
	ERRCHECK(result_);
}

void AudioSystem::release(std::string audioId) 
{
	result_ = sounds_[audioId]->release();
	ERRCHECK(result_);
}
