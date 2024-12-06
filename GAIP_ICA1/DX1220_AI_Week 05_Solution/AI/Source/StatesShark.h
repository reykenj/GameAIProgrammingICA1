#ifndef STATES_SHARK_H
#define STATES_SHARK_H

#include "State.h"
#include "GameObject.h"

class StateCrazy : public State
{
	GameObject *m_go;
	float m_elapsed;
public:
	StateCrazy(const std::string &stateID, GameObject *go);
	virtual ~StateCrazy();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};

class StateNaughty : public State
{
	GameObject *m_go;
	float m_elapsed;

public:
	StateNaughty(const std::string &stateID, GameObject *go);
	virtual ~StateNaughty();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};

class StateHappy : public State
{
	GameObject *m_go;
public:
	StateHappy(const std::string &stateID, GameObject *go);
	virtual ~StateHappy();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};

#endif