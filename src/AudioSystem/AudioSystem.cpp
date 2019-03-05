#include "AudioSystem.h"
AudioSystem* AudioSystem::instance_ = nullptr;

void AudioSystem::ERRCHECK(FMOD_RESULT result) //Checks if there is any error
{
	if (result != FMOD_OK) {
		std::cout << FMOD_ErrorString(result) << std::endl;
		// printf("FMOD error %d - %s", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

void AudioSystem::init()
{
	FMOD::System_Create;
	result = FMOD::System_Create(&system);      // Create the main system object.
	ERRCHECK(result);


	result = system->init(128, FMOD_INIT_NORMAL, 0); // Initialize FMOD
	ERRCHECK(result);
}

AudioSystem* AudioSystem::Instance() {
	if (instance_ == nullptr) {
		instance_ = new AudioSystem();
	}
	return instance_;
}

void AudioSystem::addSound(const std::string& audioId, const std::string& filename) {
	std::string route = audioId;
	route = PATH_ + route;
	result = system->createSound(route.c_str(), FMOD_DEFAULT, 0, &sounds_[audioId]);
}


void AudioSystem::update() {
	result = system->update();
}

void AudioSystem::play(std::string audioId, FMOD::Channel* channel, float volume, float pitch) {
	system->playSound(sounds_[audioId], 0, false, &channel);
	channel->setVolume(volume);
	channel->setPitch(pitch);
}
void AudioSystem::pause(FMOD::Channel* channel) {
	bool paused;
	channel->getPaused(&paused);
	channel->setPaused(!paused);
}

void AudioSystem::stop(FMOD::Channel* channel) {
	channel->stop();
}

void AudioSystem::release(std::string audioId) {
	result = sounds_[audioId]->release();	ERRCHECK(result);
}
