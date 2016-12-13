#pragma once

#include "WeaponInfo.h"

class Rifle :
	public CWeaponInfo
{
public:
	Rifle();
	virtual ~Rifle();

	// Initialise this instance to default values
	void Init(void);

	void Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
};

