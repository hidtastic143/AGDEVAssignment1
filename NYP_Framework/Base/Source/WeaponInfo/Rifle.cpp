#include "Rifle.h"
#include "../Sound.h"
#include "../Projectile/Projectile.h"

Rifle::Rifle()
{
}


Rifle::~Rifle()
{
}

// Initialise this instance to default values
void Rifle::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 15;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 15;
	// The current total number of rounds currently carried by this player
	totalRounds = 210;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 210;

	// The time between shots
	timeBetweenShots = 0.12;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

void Rifle::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 5.0 seconds and travel at 500 units per second
			CProjectile* aProjectile = Create::Projectile("cube",
				position,
				(target - position).Normalized(),
				5.0f,
				300.0f,
				_source);
			aProjectile->SetIsProjectile(true);
			aProjectile->SetCollider(true);
			aProjectile->InitLOD("cube", "sphere", "cubeSG");
			aProjectile->SetAABB(Vector3(0.05f, 0.05f, 0.05f), Vector3(-0.05f, -0.05f, -0.05f));
			bFire = false;
			magRounds--;
			PlayGun();
		}
		else if (magRounds <= 0)
		{
			DryFire();
		}
	}
}