#include "SceneAssignment1.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "SceneGraph\SceneGraph.h"
#include "Enemy\Spaceship.h"
#include "SpatialPartition\SpatialPartition.h"
#include "RenderHelper.h"

#include <iostream>
using namespace std;

SceneAssignment1* SceneAssignment1::sInstance = new SceneAssignment1(SceneManager::GetInstance());

SceneAssignment1::SceneAssignment1()
{
}

SceneAssignment1::SceneAssignment1(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("SceneAssignment1", this);
}

SceneAssignment1::~SceneAssignment1()
{
	CSpatialPartition::GetInstance()->RemoveCamera();
	CSceneGraph::GetInstance()->Destroy();
}

void SceneAssignment1::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");

	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");

	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);

	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	EntityManager::GetInstance()->SetCamera(&camera);
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.5f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);

	MeshBuilder::GetInstance()->GenerateOBJ("Zhead", "OBJ//cube.obj");
	MeshBuilder::GetInstance()->GetMesh("Zhead")->textureID = LoadTGA("Image//head.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Zbody", "OBJ//cube.obj");
	MeshBuilder::GetInstance()->GetMesh("Zbody")->textureID = LoadTGA("Image//body.tga");
	MeshBuilder::GetInstance()->GenerateCube("Zlhand", Color(0, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Zlhand")->textureID = LoadTGA("Image//arm.tga");
	MeshBuilder::GetInstance()->GenerateCube("Zrhand", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Zrhand")->textureID = LoadTGA("Image//arm.tga");
	MeshBuilder::GetInstance()->GenerateCube("Zlleg", Color(1, 1, 0), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Zlleg")->textureID = LoadTGA("Image//Legs.tga");
	MeshBuilder::GetInstance()->GenerateCube("Zrleg", Color(1, 0, 0), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Zrleg")->textureID = LoadTGA("Image//Legs.tga");

	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("cube2", Color(0.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("cube3", Color(1.0f, 0.0f, 0.0f), 1.0f);

	MeshBuilder::GetInstance()->GenerateOBJ("car", "OBJ//Car.obj");
	MeshBuilder::GetInstance()->GetMesh("car")->textureID = LoadTGA("Image//carTexture.tga");

	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//grass_darkgreen.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//grass_lightgreen.tga");

	//Gun
	MeshBuilder::GetInstance()->GenerateOBJ("M4A4", "OBJ//M4A4.obj");
	MeshBuilder::GetInstance()->GetMesh("M4A4")->textureID = LoadTGA("Image//M4A4_Texture.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("SecondWeapon", "OBJ//Pistol.obj");
	MeshBuilder::GetInstance()->GetMesh("SecondWeapon")->textureID = LoadTGA("Image//Pistol_Texture.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Barricade1", "OBJ//Barricade.obj");
	MeshBuilder::GetInstance()->GetMesh("Barricade1")->textureID = LoadTGA("Image//Barricade_Texture.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Barricade2", "OBJ//Barricade.obj");
	MeshBuilder::GetInstance()->GetMesh("Barricade2")->textureID = LoadTGA("Image//Barricade_Texture.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Barricade3", "OBJ//Barricade.obj");
	MeshBuilder::GetInstance()->GetMesh("Barricade3")->textureID = LoadTGA("Image//Barricade_Texture.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Barricade4", "OBJ//Barricade.obj");
	MeshBuilder::GetInstance()->GetMesh("Barricade4")->textureID = LoadTGA("Image//Barricade_Texture.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Barricade5", "OBJ//Barricade.obj");
	MeshBuilder::GetInstance()->GetMesh("Barricade5")->textureID = LoadTGA("Image//Barricade_Texture.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Barricade6", "OBJ//Barricade.obj");
	MeshBuilder::GetInstance()->GetMesh("Barricade6")->textureID = LoadTGA("Image//Barricade_Texture.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Barricade7", "OBJ//Barricade.obj");
	MeshBuilder::GetInstance()->GetMesh("Barricade7")->textureID = LoadTGA("Image//Barricade_Texture.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Spaceship", "OBJ//SpaceShip.obj");
	MeshBuilder::GetInstance()->GetMesh("Spaceship")->textureID = LoadTGA("Image//SpaceShip_Texture.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Spaceship2", Color(1.0f, 1.0f, 0.0f), 10.0f);
	MeshBuilder::GetInstance()->GetMesh("Spaceship2")->textureID = LoadTGA("Image//SpaceShip2D.tga");

	//Bullet
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);

	//LoseScreen
	MeshBuilder::GetInstance()->GenerateQuad("Lose", Color(0.f, 0.f, 0.f), 10.f);
	MeshBuilder::GetInstance()->GetMesh("Lose")->textureID = LoadTGA("Image//LoseScreen.tga");

	//Skybox
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");

	MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 1, 1), 10.f);

	// Set up the Spatial Partition and pass it to the EntityManager to manage
	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(4000.0f, 16000.0f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	// Create entities into the scen

	//Rifle = Create::Entity("M4A4");
	//Rifle->SetPosition(Vector3(playerInfo->GetPos().x + 1.f, playerInfo->GetPos().y - 2.f, playerInfo->GetPos().z - 5));

	GenericEntity* Barricade = Create::Entity("Barricade1");
	Barricade->SetAABB(Barricade->GetScale(), -Barricade->GetScale());
	GenericEntity* Barricade2 = Create::Entity("Barricade2");
	Barricade->SetAABB(Barricade2->GetScale(), -Barricade2->GetScale());
	GenericEntity* Barricade3 = Create::Entity("Barricade3");
	Barricade->SetAABB(Barricade3->GetScale(), -Barricade3->GetScale());
	GenericEntity* Barricade4 = Create::Entity("Barricade4");
	Barricade->SetAABB(Barricade4->GetScale(), -Barricade4->GetScale());
	GenericEntity* Barricade5 = Create::Entity("Barricade5");
	Barricade->SetAABB(Barricade5->GetScale(), -Barricade5->GetScale());
	GenericEntity* Barricade6 = Create::Entity("Barricade6");
	Barricade->SetAABB(Barricade6->GetScale(), -Barricade6->GetScale());
	GenericEntity* Barricade7 = Create::Entity("Barricade7");
	Barricade->SetAABB(Barricade7->GetScale(), -Barricade7->GetScale());

	background = Create::Asset("Lose");

	playerInfo->getPrimaryWeapon()->SetMesh(MeshBuilder::GetInstance()->GetMesh("M4A4"));
	playerInfo->getSecondaryWeapon()->SetMesh(MeshBuilder::GetInstance()->GetMesh("SecondWeapon"));

	enemy = new Enemy();
	enemy->Init();

	for (int i = 0; i < 20; i++)
	{
		spaceShip = new SpaceShip();
		spaceShip->Init(playerInfo);
		spaceVec.push_back(spaceShip);
	}
	//enemy->InitLOD("Spaceship", "Spaceship", "Spaceship2");

	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
	//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
		"SKYBOX_LEFT", "SKYBOX_RIGHT",
		"SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	playerInfo->SetTerrain(groundEntity);
	enemy->setTerrain(groundEntity);

	playerInfo->SetBoundary(Vector3(24.f, 40.f, 500.f), Vector3(-24.f, 40.f, 450.f));

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 4; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}
	textObj[0]->SetText("HELLO WORLD");
}

void SceneAssignment1::Update(double dt)
{
	// Update our entities
	static float stopwatchSpawner = 0.f;
	stopwatchSpawner += 2 * dt;

	for (std::vector<SpaceShip*>::iterator it = spaceVec.begin(); it != spaceVec.end(); it++)
	{
		if (stopwatchSpawner > 2.6f)
		{
			if (!(*it)->GetReady())
			{
				(*it)->SetReady(true);
				(*it)->velocity = (playerInfo->GetPos() - (*it)->getPos()).Normalized() * (*it)->m_dSpeed;
				stopwatchSpawner = false;
			}
		}

		if ((*it)->GetReady())
		{
			(*it)->Update(dt);
		}
	}
	EntityManager::GetInstance()->Update(dt, playerInfo, spaceVec);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if (KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if (KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{
		CSceneNode* theNode = CSceneGraph::GetInstance()->GetNode(1);
		Vector3 pos = theNode->GetEntity()->GetPosition();
		theNode->GetEntity()->SetPosition(Vector3(pos.x + 50.0f, pos.y, pos.z + 50.0f));
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('N'))
	{
		CSpatialPartition::GetInstance()->PrintSelf();
	}

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
	// <THERE>

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);
	enemy->Update(dt);
	enemy->AttackPlayer(enemy->Zrhand->GetPosition(), playerInfo->GetPos());
	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	textObj[1]->SetText(ss.str());

	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Player:" << playerInfo->GetPos();
	textObj[2]->SetText(ss1.str());

	std::ostringstream ss2;
	ss2.precision(13);
	ss2 << "Boss HP:" << EntityManager::GetInstance()->Health;
	textObj[3]->SetText(ss2.str());
}

void SceneAssignment1::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	

	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;

	int WindowWidth = Application::GetInstance().GetWindowWidth();
	int WindowHeight = Application::GetInstance().GetWindowHeight();

	//spaceShip->Render();

	for (std::vector<SpaceShip*>::iterator it = spaceVec.begin(); it != spaceVec.end(); it++)
	{
		if ((*it)->GetReady() && !(*it)->isDead)
		{
			(*it)->Render();
		}
	}

		EntityManager::GetInstance()->Render();

		
		theSkyBox->Render();
		groundEntity->Render();

		if (playerInfo->getWeaponHeld() == playerInfo->getPrimaryWeapon())
			playerInfo->getPrimaryWeapon()->Render(playerInfo);
		else if (playerInfo->getWeaponHeld() == playerInfo->getSecondaryWeapon())
			playerInfo->getSecondaryWeapon()->Render(playerInfo);
	if (EntityManager::GetInstance()->Health > 0)
	{

		// Setup 2D pipeline then render 2D
		
		GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
		GraphicsManager::GetInstance()->DetachCamera();
		EntityManager::GetInstance()->RenderUI();
	}
	if (playerInfo->GetHP() <= 0 )
	{
		background->Render();
	}
		
}

void SceneAssignment1::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];
}
