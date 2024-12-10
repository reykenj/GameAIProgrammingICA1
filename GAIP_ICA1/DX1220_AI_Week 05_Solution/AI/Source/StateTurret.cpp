#include "StateTurret.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float ENERGY_DROP_RATE = 0.1f;
static const float FULL_SPEED = 8.f;
static const float HUNGRY_SPEED = 6.f;
static const float EnemyDetectionDist = 5.0f;
static const float ReloadTimeMax = 1.0f;
static const float Damage = 4.0f;
static const int MaxShots = 5;

TurretStateStandby::TurretStateStandby(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

TurretStateStandby::~TurretStateStandby()
{
}

void TurretStateStandby::Enter()
{
	m_go->moveSpeed = 0;
	reload_Elapsed = 0.0f;
	m_go->nearestEnemy = NULL;
	time_Elapsed = MESSAGE_INTERVAL;
}

void TurretStateStandby::Update(double dt)
{
	time_Elapsed += static_cast<float>(dt);
	if (m_go->Shots > 0) {
		reload_Elapsed += static_cast<float>(dt);
	}
	if (m_go->nearestEnemy && m_go->nearestEnemy->active) {
		//std::cout << "RAAAH" << std::endl;
		m_go->sm->SetNextState("TurretShooting");
	}


	if (m_go->hp <= 0.0f)
	{
		m_go->sm->SetNextState("TurretDead");
	}

	if (reload_Elapsed >= ReloadTimeMax) //ensure at least 1 second interval between messages
	{
		reload_Elapsed -= ReloadTimeMax;
		m_go->Shots--;
	}
	if (time_Elapsed >= MESSAGE_INTERVAL * 2.0f) //ensure at least 1 second interval between messages
	{
		time_Elapsed -= MESSAGE_INTERVAL * 2.0f;
		PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_OPS, EnemyDetectionDist * SceneData::GetInstance()->GetGridSize()));
	}
}




void TurretStateStandby::Exit()
{
}


TurretStateShooting::TurretStateShooting(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

TurretStateShooting::~TurretStateShooting()
{
}

void TurretStateShooting::Enter()
{
	m_go->moveSpeed = 0;
	shoot_Elapsed = 0.0f;
	time_Elapsed = MESSAGE_INTERVAL;

}

void TurretStateShooting::Update(double dt)
{
	//std::cout << "Shooting "<< std::endl;
	//if (m_go->nearestEnemy->type == GameObject::GO_VILLAGER) {
	//	std::cout << "VILLAGER " << std::endl;
	//	//m_go->nearestEnemy->pos = m_go->pos;
	//	std::cout << m_go->nearestEnemy->hp << std::endl;
	//	
	//}
	//else if (m_go->nearestEnemy->type == GameObject::GO_VILLAGER) {
	//	std::cout << "TURRET " << std::endl;
	//}
	//else if (m_go->nearestEnemy == nullptr) {
	//	std::cout << "NOTHING " << std::endl;
	//}

	time_Elapsed += static_cast<float>(dt);
	shoot_Elapsed += static_cast<float>(dt);
	if (m_go->Shots >= 5) {
		m_go->sm->SetNextState("TurretOverheat");
	}
	else if (m_go->nearestEnemy && m_go->nearestEnemy->active && m_go->nearestEnemy->hp > 0.0f) {
		m_go->target = m_go->nearestEnemy->pos;
		if (shoot_Elapsed >= 0.5f) {
			//std::cout << "Trying to Damage " << std::endl;
			shoot_Elapsed -= 0.5f;
			m_go->Shots++;
			MessageHurtEntity msgHurtEntity = MessageHurtEntity(Damage);
			m_go->nearestEnemy->Handle(&msgHurtEntity);
		}
	}
	else {
		m_go->sm->SetNextState("TurretStandby");
	}

	if (m_go->hp <= 0.0f)
	{
		m_go->sm->SetNextState("TurretDead");
	}

	if (time_Elapsed >= MESSAGE_INTERVAL * 2.0f) //ensure at least 1 second interval between messages
	{
		//m_go->nearestEnemy = NULL;
		time_Elapsed -= MESSAGE_INTERVAL * 2.0f;
		PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::NEAREST_OPS, EnemyDetectionDist * SceneData::GetInstance()->GetGridSize()));
	}
}

void TurretStateShooting::Exit()
{
}







TurretStateOverheat::TurretStateOverheat(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

TurretStateOverheat::~TurretStateOverheat()
{
}

void TurretStateOverheat::Enter()
{
	m_go->moveSpeed = 0;
	reload_Elapsed = 0.0f;
	m_go->nearestEnemy = NULL;
	time_Elapsed = MESSAGE_INTERVAL;
}

void TurretStateOverheat::Update(double dt)
{
	if (m_go->hp <= 0.0f)
	{
		m_go->sm->SetNextState("TurretDead");
	}
	if (m_go->Shots > 0) {
		reload_Elapsed += static_cast<float>(dt);

		if (reload_Elapsed >= ReloadTimeMax) //ensure at least 1 second interval between messages
		{
			reload_Elapsed -= ReloadTimeMax;
			m_go->Shots--;
		}
	}
	else {
		m_go->sm->SetNextState("TurretStandby");
	}
}




void TurretStateOverheat::Exit()
{
}




TurretStateDead::TurretStateDead(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

TurretStateDead::~TurretStateDead()
{
}

void TurretStateDead::Enter()
{
	m_go->countDown = 3.f;
	m_go->moveSpeed = 0;
}

void TurretStateDead::Update(double dt)
{
	m_go->countDown -= static_cast<float>(dt);
	if (m_go->countDown < 0)
	{
		m_go->active = false;
	}
}

void TurretStateDead::Exit()
{
}