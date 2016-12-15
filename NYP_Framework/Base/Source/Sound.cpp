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

void DeleteEngine()
{
	engine->drop();
}
