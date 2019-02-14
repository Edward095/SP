#ifndef C_SOUND_H
#define C_SOUND_H
#include <iostream>
#include "irrKlang.h"
#pragma comment(lib, "irrKlang.lib")

class c_Sound
{
public:
	c_Sound();
	~c_Sound();

	void f_Init_Sound();
	// Main Menu SFX
	void f_Start_MainMenu_music();
	void f_Pause_MainMenu_Music();
	void f_Unpause_MainMenu_Music();
	void f_MainMenu_MoveSelect();
	void f_MainMenu_ConfirmSelect();
	// Gameplay SFX

	// Garage SFX

	// Level SFX
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
	irrklang::ISoundEngine* v_MM_Music;
	irrklang::ISoundEngine* v_MM_MoveSelect;
	irrklang::ISoundEngine* v_MM_ConfirmSelect;
	irrklang::ISoundEngine* v_L1_Music; // Level 1 Music
	irrklang::ISoundEngine* v_L2_Music;
	irrklang::ISoundEngine* v_L3_Music;
};

#endif
