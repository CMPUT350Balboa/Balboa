#include "DynamicStrategyManager.h"
#include "Common.h"
#include "InformationManager.h"
#include "StrategyManager.h" //@@
//#include "ProductionManager.h"

DynamicStrategyManager::DynamicStrategyManager(void)
{
}


DynamicStrategyManager::~DynamicStrategyManager(void)
{
}

// get an instance of this
DynamicStrategyManager & DynamicStrategyManager::Instance() 
{
	static DynamicStrategyManager instance;
	return instance;
}

void DynamicStrategyManager::update() 
{


	//@@ all me from here down to other indication
	//enemyUnitData.getUnits.getNumUnits(); //@@
	//player == BWAPI::Broodwar->self()
	if (BWAPI::Broodwar->getFrameCount() > 100 && BWAPI::Broodwar->getFrameCount() < 150){ //@@
		BWAPI::Broodwar->printf("								<< ENHANCED STRATEGY MANAGER >>");
	}

	//UnitType unit = 
	//enemyUnitData.getNumUnits(UnitType::getUnitType("Protoss_Probe"));
	//enemyUnitData.getNumUnits(

		/*
		const UnitInfo & ui(iter->second);
		BWAPI::UnitType type = ui.type;
		int num_units = enemyUnitData.getNumUnits(type);
	*/

	enemyUnitData = InformationManager::Instance().getUnitDetail(BWAPI::Broodwar->self());
	
	// Map of < Unit type, count of that type >
	std::map<BWAPI::UnitType, int>		unitTypeCount;

	//@@ for all enemy units detected, we can see what types they are and how many there are, to choose a change to the strategy
	FOR_EACH_UIMAP_CONST(iter, enemyUnitData.getUnits())
	{
		const UnitInfo & ui(iter->second);
		BWAPI::UnitType type = ui.type;
		unitTypeCount[type]++;

	}

		//Hash map key is unit type: unitTypeCount.count(BWAPI::UnitTypes::Protoss_Probe)
		//Hash map value is count of that unit type: unitTypeCount[BWAPI::UnitTypes::Protoss_Probe]
		//Here we are testing whether the enemy has any of that type (is there a key)
		
		/* initial test code
		if (unitTypeCount.count(BWAPI::UnitTypes::Protoss_Probe) && unitTypeCount[BWAPI::UnitTypes::Protoss_Probe] >6)
		{
			//preliminary test showing that this works
			StrategyManager::Instance().setCurrentStrategy(3);
			
		}
		*/

		//if (type = BWAPI::UnitTypes::Protoss_Probe && num_units > 6)
		if (unitTypeCount.count(BWAPI::UnitTypes::Protoss_Gateway) && unitTypeCount[BWAPI::UnitTypes::Protoss_Gateway] > 1)
		{
			BWAPI::Broodwar->printf("Enemy strategy is probably Two Gateway Zealot Rush");
			//Counter with the same strategy
			//safest is 10/12 Gateway
			StrategyManager::Instance().setCurrentStrategy(3);
			
		}

		/*
		if (type = BWAPI::UnitTypes::Protoss_Dragoon){
			//could be dragoon build or dark templar
		
		}

		if (type = BWAPI::UnitTypes::Protoss_Dark_Templar){
			//dark templar rush

		}


		if (type = BWAPI::UnitTypes::Protoss_Zealot){
			//zealot rush
			//@FIND A WAY TO USE THIS LINE:
			//StrategyManager::Instance().setCurrentStrategy

		}

		if (type = BWAPI::UnitTypes::Protoss_Dark_Templar){
			//dark templar rush

		}
		

	}
	*///@@ end here

}