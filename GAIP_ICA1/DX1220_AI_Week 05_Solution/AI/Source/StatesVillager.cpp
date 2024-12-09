#include "StatesVillager.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float ENERGY_DROP_RATE = 0.1f;
static const float FULL_SPEED = 8.f;
static const float HUNGRY_SPEED = 6.f;
static const float EnemyDetectionDist = 3.0f;

VillagerStateTooFull::VillagerStateTooFull(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

VillagerStateTooFull::~VillagerStateTooFull()
{
}

void VillagerStateTooFull::Enter()
{
	m_go->moveSpeed = 0;
}

void VillagerStateTooFull::Update(double dt)
{
	m_go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt);
	if (m_go->energy < 10.f)
		m_go->sm->SetNextState("VillagerFull");


	if (m_go->hp <= 0.0f)
	{
		m_go->sm->SetNextState("CowDead");
	}
}

void VillagerStateTooFull::Exit()
{
}



VillagerStateFull::VillagerStateFull(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go),
	m_elapsed(MESSAGE_INTERVAL)
{
}

VillagerStateFull::~VillagerStateFull()
{
}

void VillagerStateFull::Enter()
{
	m_go->moveSpeed = FULL_SPEED;
	m_go->nearest = NULL;
	m_elapsed = MESSAGE_INTERVAL;
	enemy_elasped = MESSAGE_INTERVAL;
}

void VillagerStateFull::Update(double dt)
{
	m_elapsed += static_cast<float>(dt);
	enemy_elasped += static_cast<float>(dt);
	m_go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt);
	if (m_go->nearestEnemy && m_go->nearestEnemy->active) {
		std::cout << "RAAAH" << std::endl;
		m_go->sm->SetNextState("VillagerChaseOps");
	}
	else if (m_go->energy >= 10.f)
		m_go->sm->SetNextState("VillagerTooFull");
	else if (m_go->energy < 5.f)
		m_go->sm->SetNextState("VillagerHungry");
	if (m_go->hp <= 0.0f)
	{
		m_go->sm->SetNextState("VillagerDead");
	}
	
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;

	//once nearest is set, fish will continue to move away from shark even
	//when they have move significantly far away (until it changes state).
	//TODO: consider setting nearest to nullptr if shark is "far enough"
	if (m_go->nearest && m_go->nearest->active)
	{
		if (m_go->nearest->pos.x < m_go->pos.x)
			m_go->moveRight = false;
		else
			m_go->moveLeft = false;
		if (m_go->nearest->pos.y < m_go->pos.y)
			m_go->moveUp = false;
		else
			m_go->moveDown = false;
	}
	else //m_go->nearest is nullptr
	{
		if (m_elapsed >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			m_elapsed -= MESSAGE_INTERVAL;

			//week 4
			//send message to Scene requesting for nearest to be updated
			//message is allocated on the heap (WARNING: expensive. 
			//either refactor PostOffice to not assume heap-allocated messages,
			//or pool messages to avoid real-time heap allocation)
			const float TREE_DIST = 20.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene",
				new MessageWRU(m_go, MessageWRU::NEAREST_TREE_VILLAGER, TREE_DIST));
		}
	}
	if (enemy_elasped >= MESSAGE_INTERVAL * 2.0f) //ensure at least 1 second interval between messages
	{
		enemy_elasped -= MESSAGE_INTERVAL * 2.0f;
		PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_VILLAGER_OPS, EnemyDetectionDist *SceneData::GetInstance()->GetGridSize()));
	}
}

void VillagerStateFull::Exit()
{
}



VillagerStateHungry::VillagerStateHungry(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go),
	m_elapsed(MESSAGE_INTERVAL)
{
}

VillagerStateHungry::~VillagerStateHungry()
{
}

void VillagerStateHungry::Enter()
{
	m_go->moveSpeed = HUNGRY_SPEED;
	m_go->nearest = NULL;
	m_elapsed = MESSAGE_INTERVAL;
	m_go->Hungry = true;
	enemy_elasped = MESSAGE_INTERVAL;
}

void VillagerStateHungry::Update(double dt)
{
	m_elapsed += static_cast<float>(dt); //check against this value before sending message(so we don't send the message every frame)
	enemy_elasped += static_cast<float>(dt);
	m_go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt);

	if (m_go->nearestEnemy && m_go->nearestEnemy->active) {
		std::cout << "RAAAH" << std::endl;
		m_go->sm->SetNextState("VillagerChaseOps");
	}
	if (m_go->energy < 0.f || m_go->hp <= 0)
	{
		m_go->sm->SetNextState("VillagerDead");
	}
	else if (SceneData::GetInstance()->GetFoodEnergyCount(m_go->RED) + m_go->FoodEnergyCollected >= 10 - m_go->energy) {
		m_go->sm->SetNextState("VillagerGoToHouse");
	}
	const float tolerance = 0.5f;
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	if (m_go->nearest && m_go->nearest->active)
	{
		// Check x-axis movement
		if (std::abs(m_go->nearest->pos.x - m_go->pos.x) <= tolerance)
		{
			m_go->moveLeft = false;
			m_go->moveRight = false;
		}
		else if (m_go->nearest->pos.x > m_go->pos.x)
		{
			m_go->moveLeft = false;
		}
		else
		{
			m_go->moveRight = false;
		}

		// Check y-axis movement
		if (std::abs(m_go->nearest->pos.y - m_go->pos.y) <= tolerance)
		{
			m_go->moveUp = false;
			m_go->moveDown = false;
		}
		else if (m_go->nearest->pos.y > m_go->pos.y)
		{
			m_go->moveDown = false;
		}
		else
		{
			m_go->moveUp = false;
		}
	}
	else //go->nearest is nullptr
	{
		if (m_elapsed >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			m_elapsed -= MESSAGE_INTERVAL;
			//week 4
			//send message to Scene requesting for nearest to be updated
			//message is allocated on the heap (WARNING: expensive. 
			//either refactor PostOffice to not assume heap-allocated messages,
			//or pool messages to avoid real-time heap allocation)
			const float FOOD_DIST = 20.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_COW, FOOD_DIST));
		}
	}
	if (enemy_elasped >= MESSAGE_INTERVAL * 2.0f) //ensure at least 1 second interval between messages
	{
		enemy_elasped -= MESSAGE_INTERVAL * 2.0f;
		PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_VILLAGER_OPS, EnemyDetectionDist * SceneData::GetInstance()->GetGridSize()));
	}
}

void VillagerStateHungry::Exit()
{
}



VillagerStateDead::VillagerStateDead(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

VillagerStateDead::~VillagerStateDead()
{
}

void VillagerStateDead::Enter()
{
	m_go->countDown = 3.f;
	m_go->moveSpeed = 0;
}

void VillagerStateDead::Update(double dt)
{
	m_go->countDown -= static_cast<float>(dt);
	if (m_go->countDown < 0)
	{
		m_go->active = false;
	}
}

void VillagerStateDead::Exit()
{
}



VillagerStateCutTree::VillagerStateCutTree(const std::string& stateID, GameObject* go) : State(stateID),
m_go(go)
{
}

VillagerStateCutTree::~VillagerStateCutTree()
{
}

void VillagerStateCutTree::Enter()
{
	m_go->moveSpeed = 0;
	m_go->Stationary = true;
	m_elapsed = 3;
	message_elapsed = MESSAGE_INTERVAL;
	enemy_elasped = MESSAGE_INTERVAL;
}

void VillagerStateCutTree::Update(double dt)
{
	enemy_elasped += static_cast<float>(dt);
	message_elapsed += static_cast<float>(dt); //check against this value before sending message(so we don't send the message every frame)
	bool CutTree = false;
	m_go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt);
	if (m_go->nearestEnemy && m_go->nearestEnemy->active) {
		std::cout << "RAAAH" << std::endl;
		m_go->sm->SetNextState("VillagerChaseOps");
	}

	if (m_go->energy < 0.f || m_go->hp <= 0.0f)
	{
		m_go->sm->SetNextState("VillagerDead");
	}

	if (m_go->nearest && m_go->nearest->type == GameObject::GO_TREE)
	{
		m_elapsed -= static_cast<float>(dt);
		m_go->WoodCollected += static_cast<float>(dt);
		if (m_elapsed < 0)
		{
			m_go->nearest->active = false;
			CutTree = true;
		}
		if (CutTree) {
			m_go->sm->SetNextState("VillagerGoToHouse");
		}
		else {
			if (m_go->nearest->active == false) {
				if (m_go->energy >= 5.f)
					m_go->sm->SetNextState("VillagerFull");
				else {
					m_go->sm->SetNextState("VillagerHungry");
				}
			}
		}
		//else {
		//	m_go->energy += ENERGY_DROP_RATE * static_cast<float>(dt) * 5;
		//}
	}
	else {
		if (m_elapsed >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			m_elapsed -= MESSAGE_INTERVAL;
			const float FOOD_DIST = 20.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_TREE_VILLAGER, FOOD_DIST));
		}
	}
	if (enemy_elasped >= MESSAGE_INTERVAL * 2.0f) //ensure at least 1 second interval between messages
	{
		enemy_elasped -= MESSAGE_INTERVAL * 2.0f;
		PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_VILLAGER_OPS, EnemyDetectionDist * SceneData::GetInstance()->GetGridSize()));
	}
}

void VillagerStateCutTree::Exit()
{
	m_go->Stationary = false;
}

VillagerStateGoToHouse::VillagerStateGoToHouse(const std::string& stateID, GameObject* go) : State(stateID),
m_go(go)
{

}

VillagerStateGoToHouse::~VillagerStateGoToHouse()
{
}

void VillagerStateGoToHouse::Enter()
{
	m_go->moveSpeed = HUNGRY_SPEED;
	m_go->Stationary = false;
	m_go->nearest = NULL;
	m_elapsed = MESSAGE_INTERVAL;
}

void VillagerStateGoToHouse::Update(double dt)
{
	m_elapsed += static_cast<float>(dt); //check against this value before sending message(so we don't send the message every frame)

	m_go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt);

	//if (m_go->energy < 5.f)
	//	m_go->sm->SetNextState("VillagerFull");
	if (m_go->energy < 0.f || m_go->hp <= 0)
	{
		m_go->sm->SetNextState("VillagerDead");
	}
	const float tolerance = 0.5f;
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	if (m_go->nearest && m_go->nearest->active)
	{
		// Check x-axis movement
		if (std::abs(m_go->nearest->pos.x - m_go->pos.x) <= tolerance)
		{
			m_go->moveLeft = false;
			m_go->moveRight = false;
		}
		else if (m_go->nearest->pos.x > m_go->pos.x)
		{
			m_go->moveLeft = false;
		}
		else
		{
			m_go->moveRight = false;
		}

		// Check y-axis movement
		if (std::abs(m_go->nearest->pos.y - m_go->pos.y) <= tolerance)
		{
			m_go->moveUp = false;
			m_go->moveDown = false;
		}
		else if (m_go->nearest->pos.y > m_go->pos.y)
		{
			m_go->moveDown = false;
		}
		else
		{
			m_go->moveUp = false;
		}
	}
	else //go->nearest is nullptr
	{
		if (m_elapsed >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			m_elapsed -= MESSAGE_INTERVAL;
			//week 4
			//send message to Scene requesting for nearest to be updated
			//message is allocated on the heap (WARNING: expensive. 
			//either refactor PostOffice to not assume heap-allocated messages,
			//or pool messages to avoid real-time heap allocation)
			const float HOUSE_DIST = 20.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_HOUSE_VILLAGER, HOUSE_DIST));
		}
	}
}

void VillagerStateGoToHouse::Exit()
{
}













VillagerChaseOps::VillagerChaseOps(const std::string& stateID, GameObject* go) : State(stateID),
m_go(go)
{

}

VillagerChaseOps::~VillagerChaseOps()
{
}

void VillagerChaseOps::Enter()
{
	m_go->moveSpeed = HUNGRY_SPEED;
	m_go->Stationary = false;
	m_go->nearest = NULL;
	m_elapsed = MESSAGE_INTERVAL;
}

void VillagerChaseOps::Update(double dt)
{
	std::cout << "ChaseOps" << std::endl;
	m_elapsed += static_cast<float>(dt); //check against this value before sending message(so we don't send the message every frame)

	m_go->Stationary = false;
	m_go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt);

	const float tolerance = 0.5f;
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;

	if (m_go->energy < 0.f || m_go->hp <= 0)
	{
		m_go->sm->SetNextState("VillagerDead");
	}

	if (m_go->nearestEnemy && m_go->nearestEnemy->active && m_go->nearestEnemy->Fightable)
	{

		// Check x-axis movement
		if (std::abs(m_go->nearestEnemy->pos.x - m_go->pos.x) <= tolerance)
		{
			m_go->moveLeft = false;
			m_go->moveRight = false;
		}
		else if (m_go->nearestEnemy->pos.x > m_go->pos.x)
		{
			m_go->moveLeft = false;
		}
		else
		{
			m_go->moveRight = false;
		}

		// Check y-axis movement
		if (std::abs(m_go->nearestEnemy->pos.y - m_go->pos.y) <= tolerance)
		{
			m_go->moveUp = false;
			m_go->moveDown = false;
		}
		else if (m_go->nearestEnemy->pos.y > m_go->pos.y)
		{
			m_go->moveDown = false;
		}
		else
		{
			m_go->moveUp = false;
		}
	}
	else //go->nearest is nullptr
	{
		if (m_go->hp < m_go->Maxhp) {
			m_go->sm->SetNextState("VillagerGoToHouse");
		}
		else{
			m_go->sm->SetNextState("VillagerFull");
		}
		m_go->nearestEnemy = NULL;
	}
}

void VillagerChaseOps::Exit()
{
}
