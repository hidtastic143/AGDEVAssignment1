#include "Sound.h"

ISoundEngine* engine;

void StartUP()
{
	engine = createIrrKlangDevice();
}

void PlayGun()
{
	engine->setSoundVolume(0.5f);
	engine->play2D("Audio/PewPew.wav", false);
	engine->setSoundVolume(1.f);
}

void ReloadGun()
{
	engine->setSoundVolume(0.5f);
	engine->play2D("Audio/Reload.wav", false);
	engine->setSoundVolume(1.f);
}

void DamageBoss()
{
	if (engine->isCurrentlyPlaying("Audio/hurt1.wav"))
		return;
	engine->setSoundVolume(0.5f);
	engine->play2D("Audio/hurt1.wav", false);
	engine->setSoundVolume(1.f);
}

void DryFire()
{
	engine->setSoundVolume(0.5f);
	engine->play2D("Audio/DryFire.wav", false);
	engine->setSoundVolume(1.f);
}

void Blast()
{
	if (engine->isCurrentlyPlaying("Audio/Blast.wav"))
		return;
	engine->setSoundVolume(0.5f);
	engine->play2D("Audio/Blast.wav", false);
	engine->setSoundVolume(1.f);
}

void DeleteEngine()
{
	engine->drop();
}
