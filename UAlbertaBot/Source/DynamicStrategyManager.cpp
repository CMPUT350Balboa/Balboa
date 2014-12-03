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

// return an instance of the manager
DynamicStrategyManager & DynamicStrategyManager::Instance() 
{
	static DynamicStrategyManager instance;
	return instance;
}

void DynamicStrategyManager::update() 
{

	if (BWAPI::Broodwar->getFrameCount() == 10){ //@@
		BWAPI::Broodwar->printf("<< DYNAMIC STRATEGY MANAGER ACTIVE >>");		
	}
	
	searchNewStrategy();	
}

void DynamicStrategyManager::searchNewStrategy()
{

	enemyUnitData = InformationManager::Instance().getUnitDetail(BWAPI::Broodwar->enemy()); //@@ new function in info manager
	
	// Create the unit type and count map
	enemyTypeCountMap.clear();
	enemyTypeCountMap = getUnitTypeCountMap(enemyUnitData);

	std::string strategy = std::string();

	
	std::string current_strategy = StrategyManager::Instance().getCurrentStrategy();

	BWAPI::Race enemyRace = BWAPI::Broodwar->enemy()->getRace();

	// Protoss-enemy specific strategies
	if (enemyRace == BWAPI::Races::Protoss){
		//variables store weights of possible enemy strategies
		int zealot = 0;
		int dragoon = 0;
		int dark_templar = 0;

		zealot += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Gateway] > 1); 

		dragoon += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Assimilator] > 0);
		dragoon += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Cybernetics_Core] > 0); 
		dragoon += (BWAPI::Broodwar->enemy()->getUpgradeLevel(BWAPI::UpgradeTypes::Singularity_Charge) > 0);
		//dragoon += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Dragoon] > 0);

		dark_templar += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Assimilator] > 0);
		dark_templar += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Cybernetics_Core] > 0); 
		dark_templar += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Citadel_of_Adun] > 0); 
		dark_templar += (enemyTypeCountMap[BWAPI::UnitTypes::Protoss_Dark_Templar] > 0);
		
		if (zealot) // give weight to zealot rush, as opponent has two gateways
		{ 
			if (current_strategy == StrategyManager::PROTOSS_CANNON_DEFEND_AND_ZEALOT_RUSH) 
			{
				strategy = current_strategy;
			} else {
				strategy = StrategyManager::PROTOSS_ZEALOT_RUSH;
			}					
		}
		if (dark_templar || dragoon) // enemy building higher tech, give weight to:
		{									
			if (dark_templar > dragoon) { // focus on Dragoons to counter Dark Templar
				if (current_strategy == StrategyManager::PROTOSS_DRAGOON_DEFEND) 
				{
					strategy = current_strategy;
				} else
				{
					strategy = StrategyManager::PROTOSS_DRAGOONS;	
				}
			} 
			else // focus on Zealot rush to counter Dragoons
			{
				if (current_strategy == StrategyManager::PROTOSS_CANNON_DEFEND_AND_ZEALOT_RUSH) 
				{
					strategy = current_strategy;
				} else {
					strategy = StrategyManager::PROTOSS_ZEALOT_RUSH;
				}	
			}						
		}

		//On screen display for DynamicStrategyManager - disabled to remove any impact on frame rate
		/*
		if (Options::Debug::DRAW_UALBERTABOT_DEBUG)
		{
			BWAPI::Broodwar->drawTextScreen(140, 300, "\x04<<< BEST COUNTER STRATEGY: %s >>>", strategy.c_str());
			BWAPI::Broodwar->drawTextScreen(140, 220, "\x07 zealot %d", zealot);
			BWAPI::Broodwar->drawTextScreen(140, 230, "\x07 dragoon %d", dragoon);
			BWAPI::Broodwar->drawTextScreen(140, 240, "\x07 DT %d", dark_templar);

			if(strategy.empty()){
				BWAPI::Broodwar->drawTextScreen(140, 250, "\x07 guess_strategy none");
			} else {
				BWAPI::Broodwar->drawTextScreen(140, 250, "\x07 guess_strategy %s", strategy.c_str());
			}
			
			BWAPI::Broodwar->drawTextScreen(140, 260, "\x07 my_strategy %s", StrategyManager::Instance().getCurrentStrategy().c_str());
			//BWAPI::Broodwar->drawTextScreen(140, 270, "\x07 number_switches %d", numSwitches);
		}
		*/

	} 	

	// Check for tech tree completion

	//if we have completed full tech tree for DarkTemplar/Dragoon_Defend, don't switch to a new strategy
	if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Citadel_of_Adun) > 0) 
	{
		if (current_strategy == StrategyManager::PROTOSS_DRAGOON_DEFEND) 
		{
			strategy = current_strategy;
		} else
		{
			strategy = StrategyManager::PROTOSS_DARK_TEMPLAR;
		}
	}
	//if we have completed full tech tree for Dragoons, don't switch to a new strategy
	if (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Singularity_Charge) > 0) 
	{

		if (current_strategy == StrategyManager::PROTOSS_DRAGOON_DEFEND) 
		{
			strategy = current_strategy;
		} else
		{
			strategy =  StrategyManager::PROTOSS_DRAGOONS;
		}

	}

	// currently we don't switch out of scout rush
	if (current_strategy == StrategyManager::PROTOSS_SCOUT_RUSH)
	{
		strategy = StrategyManager::PROTOSS_SCOUT_RUSH;
	}		

	//Set newly determined strategy if different from current strategy, otherwise no change
	if (StrategyManager::Instance().getCurrentStrategy() != strategy && !strategy.empty())
	{
		StrategyManager::Instance().setCurrentStrategy(strategy);
		//BWAPI::Broodwar->printf("Previous strategy: %s, New strategy: %s", current_strategy, strategy);
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