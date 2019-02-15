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
	// Menu Music
	void f_Start_Menu_music();
	void f_Pause_Menu_Music();
	void f_Unpause_Menu_Music();
	// Menu SFX
	void f_Menu_MoveSelect();
	void f_Menu_ConfirmSelect();

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
	// Music
	irrklang::ISoundEngine* v_MM_Music;
	irrklang::ISoundEngine* v_L1_Music; 
	irrklang::ISoundEngine* v_L2_Music;
	irrklang::ISoundEngine* v_L3_Music;

	// SFX
	irrklang::ISoundEngine* v_MM_SFX;
	irrklang::ISoundSource* v_MoveSound;
	irrklang::ISoundSource* v_ConfirmSound;

	void f_InitSFX();
};

#endif
