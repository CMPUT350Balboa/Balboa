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
	UnitData							selfUnitData;
	std::map<BWAPI::UnitType, int>  	getUnitTypeCountMap(UnitData raceUnitData);
	int									numSwitches;
	std::map<BWAPI::UnitType, int>		enemyTypeCountMap;
	

public:
	//DynamicStrategyManager();
	//~DynamicStrategyManager();
	enum { ProtossZealotRush=0, ProtossDarkTemplar=1, ProtossDragoons=2, NumProtossStrategies=3 }; //@@change back
	static DynamicStrategyManager &		Instance();
	void								update();
	void								onStart();
	void								searchNewStrategy();
	
};

