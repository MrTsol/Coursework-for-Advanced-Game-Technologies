#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"

#include "InteractableWallLevel1.h"
#include "InteractableWallLevel2.h"


namespace NCL {
	namespace CSC8503 {
		class TutorialGame		{
		public:
			TutorialGame();
			~TutorialGame();

			virtual void UpdateGame(float dt);

			int score;

		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			void MainMenu();
			void InitWorld1();
			void InitWorld2();

			void InitSphereGridWorld1(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitSphereGridWorld2(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitSphereGridWorld3(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);

			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void InitSphereCollisionTorqueTest();
			void InitCubeCollisionTorqueTest();
			void InitSphereAABBTest();
			void InitGJKWorld();
			void BridgeConstraintTest();
			void BridgeConstraintTest1();
			void SimpleGJKTest();
			void SimpleAABBTest();
			void SimpleAABBTest2();

			bool SelectObject();
			void MoveSelectedObject();

			GameObject* AddFloorToWorld(const Vector3& position);

			GameObject* AddWallToWorld(const Vector3& position, const Vector3& wallSize);
			OGLTexture* basicTex1 = nullptr;
			OGLTexture* basicTex2 = nullptr;
			OGLTexture* basicTex3 = nullptr;

			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddGolfBallToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, string name = "");
			GameObject* AddCubeToLoadLevel1ToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, string name = "");
			GameObject* AddCubeToLoadLevel2ToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, string name = "");
			GameObject* AddBridgeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, string name = "");
			GameObject* AddInteractableWallLevel1ToWorld(const Vector3& position, Vector3 dimensions, float maxDistance, float speed, Vector3 movementAxis, string name = "");
			GameObject* AddInteractableWallLevel2ToWorld(const Vector3& position, Vector3 dimensions, float maxDistance, float speed, Vector3 movementAxis, string name = "");
			GameObject* AddCubeMonsterToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, string name = "");

			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;

			GameObject*			monster;

			bool useGravity;
			bool inSelectionMode;
			bool mainMenuText;
			bool controlsAndOptions;

			bool mainMenuRobot;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			
			void TestPathfinding();
			void DisplayPathfinding();
			void MonsterFollow();
			vector<Vector3> testNodes;
			OGLTexture* monsterTex = nullptr;
			

			OGLTexture* basicTex	= nullptr;

			OGLTexture* bridgeTex = nullptr;
			OGLTexture* movingWallTex = nullptr;
			OGLTexture* loadLevel1Tex = nullptr;
			OGLTexture* loadLevel2Tex = nullptr;

			OGLShader*	basicShader = nullptr;
			GolfBall* ball = nullptr;

			InteractableWallLevel1* interactableWallLevel1 = nullptr;
			InteractableWallLevel2* interactableWallLevel2 = nullptr;
		};
	}
}

