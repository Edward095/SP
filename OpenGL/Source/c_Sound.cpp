#include "c_Sound.h"



c_Sound::c_Sound()
{
}


c_Sound::~c_Sound()
{
}

void c_Sound::f_Init_Sound()
{
	// Main Menu
	v_MM_Music = irrklang::createIrrKlangDevice();
	v_MM_MoveSelect = irrklang::createIrrKlangDevice();
	v_MM_ConfirmSelect = irrklang::createIrrKlangDevice();

	// Garage

	// Gameplay
	// driving

	// Levels
	v_L1_Music = irrklang::createIrrKlangDevice();
	v_L2_Music = irrklang::createIrrKlangDevice();
	v_L3_Music = irrklang::createIrrKlangDevice();
}

void c_Sound::f_Start_MainMenu_music()
{
	if (!v_MM_Music)
	{
	}
	v_MM_Music->play2D("SFX//MainMenu_Music.wav", true);

}

void c_Sound::f_Pause_MainMenu_Music()
{
	v_MM_Music->setAllSoundsPaused();
}
void c_Sound::f_Unpause_MainMenu_Music()
{
	v_MM_Music->setAllSoundsPaused(false);
}


void c_Sound::f_MainMenu_MoveSelect()
{
	if (!v_MM_MoveSelect)
	{
	}
	v_MM_MoveSelect->play2D("SFX//MainMenu_SFX_MoveSelect.mp3");
}

void c_Sound::f_MainMenu_ConfirmSelect()
{
	v_MM_ConfirmSelect->play2D("SFX//MainMenu_SFX_ConfirmSelect.ogg");
}


void c_Sound::f_Level_1_music()
{
	v_L1_Music->play2D("SFX//Level1_Music.wav");
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
	v_L2_Music->play2D("filename");
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
	v_L3_Music->play2D("filename");
}

void c_Sound::f_PauseLevel_3_music()
{
	v_L3_Music->setAllSoundsPaused();
}

void c_Sound::f_UnpauseLevel_3_music()
{
	v_L3_Music->setAllSoundsPaused(false);
}