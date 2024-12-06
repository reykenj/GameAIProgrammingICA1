#ifndef STATES_FISH_H
#define STATES_FISH_H

#include "State.h"
#include "GameObject.h"

class CowStateTooFull : public State
{
	GameObject* m_go;
public:
	CowStateTooFull(const std::string& stateID, GameObject* go);
	virtual ~CowStateTooFull();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class CowStateFull : public State
{
	GameObject* m_go;
	float m_elapsed;
public:
	CowStateFull(const std::string& stateID, GameObject* go);
	virtual ~CowStateFull();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class CowStateHungry : public State
{
	GameObject* m_go;
	float m_elapsed;

public:
	CowStateHungry(const std::string& stateID, GameObject* go);
	virtual ~CowStateHungry();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class CowStateDead : public State
{
	GameObject* m_go;
public:
	CowStateDead(const std::string& stateID, GameObject* go);
	virtual ~CowStateDead();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif