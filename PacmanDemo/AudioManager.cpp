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
	result = m_system->createSound(pacDieSfxFilePath, FMOD_DEFAULT, 0, &m_sfxPacDie);
	result = m_system->createSound(frightenedSfxFilePath, FMOD_LOOP_NORMAL | FMOD_2D, 0, &m_sfxFrightened);
	result = m_system->createSound(eatGhostSfxFilePath, FMOD_DEFAULT, 0, &m_sfxEatGhost);
	result = m_system->createSound(siren1SfxFilePath, FMOD_LOOP_NORMAL | FMOD_2D, 0, &m_sfxSiren1);

	return true;
}

void AudioManager::playPacEatSound() {
	if (!AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chEat))
		m_system->playSound(m_sfxPacEat, 0, false, &m_chEat);
}

void AudioManager::playIntroSound() {
	m_system->playSound(m_sfxIntro, 0, false, &m_chIntro);
}

void AudioManager::playDieSound() {
	m_system->playSound(m_sfxPacDie, 0, false, &m_chDie);
}

void AudioManager::playFrightenedSound() {
	if (!AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chFrightened))
		m_system->playSound(m_sfxFrightened, 0, false, &m_chFrightened);
}

void AudioManager::playEatGhostSound() {
	m_system->playSound(m_sfxEatGhost, 0, false, &m_chEatGhost);
}

void AudioManager::playSiren1Sound() {
	if (!AudioManager::getInstance()->isPlaying(AudioManager::getInstance()->m_chSiren1))
		m_system->playSound(m_sfxSiren1, 0, false, &m_chSiren1);
}

bool AudioManager::isPlaying(FMOD::Channel* p_channel) {

	bool isPlaying;
	p_channel->isPlaying(&isPlaying);

	return isPlaying;
}

AudioManager::~AudioManager() {
    std::cout << "Deleting AudioManager." << std::endl;
}