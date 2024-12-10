#ifndef STATES_SUMMONALTAR_H
#define STATES_SUMMONALTAR_H

#include "State.h"
#include "GameObject.h"

class SummonAltarStateSpawner : public State
{
	GameObject* m_go;
	float ElaspedTime = 0;
	float MaxEliteSpawnerTime = 10.0f;
public:
	SummonAltarStateSpawner(const std::string& stateID, GameObject* go);
	virtual ~SummonAltarStateSpawner();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};


class SummonAltarStateDead : public State
{
	GameObject* m_go;
	float ElaspedTime = 0;
	float MaxEliteDeadTime = 10.0f;
public:
	SummonAltarStateDead(const std::string& stateID, GameObject* go);
	virtual ~SummonAltarStateDead();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};

#endif