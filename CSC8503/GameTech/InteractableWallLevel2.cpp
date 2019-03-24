#include "InteractableWallLevel2.h"

using namespace NCL;
using namespace CSC8503;

InteractableWallLevel2::InteractableWallLevel2(GameObject t, string name)
{

}

InteractableWallLevel2::~InteractableWallLevel2()
{

}

void InteractableWallLevel2::initState()
{
	someData.physicsObject = this->GetPhysicsObject();
	startingPosition = this->GetTransform().GetLocalPosition();

	stateMachine = new StateMachine();

	AFunc = [](void* data)
	{
		InteractableWallStateData* realData = (InteractableWallStateData*)data;
		realData->physicsObject->SetLinearVelocity(Vector3(realData->axis.x * realData->speed, realData->axis.y * realData->speed, realData->axis.z * realData->speed));
		//std::cout << "In State A!" << std::endl;
	};

	BFunc = [](void* data)
	{
		InteractableWallStateData* realData = (InteractableWallStateData*)data;
		realData->physicsObject->SetLinearVelocity(Vector3(realData->axis.x * -realData->speed, realData->axis.y * -realData->speed, realData->axis.z * -realData->speed));
		//std::cout << "In State B!" << std::endl;
	};

	stateA = new GenericState(AFunc, (void*)&this->someData);
	stateB = new GenericState(BFunc, (void*)&this->someData);

	stateMachine->AddState(stateA);
	stateMachine->AddState(stateB);

	transitionA = new GenericTransition<float&, float>(GenericTransition<float&, float>::LessThanTransition, dist, -maxDistance, stateA, stateB);
	transitionB = new GenericTransition<float&, float>(GenericTransition<float&, float>::GreaterThanTransition, dist, maxDistance, stateB, stateA);

	stateMachine->AddTransition(transitionA);
	stateMachine->AddTransition(transitionB);
}