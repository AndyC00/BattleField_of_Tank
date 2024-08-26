#ifndef  __SOUNDSYSTEM_H  
#define  __SOUNDSYSTEM_H 

// Library Includes: 
#include "fmod.hpp"

class SoundSystem 
{
public:
	SoundSystem();
	~SoundSystem();

	bool System_Create();
	void init();
	void createSound();
	void playSound();
	void update();
	void release();



};


#endif //   SOUNDSYSTEM_H 