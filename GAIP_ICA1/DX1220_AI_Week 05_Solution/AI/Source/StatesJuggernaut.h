#ifndef STATES_JUGGERNAUT_H
#define STATES_JUGGERNAUT_H

#include "State.h"
#include "GameObject.h"

class JuggernautStateRally : public State
{
	GameObject* m_go;
	float m_elapsed;

	float enemy_elasped;
public:
	JuggernautStateRally(const std::string& stateID, GameObject* go);
	virtual ~JuggernautStateRally();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};


class JuggernautStateRushAllies : public State
{
	GameObject* m_go;
	float m_elapsed;

	float enemy_elasped;
public:
	JuggernautStateRushAllies(const std::string& stateID, GameObject* go);
	virtual ~JuggernautStateRushAllies();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


class JuggernautStateAttacking : public State
{
	GameObject* m_go;
	float m_elapsed;

	float attack_elasped;
public:
	JuggernautStateAttacking(const std::string& stateID, GameObject* go);
	virtual ~JuggernautStateAttacking();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


class JuggernautStateDead : public State
{
	GameObject* m_go;
	float m_elapsed;

	float attack_elasped;
public:
	JuggernautStateDead(const std::string& stateID, GameObject* go);
	virtual ~JuggernautStateDead();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif