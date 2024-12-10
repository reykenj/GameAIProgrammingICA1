#ifndef STATES_TURRET_H
#define STATES_TURRET_H

#include "State.h"
#include "GameObject.h"
class TurretStateStandby : public State
{
	GameObject* m_go;
	float time_Elapsed = 0.0f;
	float reload_Elapsed = 0.0f;
public:
	TurretStateStandby(const std::string& stateID, GameObject* go);
	virtual ~TurretStateStandby();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};


class TurretStateShooting : public State
{
	GameObject* m_go;
	float time_Elapsed = 0.0f;
	float shoot_Elapsed = 0.0f;
	int Shots = 0;
public:
	TurretStateShooting(const std::string& stateID, GameObject* go);
	virtual ~TurretStateShooting();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};

class TurretStateOverheat : public State
{
	GameObject* m_go;
	float time_Elapsed = 0.0f;
	float reload_Elapsed = 0.0f;
public:
	TurretStateOverheat(const std::string& stateID, GameObject* go);
	virtual ~TurretStateOverheat();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};


class TurretStateDead : public State
{
	GameObject* m_go;
	float time_Elapsed = 0.0f;
public:
	TurretStateDead(const std::string& stateID, GameObject* go);
	virtual ~TurretStateDead();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};
#endif