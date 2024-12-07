#ifndef SCENE_DATA
#define SCENE_DATA

#include "SingletonTemplate.h"

class SceneData : public Singleton<SceneData>
{
	friend Singleton<SceneData>;
public:
	int GetObjectCount() const;
	void SetObjectCount(const int objectCount);
	int GetFishCount() const;
	void SetFishCount(const int fishCount);

	int GetVillagerCount() const;
	void SetVillagerCount(const int villagerCount);


	int GetNumGrid() const;
	void SetNumGrid(const int numGrid);
	float GetGridSize() const;
	void SetGridSize(const float gridSize);
	float GetGridOffset() const;
	void SetGridOffset(const float gridOffset);



	float GetWoodCount(bool RED) const;
	void AddWoodCount(const float AdditionalWood, bool RED);

	float GetFoodEnergyCount(bool RED) const;
	void AddFoodEnergyCount(const float AdditionalFood, bool RED);

private:
	SceneData();
	~SceneData();
	int m_objectCount;
	int m_fishCount;
	int m_villagerCount;
	int m_noGrid;
	float m_gridSize;
	float m_gridOffset;



	float TotalWoodRED = 0;
	float TotalFoodEnergyRED = 0;

	float TotalWoodBLUE = 0;
	float TotalFoodEnergyBLUE = 0;
};

#endif
