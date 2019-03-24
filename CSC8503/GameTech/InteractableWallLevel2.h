#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"
#include "../CSC8503Common/StateTransition.h"

namespace NCL {
	namespace CSC8503 {
		class InteractableWallLevel2 : public GameObject
		{
		public:
			InteractableWallLevel2(GameObject t, string name = "");
			~InteractableWallLevel2();

			virtual void OnCollisionBegin(GameObject* otherObject)
			{
				
			}

			virtual void OnCollisionEnd(GameObject* otherObject)
			{

			}

			void updateState()
			{
				startingPosition = Vector3(1030, 50, -550);
				stateMachine->Update();
				Vector3 offsetPosition = this->GetTransform().GetLocalPosition() - startingPosition;
				Vector3 normalisedAxis = this->someData.axis.Normalised();
				float bias = 0.0001;
				dist =((startingPosition * this->someData.axis) - (this->GetTransform().GetLocalPosition()) * this->someData.axis).x;
			}

			void initState();
			
			struct InteractableWallStateData
			{
				PhysicsObject* physicsObject = nullptr;
				float speed = 0.0f;
				Vector3 axis = Vector3(1, 0, 0);
			};

			void setSpeed(float s)
			{
				someData.speed = s;
			}

			const float getSpeed() const
			{
				return someData.speed;
			}

			void setMovementOnAxis(Vector3 axis)
			{
				someData.axis = axis;
			}
			
			const Vector3 getMovementOnAxis() const 
			{
				return someData.axis;
			}

			void setMaxDistance(float distance)
			{
				maxDistance = distance;
			}

			const float getMaxDistance() const
			{
				return maxDistance;
			}

		protected:

			StateMachine* stateMachine;
			StateFunc AFunc;
			StateFunc BFunc;
			GenericState* stateA;
			GenericState* stateB;
			GenericTransition<float&, float>* transitionA;
			GenericTransition<float&, float>* transitionB;
			Vector3 startingPosition;

			InteractableWallStateData someData;

			float dist = 0.0f;
			float maxDistance = 200.0f;
		};
	}
}