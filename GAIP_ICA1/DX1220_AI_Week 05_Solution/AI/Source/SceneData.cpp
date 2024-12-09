#include "SceneData.h"

int SceneData::GetObjectCount() const
{
	return m_objectCount;
}

void SceneData::SetObjectCount(const int objectCount)
{
	m_objectCount = objectCount;
}

int SceneData::GetFishCount() const
{
	return m_fishCount;
}

void SceneData::SetFishCount(const int fishCount)
{
	m_fishCount = fishCount;
}

int SceneData::GetVillagerCount() const
{
	return m_villagerCount;
}

void SceneData::SetVillagerCount(const int villagerCount)
{
	m_villagerCount = villagerCount;
}

SceneData::SceneData()
{
}

SceneData::~SceneData()
{
}

int SceneData::GetNumGrid() const
{
	return m_noGrid;
}

void SceneData::SetNumGrid(const int numGrid)
{
	m_noGrid = numGrid;
}

float SceneData::GetGridSize() const
{
	return m_gridSize;
}

void SceneData::SetGridSize(const float gridSize)
{
	m_gridSize = gridSize;
}

float SceneData::GetGridOffset() const
{
	return m_gridOffset;
}

void SceneData::SetGridOffset(const float gridOffset)
{
	m_gridOffset = gridOffset;
}

float SceneData::GetWoodCount(bool RED) const
{
	if (RED) {
		return TotalWoodRED;
	}
	else {
		return TotalWoodBLUE;
	}
}

void SceneData::AddWoodCount(const float AdditionalWood, bool RED)
{
	if (RED) {
		TotalWoodRED += AdditionalWood;
	}
	else {
		TotalWoodBLUE += AdditionalWood;
	}
}

float SceneData::GetFoodEnergyCount(bool RED) const
{
	if (RED) {
		return TotalFoodEnergyRED;
	}
	else {
		return TotalFoodEnergyBLUE;
	}
}

void SceneData::AddFoodEnergyCount(const float AdditionalFood, bool RED)
{
	if (RED) {
		TotalFoodEnergyRED += AdditionalFood;
	}
	else {
		TotalFoodEnergyBLUE += AdditionalFood;
	}
}

bool SceneData::CheckWithinGrid(float x, float y, float z) {
	float gridSize = SceneData::GetInstance()->GetGridSize();
	float gridOffset = SceneData::GetInstance()->GetGridOffset();
	int numGrid = SceneData::GetInstance()->GetNumGrid();

	float MinGridPosX = gridOffset + 0 * gridSize;
	float MinGridPosY = gridOffset + 0 * gridSize;
	float MaxGridPosX = gridOffset + (numGrid - 1) * gridSize;
	float MaxGridPosY = gridOffset + (numGrid - 1) * gridSize;


	return (x > MinGridPosX && y > MinGridPosY) && (x < MaxGridPosX && y < MaxGridPosY);
}
