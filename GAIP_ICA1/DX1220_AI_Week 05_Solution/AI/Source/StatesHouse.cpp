#include "StatesHouse.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float ENERGY_DROP_RATE = 0.1f;
static const float FULL_SPEED = 8.f;
static const float HUNGRY_SPEED = 6.f;

HouseStateSpawner::HouseStateSpawner(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

HouseStateSpawner::~HouseStateSpawner()
{
}

void HouseStateSpawner::Enter()
{
	m_go->moveSpeed = 0;
}

void HouseStateSpawner::Update(double dt)
{
	ElaspedTime += static_cast<float>(dt);
	if (ElaspedTime >= MaxVillagerSpawnerTime) {
		ElaspedTime = 0;
		int random = Math::RandIntMinMax(0, 10);
		if (random == 1) {
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SPAWN_VILLAGER, 1.0f));
		}
	}
}

void HouseStateSpawner::Exit()
{
}