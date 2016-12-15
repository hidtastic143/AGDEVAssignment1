#pragma once

#include "Enemy.h"

class SpaceShip : public Enemy
{
protected:
	Vector3 defaultPos, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	double m_dAcceleration;

	bool isReady;

public:
	SpaceShip();
	SpaceShip(Mesh* _modelMesh);
	virtual ~SpaceShip();

	void Init(CPlayerInfo* playerInfo);
	// Reset Enemy
	void Reset();

	// Set Position
	void setPos(const Vector3& newPos);
	// Set Target
	void setTarget(const Vector3& newTarget);
	// Set Up
	void setUp(const Vector3& newUp);
	// Setting boundary
	void setBoundary(Vector3 max, Vector3 min);
	// Setting terrain to enemy info
	void setTerrain(GroundEntity* newTerrain);

	// Get Position
	Vector3 getPos() const;
	// Get Target
	Vector3 getTarget() const;
	// Get Up
	Vector3 getUp() const;
	// Get Terrain info
	GroundEntity* getTerrain();

	// Update
	void Update(double dt = 0.0333);
	// Constrain the position
	void Constrain();
	//Render
	void Render();

	bool GetReady()const;
	void SetReady(const bool& newReady);

	Vector3 position;
	Vector3 velocity;
	double m_dSpeed;
	bool isDead;
	bool Fire;
	float Timer;
	float Health;

};