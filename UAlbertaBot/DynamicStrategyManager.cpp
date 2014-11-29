#include "Common.h"
#include "InformationManager.h"
#include "StrategyManager.h" //@@
#include "DynamicStrategyManager.h"

// This is a new class written by Michael Williams
// Enemy strategy detection and counter strategy switching
//
// See also code in ProductionManager that performs the new build order search

DynamicStrategyManager::DynamicStrategyManager()
{
}


//DynamicStrategyManager::~DynamicStrategyManager()
//{
//}

void DynamicStrategyManager::onStart() {}

// get an instance of this
DynamicStrategyManager & DynamicStrategyManager::Instance() 
{
	static DynamicStrategyManager instance;
	return instance;
}

void DynamicStrategyManager::update() 
{

	if (BWAPI::Broodwar->getFrameCount() == 10){ //@@
		BWAPI::Broodwar->printf("								<< ENHANCED STRATEGY MANAGER ACTIVE>>");
	}

	// Get the list of enemy unit data from InformationManager
	enemyUnitData = InformationManager::Instance().getUnitDetail(BWAPI::Broodwar->enemy()); //@@ my function in info manager
	
	// Create the unit type and count map
	std::map<BWAPI::UnitType, int>		unitTypeCountMap;
	unitTypeCountMap = getUnitTypeCountMap();
	//findEnemyStrategy(unitTypeCountMap); // error in externals of function, fix
	


		
		/* initial test with probes
		if (unitTypeCount.count(BWAPI::UnitTypes::Protoss_Probe) && unitTypeCount[BWAPI::UnitTypes::Protoss_Probe] >6)
		{
			//preliminary test showing that this works
			StrategyManager::Instance().setCurrentStrategy(3);
			
		}
		*/

		if (unitTypeCountMap.count(BWAPI::UnitTypes::Protoss_Gateway) && unitTypeCountMap[BWAPI::UnitTypes::Protoss_Gateway] > 1)
		{
			BWAPI::Broodwar->printf("Enemy strategy is probably Two Gateway Zealot Rush");
			//Counter with the same strategy
			//safest is 10/12 Gateway
			StrategyManager::Instance().setCurrentStrategy(3);
			
		}



}

/*Externals error - fix
void findEnemyStrategy(std::map<BWAPI::UnitType, int>  unitTypeCountMap)
{

	if (unitTypeCountMap.count(BWAPI::UnitTypes::Protoss_Gateway) && unitTypeCountMap[BWAPI::UnitTypes::Protoss_Gateway] > 1)
	{
		BWAPI::Broodwar->printf("Possible Enemy strategy: Two Gateway Zealot Rush");
		//Counter with the same strategy
		//safest is 10/12 Gateway
		StrategyManager::Instance().setCurrentStrategy(3);
			
	}


	if (unitTypeCountMap.count(BWAPI::UnitTypes::Protoss_Gateway) && unitTypeCountMap[BWAPI::UnitTypes::Protoss_Gateway] > 1){
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
*/

std::map<BWAPI::UnitType, int> DynamicStrategyManager::getUnitTypeCountMap()
{
	
	//@@ Unit type and count map
	//Hash map key is unit type: unitTypeCount.count(BWAPI::UnitTypes::Protoss_Probe) is zero if there are none of that type
	//Hash map value is count of that unit type: unitTypeCount[BWAPI::UnitTypes::Protoss_Probe]
	//Here we are testing whether the enemy has any of that type (is there a key)
	std::map<BWAPI::UnitType, int> unitCount;

	FOR_EACH_UIMAP_CONST(iter, enemyUnitData.getUnits())
	{
		const UnitInfo & unitInfo(iter->second);
		BWAPI::UnitType type = unitInfo.type;
		unitCount[type]++;

	}

	return unitCount;
}