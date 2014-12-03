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
		BWAPI::Broodwar->printf("								<< DYNAMIC STRATEGY MANAGER ACTIVE >>");
	}

	searchNewStrategy(); // error in externals of function, fix
	
}


//void DynamicStrategyManager::findEnemyStrategy(std::map<BWAPI::UnitType, int>  unitTypeCountMap)
void DynamicStrategyManager::searchNewStrategy()
{

	enemyUnitData = InformationManager::Instance().getUnitDetail(BWAPI::Broodwar->enemy()); //@@ my function in info manager
	
	// Create the unit type and count map
	//std::map<BWAPI::UnitType, int>		selfTypeCountMap;
	//std::map<BWAPI::UnitType, int>		enemyTypeCountMap;
	//selfTypeCountMap = getUnitTypeCountMap(enemyUnitData);
	enemyTypeCountMap.clear();
	enemyTypeCountMap = getUnitTypeCountMap(enemyUnitData);

	std::string strategy = std::string();
	int zealot = 0;
	int dragoon = 0;
	int dark_templar = 0;

	BWAPI::Race enemyRace = BWAPI::Broodwar->enemy()->getRace();

	if (enemyRace == BWAPI::Races::Protoss){

		zealot += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Gateway] > 1); 

		dragoon += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Assimilator] > 0);
		dragoon += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Cybernetics_Core] > 0); 
		dragoon += (BWAPI::Broodwar->enemy()->getUpgradeLevel(BWAPI::UpgradeTypes::Singularity_Charge) > 0);
		//dragoon += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Dragoon] > 0);

		dark_templar += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Assimilator] > 0);
		dark_templar += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Cybernetics_Core] > 0); 
		dark_templar += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Citadel_of_Adun] > 0); 
		dark_templar += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Dark_Templar] > 0);
		
		if (zealot) {strategy = StrategyManager::PROTOSS_ZEALOT_RUSH;}						// give weight to zealot rush, as opponent has two gateways
		if (dark_templar || dragoon)									// enemy building higher tech so:
		{									
			if (dark_templar > dragoon) {strategy = StrategyManager::PROTOSS_DRAGOONS;}	// focus on Dragoons to counter Dark Templar
			else {strategy = StrategyManager::PROTOSS_ZEALOT_RUSH;}						// focus on Zealot rush to counter Dragoons
		}



		//if we have completed full tech tree for strategy, don't switch to a new strategy
		if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Citadel_of_Adun) > 0) {strategy = StrategyManager::PROTOSS_DARK_TEMPLAR;}
		if (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Singularity_Charge) > 0) {strategy =  StrategyManager::PROTOSS_DRAGOONS;}
		
		/*if (Options::Debug::DRAW_UALBERTABOT_DEBUG)
		{
			BWAPI::Broodwar->drawTextScreen(140, 300, "\x04<<< BEST COUNTER STRATEGY: %d >>>", strategy);
			BWAPI::Broodwar->drawTextScreen(140, 220, "\x07 zealot %d", zealot);
			BWAPI::Broodwar->drawTextScreen(140, 230, "\x07 dragoon %d", dragoon);
			BWAPI::Broodwar->drawTextScreen(140, 240, "\x07 DT %d", dark_templar);

			if(strategy.empty()){
				BWAPI::Broodwar->drawTextScreen(140, 250, "\x07 guess_strategy none");
			} else {
				BWAPI::Broodwar->drawTextScreen(140, 250, "\x07 guess_strategy %s", strategy);
			}

			BWAPI::Broodwar->drawTextScreen(140, 260, "\x07 my_strategy %s", StrategyManager::Instance().getCurrentStrategy());
			BWAPI::Broodwar->drawTextScreen(140, 270, "\x07 number_switches %d", numSwitches);
		}*/
		
		
		//std::stringstream log;
		//log << "zealot, " << zealot << "dragoon ," << dragoon << "DT ," <<  dark_templar << "guess_strategy ," << strategy << "my_strategy \n" << StrategyManager::Instance().getCurrentStrategy();
		//Logger::Instance().log(log.str());
		
		if (StrategyManager::Instance().getCurrentStrategy() != strategy && !strategy.empty())
		{
			StrategyManager::Instance().setCurrentStrategy(strategy);
			++numSwitches;			
		}
	
	}
	
	if (enemyRace == BWAPI::Races::Terran){
	}

	if (enemyRace == BWAPI::Races::Zerg){
	}


}

std::map<BWAPI::UnitType, int> DynamicStrategyManager::getUnitTypeCountMap(UnitData raceUnitData)
{
	
	//@@ Unit type and count map
	//Hash map key is unit type: unitTypeCount.count(BWAPI::UnitTypes::Protoss_Probe) is zero if there are none of that type
	//Hash map value is count of that unit type: unitTypeCount[BWAPI::UnitTypes::Protoss_Probe]
	//Here we are testing whether the enemy has any of that type (is there a key)
	std::map<BWAPI::UnitType, int> unitCount;

	FOR_EACH_UIMAP_CONST(iter, raceUnitData.getUnits())
	{
		const UnitInfo & unitInfo(iter->second);
		BWAPI::UnitType type = unitInfo.type;
		unitCount[type]++;

	}

	return unitCount;
}