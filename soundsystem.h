#ifndef  __SOUNDSYSTEM_H  
#define  __SOUNDSYSTEM_H 

// Library Includes: 
#include "fmod.hpp"

using FMOD::System;
using FMOD::Sound;
using FMOD::Channel;
using FMOD::ChannelGroup;

class SoundSystem 
{
public:
	SoundSystem();
	~SoundSystem();

	void init();
	FMOD_RESULT createSound(const char* addressname, FMOD_MODE mode, Sound** sound);
	void playSound(Sound* sound, ChannelGroup* channelgroup, bool paused, Channel** channel);
	void update();
	void release();

private:
	FMOD::System* system;

};


#endif //   SOUNDSYSTEM_H