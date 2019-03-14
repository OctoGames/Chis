#include "AudioSource.h"

std::string AudioSource::name_ = "AudioSource";

AudioSource::AudioSource(GameObject* container, const std::string& audioId, const std::string& filename, bool enabled) :
	Component(container, enabled),
	channel_(nullptr),
	filename_(filename),
	audioId_(audioId),
	volume_(1.0),
	pitch_(1.0)
{
	AudioSystem::Instance()->addSound(audioId_, filename_);
}

AudioSource::~AudioSource()
{

}

void AudioSource::play()
{
	AudioSystem::Instance()->play(audioId_, channel_, volume_, pitch_);
}

void AudioSource::pause()
{
	AudioSystem::Instance()->pause(channel_);
}

void AudioSource::stop()
{
	AudioSystem::Instance()->stop(channel_);
}