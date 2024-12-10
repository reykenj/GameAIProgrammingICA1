#ifndef STATES_BOMB_H
#define STATES_BOMB_H

#include "State.h"
#include "GameObject.h"

class BombStateWalking : public State
{
	GameObject* m_go;
	float m_elapsed = 0;
	float enemy_elapsed = 0;
public:
	BombStateWalking(const std::string& stateID, GameObject* go);
	virtual ~BombStateWalking();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};


class BombStateSitting : public State
{
	GameObject* m_go;
	float m_elapsed = 0;
	float enemy_elapsed = 0;

public:
	BombStateSitting(const std::string& stateID, GameObject* go);
	virtual ~BombStateSitting();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};



class BombStateRunning : public State
{
	GameObject* m_go;
	float ElaspedTime = 0;
public:
	BombStateRunning(const std::string& stateID, GameObject* go);
	virtual ~BombStateRunning();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};


class BombStateDead : public State
{
	GameObject* m_go;
	float ElaspedTime = 0;
public:
	BombStateDead(const std::string& stateID, GameObject* go);
	virtual ~BombStateDead();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};



#endif