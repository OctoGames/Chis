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
	//delete channel_;
	//channel_ = nullptr;
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
	AudioManager::Instance()->addSound(audioId_, filename_);
	setEnabled(enabled_);
}

void AudioSource::play()
{
	AudioManager::Instance()->play(audioId_, channel_, volume_, pitch_);
}

void AudioSource::pause()
{
	AudioManager::Instance()->pause(channel_);
}

void AudioSource::stop()
{
	AudioManager::Instance()->stop(channel_);
}