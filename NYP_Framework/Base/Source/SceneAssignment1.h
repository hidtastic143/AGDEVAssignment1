#ifndef SCENE_ASSIGNMENT_1_H
#define SCENE_ASSIGNMENT_1_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "SkyBox\SkyBoxEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "SceneGraph/UpdateTransformation.h"
#include "Enemy\Enemy.h"
#include "Enemy\Spaceship.h"
#include "Enemy\Boss.h"
class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class SceneAssignment1 : public Scene
{
public:
	SceneAssignment1();
	~SceneAssignment1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void RenderMeshIn2D(Mesh *mesh, bool enableLight, float sizeX, float sizeY, float sizeZ, float x, float y);

	bool Mainmenu;
	bool Credit;

private:
	SceneAssignment1(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	SkyBoxEntity* theSkyBox;
	FPSCamera camera;
	TextEntity* textObj[6];
	Light* lights[2];

	GenericEntity* theCube;
	GenericEntity* Rifle;
	Boss* boss;
	SpaceShip* spaceShip;

	bool win;
	bool lose;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	std::vector<SpaceShip*> spaceVec;

	static SceneAssignment1* sInstance; // The pointer to the object that gets registered
};

#endif