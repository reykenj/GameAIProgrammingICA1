#include "StatesJuggernaut.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float FULL_SPEED = 8.f;
static const float EnemyDetectionDist = 3.0f;


JuggernautStateRally::JuggernautStateRally(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go),
	m_elapsed(MESSAGE_INTERVAL)
{
}

JuggernautStateRally::~JuggernautStateRally()
{
}

void JuggernautStateRally::Enter()
{
	m_go->moveSpeed = 8.0f;
	m_go->nearest = nullptr;
	m_elapsed = MESSAGE_INTERVAL;
}

void JuggernautStateRally::Update(double dt)
{
	m_go->moveSpeed = 8.0f;
	std::cout << "RALLY" << std::endl;
	std::cout << m_go->moveSpeed << std::endl;
	m_elapsed += static_cast<float>(dt); //check against this value before sending message(so we don't send the message every frame)

	const float tolerance = 0.5f;
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	if (m_go->nearest && m_go->nearest->active)
	{
		std::cout << "RALLY2" << std::endl;
		if (m_elapsed >= MESSAGE_INTERVAL / 2) {
			m_elapsed -= MESSAGE_INTERVAL / 2;
			const float StopDistance = 5.0f * SceneData::GetInstance()->GetGridSize();
			float distance = (m_go->pos - m_go->nearest->pos).Length();
			if (distance <= StopDistance)
			{
				m_go->sm->SetNextState("JuggernautStateRushAllies");
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
			std::cout << "Trying" << std::endl;
			m_elapsed -= MESSAGE_INTERVAL;
			const float HOUSE_DIST = 100.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::RALLY_ALL_VILLAGERS, 0.0f));
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_HOUSE_OPS, HOUSE_DIST));
		}
	}
}

void JuggernautStateRally::Exit()
{
}


JuggernautStateRushAllies::JuggernautStateRushAllies(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

JuggernautStateRushAllies::~JuggernautStateRushAllies()
{
}

void JuggernautStateRushAllies::Enter()
{
	m_elapsed = 0;
	m_go->moveSpeed = 0;
	m_go->Stationary = true;
	const float HOUSE_DIST = 100.f * SceneData::GetInstance()->GetGridSize();
	PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_HOUSE_OPS, HOUSE_DIST));
	//m_go->nearest = nullptr;
}

void JuggernautStateRushAllies::Update(double dt)
{
	const float JuggernautAttackWaitingTime = 2.5f;
	m_elapsed += static_cast<float>(dt); //check against this value before sending message(so we don't send the message every frame)
	if (m_go->hp <= 0)
	{
		m_go->sm->SetNextState("JuggernautStateDead");
	}
	else if (m_elapsed >= JuggernautAttackWaitingTime * 1.0f)
	{
		m_go->sm->SetNextState("JuggernautStateAttacking");
	}
}

void JuggernautStateRushAllies::Exit()
{
}



JuggernautStateAttacking::JuggernautStateAttacking(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

JuggernautStateAttacking::~JuggernautStateAttacking()
{
}

void JuggernautStateAttacking::Enter()
{
	m_go->moveSpeed = FULL_SPEED;
	m_go->Stationary = false;
	attack_elasped = 0.0f;
	m_elapsed = MESSAGE_INTERVAL;
	PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::RALLY_ALL_VILLAGERS, 0.0f));
}

void JuggernautStateAttacking::Update(double dt)
{
	std::cout << "ATTACKING" << std::endl;
	if (m_go->hp <= 0)
	{
		m_go->sm->SetNextState("JuggernautStateDead");
	}
	else {
		m_elapsed += static_cast<float>(dt);
		attack_elasped += static_cast<float>(dt);
		const float tolerance = 0.5f;
		m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
		if (m_go->nearest && m_go->nearest->active)
		{

			if (m_go->nearest->hp > 0.0f) {
				m_go->target = m_go->nearest->pos;
				if (attack_elasped >= 0.5f) {
					attack_elasped -= 0.5f;
					float distance = (m_go->pos - m_go->nearest->pos).Length();
					if (distance < 2.0f)
					{
						MessageHurtEntity msgHurtEntity = MessageHurtEntity(10.0f);
						m_go->nearest->Handle(&msgHurtEntity);
					}
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
		else {
			if (m_elapsed >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
			{
				m_elapsed -= MESSAGE_INTERVAL;
				const float HOUSE_DIST = 100.f * SceneData::GetInstance()->GetGridSize();
				PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_HOUSE_OPS, HOUSE_DIST));
			}
		}
	}
}

void JuggernautStateAttacking::Exit()
{
}



JuggernautStateDead::JuggernautStateDead(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

JuggernautStateDead::~JuggernautStateDead()
{
}

void JuggernautStateDead::Enter()
{
	m_go->countDown = 3.f;
	m_go->moveSpeed = 0;
}

void JuggernautStateDead::Update(double dt)
{
	m_go->countDown -= static_cast<float>(dt);
	if (m_go->countDown < 0)
	{
		m_go->active = false;
	}
}

void JuggernautStateDead::Exit()
{
}