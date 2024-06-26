#ifndef _AUDIO_MANAGER_H
#define _AUDIO_MANAGER_H

#include "Globals.h"
#include "fmod.hpp"

// Singleton klasa
class AudioManager {

private:

	static AudioManager* instance;

	FMOD::System* m_system;

	bool initFmod();
	bool loadAudio();

	bool updateSirenSound;

	AudioManager();

public:

	static AudioManager* getInstance();

	FMOD::Sound* m_sfxPacEat;
	FMOD::Sound* m_sfxIntro;
	FMOD::Sound* m_sfxPacDie;
	FMOD::Sound* m_sfxFrightened;
	FMOD::Sound* m_sfxEatGhost;
	FMOD::Sound* m_sfxSiren;
	FMOD::Sound* m_sfxSiren1;
	FMOD::Sound* m_sfxRetreating;
	FMOD::Sound* m_sfxExtend;

	Vector<FMOD::Sound*> m_sirens;

	FMOD::Channel* m_chEat;
	FMOD::Channel* m_chIntro;
	FMOD::Channel* m_chDie;
	FMOD::Channel* m_chFrightened;
	FMOD::Channel* m_chEatGhost;
	FMOD::Channel* m_chSiren;
	FMOD::Channel* m_chRetreating;
	FMOD::Channel* m_chExtend;

	void playPacEatSound();
	void playIntroSound();
	void playDieSound();
	void playFrightenedSound();
	void playEatGhostSound();
	void playSirenSound();
	void playRetreatingSound();
	void playExtendSound();

	void changeSirenSound();

	bool isPlaying(FMOD::Channel* p_channel);

	~AudioManager();
};
#endif