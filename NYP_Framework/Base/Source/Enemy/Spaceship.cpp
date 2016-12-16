#include "Spaceship.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Projectile/Projectile.h"

SpaceShip::SpaceShip()
: defaultPos(Vector3(0, 0, 0))
, defaultTarget(Vector3(0, 0, 0))
, defaultUp(Vector3(0, 0, 0))
, target(Vector3(0, 0, 0))
, up(Vector3(0, 0, 0))
, maxBoundary(Vector3(0, 0, 0))
, minBoundary(Vector3(0, 0, 0))
, m_pTerrain(NULL)
{
}

SpaceShip::SpaceShip(Mesh* _modelMesh)
: defaultPos(Vector3(0, 0, 0))
, defaultTarget(Vector3(0, 0, 0))
, defaultUp(Vector3(0, 0, 0))
, target(Vector3(0, 0, 0))
, up(Vector3(0, 0, 0))
, maxBoundary(Vector3(0, 0, 0))
, minBoundary(Vector3(0, 0, 0))
, m_pTerrain(NULL)
{

}

SpaceShip::~SpaceShip()
{
}

void SpaceShip::Init(CPlayerInfo* playerInfo)
{
	position.Set(Math::RandFloatMinMax(-40, 40), Math::RandFloatMinMax(0, 10), Math::RandFloatMinMax(100, 200));
	m_dSpeed = 35.f;
	InitLOD("Spaceship", "cube", "Spaceship2");
	isReady = false;
	isDead = false;
	/*InitLOD("")*/
}

void SpaceShip::Reset()
{
	position = defaultPos;
	target = defaultTarget;
	up = defaultUp;
}

void SpaceShip::setPos(const Vector3& newPos)
{
	position = newPos;
}

void SpaceShip::setTarget(const Vector3& newTarget)
{
	target = newTarget;
}

void SpaceShip::setUp(const Vector3& newUp)
{
	up = newUp;
}

void SpaceShip::setBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

void SpaceShip::setTerrain(GroundEntity* new_Terrain)
{
	if (new_Terrain != NULL)
	{
		m_pTerrain = new_Terrain;
		setBoundary(m_pTerrain->GetMaxBoundary(), m_pTerrain->GetMinBoundary());
	}
}

Vector3 SpaceShip::getPos() const
{
	return position;
}

Vector3 SpaceShip::getTarget() const
{
	return target;
}

Vector3 SpaceShip::getUp() const
{
	return up;
}

GroundEntity* SpaceShip::getTerrain()
{
	return m_pTerrain;
}

void SpaceShip::Update(double dt)
{
	//Vector3 view = (target - position).Normalized();
	//position += view * (float)m_dSpeed * (float)dt;

	//Constrain();

	position += velocity * dt;
	Timer += dt;
	/*if (Timer > 10 && !Fire)
	{
		Fire = true;
		Timer = 0;
	}*/

	//Movement(Update the way you want)
}

void SpaceShip::Constrain()
{
	if (position.x > maxBoundary.x - 1.f)
		position.x = maxBoundary.x - 1.f;
	if (position.x < minBoundary.x + 1.f)
		position.x = minBoundary.x + 1.f;
	if (position.z > maxBoundary.z - 1.f)
		position.z = maxBoundary.z - 1.f;
	if (position.z < minBoundary.z + 1.f)
		position.z = minBoundary.z + 1.f;

	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

void SpaceShip::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus())
	{
		if (theDetailLevel != NO_DETAILS)
		{
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}

bool SpaceShip::GetReady()const
{
	return isReady;
}

void SpaceShip::SetReady(const bool& newReady)
{
	isReady = newReady;
}