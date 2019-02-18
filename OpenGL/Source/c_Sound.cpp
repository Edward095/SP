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
	s_MM_Music = irrklang::createIrrKlangDevice();
	s_MM_SFX = irrklang::createIrrKlangDevice();

	// Levels
	s_L1_Music = irrklang::createIrrKlangDevice();
	s_L2_Music = irrklang::createIrrKlangDevice();
	s_L3_Music = irrklang::createIrrKlangDevice();

	// Gameplay
	s_GP_SFX = irrklang::createIrrKlangDevice();

	f_InitSFX();
}

void c_Sound::f_Start_Menu_music()
{
	if (!s_MM_Music)
	{
	}
	s_MM_Music->play2D("SFX//MainMenu_Music.wav", true);

}

void c_Sound::f_Pause_Menu_Music()
{
	s_MM_Music->setAllSoundsPaused();
}
void c_Sound::f_Unpause_Menu_Music()
{
	s_MM_Music->setAllSoundsPaused(false);
}


void c_Sound::f_Menu_MoveSelect()
{
	s_MM_SFX->play2D(s_MoveSound);
}

void c_Sound::f_Menu_ConfirmSelect()
{
	s_MM_SFX->play2D(s_ConfirmSound);
}


void c_Sound::f_Level_1_music()
{
	s_L1_Music->play2D("SFX//Level1_Music.mp3");
}

void c_Sound::f_PauseLevel_1_music()
{
	s_L1_Music->setAllSoundsPaused();
}

void c_Sound::f_UnpauseLevel_1_music()
{
	s_L1_Music->setAllSoundsPaused(false);
}

void c_Sound::f_Level_2_music()
{
	s_L2_Music->play2D("SFX//Level2_Music.mp3");
}

void c_Sound::f_PauseLevel_2_music()
{
	s_L2_Music->setAllSoundsPaused();
}

void c_Sound::f_UnpauseLevel_2_music()
{
	s_L2_Music->setAllSoundsPaused(false);
}

void c_Sound::f_Level_3_music()
{
	s_L3_Music->play2D("SFX//Level3_Music.mp3");
}

void c_Sound::f_PauseLevel_3_music()
{
	s_L3_Music->setAllSoundsPaused();
}

void c_Sound::f_UnpauseLevel_3_music()
{
	s_L3_Music->setAllSoundsPaused(false);
}

void c_Sound::f_Game_Nitro()
{
	s_GP_SFX->play2D(s_Nitro);
}

void c_Sound::f_InitSFX()
{
	// Menu
	s_MoveSound = s_MM_SFX->addSoundSourceFromFile("SFX//MainMenu_SFX_MoveSelect.mp3");
	s_ConfirmSound = s_MM_SFX->addSoundSourceFromFile("SFX//MainMenu_SFX_ConfirmSelect.ogg");

	// Gameplay
	s_Nitro = s_GP_SFX->addSoundSourceFromFile("SFX//Game_SFX_Nitro.mp3");
}

void c_Sound::f_AdjustMusicVolume(float v_Volume)
{
	s_MM_Music->setSoundVolume(v_Volume);
	s_L1_Music->setSoundVolume(v_Volume);
	s_L2_Music->setSoundVolume(v_Volume);
	s_L3_Music->setSoundVolume(v_Volume);
}

void c_Sound::f_AdjustSFXVolume(float v_Volume)
{
	s_GP_SFX->setSoundVolume(v_Volume);
}