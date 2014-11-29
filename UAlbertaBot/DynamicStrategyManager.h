#pragma once

#include "Common.h"
#include "BWTA.h"

#include "UnitData.h"

#include "..\..\SparCraft\source\SparCraft.h"

// This is a new class written by Michael Williams
// Enemy strategy detection and counter strategy switching
//
// See also code in ProductionManager that performs the new build order search

class DynamicStrategyManager
{
	DynamicStrategyManager();

	UnitData							enemyUnitData;	
	std::map<BWAPI::UnitType, int>  	getUnitTypeCountMap();
	

public:
	//DynamicStrategyManager();
	//~DynamicStrategyManager();

	static DynamicStrategyManager &		Instance();
	void								update();
	void								onStart();
	void								findEnemyStrategy(std::map<BWAPI::UnitType, int>  unitTypeCountMap);
	
};

