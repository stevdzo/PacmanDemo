#ifndef _AUDIO_MANAGER_H
#define _AUDIO_MANAGER_H

#include "Globals.h"
#include "fmod.hpp"

// Singleton class
class AudioManager {

private:

	static AudioManager* instance;

	FMOD::System* m_system;

	bool initFmod();
	bool loadAudio();

	AudioManager();

public:

	static AudioManager* getInstance();

	FMOD::Sound* m_sfxPacEat;
	FMOD::Sound* m_sfxIntro;
	FMOD::Sound* m_sfxPacDie;
	FMOD::Sound* m_sfxFrightened;
	FMOD::Sound* m_sfxEatGhost;

	FMOD::Channel* m_chEat;
	FMOD::Channel* m_chIntro;
	FMOD::Channel* m_chDie;
	FMOD::Channel* m_chFrightened;
	FMOD::Channel* m_chEatGhost;

	void playPacEatSound();
	void playIntroSound();
	void playDieSound();
	void playFrightenedSound();
	void playEatGhostSound();

	bool isPlaying(FMOD::Channel* p_channel);

	~AudioManager();
};
#endif