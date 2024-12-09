#include "SceneICA1.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "StatesVillager.h"
#include "StatesCow.h"
#include "StatesShark.h"
#include "StatesHouse.h"
#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"

SceneICA1::SceneICA1()
{
}

SceneICA1::~SceneICA1()
{
}

void SceneICA1::Init()
{
	SceneBase::Init();

	GrassSpawnMaxTime = 5.0f;
	CowSpawnMaxTime = 10.0f;
	TreeSpawnMaxTime = 10.0f;
	currentTime = 0.0f ;

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	SceneData::GetInstance()->SetObjectCount(0);
	SceneData::GetInstance()->SetVillagerCount(0);
	SceneData::GetInstance()->SetNumGrid(20);
	SceneData::GetInstance()->SetGridSize(m_worldHeight / SceneData::GetInstance()->GetNumGrid());
	SceneData::GetInstance()->SetGridOffset(SceneData::GetInstance()->GetGridSize() * 0.5f);
	m_hourOfTheDay = 0;

	float gridSize = SceneData::GetInstance()->GetGridSize();
	float gridOffset = SceneData::GetInstance()->GetGridOffset();
	int numGrid = SceneData::GetInstance()->GetNumGrid();
	GameObject* go = FetchGO(GameObject::GO_SHARK);
	go->scale.Set(gridSize, gridSize, gridSize);
	go->pos.Set(gridOffset + Math::RandIntMinMax(0, numGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, numGrid - 1) * gridSize, 0);
	go->target = go->pos;

	{
		Vector3 RandomPosition = Vector3(gridOffset + Math::RandIntMinMax(1, numGrid - 2) * gridSize, gridOffset + Math::RandIntMinMax(1, numGrid - 2) * gridSize, 0);
	
		GameObject* house = FetchGO(GameObject::GO_HOUSE);
		house->scale.Set(gridSize, gridSize, gridSize);
		house->pos.Set(RandomPosition.x, RandomPosition.y, 0);
		house->target = go->pos;
		house->steps = 0;
		//house->energy = 8.f;
		house->nearest = NULL;
		house->Stationary = true;
		house->Collision = true;
		//grass->sm->SetNextState("VillagerFull");

		for (int x = -1; x < 2; x += 2) {
			for (int y = -1; y < 2; y += 2) {
				GameObject* go = FetchGO(GameObject::GO_VILLAGER);
				go->scale.Set(gridSize, gridSize, gridSize);
				go->pos.Set(RandomPosition.x + x * gridSize, RandomPosition.y + y * gridSize, 0);
				go->target = go->pos;
				go->steps = 0;
				//go->energy = 8.f;
				go->nearest = NULL;
				go->nearestEnemy = NULL;
				go->Collision = true;
				go->sm->SetNextState("VillagerFull");
			}
		}
	}


	{
		Vector3 RandomPosition = Vector3(gridOffset + Math::RandIntMinMax(1, numGrid - 2) * gridSize, gridOffset + Math::RandIntMinMax(1, numGrid - 2) * gridSize, 0);

		GameObject* house = FetchGO(GameObject::GO_HOUSE);
		house->scale.Set(gridSize, gridSize, gridSize);
		house->pos.Set(RandomPosition.x, RandomPosition.y, 0);
		house->target = go->pos;
		house->steps = 0;
		//house->energy = 8.f;
		house->nearest = NULL;
		house->Stationary = true;
		house->Collision = true;
		house->RED = true;
		//grass->sm->SetNextState("VillagerFull");

		for (int x = -1; x < 2; x += 2) {
			for (int y = -1; y < 2; y += 2) {
				GameObject* go = FetchGO(GameObject::GO_VILLAGER);
				go->scale.Set(gridSize, gridSize, gridSize);
				go->pos.Set(RandomPosition.x + x * gridSize, RandomPosition.y + y * gridSize, 0);
				go->target = go->pos;
				go->steps = 0;
				//go->energy = 8.f;
				go->nearest = NULL;
				go->nearestEnemy = NULL;
				go->Collision = true;
				go->RED = true;
				go->sm->SetNextState("VillagerFull");
			}
		}
	}

	int noGrid = SceneData::GetInstance()->GetNumGrid();

	for (int i = 0; i < 10; i++) {

		GameObject* grass = FetchGO(GameObject::GO_GRASS);
		grass->scale.Set(gridSize, gridSize, gridSize);
		grass->pos.Set(gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, 0);
		grass->target = grass->pos;
		grass->steps = 0;
		grass->moveSpeed = 0;
		grass->Stationary = true;
		grass->GridSizeMultiplier = 1.5f;
		//go->energy = 8.f;
		grass->nearest = NULL;
		//go->sm->SetNextState("Full");

		GameObject* cow = FetchGO(GameObject::GO_COW);
		cow->scale.Set(gridSize, gridSize, gridSize);
		cow->pos.Set(gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, 0);
		cow->target = cow->pos;
		cow->steps = 0;
		cow->moveSpeed = 1.0f;
		//go->energy = 8.f;
		//go->hp = 10.0f;
		cow->nearest = NULL;
		cow->sm->SetNextState("CowFull");
		cow->Collision = true;

		GameObject* tree = FetchGO(GameObject::GO_TREE);
		tree->scale.Set(gridSize, gridSize, gridSize);
		tree->pos.Set(gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, 0);
		tree->target = tree->pos;
		tree->steps = 0;
		tree->moveSpeed = 0.0f;
		tree->Stationary = true;
		tree->GridSizeMultiplier = 1.5f;
		//go->energy = 8.f;
		tree->nearest = NULL;
	}
	


	//week 4
	//register this scene with the "post office"
	//post office will now be capable of addressing this scene with messages
	PostOffice::GetInstance()->Register("Scene", this);
}

GameObject* SceneICA1::FetchGO(GameObject::GAMEOBJECT_TYPE type)
{
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (!go->active && go->type == type)
		{
			go->active = true;
			go->hp = go->Maxhp;
			go->energy = go->Maxenergy;
			go->WoodCollected = 0;
			go->FoodEnergyCollected = 0;
			return go;
		}
	}
	for (unsigned i = 0; i < 5; ++i)
	{
		GameObject* go = new GameObject(type);
		m_goList.push_back(go);
		if (type == GameObject::GO_VILLAGER)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new VillagerStateTooFull("VillagerTooFull", go));
			go->sm->AddState(new VillagerStateFull("VillagerFull", go));
			go->sm->AddState(new VillagerStateHungry("VillagerHungry", go));
			go->sm->AddState(new VillagerStateDead("VillagerDead", go));
			go->sm->AddState(new VillagerStateCutTree("VillagerCuttingTree", go));
			go->sm->AddState(new VillagerStateGoToHouse("VillagerGoToHouse", go));
			go->sm->AddState(new VillagerChaseOps("VillagerChaseOps", go));
			go->Maxenergy = 9.0f;
			go->Maxhp = 10.0f;
		} // Maybe later have states where grass / trees can grow farther if a certain time is reached
		else if (type == GameObject::GO_COW)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new CowStateTooFull("CowTooFull", go));
			go->sm->AddState(new CowStateFull("CowFull", go));
			go->sm->AddState(new CowStateHungry("CowHungry", go));
			go->sm->AddState(new CowStateDead("CowDead", go));
			go->sm->AddState(new CowStateEating("CowEating", go));
			go->Maxenergy = 9.0f;
			go->Maxhp = 10.0f;
		}
		else if (type == GameObject::GO_SHARK)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateCrazy("Crazy", go));
			go->sm->AddState(new StateNaughty("Naughty", go));
			go->sm->AddState(new StateHappy("Happy", go));
			go->Maxenergy = 9.0f;
			go->Maxhp = 10.0f;
		}
		else if (type == GameObject::GO_HOUSE)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new HouseStateSpawner("HouseSpawner", go));
			go->sm->SetNextState("HouseSpawner");
			go->Maxenergy = 9.0f;
			go->Maxhp = 10.0f;
		}
	}
	return FetchGO(type);
}

void SceneICA1::Update(double dt)
{
	SceneBase::Update(dt);

	static const float HOUR_SPEED = 1.f;

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	float gridSize = SceneData::GetInstance()->GetGridSize();
	float gridOffset = SceneData::GetInstance()->GetGridOffset();
	int noGrid = SceneData::GetInstance()->GetNumGrid();

	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_speed += 0.1f;
	}

	m_hourOfTheDay += HOUR_SPEED * static_cast<float>(dt) * m_speed;
	if (m_hourOfTheDay >= 24.f)
		m_hourOfTheDay = 0;


	//if(m_hourOfTheDay < 12.0f)
	if (currentTime <= GrassSpawnMaxTime && currentTime + dt >= GrassSpawnMaxTime) {

		GameObject* go = FetchGO(GameObject::GO_GRASS);
		go->scale.Set(gridSize, gridSize, gridSize);
		go->pos.Set(gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, 0);
		go->target = go->pos;
		go->steps = 0;
		go->moveSpeed = 0;
		go->Stationary = true;
		go->GridSizeMultiplier = 1.5f;
		//go->energy = 8.f;
		go->nearest = NULL;
		//go->sm->SetNextState("Full");
	}

	if (currentTime <= CowSpawnMaxTime && currentTime + dt >= CowSpawnMaxTime) {

		GameObject* go = FetchGO(GameObject::GO_COW);
		go->scale.Set(gridSize, gridSize, gridSize);
		go->pos.Set(gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, 0);
		go->target = go->pos;
		go->steps = 0;
		go->moveSpeed = 1.0f;
		//go->energy = 8.f;
		//go->hp = 10.0f;
		go->nearest = NULL;
		go->sm->SetNextState("CowFull");
		go->Collision = true;
	}

	if (currentTime <= TreeSpawnMaxTime && currentTime + dt >= TreeSpawnMaxTime) {

		GameObject* go = FetchGO(GameObject::GO_TREE);
		go->scale.Set(gridSize, gridSize, gridSize);
		go->pos.Set(gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, 0);
		go->target = go->pos;
		go->steps = 0;
		go->moveSpeed = 0.0f;
		go->Stationary = true;
		go->GridSizeMultiplier = 1.5f;
		//go->energy = 8.f;
		go->nearest = NULL;
		//go->sm->SetNextState("CowFull");
		currentTime = 0;
	}

	currentTime += dt;


	//Input Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}
	static bool bSpaceState = false;
	if (!bSpaceState && Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = true;
		GameObject* go = FetchGO(GameObject::GO_VILLAGER);
		go->scale.Set(gridSize, gridSize, gridSize);
		go->pos.Set(gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, 0);
		go->target = go->pos;
		go->steps = 0;
		go->energy = 8.f;
		go->nearest = NULL;
		go->Collision = true;
		go->sm->SetNextState("VillagerFull");
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
	}
	static bool bVState = false;
	if (!bVState && Application::IsKeyPressed('V'))
	{
		bVState = true;
		GameObject* go = FetchGO(GameObject::GO_FISHFOOD);
		go->scale.Set(gridSize, gridSize, gridSize);
		go->pos.Set(gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, 0);
		go->target = go->pos;
		go->moveSpeed = 1.f;
	}
	else if (bVState && !Application::IsKeyPressed('V'))
	{
		bVState = false;
	}
	static bool bBState = false;
	if (!bBState && Application::IsKeyPressed('B'))
	{
		bBState = true;
		GameObject* go = FetchGO(GameObject::GO_SHARK);
		go->scale.Set(gridSize, gridSize, gridSize);
		go->pos.Set(gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, 0);
		go->target = go->pos;
	}
	else if (bBState && !Application::IsKeyPressed('B'))
	{
		bBState = false;
	}

	std::vector<GameObject*> templist = m_goList;
	//StateMachine
	for (std::vector<GameObject*>::iterator it = templist.begin(); it != templist.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (!go->active)
			continue;
		if (go->sm)
			go->sm->Update(dt);

		go->Fightable = true;
	}
	//do collision detection and response
	for (std::vector<GameObject*>::iterator it = templist.begin(); it != templist.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (!go->active)
			continue;
		if (go->Collision)
		{
			for (std::vector<GameObject*>::iterator it2 = templist.begin(); it2 != templist.end(); ++it2)
			{
				GameObject* go2 = (GameObject*)*it2;
				if (!go2->active)
					continue;

				float distance = (go->pos - go2->pos).Length();
				if (distance < gridSize * go->GridSizeMultiplier || distance < gridSize * go2->GridSizeMultiplier)
				{
					go->OnCollision(go2, dt);
				}
			}
		}
	}

	//Movement Section
	for (std::vector<GameObject*>::iterator it = templist.begin(); it != templist.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (!go->active)
			continue;
		if (go->Stationary)
			continue;
		Vector3 dir = go->target - go->pos;
		if (dir.Length() < go->moveSpeed * dt * m_speed)
		{
			//GO->pos reach target
			go->pos = go->target;
			float random = Math::RandFloatMinMax(0.f, 1.f);
			if (random < 0.25f && go->moveRight)
				go->target = go->pos + Vector3(gridSize, 0, 0);
			else if (random < 0.5f && go->moveLeft)
				go->target = go->pos + Vector3(-gridSize, 0, 0);
			else if (random < 0.75f && go->moveUp)
				go->target = go->pos + Vector3(0, gridSize, 0);
			else if (go->moveDown)
				go->target = go->pos + Vector3(0, -gridSize, 0);
			if (go->target.x < 0 || go->target.x > noGrid * gridSize || go->target.y < 0 || go->target.y > noGrid * gridSize)
				go->target = go->pos;
		}
		else
		{
			try
			{
				dir.Normalize();
				go->pos += dir * go->moveSpeed * static_cast<float>(dt) * m_speed;
			}
			catch (DivideByZero)
			{
			}
		}
	}

	//week 4
	//Counting objects
	int objectCount = 0;
	m_numGO[GameObject::GO_VILLAGER] = m_numGO[GameObject::GO_SHARK] = m_numGO[GameObject::GO_FISHFOOD] = 0;

	//create message on the stack, then pass the address of message to each gameobject
	//i.e. everyone shares the same message. fewer allocation of memory.
	MessageCheckActive msgCheckActive = MessageCheckActive();
	MessageCheckFish msgCheckFish = MessageCheckFish();
	MessageCheckFood msgCheckFood = MessageCheckFood();
	MessageCheckShark msgCheckShark = MessageCheckShark();
	for (GameObject* go : templist)
	{
		//since PostOffice does not support sending to multiple observers under
		//a single key, we opt for this approach
		//consider changing PostOffice to support this functionality if you want! :)
		objectCount += static_cast<int>(go->Handle(&msgCheckActive));
		m_numGO[GameObject::GO_VILLAGER] += static_cast<int>(go->Handle(&msgCheckFish));
		m_numGO[GameObject::GO_FISHFOOD] += static_cast<int>(go->Handle(&msgCheckFood));
		m_numGO[GameObject::GO_SHARK] += static_cast<int>(go->Handle(&msgCheckShark));
	}

	SceneData::GetInstance()->SetObjectCount(objectCount);
	SceneData::GetInstance()->SetVillagerCount(m_numGO[GameObject::GO_VILLAGER]);
}


void SceneICA1::RenderGO(GameObject* go)
{
	std::ostringstream ss;
	switch (go->type)
	{
	case GameObject::GO_BALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);

		ss.str("");
		ss.precision(3);
		ss << go->id;
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_GRASS:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		modelStack.PushMatrix();
		modelStack.Rotate(180, 0, 0, 1);
		RenderMesh(meshList[GEO_GRASS], false);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		break;
	case GameObject::GO_HOUSE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x * go->GridSizeMultiplier * (go->hp / go->Maxhp), go->scale.y * go->GridSizeMultiplier * (go->hp / go->Maxhp), go->scale.z);
		modelStack.PushMatrix();
		modelStack.Rotate(180, 0, 0, 1);
		RenderMesh(meshList[GEO_HOUSE], false);
		modelStack.PopMatrix();
		// TO Distinguish between each other
		modelStack.PushMatrix();
		modelStack.Rotate(0, 0, 0, 1);
		modelStack.Scale(0.75f, 0.75f, go->scale.z);
		if (go->RED)
			RenderMesh(meshList[GEO_BALL], false);
		else {
			RenderMesh(meshList[GEO_BLUE_BALL], false);
		}
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		break;
	case GameObject::GO_TREE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		modelStack.PushMatrix();
		modelStack.Rotate(180, 0, 0, 1);
		RenderMesh(meshList[GEO_TREE], false);
		modelStack.PopMatrix();
		//ss.str("");
		//ss.precision(3);
		//ss << go->id;
		//RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_COW:
		{
		const int offset = 0;
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, zOffset);
		modelStack.Scale(go->scale.x - offset, go->scale.y - offset, go->scale.z);
		modelStack.PushMatrix();
		modelStack.Rotate(180, 0, 0, 1);
		}

		if (go->sm)
		{
			if (go->sm->GetCurrentState() == "CowTooFull")
				RenderMesh(meshList[GEO_TOOFULL], false);
			else if (go->sm->GetCurrentState() == "CowFull")
				RenderMesh(meshList[GEO_NEUTRAL_COW], false);
			else if (go->sm->GetCurrentState() == "CowHungry")
				RenderMesh(meshList[GEO_HUNGRY], false);
			else
				RenderMesh(meshList[GEO_DEAD], false);
		}
		{
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			const Vector3 displacement = go->target - go->pos;
			modelStack.Rotate(Math::RadianToDegree(atan2(displacement.y, displacement.x)), 0, 0, 1);
			modelStack.Scale(displacement.Length() / SceneData::GetInstance()->GetGridSize(), .3f, 1.f);
			RenderMesh(meshList[GEO_LINE], false);
			modelStack.PopMatrix();

			//// TO Distinguish between each other
			//modelStack.PushMatrix();
			//modelStack.Rotate(0, 0, 0, 1);
			//modelStack.Scale(0.75f, 0.75f, go->scale.z);
			//RenderMesh(meshList[GEO_BLUE_BALL], false);
			//modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		ss.precision(3);
		ss << "[" << go->pos.x << ", " << go->pos.y << "]";
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		modelStack.Translate(-SceneData::GetInstance()->GetGridSize() / 4, SceneData::GetInstance()->GetGridSize() / 4, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		ss.str("");
		ss.precision(3);
		ss << go->energy;
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		modelStack.Translate(0, -SceneData::GetInstance()->GetGridSize() / 4, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		break;
	case GameObject::GO_VILLAGER:
		{
		const int offset = 0;
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, zOffset);
		modelStack.Scale(go->scale.x - offset, go->scale.y - offset, go->scale.z);
		modelStack.PushMatrix();
		modelStack.Rotate(180, 0, 0, 1);
		}

		if (go->sm)
		{
			if (go->sm->GetCurrentState() == "VillagerTooFull")
				RenderMesh(meshList[GEO_TOOFULL], false);
			else if (go->sm->GetCurrentState() == "VillagerFull")
				RenderMesh(meshList[GEO_NEUTRAL_VILLAGER], false);
			else if (go->sm->GetCurrentState() == "VillagerHungry")
				RenderMesh(meshList[GEO_HUNGRY], false);
			else
				RenderMesh(meshList[GEO_DEAD], false);
		}
		{
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			const Vector3 displacement = go->target - go->pos;
			modelStack.Rotate(Math::RadianToDegree(atan2(displacement.y, displacement.x)), 0, 0, 1);
			modelStack.Scale(displacement.Length() / SceneData::GetInstance()->GetGridSize(), .3f, 1.f);
			RenderMesh(meshList[GEO_LINE], false);
			modelStack.PopMatrix();

			// TO Distinguish between each other
			modelStack.PushMatrix();
			modelStack.Rotate(0, 0, 0, 1);
			modelStack.Scale(0.75f, 0.75f, go->scale.z);
			if (go->RED)
				RenderMesh(meshList[GEO_BALL], false);
			else {
				RenderMesh(meshList[GEO_BLUE_BALL], false);
			}
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		ss.precision(3);
		ss << "[" << go->pos.x << ", " << go->pos.y << "]";
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		modelStack.Translate(-SceneData::GetInstance()->GetGridSize() / 4, SceneData::GetInstance()->GetGridSize() / 4, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		ss.str("");
		ss.precision(3);
		ss << go->energy;
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		modelStack.Translate(0, -SceneData::GetInstance()->GetGridSize() / 4, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SHARK:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, zOffset);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);

		if (go->sm)
		{
			if (go->sm->GetCurrentState() == "Crazy")
				RenderMesh(meshList[GEO_CRAZY], false);
			else if (go->sm->GetCurrentState() == "Happy")
				RenderMesh(meshList[GEO_HAPPY], false);
			else
				RenderMesh(meshList[GEO_SHARK], false);
		}
		modelStack.PushMatrix();
		ss.str("");
		ss.precision(3);
		ss << "[" << go->pos.x << ", " << go->pos.y << "]";
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		modelStack.Translate(-SceneData::GetInstance()->GetGridSize() / 4, -SceneData::GetInstance()->GetGridSize() / 4, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FISHFOOD:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, zOffset);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_FISHFOOD], false);
		modelStack.PushMatrix();
		ss.str("");
		ss.precision(3);
		ss << "[" << go->pos.x << ", " << go->pos.y << "]";
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		modelStack.Translate(-SceneData::GetInstance()->GetGridSize() / 4, -SceneData::GetInstance()->GetGridSize() / 4, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		break;
	}
}

void SceneICA1::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(m_worldHeight * 0.5f, m_worldHeight * 0.5f, -1.f);
	modelStack.Scale(m_worldHeight, m_worldHeight, m_worldHeight);
	RenderMesh(meshList[GEO_BG], false);
	modelStack.PopMatrix();

	zOffset = 0;
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (go->active)
		{
			zOffset += 0.001f;
			RenderGO(go);
		}
	}

	//On screen text

	std::ostringstream ss;

	ss.precision(3);

	float fCounter = 0;
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if ((go->active) && (go->type == GameObject::GO_VILLAGER) && (go->nearest))
		{
			ss.str("");
			ss << "Fish:" << go->id << "[" << go->pos.x << "," << go->pos.y <<
				"]>[" << go->nearest->pos.x << "," << go->nearest->pos.y << "]";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 56 - fCounter);
			fCounter += 2.0f;
		}
	}

	ss.str("");
	ss.precision(3);
	ss << "Speed:" << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 6);

	ss.str("");
	ss.precision(5);
	ss << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 3);

	ss.str("");
	ss << "Count:" << SceneData::GetInstance()->GetObjectCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	/*ss.str("");*/
	//ss << "Fishes:" << m_numGO[GameObject::GO_VILLAGER];
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 18);

	//ss.str("");
	//ss << "Shark:" << m_numGO[GameObject::GO_SHARK];
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 15);

	//ss.str("");
	//ss << "Food:" << m_numGO[GameObject::GO_FISHFOOD];
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 12);
	ss.str("");
	ss << "BLUEFoodCollected:" << SceneData::GetInstance()->GetFoodEnergyCount(false);
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 21);
	ss.str("");
	ss << "REDFoodCollected:" << SceneData::GetInstance()->GetFoodEnergyCount(true);
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 24);

	ss.str("");
	ss << "BLUEWoodCollected:" << SceneData::GetInstance()->GetWoodCount(false);
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 18);
	ss.str("");
	ss << "REDWoodCollected:" << SceneData::GetInstance()->GetWoodCount(true);
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 15);


	RenderTextOnScreen(meshList[GEO_TEXT], "Aquarium", Color(0, 1, 0), 3, 50, 0);
}

void SceneICA1::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject* go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if (m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}

//week 4
//handle all incoming messages from PostOffice
bool SceneICA1::Handle(Message* message)
{
	MessageWRU* messageWRU = dynamic_cast<MessageWRU*>(message);
	if (messageWRU)
	{
		float gridSize = SceneData::GetInstance()->GetGridSize();
		float gridOffset = SceneData::GetInstance()->GetGridOffset();
		int numGrid = SceneData::GetInstance()->GetNumGrid();
		//get pointer to the entity who fired the event
		//a FISH looking for fish food OR SHARK looking for fish etc.
		GameObject* go = messageWRU->go;
		go->nearest = nullptr;

		float nearestDistance = FLT_MAX; //FLT_MAX is max possible float value
		float highestEnergy = FLT_MIN; //FLT_MIN is min possible positive float value

		//scan through entire list looking for target object
		for (GameObject* go2 : m_goList)
		{
			if (!go2->active)
				continue;

			//message indicates go is hunting for nearest fish food
			if (messageWRU->type == MessageWRU::NEAREST_FISHFOOD &&
				go2->type == GameObject::GO_FISHFOOD)
			{
				float distance = (go->pos - go2->pos).Length();
				if (distance < messageWRU->threshold && distance < nearestDistance)
				{
					nearestDistance = distance;
					go->nearest = go2;
				}
			}
			//message indicates go is seeking nearest shark
			else if (messageWRU->type == MessageWRU::NEAREST_SHARK &&
				go2->type == GameObject::GO_SHARK)
			{
				float distance = (go->pos - go2->pos).Length();
				if (distance < messageWRU->threshold && distance < nearestDistance)
				{
					nearestDistance = distance;
					go->nearest = go2;
				}
			}
			////message indicated go is hunting for highest energy fish
			else if (messageWRU->type == MessageWRU::NEAREST_GRASS &&
				go2->type == GameObject::GO_GRASS)
			{
				float distance = (go->pos - go2->pos).Length();
				if (distance < messageWRU->threshold && distance < nearestDistance)
				{
					nearestDistance = distance;
					go->nearest = go2;
				}
			}
			else if (messageWRU->type == MessageWRU::NEAREST_TREE_VILLAGER &&
				go2->type == GameObject::GO_TREE)
			{
				float distance = (go->pos - go2->pos).Length();
				if (distance < messageWRU->threshold && distance < nearestDistance)
				{
					nearestDistance = distance;
					go->nearest = go2;
				}
			}
			else if (messageWRU->type == MessageWRU::NEAREST_VILLAGER &&
				go2->type == GameObject::GO_VILLAGER)
			{
				float distance = (go->pos - go2->pos).Length();
				if (distance < messageWRU->threshold && distance < nearestDistance)
				{
					nearestDistance = distance;
					go->nearest = go2;
				}
			}
			else if (messageWRU->type == MessageWRU::NEAREST_VILLAGER_OPS &&
				go2->type == GameObject::GO_VILLAGER && go->RED != go2->RED && go2->Fightable)
			{
				float distance = (go->pos - go2->pos).Length();
				if (distance < messageWRU->threshold && distance < nearestDistance)
				{
					nearestDistance = distance;
					go->nearestEnemy = go2;
					std::cout << "FOUND ENEMY" << std::endl;
				}
				//std::cout << "Searching for Villager OPS" << std::endl;
			}
			else if (messageWRU->type == MessageWRU::NEAREST_COW &&
				go2->type == GameObject::GO_COW)
			{
				float distance = (go->pos - go2->pos).Length();
				if (distance < messageWRU->threshold && distance < nearestDistance)
				{
					nearestDistance = distance;
					go->nearest = go2;
				}
			}
			else if (messageWRU->type == MessageWRU::NEAREST_HOUSE_VILLAGER &&
				go2->type == GameObject::GO_HOUSE && go->RED == go2->RED)
			{

				float distance = (go->pos - go2->pos).Length();
				if (distance < messageWRU->threshold && distance < nearestDistance)
				{
					nearestDistance = distance;
					go->nearest = go2;
				}
			}
		}
		if (messageWRU->type == MessageWRU::SPAWN_HOUSE)
		{
			//std::cout << "Making HouseStart " << std::endl;
			Vector3 SpawnPosition;
			bool SpawnLeft;
			bool SpawnRight;
			bool SpawnTop;
			bool SpawnBottom;

			SpawnLeft = SpawnRight = SpawnTop = SpawnBottom = false;
			Vector3 temp = go->pos + Vector3(gridSize * messageWRU->threshold, 0, 0);
			if (SceneData::GetInstance()->CheckWithinGrid(temp.x, temp.y, temp.z)) {
				SpawnRight = true;
			}
			temp = go->pos + Vector3(-gridSize * messageWRU->threshold, 0, 0);
			if (SceneData::GetInstance()->CheckWithinGrid(temp.x, temp.y, temp.z)) {
				SpawnLeft = true;
			}
			temp = go->pos + Vector3(0, gridSize * messageWRU->threshold, 0);
			if (SceneData::GetInstance()->CheckWithinGrid(temp.x, temp.y, temp.z)) {
				SpawnTop = true;
			}
			temp = go->pos + Vector3(0, -gridSize * messageWRU->threshold, 0);
			if (SceneData::GetInstance()->CheckWithinGrid(temp.x, temp.y, temp.z)) {
				SpawnBottom = true;
			}


			float random = Math::RandFloatMinMax(0.f, 1.f);
			if (random < 0.25f && SpawnRight)
				SpawnPosition = go->pos + Vector3(gridSize * messageWRU->threshold, 0, 0);
			else if (random < 0.5f && SpawnLeft)
				SpawnPosition = go->pos + Vector3(-gridSize * messageWRU->threshold, 0, 0);
			else if (random < 0.75f && SpawnTop)
				SpawnPosition = go->pos + Vector3(0, gridSize * messageWRU->threshold, 0);
			else if (SpawnBottom)
				SpawnPosition = go->pos + Vector3(0, -gridSize * messageWRU->threshold, 0);


			GameObject* house = FetchGO(GameObject::GO_HOUSE);
			house->scale.Set(gridSize, gridSize, gridSize);
			house->pos.Set(SpawnPosition.x, SpawnPosition.y, SpawnPosition.z);
			house->target = go->pos;
			house->steps = 0;
			house->RED = go->RED;
			//house->energy = 8.f;
			house->nearest = NULL;
			house->Stationary = true;
			house->Collision = true;

			//	go->pos.Set(gridOffset + Math::RandIntMinMax(0, numGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, numGrid - 1) * gridSize, 0);
			//std::cout << "Making House " << std::endl;
		}

		else if (messageWRU->type == MessageWRU::SPAWN_VILLAGER)
		{
			GameObject* villager = FetchGO(GameObject::GO_VILLAGER);
			villager->scale.Set(gridSize, gridSize, gridSize);
			villager->pos.Set(go->pos.x , go->pos.y, go->pos.z);
			villager->target = villager->pos;
			villager->RED = go->RED;
			villager->steps = 0;
			villager->nearest = NULL;
			villager->nearestEnemy = NULL;
			villager->Collision = true;
			villager->sm->SetNextState("VillagerFull");
		}

		delete message; //remember, the message is allocated on the heap!
		return true;
	}

	delete message; //remember, the message is allocated on the heap!
	return false;
}