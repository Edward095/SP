#ifndef C_SOUND_H
#define C_SOUND_H
#include "irrKlang.h"

class c_Sound
{
public:
	c_Sound();
	~c_Sound();

	void f_Init_Sound();

	void f_Start_MainMenu_music();
private:
	irrklang::ISoundEngine* v_MM_Music;
};

#endif
