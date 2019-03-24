#include "TutorialGame.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"

#include "../CSC8503Common/PositionConstraint.h"

#include "InteractableWallLevel1.h"
#include "InteractableWallLevel1.h"
#include "../CSC8503Common/NavigationGrid.h"

using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame()	{
	world		= new GameWorld();
	renderer	= new GameTechRenderer(*world);
	physics		= new PhysicsSystem(*world);

	//forceMagnitude	= 10.0f;
	forceMagnitude = 500.0f;

	//useGravity		= false;
	useGravity = true;
	physics->UseGravity(useGravity);

	inSelectionMode = false;

	mainMenuText = true;
	controlsAndOptions = true;

	Debug::SetRenderer(renderer);

	InitialiseAssets();
}

void TutorialGame::InitialiseAssets() {
	cubeMesh = new OGLMesh("cube.msh");
	cubeMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	cubeMesh->UploadToGPU();

	sphereMesh = new OGLMesh("sphere.msh");
	sphereMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	sphereMesh->UploadToGPU();

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("Goal.JPG");
	bridgeTex = (OGLTexture*)TextureLoader::LoadAPITexture("Bridge.png");
	movingWallTex = (OGLTexture*)TextureLoader::LoadAPITexture("Brick.png");
	basicTex1 = (OGLTexture*)TextureLoader::LoadAPITexture("Ground.jpg");
	basicTex2 = (OGLTexture*)TextureLoader::LoadAPITexture("Walls.jpg");
	basicTex3 = (OGLTexture*)TextureLoader::LoadAPITexture("GolfBall.jpg");
	loadLevel1Tex = (OGLTexture*)TextureLoader::LoadAPITexture("Level1.png");
	loadLevel2Tex = (OGLTexture*)TextureLoader::LoadAPITexture("Level2.png");
	monsterTex = (OGLTexture*)TextureLoader::LoadAPITexture("Monster.png");

	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");
	InitCamera();
	MainMenu();
}

TutorialGame::~TutorialGame()	{
	delete cubeMesh;
	delete sphereMesh;
	delete basicTex;
	delete basicShader;

	delete physics;
	delete renderer;
	delete world;
}

void TutorialGame::UpdateGame(float dt) {
	if (!inSelectionMode) {
		world->GetMainCamera()->UpdateCamera(dt);
	}

	UpdateKeys();

	/*if (useGravity) {
		Debug::Print("(G)ravity on", Vector2(10, 40));
	}
	else {
		Debug::Print("(G)ravity off", Vector2(10, 40));
	}*/

	if (mainMenuText)
	{
		renderer->DrawString("WELCOME TO THE", Vector2(600, 1100), Vector4(0, 0, 0, 1));
		renderer->DrawString("A-MAZE-ING GOLF!", Vector2(550, 1050), Vector4(0, 0, 0, 1));
		renderer->DrawString("Press 1 or 2 to load a level", Vector2(200, 1000), Vector4(0, 0, 0, 1));
		renderer->DrawString("Or hit one of the images with ball to load it", Vector2(20, 950), Vector4(0, 0, 0, 1));
		renderer->DrawString("Press C for more Controls and Options", Vector2(0, 60), Vector4(0, 0, 0, 1));
	}
	else
	{

	}

	if (interactableWallLevel1)
		interactableWallLevel1->updateState();

	if (interactableWallLevel2)
		interactableWallLevel2->updateState();

	if (controlsAndOptions && mainMenuText)
	{
		renderer->DrawString("___________________________________________", Vector2(0, 900), Vector4(0, 0, 0, 1));
		renderer->DrawString("Press M to load the main menu", Vector2(0, 850), Vector4(0, 0, 0, 1));
		renderer->DrawString("Press 1 or R + 1 to reload the First Level", Vector2(0, 800), Vector4(0, 0, 0, 1));
		renderer->DrawString("Press 2 or R + 2 to reload the Second Level", Vector2(0, 750), Vector4(0, 0, 0, 1));
		renderer->DrawString("Click the ball using left click", Vector2(0, 700), Vector4(0, 0, 0, 1));
		renderer->DrawString("and push it around with the right click", Vector2(0, 650), Vector4(0, 0, 0, 1));
		renderer->DrawString("Or use W A S D to move the ball around", Vector2(0, 600), Vector4(0, 0, 0, 1));
		renderer->DrawString("Click SPACE to jump", Vector2(0, 550), Vector4(0, 0, 0, 1));
		renderer->DrawString("If ball selected press B to snap the camera to its location", Vector2(0, 500), Vector4(0, 0, 0, 1));
		renderer->DrawString("If not selected the camera will move to where the ball start)", Vector2(0, 450), Vector4(0, 0, 0, 1));
		renderer->DrawString("Press Q to change to camera mode", Vector2(0, 400), Vector4(0, 0, 0, 1));
		renderer->DrawString("Use to the mouse wheel to increase/decrease the force", Vector2(0, 350), Vector4(0, 0, 0, 1));
		renderer->DrawString("(Goes negative as well, for cool backwards tricks)", Vector2(0, 300), Vector4(0, 0, 0, 1));
		renderer->DrawString("Press C to close the Controls and Options", Vector2(0, 250), Vector4(0, 0, 0, 1));
		renderer->DrawString("___________________________________________", Vector2(0, 225), Vector4(0, 0, 0, 1));
	}
	else
	{

	}

	if (ball)
	{
		if (ball->returnGoal())
		{
			renderer->DrawString("WINNER WINNER", Vector2(620, 1100), Vector4(0, 0, 0, 1));
			renderer->DrawString("CHICKEN DINNER", Vector2(600, 1050), Vector4(0, 0, 0, 1));
			renderer->DrawString("CONGRATULATIONS FOR COMPLETING THE MAZE", Vector2(65, 1000), Vector4(0, 0, 0, 1));
			renderer->DrawString("Press 1 or R + 1 to load/reload Level 1", Vector2(65, 950), Vector4(0, 0, 0, 1));
			renderer->DrawString("Press 2 or R + 2 to load/reload Level 2", Vector2(65, 900), Vector4(0, 0, 0, 1));
			renderer->DrawString("Press M to go back to the MAIN MENU/AREA", Vector2(50, 850), Vector4(0, 0, 0, 1));
			selectionObject = nullptr;
		}
	}

	SelectObject();
	MoveSelectedObject();

	world->UpdateWorld(dt);
	renderer->Update(dt);
	physics->Update(dt);

	Debug::FlushRenderables();
	renderer->Render();

	if (ball)
	{
		if (ball->returnDeath1())
		{
			InitWorld1();
		}
		if (ball->returnDeath2())
		{
			InitWorld2();
		}

		if (ball->returnLevel1())
		{
			InitWorld1();
		}

		if (ball->returnLevel2())
		{
			InitWorld2();
		}

		if (ball->returnMainMenu())
		{
			MainMenu();
		}
	}

	if (mainMenuRobot)
	{
		TestPathfinding();
		DisplayPathfinding();
		MonsterFollow();
	}
	else
	{

	}
}

void TutorialGame::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_M))
	{
		MainMenu();
		selectionObject = nullptr;
		interactableWallLevel1 = nullptr;
		interactableWallLevel2 = nullptr;
		mainMenuText = true;
	}

	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_C))
	{
		selectionObject = nullptr;
		controlsAndOptions = !controlsAndOptions;
	}

	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_1) || (Window::GetKeyboard()->KeyPressed(KEYBOARD_R) && (Window::GetKeyboard()->KeyPressed(KEYBOARD_1))))
	{
		InitWorld1();
		selectionObject = nullptr;
		interactableWallLevel2 = nullptr;
		mainMenuText = false;
	}

	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_2) || (Window::GetKeyboard()->KeyPressed(KEYBOARD_R) && (Window::GetKeyboard()->KeyPressed(KEYBOARD_2))))
	{
		InitWorld2();
		selectionObject = nullptr;
		interactableWallLevel1 = nullptr;
		mainMenuText = false;
	}

	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_B)) {
		InitCamera();
	}

	//if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::KEYBOARD_G)) {
	//	useGravity = !useGravity; //Toggle gravity!
	//	physics->UseGravity(useGravity);
	//}
	/*if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F8)) {
		world->ShuffleObjects(false);
	}*/
	
	if (inSelectionMode && selectionObject) {
		
		/*if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-100, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(100, 0, 0));
		}*/

		/*if (Window::GetKeyboard()->KeyDown(KEYBOARD_7)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 100, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_8)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -100, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(100, 0, 0));
		}*/

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -100));
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(-100, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 100));
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(100, 0, 0));
		}

		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_SPACE)) {
			selectionObject->GetPhysicsObject()->ApplyLinearImpulse(Vector3(0, 50, 0));
		}
	}
}

void TutorialGame::InitCamera() {
	world->GetMainCamera()->SetNearPlane(3.0f);
	world->GetMainCamera()->SetFarPlane(4200.0f);
	world->GetMainCamera()->SetPitch(-35.0f);
	world->GetMainCamera()->SetYaw(320.0f);
	world->GetMainCamera()->SetPosition(Vector3(-50, 120, 200));

	if (selectionObject)
	{
		world->GetMainCamera()->SetPitch(-90.0f);
		world->GetMainCamera()->SetPosition(selectionObject->GetTransform().GetLocalPosition() + Vector3(0, 500, 0));
	}
	else
	{
		world->GetMainCamera()->SetPosition(Vector3(-500, 500, 500));
	}
}

void TutorialGame::MainMenu()
{
	controlsAndOptions = false;
	selectionObject = nullptr;
	world->ClearAndErase();
	physics->Clear();

	interactableWallLevel1 = nullptr;
	interactableWallLevel2 = nullptr;
	mainMenuText = true;

	mainMenuRobot = true;

	InitSphereGridWorld1(1, 1, 50.0f, 50.0f, 10.0f);
}

void TutorialGame::InitWorld1()
{
	interactableWallLevel2 = nullptr;
	mainMenuText = false;
	mainMenuRobot = false;
	selectionObject = nullptr;
	world->ClearAndErase();
	physics->Clear();

	InitSphereGridWorld2(1, 1, 50.0f, 50.0f, 10.0f);
	interactableWallLevel1 = (InteractableWallLevel1*)AddInteractableWallLevel1ToWorld(Vector3(-540, -40, 735), Vector3(10, 50, 400), 10.0f, 400.0f, Vector3(0, 0, 1), "Interactable Wall Level1");
}

void TutorialGame::InitWorld2()
{
	interactableWallLevel1 = nullptr;
	mainMenuText = false;
	mainMenuRobot = false;
	selectionObject = nullptr;
	world->ClearAndErase();
	physics->Clear();
	InitSphereGridWorld3(1, 1, 50.0f, 50.0f, 10.0f);

	BridgeConstraintTest();
	BridgeConstraintTest1();
	interactableWallLevel2 = (InteractableWallLevel2*)AddInteractableWallLevel2ToWorld(Vector3(1030, 50, -550), Vector3(150, 40, 10), 200.0f, 400.0f, Vector3(1, 0, 0), "Interactable Wall Level2");
}

GameObject* TutorialGame::AddFloorToWorld(const Vector3& position) {
	GameObject* floor = new GameObject();

	Vector3 floorSize = Vector3(1510, 10, 1510);

	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform().SetWorldScale(floorSize);
	floor->GetTransform().SetWorldPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex1, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(floor);

	return floor;
}

GameObject* TutorialGame::AddWallToWorld(const Vector3& position, const Vector3& wallSize) {
	GameObject* wall = new GameObject();

	Vector3 WallSize = Vector3(1000, 10, 1000);
	AABBVolume* volume = new AABBVolume(wallSize);
	wall->SetBoundingVolume((CollisionVolume*)volume);
	wall->GetTransform().SetWorldScale(wallSize);
	wall->GetTransform().SetWorldPosition(position);

	wall->SetRenderObject(new RenderObject(&wall->GetTransform(), cubeMesh, basicTex2, basicShader));
	wall->SetPhysicsObject(new PhysicsObject(&wall->GetTransform(), wall->GetBoundingVolume()));

	wall->GetPhysicsObject()->SetInverseMass(0);
	wall->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(wall);

	return wall;
}

GameObject* TutorialGame::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject();

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);
	sphere->GetTransform().SetWorldScale(sphereSize);
	sphere->GetTransform().SetWorldPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex3, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

GameObject * TutorialGame::AddGolfBallToWorld(const Vector3 & position, float radius, float inverseMass)
{
	GolfBall* ball = new GolfBall();

	this->ball = ball;

	Vector3 ballSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	ball->SetBoundingVolume((CollisionVolume*)volume);
	ball->GetTransform().SetWorldScale(ballSize);
	ball->GetTransform().SetWorldPosition(position);
	ball->SetName("ball");

	ball->SetRenderObject(new RenderObject(&ball->GetTransform(), sphereMesh, basicTex3, basicShader));
	ball->SetPhysicsObject(new PhysicsObject(&ball->GetTransform(), ball->GetBoundingVolume()));

	ball->GetPhysicsObject()->SetInverseMass(inverseMass);
	ball->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(ball);

	return ball;
}

GameObject* TutorialGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, string name) {
	GameObject* cube = new GameObject();
	cube->SetName(name);

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetWorldPosition(position);
	cube->GetTransform().SetWorldScale(dimensions);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddCubeToLoadLevel1ToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, string name) {
	GameObject* cubeToLoadLevel1 = new GameObject();
	cubeToLoadLevel1->SetName(name);

	AABBVolume* volume = new AABBVolume(dimensions);

	cubeToLoadLevel1->SetBoundingVolume((CollisionVolume*)volume);

	cubeToLoadLevel1->GetTransform().SetWorldPosition(position);
	cubeToLoadLevel1->GetTransform().SetWorldScale(dimensions);

	cubeToLoadLevel1->SetName("Load Level1");

	cubeToLoadLevel1->SetRenderObject(new RenderObject(&cubeToLoadLevel1->GetTransform(), cubeMesh, loadLevel1Tex, basicShader));
	cubeToLoadLevel1->SetPhysicsObject(new PhysicsObject(&cubeToLoadLevel1->GetTransform(), cubeToLoadLevel1->GetBoundingVolume()));

	cubeToLoadLevel1->GetPhysicsObject()->SetInverseMass(inverseMass);
	cubeToLoadLevel1->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cubeToLoadLevel1);

	return cubeToLoadLevel1;
}

GameObject* TutorialGame::AddCubeToLoadLevel2ToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, string name) {
	GameObject* cubeToLoadLevel2 = new GameObject();
	cubeToLoadLevel2->SetName(name);

	AABBVolume* volume = new AABBVolume(dimensions);

	cubeToLoadLevel2->SetBoundingVolume((CollisionVolume*)volume);

	cubeToLoadLevel2->GetTransform().SetWorldPosition(position);
	cubeToLoadLevel2->GetTransform().SetWorldScale(dimensions);

	cubeToLoadLevel2->SetName("Load Level2");

	cubeToLoadLevel2->SetRenderObject(new RenderObject(&cubeToLoadLevel2->GetTransform(), cubeMesh, loadLevel2Tex, basicShader));
	cubeToLoadLevel2->SetPhysicsObject(new PhysicsObject(&cubeToLoadLevel2->GetTransform(), cubeToLoadLevel2->GetBoundingVolume()));

	cubeToLoadLevel2->GetPhysicsObject()->SetInverseMass(inverseMass);
	cubeToLoadLevel2->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cubeToLoadLevel2);

	return cubeToLoadLevel2;
}

GameObject* TutorialGame::AddBridgeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, string name) {
	GameObject* cube = new GameObject();
	cube->SetName(name);

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform().SetWorldPosition(position);
	cube->GetTransform().SetWorldScale(dimensions);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, bridgeTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddInteractableWallLevel1ToWorld(const Vector3& position, Vector3 dimensions, float maxDistance, float speed, Vector3 movementAxis, string name)
{
	InteractableWallLevel1* interactableWallLevel1 = new InteractableWallLevel1(name);

	//AABBVolume* volume = new AABBVolume(dimensions);
	OBBVolume* volume = new OBBVolume(dimensions);

	interactableWallLevel1->SetBoundingVolume((CollisionVolume*)volume);

	interactableWallLevel1->GetTransform().SetWorldPosition(position);
	interactableWallLevel1->GetTransform().SetWorldScale(dimensions);

	interactableWallLevel1->SetName("Interactable Wall Level1");

	interactableWallLevel1->SetRenderObject(new RenderObject(&interactableWallLevel1->GetTransform(), cubeMesh, movingWallTex, basicShader));
	interactableWallLevel1->SetPhysicsObject(new PhysicsObject(&interactableWallLevel1->GetTransform(), interactableWallLevel1->GetBoundingVolume()));

	interactableWallLevel1->GetPhysicsObject()->SetInverseMass(0.0f);
	interactableWallLevel1->GetPhysicsObject()->InitCubeInertia();

	interactableWallLevel1->setSpeed(speed);
	interactableWallLevel1->setMovementOnAxis(movementAxis);
	interactableWallLevel1->setMaxDistance(maxDistance);
	interactableWallLevel1->initState();

	world->AddGameObject(interactableWallLevel1);

	return interactableWallLevel1;
}

GameObject* TutorialGame::AddInteractableWallLevel2ToWorld(const Vector3& position, Vector3 dimensions, float maxDistance, float speed, Vector3 movementAxis, string name)
{
	InteractableWallLevel2* interactableWallLevel2 = new InteractableWallLevel2(name);

	//AABBVolume* volume = new AABBVolume(dimensions);
	OBBVolume* volume = new OBBVolume(dimensions);

	interactableWallLevel2->SetBoundingVolume((CollisionVolume*)volume);

	interactableWallLevel2->GetTransform().SetWorldPosition(position);
	interactableWallLevel2->GetTransform().SetWorldScale(dimensions);

	interactableWallLevel2->SetName("Interactable Wall Level2");

	interactableWallLevel2->SetRenderObject(new RenderObject(&interactableWallLevel2->GetTransform(), cubeMesh, movingWallTex, basicShader));
	interactableWallLevel2->SetPhysicsObject(new PhysicsObject(&interactableWallLevel2->GetTransform(), interactableWallLevel2->GetBoundingVolume()));

	interactableWallLevel2->GetPhysicsObject()->SetInverseMass(0.0f);
	interactableWallLevel2->GetPhysicsObject()->InitCubeInertia();

	interactableWallLevel2->setSpeed(speed);
	interactableWallLevel2->setMovementOnAxis(movementAxis);
	interactableWallLevel2->setMaxDistance(maxDistance);
	interactableWallLevel2->initState();

	world->AddGameObject(interactableWallLevel2);

	return interactableWallLevel2;
}

void TutorialGame::InitSphereGridWorld1(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, radius, z * rowSpacing);
			AddGolfBallToWorld(position, radius);
		}
	}
	AddFloorToWorld(Vector3(0, -100, 0));

	AddCubeToLoadLevel1ToWorld(Vector3(-300, 30, -500), Vector3(90, 120, 1), 0.f, "Load Level1");
	AddCubeToLoadLevel2ToWorld(Vector3(300, 30, -500), Vector3(90, 120, 1), 0.f, "Load Level2");

	monster = AddCubeMonsterToWorld(Vector3(1200, 30, -700), Vector3(30, 30, 30), 1.0f, "Monster");

	AddWallToWorld(Vector3(1500, -40, 0), Vector3(10, 50, 1510)); //Right Wall
	AddWallToWorld(Vector3(-1500, -40, 0), Vector3(10, 50, 1510)); //Left Wall
	AddWallToWorld(Vector3(0, -40, 1500), Vector3(1490, 50, 10)); //Back Wall
	AddWallToWorld(Vector3(0, -40, -1500), Vector3(1490, 50, 10)); //Front Wall
}

void TutorialGame::InitSphereGridWorld2(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, radius, z * rowSpacing);
			AddGolfBallToWorld(position, radius);
		}
	}
	AddFloorToWorld(Vector3(0, -100, 0));

	//LEVEL1
	//Outer walls
	AddWallToWorld(Vector3(1500, -40, 0),Vector3(10, 50, 1510)); //Right Wall
	AddWallToWorld(Vector3(-1500, -40, 0), Vector3(10, 50, 1510)); //Left Wall
	AddWallToWorld(Vector3(0, -40, 1500), Vector3(1490, 50, 10)); //Back Wall
	AddWallToWorld(Vector3(0, -40, -1500), Vector3(1490, 50, 10)); //Front Wall
	//Center
	AddWallToWorld(Vector3(200, -40, 0), Vector3(5, 50, 105)); //Right Center Wall
	AddWallToWorld(Vector3(-200, -40, 0), Vector3(5, 50, 105)); //Left Center Wall
	AddWallToWorld(Vector3(0, -40, 100), Vector3(195, 50, 5)); //Back Center Wall
	AddWallToWorld(Vector3(125, -40, -110), Vector3(80, 50, 5)); //Right Front Center Wall
	AddWallToWorld(Vector3(-125, -40, -110), Vector3(80, 50, 5)); //Left Front Center Wall
	//Game Area
	//AddWallToWorld(Vector3(0, -40, -200), Vector3(300, 50, 5)); //In front of the center
	AddWallToWorld(Vector3(305, -40, -200), Vector3(5, 50, 200)); //Right of the center
	AddWallToWorld(Vector3(-305, -40, -200), Vector3(5, 50, 200)); //Left of the center
	AddWallToWorld(Vector3(305, -40, 5), Vector3(100, 50, 5)); //Right right of the center
	AddWallToWorld(Vector3(-305, -40, 5), Vector3(100, 50, 5)); //Left left of the center
	AddWallToWorld(Vector3(180, -40, -405), Vector3(130, 50, 5)); //2d right wall in front of the center
	AddWallToWorld(Vector3(-180, -40, -405), Vector3(130, 50, 5)); //2nd left wall in front of the center
	AddWallToWorld(Vector3(55, -40, -665), Vector3(5, 50, 255)); //Right wall of the first aisle in front of the center
	AddWallToWorld(Vector3(-55, -40, -615), Vector3(5, 50, 205)); //Left wall of the first aisle in front of the center
	AddWallToWorld(Vector3(-240, -40, -925), Vector3(300, 50, 5)); //Top of the follow up aisle
	AddWallToWorld(Vector3(-250, -40, -825), Vector3(200, 50, 5)); //Bottom of the follow up aisle
	AddWallToWorld(Vector3(-545, -40, -425), Vector3(5, 50, 505)); //Left of the follow up follow up aisle
	AddWallToWorld(Vector3(-445, -40, -315), Vector3(5, 50, 505)); //Right of the follow up follow up aisle
	AddWallToWorld(Vector3(-745, -40, 185), Vector3(295, 50, 5)); //Bottom wall left of the center
	AddWallToWorld(Vector3(-790, -40, 85), Vector3(250, 50, 5)); //Top wall left of the center
	AddWallToWorld(Vector3(-1045, -40, -255), Vector3(5, 50, 55)); //Right left front wall of the first 'chill area'
	AddWallToWorld(Vector3(-1045, -40, -10), Vector3(5, 50, 100)); //Right right front wall of the first 'chill area'
	AddWallToWorld(Vector3(-1045, -40, 380), Vector3(5, 50, 200)); //Left front wall of the first 'chill area'
	AddWallToWorld(Vector3(-1130, -40, -305), Vector3(80, 50, 5)); //Right top front wall of the first 'chill area'
	AddWallToWorld(Vector3(-1410, -40, -305), Vector3(80, 50, 5)); //Right top back wall of the first 'chill area'
	AddWallToWorld(Vector3(-1220, -40, 575), Vector3(170, 50, 5)); //Left bottom wall of the first 'chill area'
	//AddWallToWorld(Vector3(-1270, -40, -405), Vector3(50, 50, 5)); //Right of the 'chill area'
	AddWallToWorld(Vector3(-1215, -40, -800), Vector3(5, 50, 500)); //Right wall right of the 'chill area'
	AddWallToWorld(Vector3(-1325, -40, -850), Vector3(5, 50, 550)); //Left wall left of the 'chill area'
	AddWallToWorld(Vector3(20, -40, -1405), Vector3(1350, 50, 5)); //Upper Top inner big wall
	AddWallToWorld(Vector3(10, -40, -1305), Vector3(1230, 50, 5)); //Bottom top inner big wall
	AddWallToWorld(Vector3(1365, -40, 0), Vector3(5, 50, 1400)); //Right outer inner big wall
	AddWallToWorld(Vector3(1245, -40, -10), Vector3(5, 50, 1300)); //Left inner inner big wall
	AddWallToWorld(Vector3(-65, -40, 1395), Vector3(1425, 50, 5)); //Bottom outer inner big wall
	AddWallToWorld(Vector3(100, -40, 1285), Vector3(1140, 50, 5)); //Bottom inner inner big wall
	AddWallToWorld(Vector3(-1045, -40, 970), Vector3(5, 50, 320)); //Front wall of the second 'chill area'
	AddWallToWorld(Vector3(-1220, -40, 655), Vector3(170, 50, 5)); //Rear wall of the second 'chill area'
	AddWallToWorld(Vector3(-1395, -40, 615), Vector3(5, 50, 45)); //Connection of the 2 'chill areas'
	AddWallToWorld(Vector3(-250, -40, 285), Vector3(700, 50, 5)); //Front wall of the 'big obstacles area'
	AddWallToWorld(Vector3(100, -40, 1185), Vector3(1050, 50, 5)); //Back wall of the 'big obstacles area'
	AddWallToWorld(Vector3(-955, -40, 735), Vector3(5, 50, 455)); //Left wall of the ' big obstacles area'
	AddWallToWorld(Vector3(1155, -40, -10), Vector3(5, 50, 1200)); //Right wall of the 'small/big obstacles area'
	AddWallToWorld(Vector3(455, -40, -110), Vector3(5, 50, 1000)); //Left wall of the 'small obstacles area'
	AddWallToWorld(Vector3(180, -40, -1215), Vector3(980, 50, 5)); //Top wall of the isle before the 'small obstacles area'
	AddWallToWorld(Vector3(-120, -40, -1115), Vector3(580, 50, 5)); //Bottom wall of the isle before the 'small obstacles area'
	AddWallToWorld(Vector3(-805, -40, -715), Vector3(5, 50, 505)); //Left wall of the isle before the isle before the 'small obstacles area'
	AddWallToWorld(Vector3(-705, -40, -615), Vector3(5, 50, 505)); //Right wall of the isle before the isle before the 'small obstacles area'
	AddWallToWorld(Vector3(-920, -40, -205), Vector3(120, 50, 5)); //Top wall of the starting point that leads to the 'small obstacles area'
	AddWallToWorld(Vector3(-870, -40, -105), Vector3(170, 50, 5)); //Bottom wall of the starting point that leads to the 'small obstacles area'
	//AddWallToWorld(Vector3(-955, -40, -155), Vector3(5, 50, 45)); //First wall of the starting point that leads to the 'small obstacles area'

	//Walls in 'small/big obstacles areas'
	AddWallToWorld(Vector3(950, -40, -900), Vector3(200, 50, 5)); //1 right
	AddWallToWorld(Vector3(710, -40, -700), Vector3(250, 50, 5)); //2 left
	AddWallToWorld(Vector3(940, -40, -500), Vector3(220, 50, 5)); //3 right
	AddWallToWorld(Vector3(730, -40, -200), Vector3(270, 50, 5)); //4 left
	AddWallToWorld(Vector3(1000, -40, -20), Vector3(150, 50, 5)); //5 left
	AddWallToWorld(Vector3(900, -40, 285), Vector3(250, 50, 5)); //6 left
	AddWallToWorld(Vector3(710, -40, 685), Vector3(250, 50, 5)); //7 right
	AddWallToWorld(Vector3(755, -40, 1010), Vector3(5, 50, 170)); //1 bot bot
	AddWallToWorld(Vector3(155, -40, 930), Vector3(5, 50, 250)); //2 bot bot
	AddWallToWorld(Vector3(-205, -40, 590), Vector3(5, 50, 300)); //3 top bot
	//Final Destination
	AddWallToWorld(Vector3(-555, -40, 735), Vector3(2, 50, 350)); //Front wall
	//AddWallToWorld(Vector3(-755, -70, 735), Vector3(20, 20, 20)); //WIN!
	AddCubeToWorld(Vector3(-755, -90, 735), Vector3(200, 1, 450), 0.f, "GOAL"); //Goal
}

void TutorialGame::InitSphereGridWorld3(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, radius, z * rowSpacing);
			AddGolfBallToWorld(position, radius);
		}
	}
	AddFloorToWorld(Vector3(0, -100, 0));

	//LEVEL2
	AddWallToWorld(Vector3(1500, -40, 0), Vector3(10, 50, 1510)); //Right Wall
	AddWallToWorld(Vector3(-1500, -40, 0), Vector3(10, 50, 1510)); //Left Wall
	AddWallToWorld(Vector3(0, -40, 1500), Vector3(1490, 50, 10)); //Back Wall
	AddWallToWorld(Vector3(0, -40, -1500), Vector3(1490, 50, 10)); //Front Wall

	AddWallToWorld(Vector3(-360, -40, -900), Vector3(850, 50, 10)); //Front outer inner Wall
	AddWallToWorld(Vector3(-290, -40, -720), Vector3(600, 50, 10)); //Front inner inner Wall
	AddWallToWorld(Vector3(500, -40, 190), Vector3(10, 50, 1100)); //Right outer inner big Wall
	AddWallToWorld(Vector3(300, -40, 390), Vector3(10, 50, 1100)); //Right inner inner big Wall
	AddWallToWorld(Vector3(-1200, -40, 160), Vector3(10, 50, 1050)); //Left outer inner big Wall
	AddWallToWorld(Vector3(-900, -40, 70), Vector3(10, 50, 800)); //Left inner inner big Wall
	AddWallToWorld(Vector3(-410, -40, 880), Vector3(500, 50, 10)); //Bottom inner inner Wall
	AddWallToWorld(Vector3(-550, -40, 1200), Vector3(640, 50, 10)); //Bottom outer inner Wall

	AddWallToWorld(Vector3(-10, -40, 200), Vector3(300, 50, 10)); //Starting point bottom wall
	AddWallToWorld(Vector3(-320, -40, -90), Vector3(10, 50, 300)); //Staring point left wall

	AddWallToWorld(Vector3(-260, -40, 500), Vector3(350, 50, 10)); //Wall at the bottom of starting point
	AddWallToWorld(Vector3(-620, -40, 60), Vector3(10, 50, 450)); //Wall on the left of the starting point

	AddWallToWorld(Vector3(-1100, -40, -1340), Vector3(10, 50, 150)); //Top obstacles 1
	AddWallToWorld(Vector3(-700, -40, -1340), Vector3(10, 50, 150)); //Top obstacles 2
	AddWallToWorld(Vector3(-300, -40, -1340), Vector3(10, 50, 150)); //Top obstacles 3
	AddWallToWorld(Vector3(100, -40, -1340), Vector3(10, 50, 150)); //Top obstacles 4
	AddWallToWorld(Vector3(500, -40, -1340), Vector3(10, 50, 150)); //Top obstacles 5

	AddWallToWorld(Vector3(-900, -40, -1060), Vector3(10, 50, 150)); //Bot obstacles 1
	AddWallToWorld(Vector3(-500, -40, -1060), Vector3(10, 50, 150)); //Bot obstacles 2
	AddWallToWorld(Vector3(-100, -40, -1060), Vector3(10, 50, 150)); //Bot obstacles 3
	AddWallToWorld(Vector3(300, -40, -1060), Vector3(10, 50, 150)); //Bot obstacles 4

	AddWallToWorld(Vector3(1000, -45, -735), Vector3(300, 45, 10)); //In front of win area wall

	//AddWallToWorld(Vector3(1000, -70, -200), Vector3(20, 20, 20)); //WIN!
	AddCubeToWorld(Vector3(1000, -90, -200), Vector3(200, 1, 150), 0.f, "GOAL"); //Goal
	AddWallToWorld(Vector3(800, -40, -200), Vector3(10, 50, 150)); //Left win area wall
	AddWallToWorld(Vector3(1200, -40, -200), Vector3(10, 50, 150)); //Right win area wall
	AddWallToWorld(Vector3(1000, -40, -40), Vector3(210, 50, 10)); //Back win area wall
	AddWallToWorld(Vector3(1000, -40, -360), Vector3(210, 50, 10)); //Front win area wall

	AddWallToWorld(Vector3(1000, -40, 750), Vector3(10, 50, 500)); //Behind win area wall
}

void TutorialGame::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 10.0f;
	Vector3 cubeDims = Vector3(10, 10, 10);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, cubeDims.y, z * rowSpacing);

			if (rand() % 2) {
				AddCubeToWorld(position, cubeDims);
			}
			else {
				AddSphereToWorld(position, sphereRadius);
			}
		}
	}
	AddFloorToWorld(Vector3(0, -100, 0));
}

void TutorialGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, cubeDims.y, z * rowSpacing);
			AddCubeToWorld(position, cubeDims, 1.0f);
		}
	}
	AddFloorToWorld(Vector3(10, -100, 1));
}


void TutorialGame::InitSphereCollisionTorqueTest() {
	AddSphereToWorld(Vector3(15, 0, 0), 10.0f);
	AddSphereToWorld(Vector3(-25, 0, 0), 10.0f);
	AddSphereToWorld(Vector3(-50, 0, 0), 10.0f);

	AddCubeToWorld(Vector3(-50, 0, -50), Vector3(60, 10, 10), 10.0f);

	AddFloorToWorld(Vector3(0, -100, 0));
}


void TutorialGame::InitCubeCollisionTorqueTest() {
	Vector3 cubeSize(10, 10, 10);
	AddCubeToWorld(Vector3(15, 0, 0), cubeSize, 10.0f);
	AddCubeToWorld(Vector3(-25, 0, 0), cubeSize, 10.0f);
	AddCubeToWorld(Vector3(-50, 0, 0), cubeSize, 10.0f);

	AddCubeToWorld(Vector3(-50, 0, -50), Vector3(60, 10, 10), 10.0f);

	AddFloorToWorld(Vector3(0, -100, 0));
}

void TutorialGame::InitSphereAABBTest() {
	Vector3 cubeSize(10, 10, 10);

	AddCubeToWorld(Vector3(0, 0, 0), cubeSize, 10.0f);
	AddSphereToWorld(Vector3(2, 0, 0), 5.0f, 10.0f);
}

void TutorialGame::InitGJKWorld() {
	Vector3 dimensions(20, 2, 10);
	float inverseMass = 10.0f;

	for (int i = 0; i < 2; ++i) {
		GameObject* cube = new GameObject();

		OBBVolume* volume = new OBBVolume(dimensions);

		cube->SetBoundingVolume((CollisionVolume*)volume);

		cube->GetTransform().SetWorldPosition(Vector3(0, 0, 0));
		cube->GetTransform().SetWorldScale(dimensions);

		if (i == 1) {
			cube->GetTransform().SetLocalOrientation(Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 90.0f));
		}

		cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
		cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

		cube->GetPhysicsObject()->SetInverseMass(inverseMass);
		cube->GetPhysicsObject()->InitCubeInertia();

		world->AddGameObject(cube);
	}
}

void TutorialGame::BridgeConstraintTest()
{
	Vector3 cubeSize = Vector3(70, 1, 10);

	float invCubeMass = 5;
	int numLinks = 13;
	float maxDistance = 24;
	float cubeDistance = 24;

	Vector3 startPos = Vector3(670, 0, -1100);

	GameObject* start = AddBridgeToWorld(startPos + Vector3(360, -90, 0), cubeSize, 0);

	GameObject* end = AddBridgeToWorld(startPos + Vector3((numLinks + 2) * cubeDistance, 0, 360), cubeSize, 0);

	GameObject* previous = start;

	for (int i = 0; i < numLinks; ++i) {
		GameObject* block = AddBridgeToWorld(startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize, invCubeMass);

		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);

		world->AddConstraint(constraint);
		previous = block;
	}
	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
	world->AddConstraint(constraint);
}

void TutorialGame::BridgeConstraintTest1()
{
	Vector3 cubeSize = Vector3(70, 1, 10);

	float invCubeMass = 5;
	int numLinks = 15;
	float maxDistance = 21;
	float cubeDistance = 21;

	Vector3 startPos = Vector3(670, 0, -720);

	GameObject* start = AddBridgeToWorld(startPos + Vector3(360, 0, 0), cubeSize, 0);

	GameObject* end = AddBridgeToWorld(startPos + Vector3((numLinks + 2) * cubeDistance, 10, 360), cubeSize, 0);

	GameObject* previous = start;

	for (int i = 0; i < numLinks; ++i) {
		GameObject* block = AddBridgeToWorld(startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize, invCubeMass);

		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);

		world->AddConstraint(constraint);
		previous = block;
	}
	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
	world->AddConstraint(constraint);
}

void TutorialGame::SimpleGJKTest() {
	Vector3 dimensions		= Vector3(5, 5, 5);
	Vector3 floorDimensions = Vector3(100, 2, 100);

	GameObject* fallingCube = AddCubeToWorld(Vector3(0, 20, 0), dimensions, 10.0f);
	GameObject* newFloor	= AddCubeToWorld(Vector3(0, 0, 0), floorDimensions, 0.0f);

	delete fallingCube->GetBoundingVolume();
	delete newFloor->GetBoundingVolume();

	fallingCube->SetBoundingVolume((CollisionVolume*)new OBBVolume(dimensions));
	newFloor->SetBoundingVolume((CollisionVolume*)new OBBVolume(floorDimensions));

}

void TutorialGame::SimpleAABBTest() {
	Vector3 dimensions		= Vector3(5, 5, 5);
	Vector3 floorDimensions = Vector3(100, 2, 100);

	GameObject* newFloor	= AddCubeToWorld(Vector3(0, 0, 0), floorDimensions, 0.0f);
	GameObject* fallingCube = AddCubeToWorld(Vector3(10, 20, 0), dimensions, 10.0f);
}

void TutorialGame::SimpleAABBTest2() {
	Vector3 dimensions		= Vector3(5, 5, 5);
	Vector3 floorDimensions = Vector3(8, 2, 8);

	GameObject* newFloor	= AddCubeToWorld(Vector3(0, 0, 0), floorDimensions, 0.0f);
	GameObject* fallingCube = AddCubeToWorld(Vector3(8, 20, 0), dimensions, 10.0f);
}

bool TutorialGame::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::MOUSE_LEFT)) {
			if (selectionObject) {
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				selectionObject = (GameObject*)closestCollision.node;
				if(selectionObject->GetName() == "ball")
					selectionObject->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		
	}
	return false;
}

void TutorialGame::MoveSelectedObject() {
	renderer->DrawString("Click Force:" + std::to_string(forceMagnitude), Vector2(0, 0), Vector4(0, 0, 0, 1));
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 500.0f;

	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_M) || Window::GetKeyboard()->KeyPressed(KEYBOARD_1) || Window::GetKeyboard()->KeyPressed(KEYBOARD_R) || Window::GetKeyboard()->KeyPressed(KEYBOARD_2))
	{
		score = 0;
	}

	renderer->DrawString("Score: " + std::to_string(score), Vector2(0, 30), Vector4(0, 0, 0, 1));

	if (!selectionObject) {
		return;//we haven't selected anything!
	}
	//Push the selected object!
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::MOUSE_RIGHT)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
				if(selectionObject->GetName() == "ball")
					++score;
			}
		}
	}
}

GameObject* TutorialGame::AddCubeMonsterToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, string name) {
	GameObject* monster = new GameObject();
	monster->SetName("Monster");

	AABBVolume* volume = new AABBVolume(dimensions);

	monster->SetBoundingVolume((CollisionVolume*)volume);

	monster->GetTransform().SetWorldPosition(position);
	monster->GetTransform().SetWorldScale(dimensions);

	monster->SetRenderObject(new RenderObject(&monster->GetTransform(), cubeMesh, monsterTex, basicShader));
	monster->SetPhysicsObject(new PhysicsObject(&monster->GetTransform(), monster->GetBoundingVolume()));

	monster->GetPhysicsObject()->SetInverseMass(inverseMass);
	monster->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(monster);

	return monster;
}

void TutorialGame::TestPathfinding()
{
	NavigationGrid grid("CourseworkGrid.txt" , Vector3(-1500  - 50, 0, -1500 - 50));

	NavigationPath outPath;
	int scale = 100;
	Vector3 startPos = monster->GetTransform().GetWorldPosition();
	Vector3 endPos = ball->GetTransform().GetWorldPosition();
	

	bool found = grid.FindPath(startPos, endPos, outPath);

	testNodes.clear();
	Vector3 pos;
	while (outPath.PopWaypoint(pos))
	{
		testNodes.push_back(pos - Vector3(1510, 0, 1510));
	}
}

void TutorialGame::DisplayPathfinding()
{
	for (int i = 1; i < testNodes.size(); ++i)
	{
		Vector3 a = testNodes[i - 1];
		Vector3 b = testNodes[i];

		Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
	}
}

void TutorialGame::MonsterFollow()
{
	if (testNodes.size() < 2) {
		return;
	}
	float distance;
	Vector3 direction;
	Vector3 monsterLocation;
	Vector3 a = (testNodes[1]+ testNodes[0]) * 0.5f;

	a = testNodes[1];
	monsterLocation = monster->GetTransform().GetWorldPosition();

	Debug::DrawLine(monsterLocation, a, Vector4(0.6, 0.2, 0.8, 1));

	distance = (a - monsterLocation).Length();
	direction = (a - monsterLocation).Normalised();
	//direction.y = 0;
	monster->GetPhysicsObject()->AddForce(direction * 1000);
	//std::cout << testNodes.size() << std::endl;
}