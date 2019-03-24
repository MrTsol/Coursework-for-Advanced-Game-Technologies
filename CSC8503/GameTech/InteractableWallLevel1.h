#pragma once
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"
#include "../CSC8503Common/StateTransition.h"

namespace NCL {
	namespace CSC8503 {
		class InteractableWallLevel1 : public GameObject
		{
		public:
			InteractableWallLevel1(GameObject t, string name = "");
			~InteractableWallLevel1();

			virtual void OnCollisionBegin(GameObject* otherObject)
			{

			}

			virtual void OnCollisionEnd(GameObject* otherObject)
			{

			}

			void updateState()
			{
				startingPosition = Vector3(-540, -40, 735);
				stateMachine->Update();
				Vector3 offsetPosition = this->GetTransform().GetLocalPosition() - startingPosition;
				Vector3 normalisedAxis = this->someData.axis.Normalised();
				float bias = 0.0001;
				dist = ((startingPosition * this->someData.axis) - (this->GetTransform().GetLocalPosition()) * this->someData.axis).z;
			}

			void initState();

			struct InteractableWallStateData
			{
				PhysicsObject* physicsObject = nullptr;
				float speed = 0.0f;
				Vector3 axis = Vector3(0, 0, 1);
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
			float maxDistance = 10.0f;
		};
	}
}