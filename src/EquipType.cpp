// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Copyright © 2013-14 Meteoric Games Ltd
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "EquipType.h"
#include "galaxy/StarSystem.h" // for ECON_* enum
#include "Lang.h"

const EquipType Equip::types[Equip::TYPE_MAX] = {
	{ Lang::NONE, 0,
	  Equip::SLOT_CARGO, -1, {},
	  0, 0, 0, 0, true, 0
	},{
	  Lang::HYDROGEN, Lang::HYDROGEN_DESCRIPTION,
	  Equip::SLOT_HYDROGENTANK, -1, {},
	  100, 1, 0, ECON_MINING, true, 0
	},{
	  Lang::LIQUID_OXYGEN, Lang::LIQUID_OXYGEN_DESCRIPTION,
	  Equip::SLOT_CARGO, -1, { Equip::WATER, Equip::INDUSTRIAL_MACHINERY },
	  150, 1, 0, ECON_MINING, true, 0
	},{
	  Lang::METAL_ORE, 0,
	  Equip::SLOT_CARGO, -1, { Equip::MINING_MACHINERY },
	  300, 1, 0, ECON_MINING, true, 0
	},{
	  Lang::CARBON_ORE, Lang::CARBON_ORE_DESCRIPTION,
	  Equip::SLOT_CARGO, -1, { Equip::MINING_MACHINERY },
	  500, 1, 0, ECON_MINING, true, 0
	},{
	  Lang::METAL_ALLOYS, 0,
	  Equip::SLOT_CARGO, -1, { Equip::METAL_ORE, Equip::INDUSTRIAL_MACHINERY },
	  800, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::PLASTICS, 0,
	  Equip::SLOT_CARGO, -1, { Equip::CARBON_ORE, Equip::INDUSTRIAL_MACHINERY },
	  1200, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::FRUIT_AND_VEG, 0,
	  Equip::SLOT_CARGO, -1, { Equip::FARM_MACHINERY, Equip::FERTILIZER },
	  1200, 1, 0, ECON_AGRICULTURE, true, 0
	},{
	  Lang::ANIMAL_MEAT, 0,
	  Equip::SLOT_CARGO, -1, { Equip::FARM_MACHINERY, Equip::FERTILIZER },
	  1800, 1, 0, ECON_AGRICULTURE, true, 0
	},{
	  Lang::LIVE_ANIMALS, 0,
	  Equip::SLOT_CARGO, -1, { Equip::FARM_MACHINERY, Equip::FERTILIZER },
	  3200, 1, 0, ECON_AGRICULTURE, true, 0
	},{
	  Lang::LIQUOR, 0,
	  Equip::SLOT_CARGO, -1, { Equip::FARM_MACHINERY, Equip::FERTILIZER },
	  800, 1, 0, ECON_AGRICULTURE, true, 0
	},{
	  Lang::GRAIN, 0,
	  Equip::SLOT_CARGO, -1, { Equip::FARM_MACHINERY, Equip::FERTILIZER },
	  1000, 1, 0, ECON_AGRICULTURE, true, 0
	},{
	  Lang::TEXTILES, 0,
	  Equip::SLOT_CARGO, -1, { Equip::PLASTICS },
	  850, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::FERTILIZER, 0,
	  Equip::SLOT_CARGO, -1, { Equip::CARBON_ORE },
	  400, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::WATER, 0,
	  Equip::SLOT_CARGO, -1, { Equip::MINING_MACHINERY },
	  120, 1, 0, ECON_MINING, true, 0
	},{
	  Lang::MEDICINES,0,
	  Equip::SLOT_CARGO, -1, { Equip::COMPUTERS, Equip::CARBON_ORE },
	  2200, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::CONSUMER_GOODS,0,
	  Equip::SLOT_CARGO, -1, { Equip::PLASTICS, Equip::TEXTILES },
	  14000, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::COMPUTERS,0,
	  Equip::SLOT_CARGO, -1, { Equip::PRECIOUS_METALS, Equip::INDUSTRIAL_MACHINERY },
	  8000, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::ROBOTS,0,
	  Equip::SLOT_CARGO, -1, { Equip::PLASTICS, Equip::COMPUTERS },
	  6300, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::PRECIOUS_METALS, 0,
	  Equip::SLOT_CARGO, -1, { Equip::MINING_MACHINERY },
	  18000, 1, 0, ECON_MINING, true, 0
	},{
	  Lang::INDUSTRIAL_MACHINERY,0,
	  Equip::SLOT_CARGO, -1, { Equip::METAL_ALLOYS, Equip::ROBOTS },
	  1300, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::FARM_MACHINERY,0,
	  Equip::SLOT_CARGO, -1, { Equip::METAL_ALLOYS, Equip::ROBOTS },
	  1100, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::MINING_MACHINERY,0,
	  Equip::SLOT_CARGO, -1, { Equip::METAL_ALLOYS, Equip::ROBOTS },
	  1200, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::AIR_PROCESSORS,0,
	  Equip::SLOT_CARGO, -1, { Equip::PLASTICS, Equip::INDUSTRIAL_MACHINERY },
	  2000, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::SLAVES,0,
	  Equip::SLOT_CARGO, -1, { },
	  23200, 1, 0, ECON_AGRICULTURE, true, 0
	},{
	  Lang::HAND_WEAPONS,0,
	  Equip::SLOT_CARGO, -1, { Equip::COMPUTERS },
	  12400, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::BATTLE_WEAPONS,0,
	  Equip::SLOT_CARGO, -1, { Equip::INDUSTRIAL_MACHINERY, Equip::METAL_ALLOYS },
	  22000, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::NERVE_GAS,0,
	  Equip::SLOT_CARGO, -1, {},
	  26500, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::NARCOTICS,0,
	  Equip::SLOT_CARGO, -1, {},
	  15700, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::MILITARY_FUEL,0,
	  Equip::SLOT_CARGO, -1, { Equip::HYDROGEN },
	  6000, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::RUBBISH,0,
	  Equip::SLOT_CARGO, -1, { },
	  -10, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::RADIOACTIVES,0,
	  Equip::SLOT_CARGO, -1, { },
	  -35, 1, 0, ECON_INDUSTRY, true, 0
	},{
	  Lang::MISSILE_UNGUIDED,0,
	  Equip::SLOT_MISSILE, -1, {},
	  3000, 0, 0, 0, true, 0
	},{
	  Lang::MISSILE_GUIDED,0,
	  Equip::SLOT_MISSILE, -1, {},
	  5000, 0, 0, 0, true, 0
	},{
	  Lang::MISSILE_SMART,0,
	  Equip::SLOT_MISSILE, -1, {},
	  9500, 0, 0, 0, true, 0
	},{
	  Lang::MISSILE_NAVAL,0,
	  Equip::SLOT_MISSILE, -1, {},
	  16000, 0, 0, 0, true, 0
	},{
	  Lang::ATMOSPHERIC_SHIELDING,
	  Lang::ATMOSPHERIC_SHIELDING_DESCRIPTION,
	  Equip::SLOT_ATMOSHIELD, -1, {},
	  20000, 0, 1, 0, true, 0
	},{
	  Lang::ECM_BASIC,
	  Lang::ECM_BASIC_DESCRIPTION,
	  Equip::SLOT_ECM, -1, {},
	  600000, 1, 2, 0, true, 5.0,
	},{
	  Lang::SCANNER,
	  Lang::SCANNER_DESCRIPTION,
	  Equip::SLOT_SCANNER, -1, {},
	  68000, 0, 0, 0, true, 0
	},{
	  Lang::ECM_ADVANCED,
	  Lang::ECM_ADVANCED_DESCRIPTION,
	  Equip::SLOT_ECM, -1, {},
	  1520000, 1, 3, 0, true, 5.0
	},{
	  Lang::UNOCCUPIED_CABIN,
	  Lang::UNOCCUPIED_CABIN_DESCRIPTION,
	  Equip::SLOT_CABIN, -1, {},
	  135000, 3, 1, 0, true, 5.0
	},{
	  Lang::PASSENGER_CABIN,0,
	  Equip::SLOT_CABIN, -1, {},
	  -135000, 3, 1, 0, false, 5.0
	},{
	  Lang::SHIELD_GENERATOR,
	  Lang::SHIELD_GENERATOR_DESCRIPTION,
	  Equip::SLOT_SHIELD, -1, {},
	  250000, 1, 1, 0, true, 5.0
	},{
	  Lang::LASER_COOLING_BOOSTER,
	  Lang::LASER_COOLING_BOOSTER_DESCRIPTION,
	  Equip::SLOT_LASERCOOLER, -1, {},
	  38000, 0, 2, 0, true, 0
	},{
	  Lang::CARGO_LIFE_SUPPORT,
	  Lang::CARGO_LIFE_SUPPORT_DESCRIPTION,
	  Equip::SLOT_CARGOLIFESUPPORT, -1, {},
	  70000, 1, 1, 0, true, 0
	},{
	  Lang::AUTOPILOT,
	  Lang::AUTOPILOT_DESCRIPTION,
	  Equip::SLOT_AUTOPILOT, -1, {},
	  140000, 0, 1, 0, true, 0
	},{
	  Lang::RADAR_MAPPER,
	  Lang::RADAR_MAPPER_DESCRIPTION,
	  Equip::SLOT_RADARMAPPER, -1, {},
	  90000, 0, 1, 0, true, 0
	},{
	  Lang::FUEL_SCOOP,
	  Lang::FUEL_SCOOP_DESCRIPTION,
	  Equip::SLOT_FUELSCOOP, -1, {},
	  350000, 1, 1, 0, true, 0
	},{
	  Lang::CARGO_SCOOP,
	  Lang::CARGO_SCOOP_DESCRIPTION,
	  Equip::SLOT_CARGOSCOOP, -1, {},
	  390000, 1, 1, 0, true, 0
	},{
	  Lang::HYPERCLOUD_ANALYZER,
	  Lang::HYPERCLOUD_ANALYZER_DESCRIPTION,
	  Equip::SLOT_HYPERCLOUD, -1, {},
	  150000, 0, 1, 0, true, 0
	},{
	  Lang::HULL_AUTOREPAIR,
	  Lang::HULL_AUTOREPAIR_DESCRIPTION,
	  Equip::SLOT_HULLAUTOREPAIR, -1, {},
	  1600000, 40, 1, 0, true, 0
	},{
	  Lang::SHIELD_ENERGY_BOOSTER,
	  Lang::SHIELD_ENERGY_BOOSTER_DESCRIPTION,
	  Equip::SLOT_ENERGYBOOSTER, -1, {},
	  1000000, 1, 2, 0, true, 0
	},{
	  Lang::DRIVE_CLASS1,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  70000, 1, 1, 0, true, 0
	},{
	  Lang::DRIVE_CLASS2,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  130000, 1, 2, 0, true, 0
	},{
	  Lang::DRIVE_CLASS3,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  250000, 1, 3, 0, true, 0
	},{
	  Lang::DRIVE_CLASS4,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  500000, 1, 4, 0, true, 0
	},{
	  Lang::DRIVE_CLASS5,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  1000000, 1, 5, 0, true, 0
	},{
	  Lang::DRIVE_CLASS6,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  2000000, 1, 6, 0, true, 0
	},{
	  Lang::DRIVE_CLASS7,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  3000000, 1, 7, 0, true, 0
	},{
	  Lang::DRIVE_CLASS8,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  6000000, 1, 8, 0, true, 0
	},{
	  Lang::DRIVE_CLASS9,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  12000000, 1, 9, 0, true, 0
	},{
	  Lang::DRIVE_CLASS10,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  24000000, 1, 10, 0, true, 0
	},{
	  Lang::DRIVE_CLASS11,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  48000000, 15, 30, 0, true, 0
	},{
	  Lang::DRIVE_CLASS12,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  96000000, 15, 34, 0, true, 0
	},{
	  Lang::DRIVE_CLASS13,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  200000000, 15, 38, 0, true, 0
	},{
	  Lang::DRIVE_CLASS14,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  300000000, 15, 42, 0, true, 0
	},{
	  Lang::DRIVE_CLASS15,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  400000000, 15, 46, 0, true, 0
	},{
	  Lang::DRIVE_CLASS16,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  500000000, 100, 90, 0, true, 0
	},{
	  Lang::DRIVE_CLASS17,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  600000000, 100, 110, 0, true, 0
	},{
	  Lang::DRIVE_CLASS18,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  800000000, 100, 130, 0, true, 0
	},{
	  Lang::DRIVE_CLASS19,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  1000000000, 100, 150, 0, true, 0
	},{
	  Lang::DRIVE_CLASS20,0,
	  Equip::SLOT_ENGINE, -1, {Equip::HYDROGEN},
	  1200000000, 100, 170, 0, true, 0
	},{
	  Lang::DRIVE_MIL1,0,
	  Equip::SLOT_ENGINE, -1, {Equip::MILITARY_FUEL},
	  2300000, 3, 1, 0, true, 0
	},{
	  Lang::DRIVE_MIL2,0,
	  Equip::SLOT_ENGINE, -1, {Equip::MILITARY_FUEL},
	  4700000, 8, 2, 0, true, 0
	},{
	  Lang::DRIVE_MIL3,0,
	  Equip::SLOT_ENGINE, -1, {Equip::MILITARY_FUEL},
	  8500000, 16, 3, 0, true, 0
	},{
	  Lang::DRIVE_MIL4,0,
	  Equip::SLOT_ENGINE, -1, {Equip::MILITARY_FUEL},
	  21400000, 30, 4, 0, true, 0
	},{
	  Lang::PULSECANNON_1MW,0,
	  Equip::SLOT_LASER, 1, {},
	  60000, 1, 1, 0, true, 0
	},{
	  Lang::PULSECANNON_DUAL_1MW,0,
	  Equip::SLOT_LASER, 2, {},
	  110000, 1, 2, 0, true, 0
	},{
	  Lang::PULSECANNON_2MW,0,
	  Equip::SLOT_LASER, 3, {},
	  100000, 1, 2, 0, true, 0
	},{
	  Lang::PULSECANNON_RAPID_2MW,0,
	  Equip::SLOT_LASER, 4, {},
	  180000, 1, 2, 0, true, 0
	},{
	  Lang::PULSECANNON_4MW,0,
	  Equip::SLOT_LASER, 5, {},
	  220000, 1, 4, 0, true, 0
	},{
	  Lang::PULSECANNON_10MW,0,
	  Equip::SLOT_LASER, 6, {},
	  490000, 2, 10, 0, true, 0
	},{
	  Lang::PULSECANNON_20MW,0,
	  Equip::SLOT_LASER, 7, {},
	  1200000, 3, 20, 0, true, 0
	},{
	  Lang::MININGCANNON_17MW,
	  Lang::MININGCANNON_17MW_DESCRIPTION,
	  Equip::SLOT_LASER, 8, {},
	  1060000, 4, 17, 0, true, 0
	},{
	  Lang::SMALL_PLASMA_ACCEL,0,
	  Equip::SLOT_LASER, 9, {},
	  12000000, 5, 50, 0, true, 0
	},{
	  Lang::LARGE_PLASMA_ACCEL,0,
	  Equip::SLOT_LASER, 10, {},
	  39000000, 8, 100, 0, true, 0
	},{
	  Lang::BASIC_FREIGHT_TELEPORTER,
	  Lang::BASIC_FREIGHT_TELEPORTER_DESCRIPTION,
	  Equip::SLOT_FREIGHTTELEPORTER, -1, {},
	  3900000, 1, 1, 0, true, 5.0f
	},{
	  Lang::ADVANCED_FREIGHT_TELEPORTER,
	  Lang::ADVANCED_FREIGHT_TELEPORTER_DESCRIPTION,
	  Equip::SLOT_FREIGHTTELEPORTER, -1, {},
	  7300000, 1, 2, 0, true, 5.0f
	}
};

const LaserType Equip::lasers[] = {
	{
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0,
		Color(0, 0, 0, 0),
	},{		// 1mw pulse
		8.0f, 3000.0f, 1000.0f, 0.25f, 30.0f, 5.0f, 0,
		Color(0.1f, 51, 255, 255),
	},{		// 1mw df pulse
		8.0f, 3000.0f, 1000.0f, 0.25f, 30.0f, 5.0f, Equip::LASER_DUAL,
		Color(0.1f, 51, 255, 255),
	},{	// 2mw pulse
		8.0f, 3000.0f, 2000.0f, 0.25f, 30.0f, 5.0f, 0,
		Color(255, 128, 51, 255),
	},{	// 2mw rf pulse
		8.0f, 3000.0f, 2000.0f, 0.13f, 30.0f, 5.0f, 0,
		Color(255, 128, 51, 255),
	},{		// 4mw pulse
		8.0f, 3000.0f, 4000.0f, 0.25f, 30.0f, 5.0f, 0,
		Color(255, 255, 51, 255),
	},{		// 10mw pulse
		8.0f, 3000.0f, 10000.0f, 0.25f, 30.0f, 5.0f, 0,
		Color(51, 255, 51, 255),
	},{		// 20mw pulse
		8.0f, 3000.0f, 20000.0f, 0.25f, 30.0f, 5.0f, 0,
		Color(0.1f, 51, 255, 255),
	},{		// 17mw mining
		8.0f, 3000.0f, 17000.0f, 2.0f, 30.0f, 5.0f, Equip::LASER_MINING,
		Color(51, 128, 255, 255),
	},{		// small plasma accel
		8.0f, 3000.0f, 50000.0f, 0.3f, 42.0f, 7.0f, 0,
		Color(51, 255, 255, 255),
	},{		// large plasma accel
		8.0f, 3000.0f, 100000.0f, 0.3f, 54.0f, 9.0f, 0,
		Color(128, 255, 255, 255),
	}
};
