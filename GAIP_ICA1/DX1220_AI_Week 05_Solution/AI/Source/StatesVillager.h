#ifndef STATES_FISH_H
#define STATES_FISH_H

#include "State.h"
#include "GameObject.h"

class VillagerStateTooFull : public State
{
	GameObject* m_go;
public:
	VillagerStateTooFull(const std::string& stateID, GameObject* go);
	virtual ~VillagerStateTooFull();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};

class VillagerStateFull : public State
{
	GameObject* m_go;
	float m_elapsed;
public:
	VillagerStateFull(const std::string& stateID, GameObject* go);
	virtual ~VillagerStateFull();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};

class VillagerStateHungry : public State
{
	GameObject* m_go;
	float m_elapsed;

public:
	VillagerStateHungry(const std::string& stateID, GameObject* go);
	virtual ~VillagerStateHungry();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};

class VillagerStateDead : public State
{
	GameObject* m_go;
public:
	VillagerStateDead(const std::string& stateID, GameObject* go);
	virtual ~VillagerStateDead();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};

#endif