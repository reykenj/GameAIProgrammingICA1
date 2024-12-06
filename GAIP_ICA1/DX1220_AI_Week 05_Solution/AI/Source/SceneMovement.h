#ifndef SCENE_MOVEMENT_H
#define SCENE_MOVEMENT_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "ObjectBase.h"

class SceneMovement : public SceneBase, public ObjectBase
{
public:
	SceneMovement();
	~SceneMovement();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);
	bool Handle(Message* message);

	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);
protected:

	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ghost;

	float m_hourOfTheDay;
	int m_numGO[GameObject::GO_TOTAL];
	float zOffset;
};

#endif