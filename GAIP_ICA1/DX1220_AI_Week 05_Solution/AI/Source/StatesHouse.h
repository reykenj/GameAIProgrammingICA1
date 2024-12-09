#ifndef STATES_HOUSE_H
#define STATES_HOUSE_H

#include "State.h"
#include "GameObject.h"

class HouseStateSpawner : public State
{
	GameObject* m_go;
	float ElaspedTime = 0;
	float MaxVillagerSpawnerTime = 10.0f;
public:
	HouseStateSpawner(const std::string& stateID, GameObject* go);
	virtual ~HouseStateSpawner();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};

#endif