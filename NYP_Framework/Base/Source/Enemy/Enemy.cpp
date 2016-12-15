#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
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
	GenericEntity* Zbody = Create::Entity("Zbody");
	Zbody->SetIsZombieBody(true);
	//Zbody->SetCollider(true);
	Zbody->SetAABB(Zbody->GetScale() - Vector3(15, 20, 15), -Zbody->GetScale() - Vector3(-15, -20, -15));
	//Zbody->InitLOD("Zbody", "sphere", "cubeSG");
	CSceneNode* parent = CSceneGraph::GetInstance()->AddNode(Zbody);

	GenericEntity* Zhead = Create::Entity("Zhead");
	Zhead->SetIsZombieHead(true);
	Zhead->SetCollider(true);
	Zhead->SetAABB(Zhead->GetScale() - Vector3(10, 10, 10), -Zhead->GetScale() - Vector3(-10, -10, -10));
	CSceneNode* childHead = parent->AddChild(Zhead);

	GenericEntity* Zlhand = Create::Entity("Zlhand");
	Zlhand->SetAABB(Zlhand->GetScale() - Vector3(5, 5, 5), -Zlhand->GetScale() - Vector3(-5, -5, -5));
	Zlhand->SetIsZombieHand(true);
	CSceneNode* childLeftHand = parent->AddChild(Zlhand);
	///childLeftHand->ApplyRotate(90, 1, 0, 0);
	////childLeftHand->ApplyTranslate(3.5f, 2.f, -	2.f);

	Zrhand = Create::Entity("Zrhand");
	Zrhand->SetAABB(Zrhand->GetScale() - Vector3(5, 5, 5), -Zrhand->GetScale() - Vector3(-5, -5, -5));
	Zrhand->SetIsZombieHand(true);
	CSceneNode* childRightHand = parent->AddChild(Zrhand);
	//childRightHand->ApplyRotate(90, 1, 0, 0);
	//childRightHand->ApplyTranslate(-3.5f, 2.f, -2.f);

	GenericEntity* Zlleg = Create::Entity("Zlleg");
	Zlleg->SetAABB(Zlleg->GetScale(), -Zlleg->GetScale());
	Zlleg->SetIsZombieLeg(true);
	CSceneNode* childLeftLeg = parent->AddChild(Zlleg);
	//childLeftLeg->ApplyTranslate(1.f, -5.f, 0.f);

	GenericEntity* Zrleg = Create::Entity("Zrleg");
	Zrleg->SetAABB(Zrleg->GetScale(), -Zrleg->GetScale());
	Zrleg->SetIsZombieLeg(true);
	CSceneNode* childRightLeg = parent->AddChild(Zrleg);
	//childRightLeg->ApplyTranslate(-1.f, -5.f, 0.f);

	Fire = false;
	Timer = 0;
	Health = 55555;
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

void Enemy::AttackPlayer(Vector3 position, Vector3 target)
{
	if (Fire)
	{
		CProjectile* Car = Create::Projectiles("car",
			Vector3(position.x, position.y, position.z + 20),
			(target - Vector3(position.x, position.y, position.z + 20)).Normalized(),
			5.0f,
			200.f);

		
		Car->SetIsProjectile(true);
		Car->SetCollider(true);
		Car->SetAABB(Vector3(2, 2, 2), Vector3(-2, -2, -2));
		
		Fire = false;
	}
}