#include "AudioManager.h"

AudioManager* AudioManager::instance_ = nullptr;

AudioManager::AudioManager()
{
	result_ = FMOD::System_Create(&system_);
	ERRCHECK(result_);

	result_ = system_->init(128, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result_);
}

AudioManager* AudioManager::Instance() 
{
	if (instance_ == nullptr) {
		instance_ = new AudioManager();
	}
	return instance_;
}

void AudioManager::ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK) 
	{
		printf("FMOD error %d - %s", result, FMOD_ErrorString(result));
	}
}

void AudioManager::addSound(const std::string& audioId, const std::string& filename) 
{
	std::string filepath = PATH_ + filename;
	result_ = system_->createSound(filepath.c_str(), FMOD_DEFAULT, 0, &sounds_[audioId]);
	ERRCHECK(result_);
}

void AudioManager::update() 
{
	result_ = system_->update();
	ERRCHECK(result_);
}

void AudioManager::play(std::string audioId, FMOD::Channel* channel, float volume, float pitch) 
{
	result_ = system_->playSound(sounds_[audioId], 0, false, &channel);
	ERRCHECK(result_);

	result_ = channel->setVolume(volume);
	ERRCHECK(result_);

	result_ = channel->setPitch(pitch);
	ERRCHECK(result_);
}
void AudioManager::pause(FMOD::Channel* channel) 
{
	bool paused;

	result_ = channel->getPaused(&paused);
	ERRCHECK(result_);

	result_ = channel->setPaused(!paused);
	ERRCHECK(result_);
}

void AudioManager::stop(FMOD::Channel* channel) 
{
	result_ = channel->stop();
	ERRCHECK(result_);
}

void AudioManager::release(std::string audioId) 
{
	result_ = sounds_[audioId]->release();
	ERRCHECK(result_);
}
