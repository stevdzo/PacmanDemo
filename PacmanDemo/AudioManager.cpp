#include "AudioManager.h"

AudioManager* AudioManager::instance = nullptr;

AudioManager::AudioManager() {
	std::cout << "Initializing AudioManager." << std::endl;
	initFmod();
	loadAudio();
}

AudioManager* AudioManager::getInstance() {
	if (instance == nullptr)
		instance = new AudioManager();		
    return instance;
}

bool AudioManager::initFmod() {
	FMOD_RESULT result;
	result = FMOD::System_Create(&m_system);
	if (result != FMOD_OK)
		return false;

	result = m_system->init(50, FMOD_INIT_NORMAL, NULL);
	if (result != FMOD_OK)
		return false;
	return true;
}

bool AudioManager::loadAudio() {

	FMOD_RESULT result;

	result = m_system->createSound(pacEatSfxFilePath, FMOD_DEFAULT, 0, &m_sfxPacEat);
	result = m_system->createSound(introSfxFilePath,  FMOD_DEFAULT | FMOD_2D, 0, &m_sfxIntro);

	return true;
}

void AudioManager::playPacEatSound() {
	m_system->playSound(m_sfxPacEat, 0, false, &m_chEat);
}

void AudioManager::playIntroSound() {
	FMOD::Channel* channel;
	m_system->playSound(m_sfxIntro, 0, false, &channel);
}

bool AudioManager::isPlaying(FMOD::Channel* p_channel) {

	bool isPlaying;
	p_channel->isPlaying(&isPlaying);

	return isPlaying;
}

AudioManager::~AudioManager() {
    std::cout << "Deleting AudioManager." << std::endl;
}