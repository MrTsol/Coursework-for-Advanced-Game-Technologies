#include "../../Common/Window.h"

#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/State.h"

#include "../CSC8503Common/GameServer.h"
#include "../CSC8503Common/GameClient.h"

#include "../CSC8503Common/NavigationGrid.h"

#include "TutorialGame.h"
#include "NetworkedGame.h"

using namespace NCL;
using namespace CSC8503;

void TestStateMachine()
{
	StateMachine* testMachine = new StateMachine();

	int someData = 0;

	StateFunc AFunc = [](void* data)
	{
		int* realData = (int*)data;
		(*realData)++;
		std::cout << "In State A!" << std::endl;
	};

	StateFunc BFunc = [](void* data)
	{
		int* realData = (int*)data;
		(*realData)--;
		std::cout << "In State B!" << std::endl;
	};

	GenericState* stateA = new GenericState(AFunc, (void*)&someData);
	GenericState* stateB = new GenericState(BFunc, (void*)&someData);
	testMachine->AddState(stateA);
	testMachine->AddState(stateB);

	GenericTransition<int&, int>* transitionA = new GenericTransition<int&, int>(GenericTransition<int&, int>::GreaterThanTransition, someData, 10, stateA, stateB); //If greater than 10, A to B
	GenericTransition<int&, int>* transitionB = new GenericTransition<int&, int>(GenericTransition<int&, int>::EqualsTransition, someData, 0, stateB, stateA); //If equals 0, B to A

	testMachine->AddTransition(transitionA);
	testMachine->AddTransition(transitionB);

	for (int i = 0; i < 100; ++i)
	{
		testMachine->Update(); //Run the state machine!
	}
	delete testMachine;
}//End of TestStateMachine function!

class TestPacketReceiver : public PacketReceiver
{
public:
	TestPacketReceiver(string name)
	{
		this->name = name;
	}

	void ReceivePacket(int type, GamePacket* payload, int source)
	{
		if (type == String)
		{
			StringPacket* realPacket = (StringPacket*)payload;

			string msg = realPacket->GetStringFromData();

			std::cout << name << " received message: " << msg << std::endl;
		}
	}
protected:
	string name;
};

void TestNetworking()
{
	NetworkBase::Initialise();

	TestPacketReceiver serverReceiver("Server");
	TestPacketReceiver clientReceiver("Client");

	int port = NetworkBase::GetDefaultPort();

	GameServer* server = new GameServer(port, 1);
	GameClient* client = new GameClient();

	server->RegisterPacketHandler(String, &serverReceiver);
	client->RegisterPacketHandler(String, &clientReceiver);

	bool canConnet = client->Connect(127, 0, 0, 1, port);

	for (int i = 0; i < 100; ++i)
	{
		server->SendGlobalMessage(StringPacket("Server says hello! " + std::to_string(i)));
		
		client->SendPacket(StringPacket("Client says hello! " + std::to_string(i)));

		server->UpdateServer();
		client->UpdateClient();

		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	NetworkBase::Destroy();
}

int main() {
	Window*w = Window::CreateGameWindow("CSC8503 Game technology!", 1600, 1000, false);

	if (!w->HasInitialised()) {
		return -1;
	}	
	TestStateMachine();
	TestNetworking();
	//TestPathfinding();
	
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);

	TutorialGame* g = new TutorialGame();

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float dt = w->GetTimer()->GetTimeDelta() / 1000.0f;

		if (dt > 1.0f) {
			continue;
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_NEXT)) {
			w->ShowConsole(false);
		}

		//DisplayPathfinding();

		w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));

		g->UpdateGame(dt);
	}
	Window::DestroyGameWindow();
}