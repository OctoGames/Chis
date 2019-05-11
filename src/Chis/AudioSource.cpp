#include "AudioSource.h"

std::string AudioSource::name_ = "AudioSource";

AudioSource::AudioSource() :
	channel_(nullptr),
	filename_(""),
	audioId_(""),
	volume_(0.0f),
	pitch_(0.0f),
	loop_(false)
{
}

AudioSource::~AudioSource()
{
}

void AudioSource::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();

	it = params.find("enabled_as"); if (it != params.end()) enabled_ = params.at("enabled_as").b;
	it = params.find("filename"); if (it != params.end()) filename_ = params.at("filename").s;
	it = params.find("audio_id"); if (it != params.end()) audioId_ = params.at("audio_id").s;
	it = params.find("volume"); if (it != params.end()) volume_ = params.at("volume").f;
	it = params.find("pitch"); if (it != params.end()) pitch_ = params.at("pitch").f;
	it = params.find("loop"); if (it != params.end()) loop_ = params.at("loop").b;
}

Component * AudioSource::clone()
{
	AudioSource* clonedComponent = new AudioSource();
	
	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->filename_ = this->filename_;
	clonedComponent->audioId_ = this->audioId_;
	clonedComponent->volume_ = this->volume_;
	clonedComponent->pitch_ = this->pitch_;
	clonedComponent->loop_ = this->loop_;

	return clonedComponent;
}

void AudioSource::init()
{
	AudioManager::Instance()->addSound(audioId_, filename_);
	setEnabled(enabled_);
}

void AudioSource::play()
{
	channel_ = AudioManager::Instance()->play(audioId_, volume_, pitch_, loop_);
}

void AudioSource::pause()
{
	AudioManager::Instance()->pause(channel_);
}

void AudioSource::stop()
{
	AudioManager::Instance()->stop(channel_);
}