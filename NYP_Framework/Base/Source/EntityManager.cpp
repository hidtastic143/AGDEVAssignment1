#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "Projectile/Laser.h"
#include "SceneGraph\SceneGraph.h"
#include "PlayerInfo\PlayerInfo.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "Enemy\Boss.h"
#include "Enemy\Spaceship.h"
#include "../Source/Sound.h"
#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt, CPlayerInfo* playerInfo, std::vector<SpaceShip*> spaceVec, Boss* boss)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	// Render the Scene Graph
	CSceneGraph::GetInstance()->Update();

	// Render the Spatial Partition
	if (theSpatialPartition)
		theSpatialPartition->Update();

	// Check for Collision amongst entities with collider properties
	CheckForCollision(playerInfo, spaceVec, boss);

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	
	for (it = entityList.begin(); it != end; ++it)
	{
		// This needs to be edited
		if (CSceneGraph::GetInstance()->GetNode(*it) != NULL)
		{

		}
		else/*if (((cameraInfo->GetCameraTarget() - cameraInfo->GetCameraPos()).Dot((*it)->GetPosition() * (*it)->GetScale().x - cameraInfo->GetCameraPos())) > Math::EPSILON)*/
		{
			(*it)->Render();
		}
	}

	// Render the Scene Graph
	CSceneGraph::GetInstance()->Render();

	// Render the Spatial Partition
	if (theSpatialPartition)
		theSpatialPartition->Render();
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity, bool bAddToSpatialPartition)
{
	entityList.push_back(_newEntity);

	// Add to the Spatial Partition
	if (theSpatialPartition && bAddToSpatialPartition)
		theSpatialPartition->Add(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);

		// Remove from SceneNode too
		if (!CSceneGraph::GetInstance()->DeleteNode(_existingEntity))
		{
			cout << "EntityManager::RemoveEntity: Unable to remove this entity from Scene Graph" << endl;
		}
		else
		{
			// Add to the Spatial Partition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEntity);
		}

		return true;	
	}
	// Return false if not found
	return false;
}

// Mark an entity for deletion
bool EntityManager::MarkForDeletion(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		(*findIter)->SetIsDone(true);
		return true;
	}
	// Return false if not found
	return false;
}

// Set a handle to the Spatial Partition to this class
void EntityManager::SetSpatialPartition(CSpatialPartition* theSpatialPartition)
{
	this->theSpatialPartition = theSpatialPartition;
}

// Constructor
EntityManager::EntityManager()
	: theSpatialPartition(NULL)
	, Health(55555)
{
}

// Destructor
EntityManager::~EntityManager()
{
	// Drop the Spatial Partition instance
	CSpatialPartition::GetInstance()->DropInstance();

	// Clear out the Scene Graph
	CSceneGraph::GetInstance()->Destroy();
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{	
	// Check if this object is overlapping that object

	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		||
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	{
		return true;
	}

	//// Check if that object is overlapping this object
	//
	if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
		||
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	{
		return true;
	}

	//// Check if this object is within that object
	//
	if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
		&&
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
		return true;

	//// Check if that object is within this object
	//
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		&&
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
		return true;

	/*if (thatMaxAABB.x > thisMaxAABB.x && thatMinAABB.x <= thisMaxAABB.x)
		return true;
	if (thatMaxAABB.x >= thisMinAABB.x && thatMinAABB.x < thisMinAABB.x)
		return true;
	if (thatMaxAABB.z > thisMaxAABB.z && thatMinAABB.z <= thisMaxAABB.z)
		return true;
	if (thatMaxAABB.z >= thisMinAABB.z && thatMinAABB.z < thisMinAABB.z)
		return true;
	if (thatMaxAABB.y > thisMaxAABB.y && thatMinAABB.y <= thisMaxAABB.y)
		return true;
	if (thatMaxAABB.y >= thisMinAABB.y && thatMinAABB.y < thisMinAABB.y)
		return true;*/


	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// if Radius of bounding sphere of ThisEntity plus Radius of bounding sphere of ThatEntity is 
	// greater than the distance squared between the 2 reference points of the 2 entities,
	// then it could mean that they are colliding with each other.
	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
		DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()) * 2.0)
	{
		return true;
	}

	return false;
}

// Check where a line segment between two positions intersects a plane
bool EntityManager::GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit)
{
	if ((fDst1 * fDst2) >= 0.0f) 
		return false;
	if (fDst1 == fDst2) 
		return false;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return true;
}

// Check two positions are within a box region
bool EntityManager::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
	return false;
}

// Check for intersection between a line segment and a plane
bool EntityManager::CheckLineSegmentPlane(	Vector3 line_start, Vector3 line_end, 
											Vector3 minAABB, Vector3 maxAABB,
											Vector3 &Hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y - minAABB.y, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - minAABB.z, line_end.z - minAABB.z, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 3))
		|| (GetIntersection(line_start.x - maxAABB.x, line_end.x - maxAABB.x, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - maxAABB.y, line_end.y - maxAABB.y, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - maxAABB.z, line_end.z - maxAABB.z, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 3)))
		return true;

	return false;
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(CPlayerInfo* playerInfo, std::vector<SpaceShip*> spaceVec, Boss* boss)
{
	// Check for Collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	colliderThisEnd = entityList.end();
	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		// Check if this entity is a CLaser type
		if ((*colliderThis)->GetIsLaser())
		{
			// Dynamic cast it to a CLaser class
			CLaser* thisEntity = dynamic_cast<CLaser*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					Vector3 hitPosition = Vector3(0, 0, 0);

					// Get the minAABB and maxAABB for (*colliderThat)
					CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);
					Vector3 thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
					Vector3 thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();

					if (CheckLineSegmentPlane(thisEntity->GetPosition(),
						thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(),
						thatMinAABB, thatMaxAABB,
						hitPosition) == true)
					{
						(*colliderThis)->SetIsDone(true);
						(*colliderThat)->SetIsDone(true);

						// Remove from Scene Graph
						if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)))
						{
							cout << "*** This Entity removed ***" << endl;
						}
						// Remove from Scene Graph
						if (CSceneGraph::GetInstance()->DeleteNode((*colliderThat)))
						{
							cout << "*** That Entity removed ***" << endl;
						}

					}
				}
			}
		}
		else if ((*colliderThis)->GetIsProjectile())
		{
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);
			colliderThatEnd = entityList.end();
			int counter = 0;

			/*if (CheckAABBCollision(thisEntity, playerInfo))
			{
				thisEntity->SetIsDone(true);
				playerInfo->SetHP(playerInfo->GetHP() - 5);
				std::cout << "HP : " << playerInfo->GetHP() << std::endl;
			}*/

			if ((thisEntity->GetPosition() - playerInfo->GetPos()).LengthSquared() <= 3.f )
			{
				thisEntity->SetIsDone(true);
				playerInfo->SetHP(playerInfo->GetHP() - 10);
				std::cout << "HP : " << playerInfo->GetHP() << std::endl;
				Blast();
			}

			for (std::vector<SpaceShip*>::iterator it = spaceVec.begin(); it != spaceVec.end(); it++)
			{
				if (((*it)->position - thisEntity->GetPosition()).LengthSquared() <= 25.f)
				{
					(*it)->isDead = true;
					thisEntity->SetIsDone(true);
				}
			}


			for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
				EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);

				if (colliderThat == colliderThis)
					continue;

				if (boss->GetHealth() > 0)
				{
					if ((*colliderThat)->GetIsZombieHead())
					{
						if (CheckAABBCollision(thisEntity, thatEntity))
 						{
							boss->setHealth(boss->GetHealth() - 1000);
							DamageBoss();
							thisEntity->SetIsDone(true);
							CSceneGraph::GetInstance()->DeleteNode((*colliderThis));
							//CSceneGraph::GetInstance()->DeleteNode((*colliderThat));
						}

					}
					else if ((*colliderThat)->GetIsZombieBody())
					{
						if (CheckAABBCollision(thisEntity, thatEntity))
						{
							boss->setHealth(boss->GetHealth() - 250);
							DamageBoss();
							thisEntity->SetIsDone(true);
							//thatEntity->SetIsDone(true);
							CSceneGraph::GetInstance()->DeleteNode((*colliderThis));
							//CSceneGraph::GetInstance()->DeleteNode((*colliderThat));
						}
					}
					else if ((*colliderThat)->GetIsZombieHand())
					{
						if (CheckAABBCollision(thisEntity, thatEntity))
						{
							boss->setHealth(boss->GetHealth() - 100);
							DamageBoss();
							thisEntity->SetIsDone(true);
							//thatEntity->SetIsDone(true);
							CSceneGraph::GetInstance()->DeleteNode((*colliderThis));
							//CSceneGraph::GetInstance()->DeleteNode((*colliderThat));
						}
					}
					else if ((*colliderThat)->GetIsZombieLeg())
					{
						if (CheckAABBCollision(thisEntity, thatEntity))
						{						
							boss->setHealth(boss->GetHealth() - 100);
							DamageBoss();
							thisEntity->SetIsDone(true);
							//thatEntity->SetIsDone(true);
							CSceneGraph::GetInstance()->DeleteNode((*colliderThis));
							//CSceneGraph::GetInstance()->DeleteNode((*colliderThat));

						}
					}

					else if((*colliderThat)->HasCollider())
					{
						if (CheckAABBCollision(thisEntity, thatEntity))
						{
							(*colliderThis)->SetIsDone(true);
							(*colliderThat)->SetIsDone(true);

							// Remove from Scene Graph
							if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)))
							{
								cout << "*** This Entity removed ***" << endl;
							}
							// Remove from Scene Graph
							if (CSceneGraph::GetInstance()->DeleteNode((*colliderThat)))
							{
								cout << "*** That Entity removed ***" << endl;
							}
						}
					}
				}
				else if (boss->GetHealth() <= 0)
				{
					if ((*colliderThat)->GetIsZombieHead())
					{
						thatEntity->SetIsDone(true);
						CSceneGraph::GetInstance()->DeleteNode((*colliderThat));
					}
					if ((*colliderThat)->GetIsZombieBody())
					{
						thatEntity->SetIsDone(true);
						CSceneGraph::GetInstance()->DeleteNode((*colliderThat));
					}
					if ((*colliderThat)->GetIsZombieHand())
					{
						thatEntity->SetIsDone(true);
						CSceneGraph::GetInstance()->DeleteNode((*colliderThat));
					}
					if ((*colliderThat)->GetIsZombieLeg())
					{
						thatEntity->SetIsDone(true);
						CSceneGraph::GetInstance()->DeleteNode((*colliderThat));
					}
				}
				
			}
		}
		else if ((*colliderThis)->GetIsCar())
		{
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);
			colliderThatEnd = entityList.end();
			int counter = 0;

			if ((thisEntity->GetPosition() - playerInfo->GetPos()).LengthSquared() <= 5.f)
			{
				thisEntity->SetIsDone(true);
				playerInfo->SetHP(playerInfo->GetHP() - 25);
				std::cout << "HP : " << playerInfo->GetHP() << std::endl;
				Blast();
			}
		}
	}
	for (std::vector<SpaceShip*>::iterator it = spaceVec.begin(); it != spaceVec.end(); it++)
	{
		if (((*it)->position - playerInfo->GetPos()).LengthSquared() <= 5.f)
		{
			if (!(*it)->isDead)
			{
				(*it)->isDead = true;
				std::cout << "DERENDERED" << std::endl;
				Blast();
				playerInfo->SetHP(playerInfo->GetHP() - 5);
			}
		}
	}
	return false;
}

void EntityManager::SetCamera(FPSCamera* assigningCamera)
{
	cameraInfo = assigningCamera;
}

bool EntityManager::CheckAABBCollision(EntityBase* ThisEntity, CPlayerInfo* playerInfo)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = playerInfo->GetPos() + playerInfo->GetMinAABB();
	Vector3 thatMaxAABB = playerInfo->GetPos() + playerInfo->GetMaxAABB();

	// Check for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	// if AABB collision check fails, then we need to check the other corners of the bounding boxes to 
	// Do more collision checks with other points on each bounding box
	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);
	//	Vector3 altThatMinAABB = Vector3(thatMinAABB.x, thatMinAABB.y, thatMaxAABB.z);
	//	Vector3 altThatMaxAABB = Vector3(thatMaxAABB.x, thatMaxAABB.y, thatMinAABB.z);

	// Check for overlap
	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}
// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// Check for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	// if AABB collision check fails, then we need to check the other corners of the bounding boxes to 
	// Do more collision checks with other points on each bounding box
	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);
	//	Vector3 altThatMinAABB = Vector3(thatMinAABB.x, thatMinAABB.y, thatMaxAABB.z);
	//	Vector3 altThatMaxAABB = Vector3(thatMaxAABB.x, thatMaxAABB.y, thatMinAABB.z);

	// Check for overlap
	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}
