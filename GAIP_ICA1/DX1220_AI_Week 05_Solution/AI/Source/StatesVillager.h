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

	float enemy_elasped;
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

	float enemy_elasped;

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

class VillagerStateCutTree : public State
{
	float enemy_elasped;

	GameObject* m_go;
	float message_elapsed;
	float m_elapsed;
public:
	VillagerStateCutTree(const std::string& stateID, GameObject* go);
	virtual ~VillagerStateCutTree();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};


class VillagerStateGoToHouse : public State
{
	GameObject* m_go;
	float message_elapsed;
	float m_elapsed;
public:
	VillagerStateGoToHouse(const std::string& stateID, GameObject* go);
	virtual ~VillagerStateGoToHouse();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};

class VillagerChaseOps : public State
{
	GameObject* m_go;
	float message_elapsed;
	float m_elapsed;
public:
	VillagerChaseOps(const std::string& stateID, GameObject* go);
	virtual ~VillagerChaseOps();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};


class VillagerCharge : public State
{
	GameObject* m_go;
	float message_elapsed;
	float m_elapsed;
	float enemy_elasped;
public:
	VillagerCharge(const std::string& stateID, GameObject* go);
	virtual ~VillagerCharge();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};




class VillagerGoToPosition : public State
{
	GameObject* m_go;
	float message_elapsed;
	float m_elapsed;
	bool Close = false;
	float enemy_elasped;
public:
	VillagerGoToPosition(const std::string& stateID, GameObject* go);
	virtual ~VillagerGoToPosition();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};


#endif