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

AudioSource::AudioSource(GameObject* container, const std::string& audioId, const std::string& filename, bool enabled) :
	Component(container, enabled),
	channel_(nullptr),
	filename_(filename),
	audioId_(audioId),
	volume_(1.0f),
	pitch_(1.0f)
{
	AudioSystem::Instance()->addSound(audioId_, filename_);
}

AudioSource::~AudioSource()
{

}

void AudioSource::init(const std::map<std::string, ValueType>& params)
{
	volume_ = params.at("volume").f;
	pitch_ = params.at("pitch").f;
	AudioSystem::Instance()->addSound(params.at("audio_id").s, params.at("file").s);
	setEnabled(params.at("enabled").b);
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