#ifndef C_SOUND_H
#define C_SOUND_H
#include <iostream>
#include "irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

class c_Sound
{
public:
	~c_Sound();
	static c_Sound* getInstance();
	void f_Init_Sound();
	void f_AdjustMusicVolume(float v_Volume);
	void f_AdjustSFXVolume(float v_Volume);

	// Menu Music
	void f_Start_Menu_music();
	void f_Pause_Menu_Music();
	void f_Unpause_Menu_Music();

	void f_WeatherRain();

	// Menu SFX
	void f_Menu_MoveSelect();
	void f_Menu_ConfirmSelect();

	// Game SFX
	void f_Game_Ability_Nitro();
	void f_Game_Ability_Teleport();
	void f_Game_Ability_Freezetime();
	void f_Game_Fanfare_Boost();
	void f_Game_Fanfare_Slow();
	void f_Game_Fanfare_Startline();

	// Level Music
	void f_Level_1_music();
	void f_PauseLevel_1_music();
	void f_UnpauseLevel_1_music();

	void f_Level_2_music();
	void f_PauseLevel_2_music();
	void f_UnpauseLevel_2_music();

	void f_Level_3_music();
	void f_PauseLevel_3_music();
	void f_UnpauseLevel_3_music();

private:
	c_Sound();
	static c_Sound* instance;

	// Music
	irrklang::ISoundEngine* s_MM_Music;
	irrklang::ISoundEngine* s_L1_Music; 
	irrklang::ISoundEngine* s_L2_Music;
	irrklang::ISoundEngine* s_L3_Music;

	irrklang::ISoundEngine* s_Weather_Rain;

	// SFX
	irrklang::ISoundEngine* s_MM_SFX;
	irrklang::ISoundSource* s_MoveSound;
	irrklang::ISoundSource* s_ConfirmSound;

	irrklang::ISoundEngine* s_GP_SFX;
	irrklang::ISoundSource* s_BoostPad;
	irrklang::ISoundSource* s_SlowPad;
	irrklang::ISoundSource* s_Nitro;
	irrklang::ISoundSource* s_Teleport;
	irrklang::ISoundSource* s_FreezeTime;
	irrklang::ISoundSource* s_StartingLine;

	void f_InitSFX();
};

#endif
