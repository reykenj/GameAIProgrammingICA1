#include "StatesCow.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float ENERGY_DROP_RATE = 0.2f;
static const float FULL_SPEED = 8.f;
static const float HUNGRY_SPEED = 4.f;

CowStateTooFull::CowStateTooFull(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

CowStateTooFull::~CowStateTooFull()
{
}

void CowStateTooFull::Enter()
{
	m_go->moveSpeed = 0;
}

void CowStateTooFull::Update(double dt)
{
	m_go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt);
	if (m_go->energy < 10.f)
		m_go->sm->SetNextState("CowFull");

	if (m_go->hp <= 0.0f)
	{
		m_go->sm->SetNextState("CowDead");
	}
}

void CowStateTooFull::Exit()
{
}



CowStateFull::CowStateFull(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go),
	m_elapsed(MESSAGE_INTERVAL)
{
}

CowStateFull::~CowStateFull()
{
}

void CowStateFull::Enter()
{
	m_go->moveSpeed = FULL_SPEED;
	m_go->nearest = NULL;
	m_elapsed = MESSAGE_INTERVAL;
}

void CowStateFull::Update(double dt)
{
	m_elapsed += static_cast<float>(dt);

	m_go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt);
	if (m_go->energy >= 10.f)
		m_go->sm->SetNextState("CowTooFull");
	else if (m_go->energy < 5.f)
		m_go->sm->SetNextState("CowHungry");


	if (m_go->hp <= 0.0f)
	{
		m_go->sm->SetNextState("CowDead");
	}
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;

	//once nearest is set, fish will continue to move away from shark even
	//when they have move significantly far away (until it changes state).
	//TODO: consider setting nearest to nullptr if shark is "far enough"
	if (m_go->nearest)
	{
		if (m_go->nearest->pos.x > m_go->pos.x)
			m_go->moveRight = false;
		else
			m_go->moveLeft = false;
		if (m_go->nearest->pos.y > m_go->pos.y)
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
			const float VILLAGER_DIST = 10.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene",
				new MessageWRU(m_go, MessageWRU::NEAREST_VILLAGER, VILLAGER_DIST));
		}
	}
}

void CowStateFull::Exit()
{
}


CowStateHungry::CowStateHungry(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go),
	m_elapsed(MESSAGE_INTERVAL)
{
}

CowStateHungry::~CowStateHungry()
{
}

void CowStateHungry::Enter()
{
	m_go->moveSpeed = HUNGRY_SPEED;
	m_go->nearest = NULL;
	m_elapsed = MESSAGE_INTERVAL;
}

void CowStateHungry::Update(double dt)
{
	m_elapsed += static_cast<float>(dt); //check against this value before sending message(so we don't send the message every frame)

	m_go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt);
	if (m_go->energy >= 5.f)
		m_go->sm->SetNextState("CowFull");
	else if (m_go->energy < 0.f || m_go->hp <= 0.0f)
	{
		m_go->sm->SetNextState("CowDead");
	}
	const float tolerance = 0.5f;
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	if (m_go->nearest)
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
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_GRASS, FOOD_DIST));
		}
	}
}

void CowStateHungry::Exit()
{
}


CowStateDead::CowStateDead(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

CowStateDead::~CowStateDead()
{
}

void CowStateDead::Enter()
{
	m_go->countDown = 3.f;
	m_go->moveSpeed = 0;
}

void CowStateDead::Update(double dt)
{
	m_go->countDown -= static_cast<float>(dt);
	if (m_go->countDown < 0)
	{
		m_go->active = false;
	}
}

void CowStateDead::Exit()
{
}


CowStateEating::CowStateEating(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

CowStateEating::~CowStateEating()
{
}

void CowStateEating::Enter()
{
	m_go->moveSpeed = 0;
	m_go->Stationary = true;
	m_elapsed = 3;
	message_elapsed = MESSAGE_INTERVAL;
}

void CowStateEating::Update(double dt)
{
	message_elapsed += static_cast<float>(dt); //check against this value before sending message(so we don't send the message every frame)

	//m_go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt);
	if (m_go->energy < 0.f || m_go->hp <= 0.0f)
	{
		m_go->sm->SetNextState("CowDead");
	}

	if (m_go->nearest && m_go->nearest->type == GameObject::GO_GRASS)
	{
		m_elapsed -= static_cast<float>(dt);
		if (m_elapsed < 0)
		{
			m_go->nearest->active = false;
		}
		if (m_go->nearest->active == false) {
			if (m_go->energy >= 5.f)
				m_go->sm->SetNextState("CowFull");
			else {
				m_go->sm->SetNextState("CowHungry");
			}
		}
		else {
			m_go->energy += ENERGY_DROP_RATE * static_cast<float>(dt) * 5;
		}
	}
	else {
		if (m_elapsed >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			m_elapsed -= MESSAGE_INTERVAL;
			const float FOOD_DIST = 20.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_GRASS, FOOD_DIST));
		}
	}

}

void CowStateEating::Exit()
{
	m_go->Stationary = false;
	//m_go->moveSpeed = HUNGRY_SPEED;
	//m_go->moveSpeed = HUNGRY_SPEED;
}
