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

SoundSystem::SoundSystem()	
{
	soundsystem = new System();
}

SoundSystem::~SoundSystem()
{
	if (soundsystem)
	{
		soundsystem->release();
		soundsystem = nullptr;
	}
}

void SoundSystem::System_Create(&system)
{
	soundsystem->System_Create(&system);
}

void SoundSystem::init()
{
	soundsystem->init(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
}

void SoundSystem::createSound(const char* addressname, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, Sound** sound)
{
	soundsystem->createSound(addressname, mode, nullptr, sound);
}

void SoundSystem::playSound()
{
	soundsystem->
}

void SoundSystem::update()
{
	soundsystem->
}

void SoundSystem::release()
{
	soundsystem->release();
}

/*
	todo:
	create system then initialise in game.cpp's initialise()
	load sound in scene where it should be
	clear the sound in gmae.cpp's destructor
*/