#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "Vector3.h"
#include "SpatialPartition\SpatialPartition.h"

class EntityBase;
class CPlayerInfo;
class SpaceShip;
class Boss;

class EntityManager : public Singleton<EntityManager>
{
	friend Singleton<EntityManager>;
public:
	void Update(double _dt, CPlayerInfo* playerInfo, std::vector<SpaceShip*> cool, Boss* boss);
	void Render();
	void RenderUI();

	void AddEntity(EntityBase* _newEntity, bool bAddToSpatialPartition=false);
	bool RemoveEntity(EntityBase* _existingEntity);
	bool MarkForDeletion(EntityBase* _existingEntity);

	void SetSpatialPartition(CSpatialPartition* theSpatialPartition);
	void SetCamera(FPSCamera* assigningCamera);

	float Health;

private:
	EntityManager();
	virtual ~EntityManager();


	// Check for overlap
	bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	bool CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check if this entity collided with another entity, but both must have collider
	bool CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);

	bool CheckAABBCollision(EntityBase *ThisEntity, CPlayerInfo* playerInfo);

	// Check for intersection between a line segment and a plane
	bool GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit);
	// Check for intersection between a line segment and a plane
	bool CheckLineSegmentPlane(	Vector3 line_start, Vector3 line_end,
								Vector3 minAABB, Vector3 maxAABB,
								Vector3 &Hit);
	// Check two positions are within a box region
	bool InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);
	// Check if any Collider is colliding with another Collider
	bool CheckForCollision(CPlayerInfo* playerInfo, std::vector<SpaceShip*> spaceVec, Boss* boss);

	std::list<EntityBase*> entityList;
	FPSCamera* cameraInfo;
	
	// Handler to Spatial Partition
	CSpatialPartition* theSpatialPartition;
};

#endif // ENTITY_MANAGER_H