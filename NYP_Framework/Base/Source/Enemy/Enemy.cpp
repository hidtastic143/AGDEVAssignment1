#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

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
	//// Set the default values
	//defaultPos.Set(0, 0, 10);
	//defaultTarget.Set(0, 0, 0);
	//defaultUp.Set(0, 1, 0);
	//// Set the current values
	//position.Set(10.0f, 0.0f, 0.0f);
	//target.Set(10.0f, 0.0f, 450.0f);
	//up.Set(0.0f, 1.0f, 0.0f);
	//// Set Boundary
	//maxBoundary.Set(1, 1, 1);
	//minBoundary.Set(-1, -1, -1);
	//// Set speed
	//m_dSpeed = 1.0;
	//// Initialise the LOD meshes
	//InitLOD("Zhead", "sphere", "cubeSG");
	//// Initialise the Collider
	//this->SetCollider(true);
	//this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));
	//// Add to EntityManager
	//EntityManager::GetInstance()->AddEntity(this, true);

	GenericEntity* Zbody = Create::Entity("Zbody");
	Zbody->SetIsZombie(true);
	Zbody->SetCollider(true);
	Zbody->SetAABB(Zbody->GetScale(), -Zbody->GetScale());
	//Zbody->InitLOD("Zbody", "sphere", "cubeSG");
	CSceneNode* parent = CSceneGraph::GetInstance()->AddNode(Zbody);

	GenericEntity* Zhead = Create::Entity("Zhead");
	Zhead->SetIsZombie(true);
	CSceneNode* childHead = parent->AddChild(Zhead);
	childHead->ApplyTranslate(0.f, 5.f, 0.f);

	GenericEntity* Zlhand = Create::Entity("Zlhand");
	//Zlhand->SetCollider(true);
	//Zlhand->SetAABB(Zlhand->GetScale(), -Zlhand->GetScale());
	Zlhand->SetIsZombie(true);
	CSceneNode* childLeftHand = parent->AddChild(Zlhand);
	childLeftHand->ApplyRotate(90, 1, 0, 0);
	childLeftHand->ApplyTranslate(3.5f, 2.f, -	2.f);

	GenericEntity* Zrhand = Create::Entity("Zrhand");
	//Zrhand->SetCollider(true);
	//Zrhand->SetAABB(Zrhand->GetScale(), -Zrhand->GetScale());
	Zrhand->SetIsZombie(true);
	CSceneNode* childRightHand = parent->AddChild(Zrhand);
	childRightHand->ApplyRotate(90, 1, 0, 0);
	childRightHand->ApplyTranslate(-3.5f, 2.f, -2.f);

	GenericEntity* Zlleg = Create::Entity("Zlleg");
	//Zlleg->SetCollider(true);
	//Zlleg->SetAABB(Zlleg->GetScale(), -Zlleg->GetScale());
	Zlleg->SetIsZombie(true);
	CSceneNode* childLeftLeg = parent->AddChild(Zlleg);
	childLeftLeg->ApplyTranslate(1.f, -5.f, 0.f);

	GenericEntity* Zrleg = Create::Entity("Zrleg");
	//Zrleg->SetCollider(true);
	//Zrleg->SetAABB(Zrleg->GetScale(), -Zrleg->GetScale());
	Zrleg->SetIsZombie(true);
	CSceneNode* childRightLeg = parent->AddChild(Zrleg);
	childRightLeg->ApplyTranslate(-1.f, -5.f, 0.f);
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
	Vector3 view = (target - position).Normalized();
	position += view * (float)m_dSpeed * (float)dt;

	Constrain();

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

void Enemy::Render()
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