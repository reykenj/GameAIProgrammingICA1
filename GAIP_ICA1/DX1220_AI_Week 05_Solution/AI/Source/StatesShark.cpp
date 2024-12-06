#include "StatesShark.h"
#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float CRAZY_SPEED = 16.f;
static const float NAUGHTY_SPEED = 12.f;
static const float HAPPY_SPEED = 8.f;

StateCrazy::StateCrazy(const std::string & stateID, GameObject * go)
	: State(stateID),
	  m_go(go),
	  m_elapsed(MESSAGE_INTERVAL)
{
}

StateCrazy::~StateCrazy()
{
}

void StateCrazy::Enter()
{
	m_go->moveSpeed = CRAZY_SPEED;
	m_go->nearest = NULL;
	m_elapsed = MESSAGE_INTERVAL;
}

void StateCrazy::Update(double dt)
{
	if (SceneData::GetInstance()->GetFishCount() < 12)
		m_go->sm->SetNextState("Naughty");
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	if (m_go->nearest)
	{
		if (m_go->nearest->pos.x > m_go->pos.x)
			m_go->moveLeft = false;
		else
			m_go->moveRight = false;
		if (m_go->nearest->pos.y > m_go->pos.y)
			m_go->moveDown = false;
		else
			m_go->moveUp = false;
	}
	else //nearest is nullptr
	{
		if (m_elapsed >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			m_elapsed -= MESSAGE_INTERVAL;

			//week 4
			//send message to Scene requesting for nearest to be updated
			//message is allocated on the heap (WARNING: expensive. 
			//either refactor PostOffice to not assume heap-allocated messages,
			//or pool messages to avoid real-time heap allocation)
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::HIGHEST_ENERGYFISH, 0)); //no need for threshold here
		}
	}
}

void StateCrazy::Exit()
{
}

StateNaughty::StateNaughty(const std::string & stateID, GameObject * go)
	: State(stateID),
	  m_go(go),
	  m_elapsed(MESSAGE_INTERVAL)
{
}

StateNaughty::~StateNaughty()
{
}

void StateNaughty::Enter()
{
	m_go->moveSpeed = NAUGHTY_SPEED;
	m_go->nearest = NULL;
	m_elapsed = MESSAGE_INTERVAL;
}

void StateNaughty::Update(double dt)
{
	m_elapsed += static_cast<float>(dt);

	if (SceneData::GetInstance()->GetFishCount() > 10)
		m_go->sm->SetNextState("Crazy");
	else if(SceneData::GetInstance()->GetFishCount() < 6)
		m_go->sm->SetNextState("Happy");
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	if (m_go->nearest)
	{
		if (m_go->nearest->pos.x > m_go->pos.x)
			m_go->moveLeft = false;
		else
			m_go->moveRight = false;
		if (m_go->nearest->pos.y > m_go->pos.y)
			m_go->moveDown = false;
		else
			m_go->moveUp = false;
	}
	else //nearest is nullptr
	{
		if (m_elapsed >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			m_elapsed -= MESSAGE_INTERVAL;

			//week 4
			//send message to Scene requesting for nearest to be updated
			//message is allocated on the heap (WARNING: expensive. 
			//either refactor PostOffice to not assume heap-allocated messages,
			//or pool messages to avoid real-time heap allocation)
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_FULLFISH, 0)); //no need for threshold here
		}
	}
}

void StateNaughty::Exit()
{
}

StateHappy::StateHappy(const std::string & stateID, GameObject * go)
	: State(stateID),
	m_go(go)
{
}

StateHappy::~StateHappy()
{
}

void StateHappy::Enter()
{
	m_go->moveSpeed = HAPPY_SPEED;
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
}

void StateHappy::Update(double dt)
{
	if (SceneData::GetInstance()->GetFishCount() > 4)
		m_go->sm->SetNextState("Naughty");
}

void StateHappy::Exit()
{
}
