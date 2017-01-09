#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Projectile/Projectile.h"

Enemy::Enemy()
	: GenericEntity(NULL)
	, defaultPos(Vector3(0, 0, 0))
	, defaultTarget(Vector3(0, 0, 0))
	, defaultUp(Vector3(0, 0, 0))
	, target(Vector3(0, 0, 0))
	, up(Vector3(0, 0, 0))
	, maxBoundary(Vector3(0, 0, 0))
	, minBoundary(Vector3(0, 0, 0))
	, m_pTerrain(NULL)
{
}

Enemy::Enemy(Mesh* _modelMesh)
	: GenericEntity(NULL)
	, defaultPos(Vector3(0, 0, 0))
	, defaultTarget(Vector3(0, 0, 0))
	, defaultUp(Vector3(0, 0, 0))
	, target(Vector3(0, 0, 0))
	, up(Vector3(0, 0, 0))
	, maxBoundary(Vector3(0, 0, 0))
	, minBoundary(Vector3(0, 0, 0))
	, m_pTerrain(NULL)
{

}

Enemy::~Enemy()
{
}

void Enemy::Init()
{

	Fire = false;
	Timer = 0;
	Health = 1;
}

void Enemy::Reset()
{
	position = defaultPos;
	target = defaultTarget;
	up = defaultUp;
}

void Enemy::setPos(const Vector3& newPos)
{
	position = newPos;
}

void Enemy::setTarget(const Vector3& newTarget)
{
	target = newTarget;
}

void Enemy::setUp(const Vector3& newUp)
{
	up = newUp;
}

void Enemy::setBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

void Enemy::setTerrain(GroundEntity* new_Terrain)
{
	if (new_Terrain != NULL)
	{
		m_pTerrain = new_Terrain;
		setBoundary(m_pTerrain->GetMaxBoundary(), m_pTerrain->GetMinBoundary());
	}
}

Vector3 Enemy::getPos() const
{
	return position;
}

Vector3 Enemy::getTarget() const
{
	return target;
}

Vector3 Enemy::getUp() const
{
	return up;
}

GroundEntity* Enemy::getTerrain()
{
	return m_pTerrain;
}

void Enemy::Update(double dt)
{
	//Vector3 view = (target - position).Normalized();
	//position += view * (float)m_dSpeed * (float)dt;

	Constrain();
	Timer+=dt;
	if (Timer > 5 && !Fire)
	{
		Fire = true;
		Timer = 0;
	}

	//Movement(Update the way you want)
}

void Enemy::Constrain()
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

void Enemy::Render(Mesh* _mesh)
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