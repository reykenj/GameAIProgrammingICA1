#include "GameObject.h"
#include "ConcreteMessages.h"
#include "SceneData.h"
#include "PostOffice.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	moveSpeed(1.f),
	energy(10.f),
	sm(NULL),
	nearest(NULL),
	nearestEnemy(NULL),
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
	Shots = 0;
	RED = false;
	BuildingDestroyer = false;
	Building = false;
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
	//MessageCheckTeamAmt
	//let's check if message is MessageCheckActive
	if (dynamic_cast<MessageCheckActive*>(message) != nullptr)
		return active;
	else if (dynamic_cast<MessageCheckFish*>(message) != nullptr)
		return active && type == GameObject::GO_FISH;
	else if (dynamic_cast<MessageCheckFood*>(message) != nullptr)
		return active && type == GameObject::GO_FISHFOOD;
	else if (dynamic_cast<MessageCheckShark*>(message) != nullptr)
		return active && type == GameObject::GO_SHARK;
	else if (MessageCheckTeamAmt* TeamCheckMessage = dynamic_cast<MessageCheckTeamAmt*>(message))
	{
		return active && type >= GameObject::GO_VILLAGER && type < GameObject::GO_BLACK && TeamCheckMessage->RED == RED;
	}
	else if (MessageHurtEntity* hurtMessage = dynamic_cast<MessageHurtEntity*>(message))
	{
		std::cout << "Damage: " << std::endl;
		std::cout << hurtMessage->damage << std::endl;
		hp -= hurtMessage->damage;
		return true;
	}

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
			if (RED == go2->RED && go2->hp > 0) {
				Fightable = false;
				if (sm->GetCurrentState() == "VillagerGoToHouse") {
					sm->SetNextState("VillagerFull");
					SceneData::GetInstance()->AddWoodCount(WoodCollected, RED);
					SceneData::GetInstance()->AddFoodEnergyCount(FoodEnergyCollected, RED);

					if (SceneData::GetInstance()->GetWoodCount(RED) > 5.0f) {
						SceneData::GetInstance()->AddWoodCount(-5.0f, RED);
						SceneData::BUILDING_TYPE BT = SceneData::GetInstance()->GetBuildingType(RED);
						if (BT == SceneData::BT_HOUSE) {
							PostOffice::GetInstance()->Send("Scene", new MessageWRU(go2, MessageWRU::SPAWN_HOUSE, 1.0f));
						}
						else if(BT == SceneData::BT_TURRET){
							PostOffice::GetInstance()->Send("Scene", new MessageWRU(go2, MessageWRU::SPAWN_TURRET, 1.0f));
						}
						else if (BT == SceneData::BT_SUMMONALTAR) {
							PostOffice::GetInstance()->Send("Scene", new MessageWRU(go2, MessageWRU::SPAWN_SUMMONALTAR, 1.0f));
						}
						SceneData::GetInstance()->SetBuildingType(SceneData::GetInstance()->RollRandomBuildingType(), RED);
					}
					WoodCollected = 0;
					FoodEnergyCollected = 0;
				}

				if ((SceneData::GetInstance()->GetFoodEnergyCount(RED) >= Maxenergy - energy && Hungry) || hp < Maxhp) {
					float RecoverySpeed = 5.0f;
					Stationary = true;
					if (hp < Maxhp) {
						hp += dt * RecoverySpeed;
						//std::cout << "HP RECOVER" << std::endl;
					}
					if (SceneData::GetInstance()->GetFoodEnergyCount(RED) >= Maxenergy - energy && Hungry) {
						SceneData::GetInstance()->AddFoodEnergyCount(-dt * RecoverySpeed, RED);
						energy += dt * RecoverySpeed;
						//std::cout << "ENERGY RECOVER" << std::endl;
						if (energy > 8.0f) {
							Hungry = false;
							//std::cout << "ENERGY RECOVER111111111" << std::endl;
						}
					}
				}
				else {
					Stationary = false;
				}
			}
		}


		if ((hp > 0 && go2->hp > 0) && go2->type == GO_VILLAGER && RED != go2->RED) {
			if (sm->GetCurrentState() == "VillagerChaseOps") {
				go2->hp--;
				Stationary = true;
				if (go2->hp <= 0) {
					WoodCollected += 3.5f + go2->WoodCollected;
					FoodEnergyCollected += 3.5f + go2->FoodEnergyCollected;
					//energy += 1.5f;
				}
				//go2->sm->SetNextState("CowEating")
			}
			//nearest = go2;
			//sm->SetNextState("CowEating");
		}
	}
	else if (type == GO_BOMB) {
		if (this != go2 && go2->type >= GO_VILLAGER && go2->type < GO_BLACK && hp > 0 && RED != go2->RED) {
			hp = 0;
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(this, MessageWRU::HIT_ALL_OPS_WITHIN_RANGE, 2.0f * SceneData::GetInstance()->GetGridSize()));
		}
	}
	else if (this->type >= GO_HOUSE && go2->type < GO_BLACK) {
		if (this != go2 && go2->type >= GO_HOUSE && go2->type < GO_BLACK) {
			Vector3 SpawnPosition;
			float gridSize = SceneData::GetInstance()->GetGridSize();
			float gridOffset = SceneData::GetInstance()->GetGridOffset();
			int numGrid = SceneData::GetInstance()->GetNumGrid();
			bool SpawnLeft;
			bool SpawnRight;
			bool SpawnTop;
			bool SpawnBottom;

			SpawnLeft = SpawnRight = SpawnTop = SpawnBottom = false;
			Vector3 temp = pos + Vector3(gridSize, 0, 0);
			if (SceneData::GetInstance()->CheckWithinGrid(temp.x, temp.y, temp.z)) {
				SpawnRight = true;
			}
			temp = pos + Vector3(-gridSize, 0, 0);
			if (SceneData::GetInstance()->CheckWithinGrid(temp.x, temp.y, temp.z)) {
				SpawnLeft = true;
			}
			temp = pos + Vector3(0, gridSize, 0);
			if (SceneData::GetInstance()->CheckWithinGrid(temp.x, temp.y, temp.z)) {
				SpawnTop = true;
			}
			temp = pos + Vector3(0, -gridSize, 0);
			if (SceneData::GetInstance()->CheckWithinGrid(temp.x, temp.y, temp.z)) {
				SpawnBottom = true;
			}


			float random = Math::RandFloatMinMax(0.f, 1.f);
			if (random < 0.25f && SpawnRight)
				SpawnPosition = pos + Vector3(gridSize, 0, 0);
			else if (random < 0.5f && SpawnLeft)
				SpawnPosition = pos + Vector3(-gridSize, 0, 0);
			else if (random < 0.75f && SpawnTop)
				SpawnPosition = pos + Vector3(0, gridSize, 0);
			else if (SpawnBottom)
				SpawnPosition = pos + Vector3(0, -gridSize, 0);
			go2->pos = SpawnPosition;
		}
	}
	return;
}
