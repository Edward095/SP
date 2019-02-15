#include "c_Sound.h"



c_Sound::c_Sound()
{
}


c_Sound::~c_Sound()
{
}

void c_Sound::f_Init_Sound()
{
	// Menu
	v_MM_Music = irrklang::createIrrKlangDevice();
	v_MM_SFX = irrklang::createIrrKlangDevice();

	// Garage

	// driving

	// Levels
	v_L1_Music = irrklang::createIrrKlangDevice();
	v_L2_Music = irrklang::createIrrKlangDevice();
	v_L3_Music = irrklang::createIrrKlangDevice();
	// Gameplay

	f_InitSFX();
}

void c_Sound::f_Start_Menu_music()
{
	if (!v_MM_Music)
	{
	}
	v_MM_Music->play2D("SFX//MainMenu_Music.wav", true);

}

void c_Sound::f_Pause_Menu_Music()
{
	v_MM_Music->setAllSoundsPaused();
}
void c_Sound::f_Unpause_Menu_Music()
{
	v_MM_Music->setAllSoundsPaused(false);
}


void c_Sound::f_Menu_MoveSelect()
{
	v_MM_SFX->play2D(v_MoveSound);
}

void c_Sound::f_Menu_ConfirmSelect()
{
	v_MM_SFX->play2D(v_ConfirmSound);
}


void c_Sound::f_Level_1_music()
{
	v_L1_Music->play2D("SFX//Level1_Music.mp3");
}

void c_Sound::f_PauseLevel_1_music()
{
	v_L1_Music->setAllSoundsPaused();
}

void c_Sound::f_UnpauseLevel_1_music()
{
	v_L1_Music->setAllSoundsPaused(false);
}

void c_Sound::f_Level_2_music()
{
	v_L2_Music->play2D("SFX//Level2_Music.mp3");
}

void c_Sound::f_PauseLevel_2_music()
{
	v_L2_Music->setAllSoundsPaused();
}

void c_Sound::f_UnpauseLevel_2_music()
{
	v_L2_Music->setAllSoundsPaused(false);
}

void c_Sound::f_Level_3_music()
{
	v_L3_Music->play2D("SFX//Level3_Music.mp3");
}

void c_Sound::f_PauseLevel_3_music()
{
	v_L3_Music->setAllSoundsPaused();
}

void c_Sound::f_UnpauseLevel_3_music()
{
	v_L3_Music->setAllSoundsPaused(false);
}

void c_Sound::f_InitSFX()
{
	v_MoveSound = v_MM_SFX->addSoundSourceFromFile("SFX//MainMenu_SFX_MoveSelect.mp3");
	v_ConfirmSound = v_MM_SFX->addSoundSourceFromFile("SFX//MainMenu_SFX_ConfirmSelect.ogg");
}