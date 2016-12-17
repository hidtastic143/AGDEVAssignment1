#include "EntityBase.h"

EntityBase::EntityBase()
	: position(0.0f, 0.0f, 0.0f)
	, scale(1.0f, 1.0f, 1.0f)
	, isDone(false)
	, m_bCollider(false)
	, bLaser(false)
	, isZombieHead(false)
	, isZombieBody(false)
	, isZombieHand(false)
	, isZombieLeg(false)
	, isProjectile(false)
	, isCar(false)
{
}

EntityBase::~EntityBase()
{
}

void EntityBase::Update(double _dt)
{
}

void EntityBase::Render()
{
}

void EntityBase::RenderUI()
{
}

bool EntityBase::IsDone()
{
	return isDone;
}

void EntityBase::SetIsDone(bool _value)
{
	isDone = _value;
}

// Check if this entity has a collider class parent
bool EntityBase::HasCollider(void) const
{
	return m_bCollider;
}

// Set the flag to indicate if this entity has a collider class parent
void EntityBase::SetCollider(const bool _value)
{
	m_bCollider = _value;
}

// Set the flag, bLaser
void EntityBase::SetIsLaser(const bool bLaser)
{
	this->bLaser = bLaser;
}

// Get the flag, bLaser
bool EntityBase::GetIsLaser(void) const
{
	return bLaser;
}

void EntityBase::SetIsZombieHead(const bool isZombieHead)
{
	this->isZombieHead = isZombieHead;
}

bool EntityBase::GetIsZombieHead(void) const
{
	return isZombieHead;
}

void EntityBase::SetIsZombieBody(const bool isZombieBody)
{
	this->isZombieBody = isZombieBody;
}

bool EntityBase::GetIsZombieBody(void) const
{
	return isZombieBody;
}
void EntityBase::SetIsZombieHand(const bool isZombieHand)
{
	this->isZombieHand = isZombieHand;
}

bool EntityBase::GetIsZombieHand(void) const
{
	return isZombieHand;
}

void EntityBase::SetIsZombieLeg(const bool isZombieLeg)
{
	this->isZombieLeg = isZombieLeg;
}

bool EntityBase::GetIsZombieLeg(void) const
{
	return isZombieLeg;
}

void EntityBase::SetIsProjectile(const bool isProjectile)
{
	this->isProjectile = isProjectile;
}

bool EntityBase::GetIsProjectile(void) const
{
	return isProjectile;
}

void EntityBase::SetIsCar(const bool isCar)
{
	this->isCar = isCar;
}

bool EntityBase::GetIsCar(void) const
{
	return isCar;
}