#ifndef CONCRETE_MESSAGE_H
#define CONCRETE_MESSAGE_H

#include "Message.h"
#include "GameObject.h"

struct MessageWRU : public Message
{
	enum SEARCH_TYPE
	{
		SEARCH_NONE = 0,
		NEAREST_SHARK,
		NEAREST_FISHFOOD,
		NEAREST_FULLFISH,
		HIGHEST_ENERGYFISH,
		NEAREST_GRASS,
		NEAREST_TREE_VILLAGER,
		NEAREST_VILLAGER,
		NEAREST_OPS,
		NEAREST_HOUSE_VILLAGER,
		SEND_ALLY_VILLAGERS_TO_POSITION,
		NEAREST_HOUSE_OPS,
		HIT_ALL_OPS_WITHIN_RANGE,
		RALLY_ALL_VILLAGERS,
		NEAREST_COW,
		SPAWN_HOUSE,
		SPAWN_TURRET,
		SPAWN_SUMMONALTAR,
		SPAWN_VILLAGER,
		SPAWN_BOMB,
		SPAWN_JUGGERNAUT
	};
	MessageWRU(GameObject *goValue, SEARCH_TYPE typeValue, float thresholdValue) : go(goValue), type(typeValue), threshold(thresholdValue) {}
	virtual ~MessageWRU() {}

	GameObject *go;
	SEARCH_TYPE type;
	float threshold;
};

struct MessageCheckActive : public Message
{
	MessageCheckActive() {}
};

struct MessageCheckFish : public Message
{
	MessageCheckFish() {}
};

struct MessageCheckFood : public Message
{
	MessageCheckFood() {}
};

struct MessageCheckShark : public Message
{
	MessageCheckShark() {}
};

struct MessageHurtEntity : public Message
{
	float damage;
	MessageHurtEntity(float Damage) {
		damage = Damage;
	}

	//void testfunc() {

	//}
};


struct MessageCheckTeamAmt : public Message
{
	bool RED;
	MessageCheckTeamAmt(bool red) {
		RED = red;
	}

	//void testfunc() {

	//}
};

#endif
