//local include:
#include "soundsystem.h"

//library include:
#include "fmod.hpp"
#include "fmod_errors.h"
#include <cstdio>


//namespace using
using FMOD::System;
using FMOD::Sound;
using FMOD::Channel;
using FMOD::ChannelGroup;

SoundSystem::SoundSystem()	
{
	FMOD::System_Create(&system);
}

SoundSystem::~SoundSystem()
{
	if (system)
	{
		system->release();
		system = nullptr;
	}
}

void SoundSystem::init()
{
	system->init(512, FMOD_INIT_NORMAL, nullptr);
}

void SoundSystem::createSound(const char* addressname, FMOD_MODE mode, Sound** sound)
{
	system->createSound(addressname, mode, nullptr, sound);
}

void SoundSystem::playSound(Sound* sound, ChannelGroup* channelgroup, bool paused, Channel** channel)
{
	system->playSound( sound, channelgroup, paused, channel);
}

void SoundSystem::update()
{
	system->update();
}

void SoundSystem::release()
{
	system->release();
}

/*
	todo:
	create system then initialise in game.cpp's initialise()
	auto thing = new SoundSystem();

	load sound in scene where it should be
	clear the sound in gmae.cpp's destructor
*/