#include "StatesBomb.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float ENERGY_DROP_RATE = 0.1f;
static const float RUNNING_SPEED = 12.f;
static const float WALKING_SPEED = 8.f;
static const float EnemyDetectionDist = 3.0f;

BombStateWalking::BombStateWalking(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

BombStateWalking::~BombStateWalking()
{
}

void BombStateWalking::Enter()
{
	enemy_elapsed = MESSAGE_INTERVAL;
	m_elapsed = MESSAGE_INTERVAL;
	m_go->nearest = nullptr;
}

void BombStateWalking::Update(double dt)
{
	m_go->moveSpeed = WALKING_SPEED;
	std::cout << "BOMB WALKIMG" << std::endl;
	m_elapsed += static_cast<float>(dt); //check against this value before sending message(so we don't send the message every frame)
	enemy_elapsed += static_cast<float>(dt);
	if (m_go->hp <= 0)
	{
		m_go->sm->SetNextState("BombDead");
	}

	if (m_go->nearestEnemy && m_go->nearestEnemy->active) {
		//std::cout << "RAAAH" << std::endl;
		m_go->sm->SetNextState("BombRunning");
	}

	const float tolerance = 0.5f;
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	if (m_go->nearest && m_go->nearest->active)
	{
		if (m_elapsed >= MESSAGE_INTERVAL / 2) {
			m_elapsed -= MESSAGE_INTERVAL / 2;
			
			const float StopDistance = 5.0f * SceneData::GetInstance()->GetGridSize();
			float distance = (m_go->pos - m_go->nearest->pos).Length();
			if (distance <= StopDistance)
			{
				m_go->sm->SetNextState("BombSitting");
			}
		}
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
			const float HOUSE_DIST = 100.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_HOUSE_OPS, HOUSE_DIST));
		}
	}
	if (enemy_elapsed >= MESSAGE_INTERVAL * 2.0f) //ensure at least 1 second interval between messages
	{
		enemy_elapsed -= MESSAGE_INTERVAL * 2.0f;
		PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_OPS, EnemyDetectionDist * SceneData::GetInstance()->GetGridSize()));
	}
}

void BombStateWalking::Exit()
{
}



BombStateSitting::BombStateSitting(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

BombStateSitting::~BombStateSitting()
{
}

void BombStateSitting::Enter()
{
	enemy_elapsed = MESSAGE_INTERVAL;
	m_elapsed = 0;
	m_go->moveSpeed = 0;
	m_go->Stationary = true;
	//m_go->nearest = nullptr;
}

void BombStateSitting::Update(double dt)
{
	const float BombHouseExplodeWaitingTime = 2.5f;
	m_elapsed += static_cast<float>(dt); //check against this value before sending message(so we don't send the message every frame)
	enemy_elapsed += static_cast<float>(dt);
	if (m_go->hp <= 0)
	{
		m_go->sm->SetNextState("BombDead");
	}
	else if (m_go->nearestEnemy && m_go->nearestEnemy->active) {
		//std::cout << "RAAAH" << std::endl;
		m_go->sm->SetNextState("BombRunning");
	}

	if (m_elapsed >= BombHouseExplodeWaitingTime * 1.0f && m_go->nearest && m_go->nearest->active)
	{
		std::cout << "Exploding Houses" << std::endl;
		m_go->nearestEnemy = m_go->nearest;
		
		m_go->sm->SetNextState("BombRunning");
	}

	if (enemy_elapsed >= MESSAGE_INTERVAL * 2.0f) //ensure at least 1 second interval between messages
	{
		enemy_elapsed -= MESSAGE_INTERVAL * 2.0f;
		PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_OPS, EnemyDetectionDist * SceneData::GetInstance()->GetGridSize()));
	}
}

void BombStateSitting::Exit()
{
}









BombStateRunning::BombStateRunning(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

BombStateRunning::~BombStateRunning()
{
}

void BombStateRunning::Enter()
{
	m_go->moveSpeed = RUNNING_SPEED;
	m_go->Stationary = false;
}

void BombStateRunning::Update(double dt)
{
	if (m_go->hp <= 0)
	{
		m_go->sm->SetNextState("BombDead");
	}
	else {

		const float tolerance = 0.5f;
		m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
		if (m_go->nearestEnemy && m_go->nearestEnemy->active)
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
	}
}

void BombStateRunning::Exit()
{
}








BombStateDead::BombStateDead(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

BombStateDead::~BombStateDead()
{
}

void BombStateDead::Enter()
{
	m_go->countDown = 3.f;
	m_go->moveSpeed = 0;
}

void BombStateDead::Update(double dt)
{
	m_go->countDown -= static_cast<float>(dt);
	if (m_go->countDown < 0)
	{
		m_go->active = false;
	}
}

void BombStateDead::Exit()
{
}