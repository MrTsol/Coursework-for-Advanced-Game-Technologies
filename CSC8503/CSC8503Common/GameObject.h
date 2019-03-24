#pragma once
#include "Transform.h"
#include "CollisionVolume.h"

#include "PhysicsObject.h"
#include "RenderObject.h"
#include "NetworkObject.h"

#include <vector>

#include "../CSC8503Common/State.h"
#include "StateMachine.h"

using std::vector;

namespace NCL {
	namespace CSC8503 {
		class NetworkObject;

		class GameObject	{
		public:
			GameObject(string name = "");
			~GameObject();

			void SetBoundingVolume(CollisionVolume* vol) {
				boundingVolume = vol;
			}

			const CollisionVolume* GetBoundingVolume() const {
				return boundingVolume;
			}

			bool IsActive() const {
				return isActive;
			}

			const Transform& GetConstTransform() const {
				return transform;
			}

			Transform& GetTransform() {
				return transform;
			}

			RenderObject* GetRenderObject() const {
				return renderObject;
			}

			PhysicsObject* GetPhysicsObject() const {
				return physicsObject;
			}

			NetworkObject* GetNetworkObject() const {
				return networkObject;
			}

			void SetRenderObject(RenderObject* newObject) {
				renderObject = newObject;
			}

			void SetPhysicsObject(PhysicsObject* newObject) {
				physicsObject = newObject;
			}

			const string& GetName() const {
				return name;
			}

			virtual void OnCollisionBegin(GameObject* otherObject) {
				//std::cout << "OnCollisionBegin event occured!\n";
			}

			virtual void OnCollisionEnd(GameObject* otherObject) {
				//std::cout << "OnCollisionEnd event occured!\n";
			}

			bool InsideAABB(const Vector3& pos, const Vector3& halfSize);

			bool GetBroadphaseAABB(Vector3&outsize) const;

			void UpdateBroadphaseAABB();

			//
			void SetName(string newName)
			{
				name = newName;
			}
			//

		protected:
			Transform			transform;

			CollisionVolume*	boundingVolume;
			PhysicsObject*		physicsObject;
			RenderObject*		renderObject;
			NetworkObject*		networkObject;

			bool	isActive;
			string	name;

			Vector3 broadphaseAABB;
		};
		//Goal and death
		class GolfBall : public GameObject
		{
		public:
			GolfBall() :GameObject()
			{

			}
			GolfBall(const GameObject *go) :GameObject(go->GetName())
			{
				CollisionVolume* vol = new CollisionVolume();
				vol->type = go->GetBoundingVolume()->type;
				this->SetBoundingVolume(vol);

				this->SetName(go->GetName());
				this->SetPhysicsObject(go->GetPhysicsObject());
				this->SetRenderObject(go->GetRenderObject());
			}

			void OnCollisionBegin(GameObject* otherObject) override
			{
				if (otherObject->GetName() == "GOAL")
				{
					goal = true;
				}

				if (otherObject->GetName() == "Interactable Wall Level1")
				{
					isDeadLevel1 = true;
				}

				if (otherObject->GetName() == "Interactable Wall Level2")
				{
					isDeadLevel2 = true;
				}

				if (otherObject->GetName() == "Load Level1")
				{
					isLevel1 = true;
				}

				if (otherObject->GetName() == "Load Level2")
				{
					isLevel2 = true;
				}

				if (otherObject->GetName() == "Monster")
				{
					isMainMenu = true;
				}
			}

			void OnCollisionEnd(GameObject* otherObject) override
			{

			}

			bool returnGoal() {
				return goal;
			}

			bool returnDeath1() {
				return isDeadLevel1;
			}

			bool returnDeath2() {
				return isDeadLevel2;
			}

			bool returnLevel1() {
				return isLevel1;
			}

			bool returnLevel2() {
				return isLevel2;
			}

			bool returnMainMenu() {
				return isMainMenu;
			}

		protected:
			bool goal = false;
			bool isDeadLevel1 = false;
			bool isDeadLevel2 = false;
			bool isLevel1 = false;
			bool isLevel2 = false;
			bool isMainMenu = false;
		};
	}
}