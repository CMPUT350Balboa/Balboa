#pragma once

#include "Common.h"
#include "BWTA.h"

#include "UnitData.h"



class DynamicStrategyManager
{

	UnitData							enemyUnitData;

public:
	DynamicStrategyManager();
	~DynamicStrategyManager();
	static DynamicStrategyManager &		Instance();
	void								update();
};

