#include "c_Sound.h"



c_Sound::c_Sound()
{
}


c_Sound::~c_Sound()
{
}

void c_Sound::f_Init_Sound()
{
	v_MM_Music = irrklang::createIrrKlangDevice();
}

void c_Sound::f_Start_MainMenu_music()
{
	v_MM_Music->play2D("", true);
}
