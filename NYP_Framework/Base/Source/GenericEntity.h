#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H

#include "EntityBase.h"
#include "Collider/Collider.h"
#include "LevelOfDetails\LevelOfDetails.h"

#include <string>
#include <fstream>
#include <iostream>

using std::ifstream;
using std::fstream;
using std::string;

class Mesh;

class GenericEntity : public EntityBase, public CCollider, public CLevelOfDetails
{
public:
	GenericEntity(Mesh* _modelMesh);
	virtual ~GenericEntity();

	virtual void Update(double _dt);
	virtual void Render();

	// Set the maxAABB and minAABB
	void SetAABB(Vector3 maxAABB, Vector3 minAABB);

	bool ReadFile(const std::string& NameofObject, std::string FileDetails);

private:
	Mesh* modelMesh;
};

namespace Create
{
<<<<<<< HEAD
	GenericEntity* Entity(	const std::string& _meshName);
	GenericEntity* Asset(	const std::string& _meshName/*,
							const Vector3& _position,
							const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)*/);
=======
	GenericEntity* Entity(	const std::string& _meshName, 
							const Vector3& _position, 
							const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
							const float _rotation = 0.f);
	GenericEntity* Asset(	const std::string& _meshName,
							const Vector3& _position,
							const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
							const float _rotation = 0.f);
>>>>>>> 21443b8092479efd4c568634283de6d5543e27ab
};

#endif // GENERIC_ENTITY_H