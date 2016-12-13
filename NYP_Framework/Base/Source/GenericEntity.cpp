#include "GenericEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

GenericEntity::GenericEntity(Mesh* _modelMesh)
	: modelMesh(_modelMesh)
{
}

GenericEntity::~GenericEntity()
{
}

void GenericEntity::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
}

void GenericEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	modelStack.Rotate(rotation, 0, 1, 0);
	if (GetLODStatus()==true)
	{
		if (theDetailLevel != NO_DETAILS)
			RenderHelper::RenderMesh(GetLODMesh());
	}
	else
		RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

// Set the maxAABB and minAABB
void GenericEntity::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}

bool GenericEntity::ReadFile(const string& NameOfObject, const string Details)
{
	string temp = "";

	ifstream file(Details.c_str(), std::ios::binary);

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, temp, ',');
			if (temp != NameOfObject)
			{
				getline(file, temp);
				continue;
			}
			else if (temp == NameOfObject)
			{
				temp.clear();
				getline(file, temp, ',');
				SetScale(stoi(temp));
				temp.clear();

				getline(file, temp, ',');
				SetPositionX(stoi(temp));
				temp.clear();

				getline(file, temp, ',');
				SetPositionY(stoi(temp));
				temp.clear();

				getline(file, temp, ',');
				SetPositionZ(stoi(temp));
				temp.clear();

				getline(file, temp, ',');
				SetRotation(stoi(temp));
				temp.clear();
				return true;
			}
		}
		temp.clear();
	}
	return true;
}

GenericEntity* Create::Entity(const string& _meshName)
{
	
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->ReadFile(_meshName, "Source//CSV//File.csv");
	result->SetCollider(false);
	EntityManager::GetInstance()->AddEntity(result, true);
	return result;
}

GenericEntity* Create::Asset(	const std::string& _meshName/*,
								const Vector3& _position,
								const Vector3& _scale*/,
								const float _rotation)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	/*result->SetPositionX(_position.x);
	result->SetPositionY(_position.y);
	result->SetPositionZ(_position.z);
	result->SetScale(_scale);*/
	result->SetRotation(_rotation);
	result->SetCollider(false);
	return result;
}




