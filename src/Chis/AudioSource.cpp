#include "AudioSource.h"

std::string AudioSource::name_ = "AudioSource";

AudioSource::AudioSource() :
	channel_(nullptr),
	filename_(""),
	audioId_(""),
	volume_(0.0f),
	pitch_(0.0f)
{
}

AudioSource::~AudioSource()
{

}

void AudioSource::load(const std::map<std::string, ValueType>& params)
{
	enabled_ = params.at("enabled_as").b;
	filename_ = params.at("filename").s;
	audioId_ = params.at("audio_id").s;
	volume_ = params.at("volume").f;
	pitch_ = params.at("pitch").f;
}

Component * AudioSource::clone()
{
	AudioSource* clonedComponent = new AudioSource();
	
	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->filename_ = this->filename_;
	clonedComponent->audioId_ = this->audioId_;
	clonedComponent->volume_ = this->volume_;
	clonedComponent->pitch_ = this->pitch_;

	return clonedComponent;
}

void AudioSource::init()
{
	AudioSystem::Instance()->addSound(audioId_, filename_);
	setEnabled(enabled_);
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