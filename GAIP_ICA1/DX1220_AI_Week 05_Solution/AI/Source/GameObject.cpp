#include "GameObject.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	moveSpeed(1.f),
	energy(10.f),
	sm(NULL),
	nearest(NULL),
	hp(10.f)
{
	static int count = 0;
	id = ++count;
	moveLeft = moveRight = moveUp = moveDown = true;
	Stationary = false;
	Collision = false;
	GridSizeMultiplier = 1.0f;
	WoodCollected = 0;
	FoodEnergyCollected = 0;
	RED = false;
}

GameObject::~GameObject()
{
	if (sm)
	{
		delete sm;
		sm = NULL;
	}
}

//week 4
bool GameObject::Handle(Message* message)
{
	//let's check if message is MessageCheckActive
	if (dynamic_cast<MessageCheckActive*>(message) != nullptr)
		return active;
	else if (dynamic_cast<MessageCheckFish*>(message) != nullptr)
		return active && type == GameObject::GO_FISH;
	else if (dynamic_cast<MessageCheckFood*>(message) != nullptr)
		return active && type == GameObject::GO_FISHFOOD;
	else if (dynamic_cast<MessageCheckShark*>(message) != nullptr)
		return active && type == GameObject::GO_SHARK;

	//note: pardon the inconsistency(when compared to SceneMovement's Handle)
	//we do NOT want to create a new message on the heap PER object for performance reasons
	return false;
}

void GameObject::OnCollision(GameObject* go2, float dt)
{
	if (this->type == GO_COW) {
		if (go2->type == GO_GRASS) {
			nearest = go2;
			sm->SetNextState("CowEating");
		}
	}
	else if (this->type == GO_VILLAGER) {
		if (go2->type == GO_COW) {
			if (sm->GetCurrentState() == "VillagerHungry") {
				go2->hp--;

				if(go2->hp <= 0){
					FoodEnergyCollected += 2.5f;
				}
				//go2->sm->SetNextState("CowEating")
			}
			//nearest = go2;
			//sm->SetNextState("CowEating");
		}
		if (go2->type == GO_TREE) {
			if (sm->GetCurrentState() == "VillagerFull") {
				nearest = go2;
				sm->SetNextState("VillagerCuttingTree");
				//go2->sm->SetNextState("CowEating")
			}
			//nearest = go2;
			//sm->SetNextState("CowEating");
		}
		if (go2->type == GO_HOUSE) {
			if (sm->GetCurrentState() == "VillagerGoToHouse") {
				sm->SetNextState("VillagerFull");
				SceneData::GetInstance()->AddWoodCount(WoodCollected, RED);
				SceneData::GetInstance()->AddFoodEnergyCount(FoodEnergyCollected, RED);
				WoodCollected = 0;
				FoodEnergyCollected = 0;
			}

			if ((SceneData::GetInstance()->GetFoodEnergyCount(RED) >= Maxenergy - energy && Hungry) || hp < Maxhp) {
				float RecoverySpeed = 5.0f;
				Stationary = true;
				if (hp < Maxhp) {
					hp += dt * RecoverySpeed;
					std::cout << "HP RECOVER" << std::endl;
				}
				if (SceneData::GetInstance()->GetFoodEnergyCount(RED) >= Maxenergy - energy) {
					SceneData::GetInstance()->AddFoodEnergyCount(-dt * RecoverySpeed, RED);
					energy += dt * RecoverySpeed;
					std::cout << "ENERGY RECOVER" << std::endl;
					if (energy > 8.0f) {
						Hungry = false;
						std::cout << "ENERGY RECOVER111111111" << std::endl;
					}
				}
			}
			else {
				Stationary = false;
			}
		}
	}
	return;
}
