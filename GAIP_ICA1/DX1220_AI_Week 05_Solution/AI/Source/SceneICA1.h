#ifndef SCENE_ICA1_H
#define SCENE_ICA1_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "ObjectBase.h"

class SceneICA1 : public SceneBase, public ObjectBase
{
public:
	SceneICA1();
	~SceneICA1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject* go);
	bool Handle(Message* message);

	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);
protected:

	std::vector<GameObject*> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject* m_ghost;

	float m_hourOfTheDay;
	int m_numGO[GameObject::GO_TOTAL];
	float zOffset;
};

#endif