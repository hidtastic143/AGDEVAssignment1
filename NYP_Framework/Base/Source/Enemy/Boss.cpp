#include "Boss.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Projectile/Projectile.h"

Boss::Boss()
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

Boss::Boss(Mesh* _modelMesh)
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

Boss::~Boss()
{
}

void Boss::Init()
{
	GenericEntity* Zbody = Create::Entity("Zbody");
	Zbody->SetIsZombieBody(true);
	//Zbody->SetCollider(true);
	Zbody->SetAABB(Zbody->GetScale() - Vector3(15, 20, 15), -Zbody->GetScale() - Vector3(-15, -20, -15));
	Zbody->InitLOD("sphere", "Zbody", "ZbodyQ");
	CSceneNode* parent = CSceneGraph::GetInstance()->AddNode(Zbody);

	GenericEntity* Zhead = Create::Entity("Zhead");
	Zhead->SetIsZombieHead(true);
	//Zhead->SetCollider(true);
	Zhead->SetAABB(Zhead->GetScale() - Vector3(10, 10, 10), -Zhead->GetScale() - Vector3(-10, -10, -10));
	Zhead->InitLOD("sphere", "Zhead", "ZheadQ");
	CSceneNode* childHead = parent->AddChild(Zhead);

	GenericEntity* Zlhand = Create::Entity("Zlhand");
	Zlhand->SetAABB(Zlhand->GetScale() - Vector3(5, 5, 5), -Zlhand->GetScale() - Vector3(-5, -5, -5));
	Zlhand->SetIsZombieHand(true);
	Zlhand->InitLOD("sphere", "Zlhand", "ZlhandQ");
	CSceneNode* childLeftHand = parent->AddChild(Zlhand);
	///childLeftHand->ApplyRotate(90, 1, 0, 0);
	////childLeftHand->ApplyTranslate(3.5f, 2.f, -	2.f);

	Zrhand = Create::Entity("Zrhand");
	Zrhand->SetAABB(Zrhand->GetScale() - Vector3(5, 5, 5), -Zrhand->GetScale() - Vector3(-5, -5, -5));
	Zrhand->SetIsZombieHand(true);
	Zrhand->InitLOD("sphere", "Zrhand", "ZrhandQ");
	CSceneNode* childRightHand = parent->AddChild(Zrhand);
	//childRightHand->ApplyRotate(90, 1, 0, 0);
	//childRightHand->ApplyTranslate(-3.5f, 2.f, -2.f);

	GenericEntity* Zlleg = Create::Entity("Zlleg");
	Zlleg->SetAABB(Zlleg->GetScale(), -Zlleg->GetScale());
	Zlleg->SetIsZombieLeg(true);
	Zlleg->InitLOD("sphere", "Zlleg", "ZllegQ");
	CSceneNode* childLeftLeg = parent->AddChild(Zlleg);
	//childLeftLeg->ApplyTranslate(1.f, -5.f, 0.f);

	GenericEntity* Zrleg = Create::Entity("Zrleg");
	Zrleg->SetAABB(Zrleg->GetScale(), -Zrleg->GetScale());
	Zrleg->SetIsZombieLeg(true);
	Zrleg->InitLOD("sphere", "Zlleg", "Zrleg");
	CSceneNode* childRightLeg = parent->AddChild(Zrleg);
	//childRightLeg->ApplyTranslate(-1.f, -5.f, 0.f);

	Fire = false;
	Timer = 0;
	health = 25000;
}

int Boss::GetHealth() const
{
	return health;
}

void Boss::setHealth(int newHealth)
{
	health = newHealth;
}

void Boss::Reset()
{
	position = defaultPos;
	target = defaultTarget;
	up = defaultUp;
}

void Boss::setPos(const Vector3& newPos)
{
	position = newPos;
}

void Boss::setTarget(const Vector3& newTarget)
{
	target = newTarget;
}

void Boss::setUp(const Vector3& newUp)
{
	up = newUp;
}

void Boss::setBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

void Boss::setTerrain(GroundEntity* new_Terrain)
{
	if (new_Terrain != NULL)
	{
		m_pTerrain = new_Terrain;
		setBoundary(m_pTerrain->GetMaxBoundary(), m_pTerrain->GetMinBoundary());
	}
}

Vector3 Boss::getPos() const
{
	return position;
}

Vector3 Boss::getTarget() const
{
	return target;
}

Vector3 Boss::getUp() const
{
	return up;
}

GroundEntity* Boss::getTerrain()
{
	return m_pTerrain;
}

void Boss::Update(double dt)
{
	//Vector3 view = (target - position).Normalized();
	//position += view * (float)m_dSpeed * (float)dt;

	Constrain();
	AttackPlayer(Zrhand->GetPosition(), CPlayerInfo::GetInstance()->GetPos());
	Timer += dt;
	if (Timer > 5 && !Fire)
	{
		Fire = true;
		Timer = 0;
	}

	//Movement(Update the way you want)
}

void Boss::Constrain()
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

void Boss::Render()
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

void Boss::AttackPlayer(Vector3 position, Vector3 target)
{
	if (Fire)
	{
		CProjectile* Car = Create::Projectiles("car",
			Vector3(position.x, position.y, position.z + 20),
			(target - Vector3(position.x, position.y, position.z + 20)).Normalized(),
			5.0f,
			200.f);


		//Car->SetIsProjectile(true);
		//Car->SetCollider(true);
		Car->SetIsCar(true);
		Car->SetAABB(Vector3(5, 5, 5), Vector3(-5, -5, -5));
		Car->InitLOD("car", "sphere", "cube3");
		Fire = false;
	}
}