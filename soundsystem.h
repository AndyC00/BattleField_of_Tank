#ifndef  __SOUNDSYSTEM_H  
#define  __SOUNDSYSTEM_H 

// Library Includes: 
#include "fmod.hpp"

class SoundSystem 
{
public:
	SoundSystem();
	~SoundSystem();

	void System_Create(&system);
	void init(int, FMOD_STUDIO_INITFLAGS, FMOD_INITFLAGS, void);
	void createSound();
	void playSound();
	void update();
	void release();

private:
	FMOD::System* soundsystem;

};


#endif //   SOUNDSYSTEM_H 