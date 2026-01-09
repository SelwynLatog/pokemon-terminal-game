#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>

#include "PokemonDatabase.h"
#include "PokemonUtility.h"
#include "Player.h"
#include "Trainer.h"
#include "ItemDatabase.h"
#include "PokemonDisplay.h"
#include "PokemonMaps.h"

using namespace std;

/*****************************************************************************
 * GLOBAL DATABASE
 *****************************************************************************/
PokemonDatabase db;

/*****************************************************************************
 * BATTLE CONFIGURATION CONSTANTS
 *****************************************************************************/
namespace BattleConfig {
    // Damage calculation
    constexpr double LEVEL_FACTOR_NUMERATOR = 2.0;
    constexpr double LEVEL_FACTOR_DENOMINATOR = 5.0;
    constexpr double LEVEL_FACTOR_ADD = 2.0;
    constexpr double BASE_DAMAGE_DIVISOR = 50.0;
    constexpr double BASE_DAMAGE_ADD = 2.0;
    
    // Multipliers
    constexpr double STAB_BONUS = 1.5;
    constexpr double CRITICAL_HIT_MULTIPLIER = 2.0;
    constexpr double ABILITY_BOOST_MULTIPLIER = 1.5;
    constexpr double TYPE_SUPER_EFFECTIVE = 2.0;
    constexpr double TYPE_NOT_VERY_EFFECTIVE = 0.5;
    constexpr double TYPE_NO_EFFECT = 0.0;
    
    // Critical hit and variance
    constexpr int CRITICAL_HIT_CHANCE = 16;  // 1/16 chance
    constexpr int VARIANCE_MIN = 85;
    constexpr int VARIANCE_RANGE = 16;
    constexpr int VARIANCE_DIVISOR = 100;
    
    // Ability health threshold
    constexpr double ABILITY_TRIGGER_THRESHOLD = 1.0 / 3.0;
    
    // Experience calculation
    constexpr int BASE_EXP_PER_LEVEL = 8;
    constexpr int EXP_DIVISOR = 7;
    constexpr int STAT_EXP_DIVISOR = 6;
    constexpr int MIN_EXP_GAIN = 1;
    
    // Run mechanics
    constexpr double RUN_BASE_MULTIPLIER = 128.0;
    constexpr double RUN_ATTEMPT_BONUS = 30.0;
    constexpr double RUN_GUARANTEED_THRESHOLD = 255.0;
    constexpr int RUN_ROLL_MAX = 256;
}

namespace WildEncounterConfig {
    // Default level ranges
    constexpr int DEFAULT_MIN_LEVEL = 2;
    constexpr int DEFAULT_MAX_LEVEL = 5;
    
    // Route 101 specific
    constexpr int ROUTE_101_MIN_LEVEL = 2;
    constexpr int ROUTE_101_MAX_LEVEL = 3;
    
    // Starter battle
    constexpr int STARTER_BATTLE_MIN_LEVEL = 2;
    constexpr int STARTER_BATTLE_MAX_LEVEL = 3;
    constexpr int STARTER_LEVEL = 5;
    
    // Route-specific Pokemon indices
    namespace Route101 {
        constexpr int POOCHYENA_INDEX = 4;
        constexpr int ZIGZAGOON_INDEX = 3;
        constexpr int WURMPLE_INDEX = 7;
    }
    
    namespace Route102 {
        constexpr int POOCHYENA_INDEX = 4;
        constexpr int ZIGZAGOON_INDEX = 3;
        constexpr int WURMPLE_INDEX = 7;
        constexpr int LOTAD_INDEX = 16;
        constexpr int SEEDOT_INDEX = 17;
        constexpr int RALTS_INDEX = 10;
        constexpr int SURSKIT_INDEX = 18;
    }
    
  	namespace Route104 {
	    constexpr int POOCHYENA_INDEX = 4;
	    constexpr int WURMPLE_INDEX = 7;
	    constexpr int TAILLOW_INDEX = 8;
	    constexpr int WINGULL_INDEX = 9;
	    constexpr int MARILL_INDEX = 40;
	}
    
    namespace PetalburgWoods {
	    constexpr int POOCHYENA_INDEX = 4;
	    constexpr int WURMPLE_INDEX = 7;
	    constexpr int TAILLOW_INDEX = 8;
	    constexpr int SHROOMISH_INDEX = 11;
	    constexpr int SLAKOTH_INDEX = 12;
	    constexpr int SILCOON_INDEX=41;
	    constexpr int CASCOON_INDEX=42;
	}
	
	namespace GraniteCave {
		constexpr int GEODUDE_INDEX= 5;
		constexpr int NOSEPASS_INDEX= 6;
		constexpr int MAKUHITA_INDEX = 13;
		constexpr int ARON_INDEX= 14;
		constexpr int ZUBAT_INDEX= 44;
		constexpr int SABLEYE_INDEX= 45;
		constexpr int ABRA_INDEX= 46;
	}
	
	namespace Route110 {
		constexpr int POOCHYENA_INDEX = 4;
		constexpr int WINGULL_INDEX = 9;
		constexpr int ODDISH_INDEX = 48;
		constexpr int ELECTRIKE_INDEX= 15;
		constexpr int PLUSLE_INDEX=50;
		constexpr int MINUN_INDEX= 51;
		constexpr int GULPIN_INDEX= 52;
	}
	
	namespace Route111Desert {
	    constexpr int SANDSHREW_INDEX = 59;
	    constexpr int TRAPINCH_INDEX = 61;
	    constexpr int CACNEA_INDEX = 62;
	    constexpr int BALTOY_INDEX = 63;
	}
	
	namespace Route112 {
	    constexpr int NUMEL_INDEX = 66;
	    constexpr int MARILL_INDEX = 40;
	}
	
	namespace JaggedPass {
	    constexpr int MACHOP_INDEX = 43;
	    constexpr int NUMEL_INDEX = 66;
	    constexpr int SPOINK_INDEX = 67;
	}
	
	namespace Route118 {
	    constexpr int ZIGZAGOON_INDEX = 3;
	    constexpr int LINOONE_INDEX = 73;
	    constexpr int WINGULL_INDEX = 9;
	    constexpr int ELECTRIKE_INDEX = 15;
	    constexpr int MANECTRIC_INDEX = 57;
	    constexpr int KECLEON_INDEX = 75;
	}
	
	namespace FieryPath {
	    constexpr int MACHOP_INDEX = 43;
	    constexpr int GRIMER_INDEX = 83;
	    constexpr int KOFFING_INDEX = 84;
	    constexpr int SLUGMA_INDEX = 68;
	    constexpr int NUMEL_INDEX = 66;
	    constexpr int TORKOAL_INDEX = 39;
	}
	
	namespace Route119 {
	    constexpr int ODDISH_INDEX = 48;
	    constexpr int ZIGZAGOON_INDEX = 3;
	    constexpr int LINOONE_INDEX = 73;
	    constexpr int KECLEON_INDEX = 75;
	    constexpr int TROPIUS_INDEX = 85;
	}
	namespace MtPyreInterior {
	    constexpr int DUSKULL_INDEX = 98;
	    constexpr int SHUPPET_INDEX = 96;
	}
	
	namespace MtPyreExterior {
	    constexpr int VULPIX_INDEX = 100;
	    constexpr int WINGULL_INDEX = 9;
	    constexpr int SHUPPET_INDEX = 96;
	}
	
	namespace MtPyreSummit {
	    constexpr int SHUPPET_INDEX = 96;
	    constexpr int DUSKULL_INDEX = 98;
	    constexpr int CHIMECHO_INDEX = 102;
	}
	
	namespace MagmaHideout {
	    constexpr int GEODUDE_INDEX = 5;
	    constexpr int GRAVELER_INDEX = 104;
	    constexpr int TORKOAL_INDEX = 39;
	}
	
	namespace SeafloorCavern {
		constexpr int ZUBAT_INDEX = 44;
		constexpr int GOLBAT_INDEX= 106;
	}
	namespace VictoryRoad {
	    constexpr int ZUBAT_INDEX = 44;
	    constexpr int GOLBAT_INDEX = 106;
	    constexpr int WHISMUR_INDEX = 19;
	    constexpr int LOUDRED_INDEX = 80;
	    constexpr int MAKUHITA_INDEX = 13;
	    constexpr int HARIYAMA_INDEX = 123;
	    constexpr int ARON_INDEX = 14;
	    constexpr int LAIRON_INDEX = 124;
}
    // Starter Pokemon indices
    constexpr int STARTER_1_INDEX = 1;
    constexpr int STARTER_2_INDEX = 2;
    constexpr int STARTER_3_INDEX = 3;
}

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/
// Game flow
void newGame();
void chooseStarter(Player &player, const PokemonDatabase &db);
void firstBattle(Player &player, const PokemonDatabase &db, const string& wildName = "ZIGZAGOON", 
                 int minLevel = WildEncounterConfig::STARTER_BATTLE_MIN_LEVEL, 
                 int maxLevel = WildEncounterConfig::STARTER_BATTLE_MAX_LEVEL);
                 
void respawnAfterWhiteout(Player &player);

// Battle system
void startBattle(Player& player, const PokemonDatabase& db, bool isTrainerBattle = false, Trainer* trainer = nullptr);
void startBattleLoop(Player &player, Pokemon& playerPokemon, Pokemon& enemyPokemon, const PokemonDatabase &db, Trainer* trainer = nullptr);
void handlePlayerAction(Player &player, Pokemon &playerPokemon, Pokemon &enemyPokemon, 
                        const PokemonDatabase &db, Trainer *trainer, bool &turnEnded, bool &battleOver);
void handleEnemyAction(Pokemon &enemyPokemon, Pokemon &playerPokemon, const PokemonDatabase &db, Trainer *trainer);

// Combat mechanics
double getTypeEffectiveness(PokemonType moveType, PokemonType targetType);
double getAbilityMultiplier(const Pokemon& attacker, const Move& move, const PokemonDatabase& db);
double calculateDamage(Pokemon& attacker, Pokemon& defender, const Move& move, const PokemonDatabase& db);
void executeAttack(Pokemon& attacker, Pokemon& defender, const Pokemon& playerPokemon, 
                   const Pokemon& enemyPokemon, int moveIndex, const PokemonDatabase& db);

// Wild encounters
Pokemon getWildPokemon(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                       int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                       
Pokemon getWildPokemonRoute101(const PokemonDatabase& db, int minLevel = WildEncounterConfig::ROUTE_101_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::ROUTE_101_MAX_LEVEL);
                               
Pokemon getWildPokemonRoute102(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonRoute104(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonPetalburgWoods(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonGraniteCave(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonRoute110(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonRoute111Desert(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonRoute112(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonJaggedPass(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonRoute118(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonFieryPath(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonRoute119(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonMtPyreInterior(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonMtPyreExterior(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonMtPyreSummit(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonMagmaHideout(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonSeafloorCavern(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);
                               
Pokemon getWildPokemonVictoryRoad(const PokemonDatabase& db, int minLevel = WildEncounterConfig::DEFAULT_MIN_LEVEL, 
                               int maxLevel = WildEncounterConfig::DEFAULT_MAX_LEVEL);                                         
// Utilities
int calculateExpGain(const Pokemon& enemyPokemon);
bool canRun(Pokemon& playerPoke, Pokemon& enemyPoke, int runAttempts);
void resetForBattle(Pokemon& poke, const vector<Move>& moveList);
void displayBattleFrame(Pokemon& playerPokemon, Pokemon& enemyPokemon);

/*****************************************************************************
 * MAIN ENTRY POINT
 *****************************************************************************/
int main() {
    srand(static_cast<unsigned int>(time(0)));
    startKey();
    pokemonLogo();
    pressAnyKey();
    newGame();
    return 0;
}
/*****************************************************************************
 * GAME FLOW: NEW GAME
 *****************************************************************************/
void newGame() {
    int choice = titleScreenMenu();
    if (choice != 0) return;
    
    storyState = INTRO_TRUCK;
    flagVisitedMom = false;
    flagHasStarter = false;
    
    Player player;
    
    drBirchIntro(player);
    
    initMaps();   
    runMapLoop(player);
}

/*****************************************************************************
 * GAME FLOW: CHOOSE STARTER POKEMON
 *****************************************************************************/
void chooseStarter(Player &player, const PokemonDatabase &db) {
    const vector<int> starterIDs = {
        WildEncounterConfig::STARTER_1_INDEX,
        WildEncounterConfig::STARTER_2_INDEX,
        WildEncounterConfig::STARTER_3_INDEX
    };
    
    vector<Pokemon> starters;
    for (int id : starterIDs) {
        starters.push_back(db.pokedex[id - 1]);
    }

    int choice = chooseStarterMenu(starters);

    player.currentPokemon.push_back(starters[choice]);
    Pokemon &chosenStarter = player.currentPokemon.back();

    chosenStarter.level = WildEncounterConfig::STARTER_LEVEL;
    chosenStarter.currentHP = chosenStarter.baseHP;
    chosenStarter.initializePP(db.moveList);
    chosenStarter.sanitizeMoves(db.moveList);

    showDialogue("You received " + chosenStarter.pokemonName + "!");
    pressAnyKey();
}

/*****************************************************************************
 * GAME FLOW: FIRST BATTLE
 *****************************************************************************/
void firstBattle(Player &player, const PokemonDatabase &db, 
                 const string& wildName, int minLevel, int maxLevel) {

    const Pokemon* wildPtr = db.findPokemon(wildName);
    if (!wildPtr) {
        cout << "Error: " << wildName << " not found in DB!" << endl;
        return;
    }

    static Pokemon wildPokemon;
    wildPokemon = *wildPtr;
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.scaleStatsToLevel();
    wildPokemon.initializePP(db.moveList);
    wildPokemon.sanitizeMoves(db.moveList);

    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    pressAnyKey();

    if (player.currentPokemon.empty()) {
        cout << "You have no Pokemon to fight with!" << endl;
        return;
    }

    Pokemon* playerActive = nullptr;
    for (auto &p : player.currentPokemon) {
        if (p.currentHP > 0) {
            playerActive = &p;
            break;
        }
    }

    if (!playerActive) {
        cout << "You have no usable Pokemon!" << endl;
        return;
    }

    startBattleLoop(player, *playerActive, wildPokemon, db);
}

/*****************************************************************************
 * TYPE EFFECTIVENESS CHART
 *****************************************************************************/
double getTypeEffectiveness(PokemonType moveType, PokemonType targetType) {
    if (targetType == PokemonType::NONE) {
        return 1.0;
    }

    switch (moveType) {
        case PokemonType::FIRE:
            if (targetType == PokemonType::GRASS || targetType == PokemonType::ICE || 
                targetType == PokemonType::BUG || targetType == PokemonType::STEEL) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::FIRE || targetType == PokemonType::WATER || 
                targetType == PokemonType::ROCK || targetType == PokemonType::DRAGON) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            break;

        case PokemonType::WATER:
            if (targetType == PokemonType::FIRE || targetType == PokemonType::GROUND || 
                targetType == PokemonType::ROCK) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::WATER || targetType == PokemonType::GRASS || 
                targetType == PokemonType::DRAGON) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            break;

        case PokemonType::GRASS:
            if (targetType == PokemonType::WATER || targetType == PokemonType::GROUND || 
                targetType == PokemonType::ROCK) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::FIRE || targetType == PokemonType::GRASS || 
                targetType == PokemonType::POISON || targetType == PokemonType::FLYING || 
                targetType == PokemonType::BUG || targetType == PokemonType::DRAGON || 
                targetType == PokemonType::STEEL) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            break;

        case PokemonType::ELECTRIC:
            if (targetType == PokemonType::WATER || targetType == PokemonType::FLYING) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::ELECTRIC || targetType == PokemonType::GRASS || 
                targetType == PokemonType::DRAGON) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            if (targetType == PokemonType::GROUND) 
                return BattleConfig::TYPE_NO_EFFECT;
            break;

        case PokemonType::ICE:
            if (targetType == PokemonType::GRASS || targetType == PokemonType::GROUND || 
                targetType == PokemonType::FLYING || targetType == PokemonType::DRAGON) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::FIRE || targetType == PokemonType::WATER || 
                targetType == PokemonType::ICE || targetType == PokemonType::STEEL) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            break;

        case PokemonType::FIGHTING:
            if (targetType == PokemonType::NORMAL || targetType == PokemonType::ICE || 
                targetType == PokemonType::ROCK || targetType == PokemonType::DARK || 
                targetType == PokemonType::STEEL) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::POISON || targetType == PokemonType::FLYING || 
                targetType == PokemonType::PSYCHIC || targetType == PokemonType::BUG || 
                targetType == PokemonType::FAIRY) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            if (targetType == PokemonType::GHOST) 
                return BattleConfig::TYPE_NO_EFFECT;
            break;

        case PokemonType::GROUND:
            if (targetType == PokemonType::FIRE || targetType == PokemonType::ELECTRIC || 
                targetType == PokemonType::POISON || targetType == PokemonType::ROCK || 
                targetType == PokemonType::STEEL) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::GRASS || targetType == PokemonType::BUG) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            if (targetType == PokemonType::FLYING) 
                return BattleConfig::TYPE_NO_EFFECT;
            break;

        case PokemonType::FLYING:
            if (targetType == PokemonType::GRASS || targetType == PokemonType::FIGHTING || 
                targetType == PokemonType::BUG) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::ELECTRIC || targetType == PokemonType::ROCK || 
                targetType == PokemonType::STEEL) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            break;

        case PokemonType::PSYCHIC:
            if (targetType == PokemonType::FIGHTING || targetType == PokemonType::POISON) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::PSYCHIC || targetType == PokemonType::STEEL) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            if (targetType == PokemonType::DARK) 
                return BattleConfig::TYPE_NO_EFFECT;
            break;

        case PokemonType::BUG:
            if (targetType == PokemonType::GRASS || targetType == PokemonType::PSYCHIC || 
                targetType == PokemonType::DARK) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::FIRE || targetType == PokemonType::FIGHTING || 
                targetType == PokemonType::POISON || targetType == PokemonType::FLYING || 
                targetType == PokemonType::GHOST || targetType == PokemonType::STEEL || 
                targetType == PokemonType::FAIRY) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            break;

        case PokemonType::ROCK:
            if (targetType == PokemonType::FIRE || targetType == PokemonType::ICE || 
                targetType == PokemonType::FLYING || targetType == PokemonType::BUG) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::FIGHTING || targetType == PokemonType::GROUND || 
                targetType == PokemonType::STEEL) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            break;

        case PokemonType::GHOST:
            if (targetType == PokemonType::PSYCHIC || targetType == PokemonType::GHOST) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::DARK) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            if (targetType == PokemonType::NORMAL) 
                return BattleConfig::TYPE_NO_EFFECT;
            break;

        case PokemonType::DRAGON:
            if (targetType == PokemonType::DRAGON) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::STEEL) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            if (targetType == PokemonType::FAIRY) 
                return BattleConfig::TYPE_NO_EFFECT;
            break;

        case PokemonType::DARK:
            if (targetType == PokemonType::PSYCHIC || targetType == PokemonType::GHOST) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::FIGHTING || targetType == PokemonType::DARK) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            break;

        case PokemonType::STEEL:
            if (targetType == PokemonType::ICE || targetType == PokemonType::ROCK || 
                targetType == PokemonType::FAIRY) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::FIRE || targetType == PokemonType::WATER || 
                targetType == PokemonType::ELECTRIC || targetType == PokemonType::STEEL) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            break;

        case PokemonType::FAIRY:
            if (targetType == PokemonType::FIGHTING || targetType == PokemonType::DRAGON || 
                targetType == PokemonType::DARK) 
                return BattleConfig::TYPE_SUPER_EFFECTIVE;
            if (targetType == PokemonType::FIRE || targetType == PokemonType::POISON || 
                targetType == PokemonType::STEEL) 
                return BattleConfig::TYPE_NOT_VERY_EFFECTIVE;
            break;

        default: 
            return 1.0;
    }

    return 1.0;
}

/*****************************************************************************
 * ABILITY MULTIPLIER CALCULATION
 *****************************************************************************/
double getAbilityMultiplier(const Pokemon& attacker, const Move& move, const PokemonDatabase& db) {
    double multiplier = 1.0;
    const Ability& ability = db.abilityList[attacker.abilityID];
    double healthRatio = (double)attacker.currentHP / attacker.baseHP;

    // Type-boosting abilities (Overgrow, Blaze, Torrent)
    if ((ability.abilityName == "OVERGROW" && move.type == PokemonType::GRASS) ||
        (ability.abilityName == "BLAZE" && move.type == PokemonType::FIRE) ||
        (ability.abilityName == "TORRENT" && move.type == PokemonType::WATER)) {
        
        if (healthRatio < BattleConfig::ABILITY_TRIGGER_THRESHOLD) {
            multiplier = BattleConfig::ABILITY_BOOST_MULTIPLIER;
        }
    }

    return multiplier;
}

/*****************************************************************************
 * DAMAGE CALCULATION FORMULA
 *****************************************************************************/
double calculateDamage(Pokemon& attacker, Pokemon& defender, const Move& move, const PokemonDatabase& db) {
    if (move.power == 0) return 0;

    int attackStat;
    int defenseStat;
    
    if (move.category == MoveCategory::PHYSICAL) {
        attackStat = attacker.baseAttack;
        defenseStat = defender.baseDefense;
    } else if (move.category == MoveCategory::SPECIAL) {
        attackStat = attacker.baseSPAttack;
        defenseStat = defender.baseSPDefense;
    } else {
        return 0; // Status moves - no dmg
    }

    // Emerald damage formula: ((2 * Level / 5 + 2) * Power * Attack / Defense) / 50 + 2
    double levelFactor = (2.0 * attacker.level / 5.0) + 2.0;
    double baseDamage = (levelFactor * move.power * attackStat / (double)defenseStat) / 50.0 + 2.0;

    // STAB (Same Type Attack Bonus) - 1.5x
    double stab = (move.type == attacker.primaryType || move.type == attacker.secondaryType) 
                  ? 1.5 : 1.0;

    // Type effectiveness
    double effectiveness = getTypeEffectiveness(move.type, defender.primaryType);
    if (defender.secondaryType != PokemonType::NONE) {
        effectiveness *= getTypeEffectiveness(move.type, defender.secondaryType);
    }

    // Critical hit - 2x in Gen 3 
    bool isCritical = (rand() % 16 == 0); // 1/16 base crit rate
    double critMultiplier = isCritical ? 2.0 : 1.0;

    // Random variance (85-100%)
    double variance = (85 + rand() % 16) / 100.0;

    // Ability multiplier (Overgrow, Blaze, Torrent)
    double abilityMultiplier = getAbilityMultiplier(attacker, move, db);

    // Final damage calculation
    double damage = baseDamage * stab * effectiveness * critMultiplier * abilityMultiplier * variance;

    // Minimum damage is 1
    if (damage < 1 && effectiveness > 0) {
        damage = 1;
    }

    return damage;
}

/*WEIGHTED ENCOUNTER RATE IS BASED ON FREQUENCY:
  Probability= num of occurences/total count X100
  { A, A, B, C } -> A = 50%, B = 25%, C = 25%
*/

/*****************************************************************************
 * WILD ENCOUNTER: GENERIC
 *****************************************************************************/
Pokemon getWildPokemon(const PokemonDatabase& db, int minLevel, int maxLevel) {
    unsigned short index = rand() % db.pokedex.size();
    Pokemon wildPokemon = db.pokedex[index];
    
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: ROUTE 101
 *****************************************************************************/
Pokemon getWildPokemonRoute101(const PokemonDatabase& db, int minLevel, int maxLevel) {
	
    const vector<int> route101Pokemon = {
	    WildEncounterConfig::Route101::POOCHYENA_INDEX, 
	    WildEncounterConfig::Route101::ZIGZAGOON_INDEX, 
	    WildEncounterConfig::Route101::ZIGZAGOON_INDEX,
	    WildEncounterConfig::Route101::ZIGZAGOON_INDEX,
	    WildEncounterConfig::Route101::WURMPLE_INDEX,
	    WildEncounterConfig::Route101::WURMPLE_INDEX,
	    WildEncounterConfig::Route101::WURMPLE_INDEX,
	    WildEncounterConfig::Route101::WURMPLE_INDEX,
	    WildEncounterConfig::Route101::WURMPLE_INDEX
	};
    unsigned short randIndex = rand() % route101Pokemon.size();
    int pokemonIndex = route101Pokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: ROUTE 102
 *****************************************************************************/
Pokemon getWildPokemonRoute102(const PokemonDatabase& db, int minLevel, int maxLevel) {
	
    const vector<int> route102Pokemon = {
        WildEncounterConfig::Route102::POOCHYENA_INDEX,
        WildEncounterConfig::Route102::POOCHYENA_INDEX,
        WildEncounterConfig::Route102::ZIGZAGOON_INDEX,
        WildEncounterConfig::Route102::ZIGZAGOON_INDEX,
        WildEncounterConfig::Route102::WURMPLE_INDEX,
        WildEncounterConfig::Route102::LOTAD_INDEX,
        WildEncounterConfig::Route102::SEEDOT_INDEX,
        WildEncounterConfig::Route102::RALTS_INDEX,
        WildEncounterConfig::Route102::SURSKIT_INDEX
    };
    
    unsigned short randIndex = rand() % route102Pokemon.size();
    int pokemonIndex = route102Pokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
     wildPokemon.scaleStatsToLevel();    
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: ROUTE 104
 *****************************************************************************/
Pokemon getWildPokemonRoute104(const PokemonDatabase& db, int minLevel, int maxLevel) {
   
    const vector<int> route104Pokemon = {
        WildEncounterConfig::Route104::POOCHYENA_INDEX,
        WildEncounterConfig::Route104::POOCHYENA_INDEX,
        WildEncounterConfig::Route104::WURMPLE_INDEX,
        WildEncounterConfig::Route104::WURMPLE_INDEX,
        WildEncounterConfig::Route104::TAILLOW_INDEX,
        WildEncounterConfig::Route104::TAILLOW_INDEX,
        WildEncounterConfig::Route104::WINGULL_INDEX,
        WildEncounterConfig::Route104::MARILL_INDEX
    };
    
    unsigned short randIndex = rand() % route104Pokemon.size();
    int pokemonIndex = route104Pokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
     wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: PETALBURG WOODS
 *****************************************************************************/
Pokemon getWildPokemonPetalburgWoods(const PokemonDatabase& db, int minLevel, int maxLevel) {
   
    const vector<int> petalburgWoodsPokemon = {
        WildEncounterConfig::PetalburgWoods::POOCHYENA_INDEX,
        WildEncounterConfig::PetalburgWoods::POOCHYENA_INDEX,
        WildEncounterConfig::PetalburgWoods::WURMPLE_INDEX,
        WildEncounterConfig::PetalburgWoods::WURMPLE_INDEX,
        WildEncounterConfig::PetalburgWoods::TAILLOW_INDEX,
        WildEncounterConfig::PetalburgWoods::TAILLOW_INDEX,
        WildEncounterConfig::PetalburgWoods::SHROOMISH_INDEX,
        WildEncounterConfig::PetalburgWoods::SLAKOTH_INDEX,
        WildEncounterConfig::PetalburgWoods::SILCOON_INDEX,
        WildEncounterConfig::PetalburgWoods::SILCOON_INDEX,
        WildEncounterConfig::PetalburgWoods::CASCOON_INDEX,
        WildEncounterConfig::PetalburgWoods::CASCOON_INDEX
    };
    
    unsigned short randIndex = rand() % petalburgWoodsPokemon.size();
    int pokemonIndex = petalburgWoodsPokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: GRANITE CAVE
 *****************************************************************************/
Pokemon getWildPokemonGraniteCave(const PokemonDatabase& db, int minLevel, int maxLevel){
    
    const vector<int> graniteCavePokemon = {
        WildEncounterConfig::GraniteCave::MAKUHITA_INDEX,
        WildEncounterConfig::GraniteCave::MAKUHITA_INDEX,
        WildEncounterConfig::GraniteCave::MAKUHITA_INDEX,
        WildEncounterConfig::GraniteCave::MAKUHITA_INDEX,
        WildEncounterConfig::GraniteCave::MAKUHITA_INDEX,
        WildEncounterConfig::GraniteCave::ZUBAT_INDEX,
        WildEncounterConfig::GraniteCave::ZUBAT_INDEX,
        WildEncounterConfig::GraniteCave::ZUBAT_INDEX,
        WildEncounterConfig::GraniteCave::GEODUDE_INDEX,
        WildEncounterConfig::GraniteCave::ABRA_INDEX,
        WildEncounterConfig::GraniteCave::SABLEYE_INDEX,
        WildEncounterConfig::GraniteCave::NOSEPASS_INDEX
    };
    
    unsigned short randIndex = rand() % graniteCavePokemon.size();
    int pokemonIndex = graniteCavePokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: ROUTE110
 *****************************************************************************/
Pokemon getWildPokemonRoute110(const PokemonDatabase& db, int minLevel, int maxLevel){
 
    const vector<int> route110Pokemon = {
        WildEncounterConfig::Route110::ELECTRIKE_INDEX,
        WildEncounterConfig::Route110::ELECTRIKE_INDEX,
        WildEncounterConfig::Route110::ELECTRIKE_INDEX,
        WildEncounterConfig::Route110::POOCHYENA_INDEX,
        WildEncounterConfig::Route110::POOCHYENA_INDEX,
        WildEncounterConfig::Route110::PLUSLE_INDEX,
        WildEncounterConfig::Route110::PLUSLE_INDEX,
        WildEncounterConfig::Route110::MINUN_INDEX,
        WildEncounterConfig::Route110::MINUN_INDEX,
        WildEncounterConfig::Route110::GULPIN_INDEX,
        WildEncounterConfig::Route110::GULPIN_INDEX,
        WildEncounterConfig::Route110::ODDISH_INDEX,
        WildEncounterConfig::Route110::WINGULL_INDEX,
    };
    
    unsigned short randIndex = rand() % route110Pokemon.size();
    int pokemonIndex = route110Pokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: ROUTE 111 desert
 *****************************************************************************/
Pokemon getWildPokemonRoute111Desert(const PokemonDatabase& db, int minLevel, int maxLevel) {
	
    const vector<int> route111DesertPokemon = {
        WildEncounterConfig::Route111Desert::SANDSHREW_INDEX,
        WildEncounterConfig::Route111Desert::SANDSHREW_INDEX,
        WildEncounterConfig::Route111Desert::SANDSHREW_INDEX,
        WildEncounterConfig::Route111Desert::SANDSHREW_INDEX,
        WildEncounterConfig::Route111Desert::SANDSHREW_INDEX,
        WildEncounterConfig::Route111Desert::SANDSHREW_INDEX,
        WildEncounterConfig::Route111Desert::SANDSHREW_INDEX,
        WildEncounterConfig::Route111Desert::TRAPINCH_INDEX,
        WildEncounterConfig::Route111Desert::TRAPINCH_INDEX,
        WildEncounterConfig::Route111Desert::TRAPINCH_INDEX,
        WildEncounterConfig::Route111Desert::TRAPINCH_INDEX,
        WildEncounterConfig::Route111Desert::TRAPINCH_INDEX,
        WildEncounterConfig::Route111Desert::TRAPINCH_INDEX,
        WildEncounterConfig::Route111Desert::TRAPINCH_INDEX,
        WildEncounterConfig::Route111Desert::BALTOY_INDEX,
        WildEncounterConfig::Route111Desert::BALTOY_INDEX,
        WildEncounterConfig::Route111Desert::BALTOY_INDEX,
        WildEncounterConfig::Route111Desert::BALTOY_INDEX,
        WildEncounterConfig::Route111Desert::BALTOY_INDEX,
        WildEncounterConfig::Route111Desert::CACNEA_INDEX
    };
    
    unsigned short randIndex = rand() % route111DesertPokemon.size();
    int pokemonIndex = route111DesertPokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: ROUTE 112
 *****************************************************************************/
Pokemon getWildPokemonRoute112(const PokemonDatabase& db, int minLevel, int maxLevel) {

    const vector<int> route112Pokemon = {
        WildEncounterConfig::Route112::NUMEL_INDEX,
        WildEncounterConfig::Route112::NUMEL_INDEX,
        WildEncounterConfig::Route112::NUMEL_INDEX,
        WildEncounterConfig::Route112::MARILL_INDEX
    };
    
    unsigned short randIndex = rand() % route112Pokemon.size();
    int pokemonIndex = route112Pokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: JAGGED PASS
 *****************************************************************************/
Pokemon getWildPokemonJaggedPass(const PokemonDatabase& db, int minLevel, int maxLevel) {
	
    const vector<int> jaggedPassPokemon = {
        WildEncounterConfig::JaggedPass::NUMEL_INDEX,
        WildEncounterConfig::JaggedPass::NUMEL_INDEX,
        WildEncounterConfig::JaggedPass::NUMEL_INDEX,
        WildEncounterConfig::JaggedPass::NUMEL_INDEX,
        WildEncounterConfig::JaggedPass::NUMEL_INDEX,
        WildEncounterConfig::JaggedPass::NUMEL_INDEX,
        WildEncounterConfig::JaggedPass::NUMEL_INDEX,
        WildEncounterConfig::JaggedPass::NUMEL_INDEX,
        WildEncounterConfig::JaggedPass::NUMEL_INDEX,
        WildEncounterConfig::JaggedPass::NUMEL_INDEX,
        WildEncounterConfig::JaggedPass::NUMEL_INDEX,
        WildEncounterConfig::JaggedPass::MACHOP_INDEX,
        WildEncounterConfig::JaggedPass::MACHOP_INDEX,
        WildEncounterConfig::JaggedPass::MACHOP_INDEX,
        WildEncounterConfig::JaggedPass::MACHOP_INDEX,
        WildEncounterConfig::JaggedPass::MACHOP_INDEX,
        WildEncounterConfig::JaggedPass::SPOINK_INDEX,
        WildEncounterConfig::JaggedPass::SPOINK_INDEX,
        WildEncounterConfig::JaggedPass::SPOINK_INDEX,
        WildEncounterConfig::JaggedPass::SPOINK_INDEX
    };
    
    unsigned short randIndex = rand() % jaggedPassPokemon.size();
    int pokemonIndex = jaggedPassPokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: ROUTE 118
 *****************************************************************************/
Pokemon getWildPokemonRoute118(const PokemonDatabase& db, int minLevel, int maxLevel) {
	
    const vector<int> route118Pokemon = {
        WildEncounterConfig::Route118::ZIGZAGOON_INDEX,
        WildEncounterConfig::Route118::ZIGZAGOON_INDEX,
        WildEncounterConfig::Route118::ZIGZAGOON_INDEX,
        WildEncounterConfig::Route118::ELECTRIKE_INDEX,
        WildEncounterConfig::Route118::ELECTRIKE_INDEX,
        WildEncounterConfig::Route118::ELECTRIKE_INDEX,
        WildEncounterConfig::Route118::WINGULL_INDEX,
        WildEncounterConfig::Route118::WINGULL_INDEX,
        WildEncounterConfig::Route118::LINOONE_INDEX,
        WildEncounterConfig::Route118::MANECTRIC_INDEX,
        WildEncounterConfig::Route118::KECLEON_INDEX
    };
    
    unsigned short randIndex = rand() % route118Pokemon.size();
    int pokemonIndex = route118Pokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: FIERY PATH
 *****************************************************************************/
Pokemon getWildPokemonFieryPath(const PokemonDatabase& db, int minLevel, int maxLevel) {

    const vector<int> fieryPathPokemon = {
        WildEncounterConfig::FieryPath::NUMEL_INDEX,
        WildEncounterConfig::FieryPath::NUMEL_INDEX,
        WildEncounterConfig::FieryPath::NUMEL_INDEX,
        WildEncounterConfig::FieryPath::GRIMER_INDEX,
        WildEncounterConfig::FieryPath::GRIMER_INDEX,
        WildEncounterConfig::FieryPath::GRIMER_INDEX,
        WildEncounterConfig::FieryPath::KOFFING_INDEX,
        WildEncounterConfig::FieryPath::KOFFING_INDEX,
        WildEncounterConfig::FieryPath::KOFFING_INDEX,
        WildEncounterConfig::FieryPath::TORKOAL_INDEX,
        WildEncounterConfig::FieryPath::TORKOAL_INDEX,
        WildEncounterConfig::FieryPath::MACHOP_INDEX,
        WildEncounterConfig::FieryPath::SLUGMA_INDEX
    };
    
    unsigned short randIndex = rand() % fieryPathPokemon.size();
    int pokemonIndex = fieryPathPokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: ROUTE 119
 *****************************************************************************/
Pokemon getWildPokemonRoute119(const PokemonDatabase& db, int minLevel, int maxLevel) {

    const vector<int> route119Pokemon = {
        WildEncounterConfig::Route119::ODDISH_INDEX,
        WildEncounterConfig::Route119::ODDISH_INDEX,
        WildEncounterConfig::Route119::ODDISH_INDEX,
        WildEncounterConfig::Route119::ZIGZAGOON_INDEX,
        WildEncounterConfig::Route119::ZIGZAGOON_INDEX,
        WildEncounterConfig::Route119::ZIGZAGOON_INDEX,
        WildEncounterConfig::Route119::LINOONE_INDEX,
        WildEncounterConfig::Route119::LINOONE_INDEX,
        WildEncounterConfig::Route119::LINOONE_INDEX,
        WildEncounterConfig::Route119::TROPIUS_INDEX,
        WildEncounterConfig::Route119::KECLEON_INDEX
    };
    
    unsigned short randIndex = rand() % route119Pokemon.size();
    int pokemonIndex = route119Pokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: MT. PYRE INTERIOR
 *****************************************************************************/
Pokemon getWildPokemonMtPyreInterior(const PokemonDatabase& db, int minLevel, int maxLevel) {

    const vector<int> mtPyreInteriorPokemon = {
        WildEncounterConfig::MtPyreInterior::DUSKULL_INDEX,
        WildEncounterConfig::MtPyreInterior::DUSKULL_INDEX,
        WildEncounterConfig::MtPyreInterior::DUSKULL_INDEX,
        WildEncounterConfig::MtPyreInterior::DUSKULL_INDEX,
        WildEncounterConfig::MtPyreInterior::DUSKULL_INDEX,
        WildEncounterConfig::MtPyreInterior::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreInterior::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreInterior::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreInterior::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreInterior::SHUPPET_INDEX
    };
    
    unsigned short randIndex = rand() % mtPyreInteriorPokemon.size();
    int pokemonIndex = mtPyreInteriorPokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: MT. PYRE EXTERIOR
 *****************************************************************************/
Pokemon getWildPokemonMtPyreExterior(const PokemonDatabase& db, int minLevel, int maxLevel) {

    const vector<int> mtPyreExteriorPokemon = {
        WildEncounterConfig::MtPyreExterior::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreExterior::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreExterior::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreExterior::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreExterior::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreExterior::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreExterior::VULPIX_INDEX,
        WildEncounterConfig::MtPyreExterior::VULPIX_INDEX,
        WildEncounterConfig::MtPyreExterior::VULPIX_INDEX,
        WildEncounterConfig::MtPyreExterior::WINGULL_INDEX
    };
    
    unsigned short randIndex = rand() % mtPyreExteriorPokemon.size();
    int pokemonIndex = mtPyreExteriorPokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: MT. PYRE SUMMIT
 *****************************************************************************/
Pokemon getWildPokemonMtPyreSummit(const PokemonDatabase& db, int minLevel, int maxLevel) {
   
    const vector<int> mtPyreSummitPokemon = {
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::SHUPPET_INDEX,
        WildEncounterConfig::MtPyreSummit::DUSKULL_INDEX,
        WildEncounterConfig::MtPyreSummit::DUSKULL_INDEX,
        WildEncounterConfig::MtPyreSummit::CHIMECHO_INDEX
    };
    
    unsigned short randIndex = rand() % mtPyreSummitPokemon.size();
    int pokemonIndex = mtPyreSummitPokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: MAGMA HIDEOUT
 *****************************************************************************/
Pokemon getWildPokemonMagmaHideout(const PokemonDatabase& db, int minLevel, int maxLevel) {

    const vector<int> magmaHideoutPokemon = {
        WildEncounterConfig::MagmaHideout::GEODUDE_INDEX,
        WildEncounterConfig::MagmaHideout::GEODUDE_INDEX,
        WildEncounterConfig::MagmaHideout::GEODUDE_INDEX,
        WildEncounterConfig::MagmaHideout::GEODUDE_INDEX,
        WildEncounterConfig::MagmaHideout::GEODUDE_INDEX,
        WildEncounterConfig::MagmaHideout::GEODUDE_INDEX,
        WildEncounterConfig::MagmaHideout::GEODUDE_INDEX,
        WildEncounterConfig::MagmaHideout::GEODUDE_INDEX,
        WildEncounterConfig::MagmaHideout::GEODUDE_INDEX,
        WildEncounterConfig::MagmaHideout::GEODUDE_INDEX,
        WildEncounterConfig::MagmaHideout::GEODUDE_INDEX,
        WildEncounterConfig::MagmaHideout::GRAVELER_INDEX,
        WildEncounterConfig::MagmaHideout::GRAVELER_INDEX,
        WildEncounterConfig::MagmaHideout::GRAVELER_INDEX,
        WildEncounterConfig::MagmaHideout::TORKOAL_INDEX,
        WildEncounterConfig::MagmaHideout::TORKOAL_INDEX,
        WildEncounterConfig::MagmaHideout::TORKOAL_INDEX,
        WildEncounterConfig::MagmaHideout::TORKOAL_INDEX,
        WildEncounterConfig::MagmaHideout::TORKOAL_INDEX,
        WildEncounterConfig::MagmaHideout::TORKOAL_INDEX
    };
    
    unsigned short randIndex = rand() % magmaHideoutPokemon.size();
    int pokemonIndex = magmaHideoutPokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * WILD ENCOUNTER: SEAFLOOR CAVERN
 *****************************************************************************/
 Pokemon getWildPokemonSeafloorCavern(const PokemonDatabase& db, int minLevel, int maxLevel) {
   
    const vector<int> seafloorCavernPokemon = {
        WildEncounterConfig::SeafloorCavern::ZUBAT_INDEX,
        WildEncounterConfig::SeafloorCavern::ZUBAT_INDEX,
        WildEncounterConfig::SeafloorCavern::ZUBAT_INDEX,
        WildEncounterConfig::SeafloorCavern::ZUBAT_INDEX,
        WildEncounterConfig::SeafloorCavern::ZUBAT_INDEX,
        WildEncounterConfig::SeafloorCavern::ZUBAT_INDEX,
        WildEncounterConfig::SeafloorCavern::ZUBAT_INDEX,
        WildEncounterConfig::SeafloorCavern::ZUBAT_INDEX,
        WildEncounterConfig::SeafloorCavern::ZUBAT_INDEX,
        WildEncounterConfig::SeafloorCavern::GOLBAT_INDEX
    };
    
    unsigned short randIndex = rand() % seafloorCavernPokemon.size();
    int pokemonIndex = seafloorCavernPokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}
 
 /*****************************************************************************
 * WILD ENCOUNTER: VICTORY ROAD
 *****************************************************************************/
Pokemon getWildPokemonVictoryRoad(const PokemonDatabase& db, int minLevel, int maxLevel) {
	
    const vector<int> victoryRoadPokemon = {
        WildEncounterConfig::VictoryRoad::GOLBAT_INDEX,
        WildEncounterConfig::VictoryRoad::GOLBAT_INDEX,
        WildEncounterConfig::VictoryRoad::GOLBAT_INDEX,
        WildEncounterConfig::VictoryRoad::GOLBAT_INDEX,
        WildEncounterConfig::VictoryRoad::GOLBAT_INDEX,
        WildEncounterConfig::VictoryRoad::HARIYAMA_INDEX,
        WildEncounterConfig::VictoryRoad::HARIYAMA_INDEX,
        WildEncounterConfig::VictoryRoad::HARIYAMA_INDEX,
        WildEncounterConfig::VictoryRoad::HARIYAMA_INDEX,
        WildEncounterConfig::VictoryRoad::HARIYAMA_INDEX,
        WildEncounterConfig::VictoryRoad::ZUBAT_INDEX,
        WildEncounterConfig::VictoryRoad::ZUBAT_INDEX,
        WildEncounterConfig::VictoryRoad::MAKUHITA_INDEX,
        WildEncounterConfig::VictoryRoad::MAKUHITA_INDEX,
        WildEncounterConfig::VictoryRoad::LOUDRED_INDEX,
        WildEncounterConfig::VictoryRoad::LOUDRED_INDEX,
        WildEncounterConfig::VictoryRoad::LAIRON_INDEX,
        WildEncounterConfig::VictoryRoad::LAIRON_INDEX,
        WildEncounterConfig::VictoryRoad::WHISMUR_INDEX,
        WildEncounterConfig::VictoryRoad::ARON_INDEX
    };
    
    unsigned short randIndex = rand() % victoryRoadPokemon.size();
    int pokemonIndex = victoryRoadPokemon[randIndex];

    Pokemon wildPokemon = db.pokedex[pokemonIndex];
    wildPokemon.level = minLevel + rand() % (maxLevel - minLevel + 1);
    wildPokemon.scaleStatsToLevel();
    wildPokemon.currentHP = wildPokemon.baseHP;
    wildPokemon.initializePP(db.moveList);
    
    string encounterMessage = "A wild " + wildPokemon.pokemonName + " appeared!";
    showDialogue(encounterMessage);
    
    return wildPokemon;
}

/*****************************************************************************
 * RUN AWAY MECHANICS
 *****************************************************************************/
bool canRun(Pokemon& playerPoke, Pokemon& enemyPoke, int runAttempts) {
    double chance = (BattleConfig::RUN_BASE_MULTIPLIER * playerPoke.baseSpeed / enemyPoke.baseSpeed) + 
                    (BattleConfig::RUN_ATTEMPT_BONUS * runAttempts);
    
    if (chance > BattleConfig::RUN_GUARANTEED_THRESHOLD) {
        return true;
    }
    
    int roll = rand() % BattleConfig::RUN_ROLL_MAX;
    return roll < static_cast<int>(chance);
}

/*****************************************************************************
 * BATTLE UTILITIES
 *****************************************************************************/
void resetForBattle(Pokemon& poke, const vector<Move>& moveList) {
    poke.currentHP = poke.baseHP;
    poke.initializePP(moveList);
}

int calculateExpGain(const Pokemon& enemyPokemon) {
    int baseExp = BattleConfig::BASE_EXP_PER_LEVEL * enemyPokemon.level;
    int expGained = baseExp / BattleConfig::EXP_DIVISOR;
    
    if (expGained < BattleConfig::MIN_EXP_GAIN) {
        expGained = BattleConfig::MIN_EXP_GAIN;
    }
    
    return expGained;
}

void displayBattleFrame(Pokemon& playerPokemon, Pokemon& enemyPokemon) {
    clearScreen();
    drawBattleFrame(playerPokemon, enemyPokemon);
}

/*****************************************************************************
 * EXECUTE ATTACK
 *****************************************************************************/
void executeAttack(Pokemon& attacker, Pokemon& defender, const Pokemon& playerPokemon, 
                   const Pokemon& enemyPokemon, int moveIndex, const PokemonDatabase& db) {
    const Move& usedMove = db.moveList.at(attacker.moveID[moveIndex] - 1);

    // Check PP
    if (attacker.currentPP[moveIndex] <= 0) {
        showBattleDialogue(usedMove.moveName + " has no PP left!");
        pressAnyKey();
        return;
    }

    // Move accuracy check
    int accuracyRoll = rand() % 100 + 1;
    if (accuracyRoll > usedMove.accuracy) {
        clearScreen();
        drawBattleFrame(playerPokemon, enemyPokemon);
        showBattleDialogue(attacker.pokemonName + " used " + usedMove.moveName + "!");
        pressAnyKey();

        clearScreen();
        drawBattleFrame(playerPokemon, enemyPokemon);
        showBattleDialogue(attacker.pokemonName + "'s " + usedMove.moveName + " missed!");
        pressAnyKey();
        return;
    }

    // Consume PP
    attacker.currentPP[moveIndex]--;

    // Show move used
    clearScreen();
    drawBattleFrame(playerPokemon, enemyPokemon);
    showBattleDialogue(attacker.pokemonName + " used " + usedMove.moveName + "!");
    pressAnyKey();

    // Calculate damage
    double dmg = calculateDamage(attacker, defender, usedMove, db);
    int damage = static_cast<int>(dmg);

    // Critical hit message
    bool isCritical = (rand() % BattleConfig::CRITICAL_HIT_CHANCE == 0);
    if (isCritical) {
        showBattleDialogue("A critical hit!");
        pressAnyKey();
    }

    // Type effectiveness messages
    double effectiveness = getTypeEffectiveness(usedMove.type, defender.primaryType);
    if (defender.secondaryType != PokemonType::NONE) {
        effectiveness *= getTypeEffectiveness(usedMove.type, defender.secondaryType);
    }

    if (effectiveness > 1.0) {
        showBattleDialogue("It's super effective!");
        pressAnyKey();
    } 
    else if (effectiveness > 0 && effectiveness < 1.0) {
        showBattleDialogue("It's not very effective...");
        pressAnyKey();
    } 
    else if (effectiveness == 0.0) {
        showBattleDialogue("It doesn't affect " + defender.pokemonName + "...");
        pressAnyKey();
    }

    // Apply damage
    defender.currentHP -= damage;
    if (defender.currentHP < 0) defender.currentHP = 0;

    // Show damage dealt
    clearScreen();
    drawBattleFrame(playerPokemon, enemyPokemon);
    showBattleDialogue("It dealt " + to_string(damage) + " damage!");
    pressAnyKey();
}

/*****************************************************************************
 * PLAYER ACTION HANDLER
 *****************************************************************************/
void handlePlayerAction(Player &player, Pokemon &playerPokemon, Pokemon &enemyPokemon, 
                        const PokemonDatabase &db, Trainer *trainer, bool &turnEnded, bool &battleOver) {

    int action = selectionMenu(playerPokemon); // 0=Fight, 1=Bag, 2=Pokemon, 3=Run

    // RUN
    if (action == 3) {
        if (trainer) {
            showBattleDialogue("You can't run from a Trainer battle!");
            pressAnyKey();
            return;
        }

        static int runAttempts = 0;
        bool escaped = canRun(playerPokemon, enemyPokemon, runAttempts);
        runAttempts++;

        if (escaped) {
            showBattleDialogue("Got away safely!");
            pressAnyKey();
            battleOver = true;
            turnEnded = true;
            return;
        } else {
            showBattleDialogue("Can't escape!");
            pressAnyKey();
            turnEnded = true;
            return;
        }
    }

    // BAG
    else if (action == 1) {
        if (player.items.empty()) {
            showDialogue("You have no ITEMS yet");
            pressAnyKey();
            clearScreen();
            return;
        }

        int itemIndex = selectItemMenu(player);
        
        if (itemIndex == -1) return;

        Item &selected = player.items[itemIndex];

        // Healing item
        if (selected.itemType == ItemType::HEALING && selected.quantity > 0) {
            selected.quantity--;
            int prevHP = playerPokemon.currentHP;
            playerPokemon.currentHP += selected.healAmount;
            if (playerPokemon.currentHP > playerPokemon.baseHP) {
                playerPokemon.currentHP = playerPokemon.baseHP;
            }

            showBattleDialogue(player.username + " used " + selected.itemName + "!");
            pressAnyKey();

            int healed = playerPokemon.currentHP - prevHP;
            if (healed > 0) {
                showBattleDialogue(playerPokemon.pokemonName + " recovered " + to_string(healed) + " HP!");
            } else {
                showBattleDialogue("But it had no effect!");
            }

            pressAnyKey();
            turnEnded = true;
            return;
        }

        // Pokeball (only in wild battles)
        else if (selected.itemType == ItemType::POKEBALL && !trainer && selected.quantity > 0) {
            selected.quantity--;
            int catchResult = throwPokeball(enemyPokemon);
            displayThrowPokeball(player, playerPokemon, enemyPokemon, catchResult);

            if (catchResult == CatchResult::SUCCESS) {
                showBattleDialogue("You caught " + enemyPokemon.pokemonName + "!");
                player.addPokemon(enemyPokemon);
                pressAnyKey();
                turnEnded = true;
                battleOver = true;
                return;
            } else {
                turnEnded = true;
                return;
            }
        }

        return;
    }

    // POKEMON (Switch)
    else if (action == 2) {
        if (player.currentPokemon.size() <= 1) {
            showBattleDialogue("You have no other POKEMON!");
            pressAnyKey();
            return;
        }

        int chosenIndex = pokemonSelectionMenu(player, true);
        if (chosenIndex == -1) return;
        if (player.currentPokemon[chosenIndex].currentHP <= 0) return;

        swap(playerPokemon, player.currentPokemon[chosenIndex]);
        showBattleDialogue("Go! " + playerPokemon.pokemonName + "!");
        pressAnyKey();

        turnEnded = true;
        return;
    }

    // FIGHT
    else if (action == 0) {
        int moveIndex = moveSelectionMenu(db.moveList, playerPokemon.moveID, playerPokemon.currentPP);
        if (moveIndex == -1) return;
        if (moveIndex < 0 || moveIndex >= (int)playerPokemon.moveID.size()) return;

        executeAttack(playerPokemon, enemyPokemon, playerPokemon, enemyPokemon, moveIndex, db);
        turnEnded = true;
        return;
    }
}

/*****************************************************************************
 * ENEMY ACTION HANDLER
 *****************************************************************************/
void handleEnemyAction(Pokemon &enemyPokemon, Pokemon &playerPokemon, const PokemonDatabase &db, Trainer *trainer) {
    if (!trainer) {
        // Wild Pokemon
        int enemyMoveIndex = rand() % enemyPokemon.moveID.size();
        executeAttack(enemyPokemon, playerPokemon, playerPokemon, enemyPokemon, enemyMoveIndex, db);
        return;
    }

    // Trainer AI: Try healing item first
    bool usedItem = tryUseHealingItem(*trainer, enemyPokemon, enemyPokemon.baseHP);
    
    if (!usedItem) {
        // Use AI to choose best move
        int enemyMoveIndex = chooseMoveAI(*trainer, playerPokemon, db);
        executeAttack(enemyPokemon, playerPokemon, playerPokemon, enemyPokemon, enemyMoveIndex, db);
    } else {
        showBattleDialogue(trainer->trainerName + " used a healing item!");
        pressAnyKey();
    }
}

/*****************************************************************************
 * MAIN BATTLE LOOP
 *****************************************************************************/
void startBattleLoop(Player &player, Pokemon &playerPokemon, Pokemon &enemyPokemon, 
                     const PokemonDatabase &db, Trainer* trainer) {
    
    // Reset trainer's Pokemon if this is a trainer battle
    if (trainer != nullptr) {
        for (int i = 0; i < (int)trainer->currentPokemon.size(); i++) {
            trainer->currentPokemon[i].currentHP = trainer->currentPokemon[i].baseHP;
            trainer->currentPokemon[i].initializePP(db.moveList);
        }
    }
    
    //track encounter
    player.encounterPokemon(enemyPokemon.pokemonName);
    
    flashTransition();
    
    int pMaxHP = playerPokemon.baseHP;
    int eMaxHP = enemyPokemon.baseHP;
    bool isTrainerBattle = (trainer != nullptr);
    bool battleOver = false;
    
    // Track current enemy index for trainer battles
    int currentEnemyIndex = -1;
    if (isTrainerBattle) {
        for (int i = 0; i < (int)trainer->currentPokemon.size(); i++) {
            if (&enemyPokemon == &trainer->currentPokemon[i]) {
                currentEnemyIndex = i;
                break;
            }
        }
    }

    while (!battleOver) {
        if (playerPokemon.currentHP <= 0 || enemyPokemon.currentHP <= 0) {
            break;
        }

        clearScreen();
        drawBattleFrame(playerPokemon, enemyPokemon);
        showBattleDialogue("What will " + playerPokemon.pokemonName + " do?");
        pressAnyKey();

        bool turnEnded = false;
        handlePlayerAction(player, playerPokemon, enemyPokemon, db, trainer, turnEnded, battleOver);

        if (battleOver) return;

        if (enemyPokemon.currentHP > 0 && turnEnded) {
            handleEnemyAction(enemyPokemon, playerPokemon, db, trainer);
        }

        // Enemy fainted
        if (enemyPokemon.currentHP <= 0) {
            clearScreen();
            drawBattleFrame(playerPokemon, enemyPokemon);
            showBattleDialogue("Enemy " + enemyPokemon.pokemonName + " has fainted!");
            pressAnyKey();

            // Calculate experience reward
            int statSum = enemyPokemon.baseAttack + enemyPokemon.baseDefense +
                          enemyPokemon.baseSPAttack + enemyPokemon.baseSPDefense + 
                          enemyPokemon.baseSpeed;
            int expYield = statSum / BattleConfig::STAT_EXP_DIVISOR;
            int expGained = (expYield * enemyPokemon.level) / BattleConfig::EXP_DIVISOR;
            
            if (expGained < BattleConfig::MIN_EXP_GAIN) {
                expGained = BattleConfig::MIN_EXP_GAIN;
            }

            playerPokemon.gainExp(expGained);
            playerPokemon.checkForNewMoves(db.moveList);

            // Trainer battle: Check for next Pokemon
            if (isTrainerBattle) {
                // Mark current Pokemon as defeated
                if (currentEnemyIndex >= 0 && currentEnemyIndex < (int)trainer->currentPokemon.size()) {
                    trainer->currentPokemon[currentEnemyIndex].currentHP = 0;
                }
                
                // Check if trainer has more Pokemon
                if (trainer->hasAlivePokemon()) {
                    int nextIndex = trainer->firstAliveIndex();
                    if (nextIndex != -1) {
                        enemyPokemon = trainer->currentPokemon[nextIndex];
                        currentEnemyIndex = nextIndex;
                        eMaxHP = enemyPokemon.baseHP;
                        
                        player.encounterPokemon(enemyPokemon.pokemonName);
                        
                        clearScreen();
                        drawBattleFrame(playerPokemon, enemyPokemon);
                        showBattleDialogue(trainer->trainerName + " sent out " + enemyPokemon.pokemonName + "!");
                        pressAnyKey();
                        continue;
                    }
                }
                
                // Trainer has no more Pokemon
                break;
            }

            // Wild Pokemon defeated - battle over
            break;
        }

        // Player Pokemon fainted
        if (playerPokemon.currentHP <= 0) {
            clearScreen();
            drawBattleFrame(playerPokemon, enemyPokemon);

            // Check if player has any alive Pokemon
            bool hasAlive = false;
            for (int i = 0; i < (int)player.currentPokemon.size(); i++) {
                if (player.currentPokemon[i].currentHP > 0) {
                    hasAlive = true;
                    break;
                }
            }

            // All Pokemon fainted - whiteout
            if (!hasAlive) {
                showBattleDialogue("You have no POKEMON left! You whited out...");
                pressAnyKey();
                respawnAfterWhiteout(player);
                battleOver = true;
                return;
            }

            // Force player to switch to another Pokemon
            int choice = -1;
            while (choice == -1) {
                choice = pokemonSelectionMenu(player, true);
                if (choice == -1) continue;
                if (player.currentPokemon[choice].currentHP <= 0) {
                    showBattleDialogue("That POKEMON has fainted! Pick another.");
                    pressAnyKey();
                    choice = -1;
                }
            }

            swap(playerPokemon, player.currentPokemon[choice]);
            clearScreen();
            drawBattleFrame(playerPokemon, enemyPokemon);
            showBattleDialogue("Go! " + playerPokemon.pokemonName + "!");
            pressAnyKey();
        }
    }
    
    if (playerPokemon.currentHP > 0) {
        playerPokemon.tryEvolution(db.pokedex, db.moveList);
    }
    
}
