#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "StateMachine.h"
#include "ObjectBase.h"

struct GameObject : public ObjectBase
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CROSS,
		GO_CIRCLE,
		GO_FISH,
		GO_SHARK,
		GO_FISHFOOD,
		GO_VILLAGER,
		GO_GRASS,
		GO_TREE,
		GO_HOUSE,
		GO_COW,
		GO_BLACK,
		GO_WHITE,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active;
	float mass;
	Vector3 target;
	int id;
	int steps;
	float energy;

	float Maxenergy;

	float WoodCollected;
	float FoodEnergyCollected;


	float hp;
	float Maxhp;
	float moveSpeed;
	float countDown;
	GameObject *nearest;
	GameObject *nearestEnemy;
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;
	StateMachine *sm;
	bool Stationary;
	bool Collision;
	float GridSizeMultiplier;

	bool RED;

	bool Hungry = false;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~GameObject();

	bool Handle(Message* message);
	void OnCollision(GameObject *go2, float dt);
};

#endif