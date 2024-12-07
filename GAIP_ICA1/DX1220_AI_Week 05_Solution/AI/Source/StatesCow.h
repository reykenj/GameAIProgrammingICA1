#ifndef STATES_COW_H
#define STATES_COW_H

#include "State.h"
#include "GameObject.h"
#include <vector>

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

class CowStateEating : public State
{
	GameObject* m_go;
	//std::vector<GameObject*> m_gos;
	GameObject* m_go2;
	float message_elapsed;
	float m_elapsed;

public:
	CowStateEating(const std::string& stateID, GameObject* go);
	virtual ~CowStateEating();

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