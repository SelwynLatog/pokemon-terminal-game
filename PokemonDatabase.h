#ifndef POKEMON_DATABASE_H
#define POKEMON_DATABASE_H

#include <conio.h>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <windows.h>

using namespace std;

/*****************************************************************************
 * CONSTANTS
 *****************************************************************************/
namespace LevelConfig {
    constexpr int STARTING_LEVEL = 5;
    constexpr int STARTING_EXP = 0;
    constexpr int BASE_EXP_TO_LEVEL = 20;
    constexpr double EXP_GROWTH_RATE = 1.2;
    
    // Stat growth per level
    constexpr int BASE_HP_GROWTH = 1;
    constexpr int HP_GROWTH_DIVISOR = 10;
    constexpr int BASE_STAT_GROWTH = 1;
    constexpr int STAT_GROWTH_DIVISOR = 20;
}

namespace DisplayConfig {
    constexpr int LEVEL_UP_BOX_WIDTH = 50;
    constexpr int EVOLUTION_DOT_COUNT = 5;
    constexpr int EVOLUTION_DOT_DELAY_MS = 600;
    constexpr int EVOLUTION_PAUSE_MS = 800;
    constexpr int EVOLUTION_REVEAL_MS = 1500;
    constexpr int EVOLUTION_FINAL_MS = 2000;
}

namespace MoveConfig {
    constexpr int MAX_MOVES = 4;
    constexpr int MOVE_ID_OFFSET = 1;  // Move ID 1-indexed
}

/*****************************************************************************
 * ENUMS
 *****************************************************************************/
enum class PokemonType {
    NORMAL, FIRE, WATER, GRASS, ELECTRIC, ICE, FIGHTING, POISON,
    GROUND, FLYING, PSYCHIC, BUG, ROCK, GHOST, DRAGON,
    DARK, STEEL, FAIRY, NONE
};

enum class MoveCategory {
    PHYSICAL, SPECIAL, STATUS
};

enum class EvolutionType {
    LEVEL, ITEM, TRADE, NONE
};

/*****************************************************************************
 * DATA STRUCTURES
 *****************************************************************************/
struct Move {
    string moveName;
    PokemonType type;
    MoveCategory category;
    int pp;
    int power;
    int accuracy;
};

struct MoveLearnSet {
    int level;
    int moveID;
};

struct Ability {
    string abilityName;
    string abilityDescription;
    
    Ability() : abilityName(""), abilityDescription("") {}
    Ability(const string& name, const string& desc)
        : abilityName(name), abilityDescription(desc) {}
};

struct Evolution {
    EvolutionType method;
    int levelRequirement;
    string evolvesTo;
    string itemRequired;
};

/*****************************************************************************
 * FORWARD DECLARATIONS (from PokemonUtility.h)
 *****************************************************************************/
int moveSelectionMenu(const vector<Move>& moveList, const vector<int>& moveIDs, const vector<int>& currentPP);
int yesNoMenu(const string& prompt, bool inlineMode);

/*****************************************************************************
 * LOCAL UTILITY FUNCTIONS
 *****************************************************************************/
static void pressAnyKeyDB() {
    cout << "Press Any Key To Continue:" << endl;
    _getch();
}

static void clearScreenDB() {
    system("cls");
}

static void flashTransitionDB(int flashes = 3, int speed = 120) {
    for (int i = 0; i < flashes; ++i) {
        system("color F0");
        Sleep(speed);
        system("color 07");
        Sleep(speed);
    }
    system("cls");
}

/*****************************************************************************
 * POKEMON STRUCTURE
 *****************************************************************************/
struct Pokemon {
    string pokemonName;
    PokemonType primaryType;
    PokemonType secondaryType = PokemonType::NONE;
    int pokemonIndex;
    
    // Base stats
    int baseHP;
    int currentHP;
    int baseAttack;
    int baseDefense;
    int baseSPAttack;
    int baseSPDefense;
    int baseSpeed;
    
    // Level system
    int level;
    int exp;
    int expToNextLevel;
    
    // Move system
    vector<int> moveID;
    vector<int> currentPP;
    vector<MoveLearnSet> learnset;
    
    int abilityID;
    Evolution evolution = {EvolutionType::NONE, 0, "", ""};
    
    Pokemon() = default;
    
    Pokemon(string name, PokemonType type1, PokemonType type2, int index,
            int hp, int atk, int def, int spatk, int spdef, int spd,
            vector<int> moves, int ability, Evolution evo)
        : pokemonName(name)
        , primaryType(type1)
        , secondaryType(type2)
        , pokemonIndex(index)
        , baseHP(hp)
        , currentHP(hp)
        , baseAttack(atk)
        , baseDefense(def)
        , baseSPAttack(spatk)
        , baseSPDefense(spdef)
        , baseSpeed(spd)
        , level(LevelConfig::STARTING_LEVEL)
        , exp(LevelConfig::STARTING_EXP)
        , expToNextLevel(LevelConfig::BASE_EXP_TO_LEVEL)
        , moveID(moves)
        , abilityID(ability)
        , evolution(evo)
    {
        currentPP.clear();
    }

    /******************
     * PP & MOVE MANAGEMENT
     ******************/
    void initializePP(const vector<Move>& moves) {
        currentPP.clear();
        currentPP.reserve(moveID.size());
        
        for (int id : moveID) {
            if (id > 0 && id <= (int)moves.size()) {
                currentPP.push_back(moves[id - MoveConfig::MOVE_ID_OFFSET].pp);
            } else {
                currentPP.push_back(0);
            }
        }
        currentHP = baseHP;
    }
     /******************
     * STAT SCALING
     ******************/
    void scaleStatsToLevel() {
	    if (level <= LevelConfig::STARTING_LEVEL) {
	        currentHP = baseHP;
	        return;
	    }
	    
	    // Calculate how many levels above starting level
	    int levelsGained = level - LevelConfig::STARTING_LEVEL;
	    
	    // Apply HP growth for each level
	    for (int i = 0; i < levelsGained; i++) {
	        int currentLevel = LevelConfig::STARTING_LEVEL + i;
	        int hpGrowth = LevelConfig::BASE_HP_GROWTH + (currentLevel / LevelConfig::HP_GROWTH_DIVISOR);
	        int statGrowth = LevelConfig::BASE_STAT_GROWTH + (currentLevel / LevelConfig::STAT_GROWTH_DIVISOR);
	        
	        baseHP += hpGrowth;
	        baseAttack += statGrowth;
	        baseDefense += statGrowth;
	        baseSPAttack += statGrowth;
	        baseSPDefense += statGrowth;
	        baseSpeed += statGrowth;
	    }
	    
	    // Set current HP to new max
	    currentHP = baseHP;
	}
    
    void sanitizeMoves(const vector<Move>& moveList) {
        for (int& id : moveID) {
            if (id <= 0 || id > (int)moveList.size()) {
                id = 1;
            }
        }
        initializePP(moveList);
    }

    /******************
     * LEVEL UP SYSTEM
     ******************/
    void gainExp(int amount) {
        if (currentHP <= 0) return;
        
        exp += amount;
        
        cout << pokemonName << " gained " << amount << " EXP!" << endl;
        cout << "CURRENT EXP: " << exp << " / " << expToNextLevel << endl;
        pressAnyKeyDB();
        
        while (exp >= expToNextLevel) {
            exp -= expToNextLevel;
            levelUp();
        }
    }
    
    void levelUp() {
        int hpGrowth = LevelConfig::BASE_HP_GROWTH + (level / LevelConfig::HP_GROWTH_DIVISOR);
        int statGrowth = LevelConfig::BASE_STAT_GROWTH + (level / LevelConfig::STAT_GROWTH_DIVISOR);
        
        level++;
        
        baseHP += hpGrowth;
        baseAttack += statGrowth;
        baseDefense += statGrowth;
        baseSPAttack += statGrowth;
        baseSPDefense += statGrowth;
        baseSpeed += statGrowth;
        
        currentHP = baseHP;
        
        displayLevelUp(hpGrowth, statGrowth, statGrowth, statGrowth, statGrowth, statGrowth);
        
        expToNextLevel = static_cast<int>(expToNextLevel * LevelConfig::EXP_GROWTH_RATE);
    }

    void displayLevelUp(int hpPlus, int atkPlus, int defPlus, int spatkPlus, int spdefPlus, int spdPlus) {
        clearScreenDB();
        
        // Header
        cout << "+" << string(DisplayConfig::LEVEL_UP_BOX_WIDTH, '-') << "+\n";
        string header = pokemonName + " LEVELED UP TO LV." + to_string(level) + "!";
        cout << "| " << header;
        int pad = DisplayConfig::LEVEL_UP_BOX_WIDTH - header.length() - 1;
        if (pad > 0) cout << string(pad, ' ');
        cout << "|\n";
        cout << "+" << string(DisplayConfig::LEVEL_UP_BOX_WIDTH, '-') << "+\n\n";
        
        // Stats with growth
        cout << "HP:          " << (baseHP - hpPlus)        << " + " << hpPlus    << " = " << baseHP << "\n";
        cout << "ATTACK:      " << (baseAttack - atkPlus)   << " + " << atkPlus   << " = " << baseAttack << "\n";
        cout << "DEFENSE:     " << (baseDefense - defPlus)  << " + " << defPlus   << " = " << baseDefense << "\n";
        cout << "SP. ATTACK:  " << (baseSPAttack - spatkPlus) << " + " << spatkPlus << " = " << baseSPAttack << "\n";
        cout << "SP. DEFENSE: " << (baseSPDefense - spdefPlus) << " + " << spdefPlus << " = " << baseSPDefense << "\n";
        cout << "SPEED:       " << (baseSpeed - spdPlus)    << " + " << spdPlus   << " = " << baseSpeed << "\n\n";
        
        pressAnyKeyDB();
    }

    /******************
     * MOVE LEARNING SYSTEM
     ******************/
    void checkForNewMoves(const vector<Move>& moveList) {
        for (const auto& ml : learnset) {
            if (ml.level != level) continue;
            
            int newMoveID = ml.moveID;
            if (newMoveID <= 0 || newMoveID > (int)moveList.size()) {
                cout << "Invalid move ID " << newMoveID << " for " << pokemonName << endl;
                continue;
            }

            const Move& newMove = moveList[newMoveID - MoveConfig::MOVE_ID_OFFSET];

            // Check if already knows this move
            bool alreadyKnows = false;
            for (int id : moveID) {
                if (id == newMoveID) {
                    alreadyKnows = true;
                    break;
                }
            }
            if (alreadyKnows) return;

            cout << pokemonName << " is trying to learn " << newMove.moveName << "!" << endl;
            pressAnyKeyDB();
            clearScreenDB();

            // Has room for new move
            if ((int)moveID.size() < MoveConfig::MAX_MOVES) {
                moveID.push_back(newMoveID);
                currentPP.push_back(newMove.pp);
                cout << pokemonName << " learned " << newMove.moveName << "!" << endl;
                pressAnyKeyDB();
                return;
            }

            // Must forget a move
            handleMoveReplacement(moveList, newMove, newMoveID);
        }
    }

private:
    void handleMoveReplacement(const vector<Move>& moveList, const Move& newMove, int newMoveID) {
        while (true) {
            clearScreenDB();
            cout << pokemonName << " wants to learn " << newMove.moveName
                 << ", but it already knows " << MoveConfig::MAX_MOVES << " moves.\n" << endl;
            cout << "Choose a move to forget:\n" << endl;

            int forgetIndex = moveSelectionMenu(moveList, moveID, currentPP);

            if (forgetIndex == -1) {
                clearScreenDB();
                cout << pokemonName << " decided not to learn " << newMove.moveName << "." << endl;
                pressAnyKeyDB();
                return;
            }

            const Move& oldMove = moveList[moveID[forgetIndex] - MoveConfig::MOVE_ID_OFFSET];
            string confirmMsg = "Forget " + oldMove.moveName + " for " + newMove.moveName + "?";

            int confirm = yesNoMenu(confirmMsg, true);

            if (confirm == 1) {
                moveID[forgetIndex] = newMoveID;
                currentPP[forgetIndex] = newMove.pp;
                clearScreenDB();
                cout << pokemonName << " forgot " << oldMove.moveName
                     << " and learned " << newMove.moveName << "!" << endl;
                pressAnyKeyDB();
                return;
            }
            // confirm == -1 or 0: loop continues
        }
    }

public:
    /******************
     * EVOLUTION SYSTEM
     ******************/
    bool canEvolve() const {
        if (evolution.method == EvolutionType::NONE) return false;
        
        switch (evolution.method) {
            case EvolutionType::LEVEL: return level >= evolution.levelRequirement;
            case EvolutionType::ITEM:  return false;
            default:                   return false;
        }
    }
    
    void tryEvolution(const vector<Pokemon>& pokedex, const vector<Move>& moveList) {
	    if (!canEvolve()) return;
	
	    string oldName = pokemonName;
	    int currentLevel = level;
	
	    // Find evolved form
	    const Pokemon* evoData = nullptr;
	    for (const auto& p : pokedex) {
	        if (p.pokemonName == evolution.evolvesTo) {
	            evoData = &p;
	            break;
	        }
	    }
	    if (!evoData) return;
	
	    // Evolution animation
	    clearScreenDB();
	    cout << "What?\n";
	    Sleep(DisplayConfig::EVOLUTION_PAUSE_MS);
	    cout << oldName << " is evolving!\n\n";
	    Sleep(DisplayConfig::EVOLUTION_REVEAL_MS);
	
	    for (int i = 0; i < DisplayConfig::EVOLUTION_DOT_COUNT; i++) {
	        clearScreenDB();
	        cout << oldName << " is evolving!\n\n";
	        for (int j = 0; j <= i; j++) cout << ". ";
	        cout << "\n";
	        Sleep(DisplayConfig::EVOLUTION_DOT_DELAY_MS);
	    }
	    flashTransitionDB();
	
	    // Get the base form from pokedex to compare properly
	    const Pokemon* baseForm = nullptr;
	    for (const auto& p : pokedex) {
	        if (p.pokemonName == oldName) {
	            baseForm = &p;
	            break;
	        }
	    }
	    
	    if (baseForm) {
	        // Calculate the difference between BASE FORMS (not current scaled stats)
	        int hpDiff = evoData->baseHP - baseForm->baseHP;
	        int atkDiff = evoData->baseAttack - baseForm->baseAttack;
	        int defDiff = evoData->baseDefense - baseForm->baseDefense;
	        int spatkDiff = evoData->baseSPAttack - baseForm->baseSPAttack;
	        int spdefDiff = evoData->baseSPDefense - baseForm->baseSPDefense;
	        int spdDiff = evoData->baseSpeed - baseForm->baseSpeed;
	
	        // Add these differences to current stats
	        baseHP += hpDiff;
	        baseAttack += atkDiff;
	        baseDefense += defDiff;
	        baseSPAttack += spatkDiff;
	        baseSPDefense += spdefDiff;
	        baseSpeed += spdDiff;
	    }
	
	    currentHP = baseHP;
	    
	    // Apply evolution data
	    pokemonName = evoData->pokemonName;
	    primaryType = evoData->primaryType;
	    secondaryType = evoData->secondaryType;
	    abilityID = evoData->abilityID;
	    learnset = evoData->learnset;
	    evolution = evoData->evolution;
	
	    initializePP(moveList);
	
	    // Final reveal
	    clearScreenDB();
	    cout << "Congratulations!\n\n";
	    Sleep(500);
	    cout << oldName << " evolved into " << pokemonName << "!\n\n";
	    Sleep(DisplayConfig::EVOLUTION_FINAL_MS);
	    pressAnyKeyDB();
	}
};

/*****************************************************************************
 * POKEMON DATABASE CONTAINER
 *****************************************************************************/
struct PokemonDatabase {
    vector<Move> moveList;
    vector<Ability> abilityList;
    vector<Pokemon> pokedex;
    
    PokemonDatabase() {
        initializeMoves();
        initializeAbilities();
        initializePokemon();
        initializeLearnsets();
    }

private:
    void initializeMoves() {
        // ========================================================================
        // GENERATION 1 MOVES (MOVE ID 1-109)
        // ========================================================================
        
        moveList.push_back({"POUND",         PokemonType::NORMAL,   MoveCategory::PHYSICAL, 35, 40, 100});
        moveList.push_back({"KARATE CHOP",   PokemonType::FIGHTING, MoveCategory::PHYSICAL, 25, 50, 100});
        moveList.push_back({"DOUBLE SLAP",   PokemonType::NORMAL,   MoveCategory::PHYSICAL, 10, 15, 85});
        moveList.push_back({"COMET PUNCH",   PokemonType::NORMAL,   MoveCategory::PHYSICAL, 15, 18, 85});
        moveList.push_back({"MEGA PUNCH",    PokemonType::NORMAL,   MoveCategory::PHYSICAL, 20, 80, 85});
        moveList.push_back({"PAY DAY",       PokemonType::NORMAL,   MoveCategory::PHYSICAL, 20, 40, 100});
        moveList.push_back({"FIRE PUNCH",    PokemonType::FIRE,     MoveCategory::PHYSICAL, 15, 75, 100});
        moveList.push_back({"ICE PUNCH",     PokemonType::ICE,      MoveCategory::PHYSICAL, 15, 75, 100});
        moveList.push_back({"THUNDER PUNCH", PokemonType::ELECTRIC, MoveCategory::PHYSICAL, 15, 75, 100});
        moveList.push_back({"SCRATCH",       PokemonType::NORMAL,   MoveCategory::PHYSICAL, 35, 40, 100});
        moveList.push_back({"VISE GRIP",     PokemonType::NORMAL,   MoveCategory::PHYSICAL, 30, 55, 100});
        moveList.push_back({"GUILLOTINE",    PokemonType::NORMAL,   MoveCategory::PHYSICAL, 5, 0, 30});
        moveList.push_back({"RAZOR WIND",    PokemonType::NORMAL,   MoveCategory::SPECIAL,  10, 80, 100});
        moveList.push_back({"SWORDS DANCE",  PokemonType::NORMAL,   MoveCategory::STATUS,   20, 0, 0});
        moveList.push_back({"CUT",           PokemonType::NORMAL,   MoveCategory::PHYSICAL, 30, 50, 95});
        moveList.push_back({"GUST",          PokemonType::FLYING,   MoveCategory::SPECIAL,  35, 40, 100});
        moveList.push_back({"WING ATTACK",   PokemonType::FLYING,   MoveCategory::PHYSICAL, 35, 60, 100});
        moveList.push_back({"WHIRLWIND",     PokemonType::NORMAL,   MoveCategory::STATUS,   20, 0, 0});
        moveList.push_back({"FLY",           PokemonType::FLYING,   MoveCategory::PHYSICAL, 15, 90, 95});
        moveList.push_back({"BIND",          PokemonType::NORMAL,   MoveCategory::PHYSICAL, 20, 15, 85});
        moveList.push_back({"SLAM",          PokemonType::NORMAL,   MoveCategory::PHYSICAL, 20, 80, 75});
        moveList.push_back({"VINE WHIP",     PokemonType::GRASS,    MoveCategory::PHYSICAL, 25, 45, 100});
        moveList.push_back({"STOMP",         PokemonType::NORMAL,   MoveCategory::PHYSICAL, 20, 65, 100});
        moveList.push_back({"DOUBLE KICK",   PokemonType::FIGHTING, MoveCategory::PHYSICAL, 30, 30, 100});
        moveList.push_back({"MEGA KICK",     PokemonType::NORMAL,   MoveCategory::PHYSICAL, 5, 120, 75});
        moveList.push_back({"JUMP KICK",     PokemonType::FIGHTING, MoveCategory::PHYSICAL, 10, 100, 95});
        moveList.push_back({"ROLLING KICK",  PokemonType::FIGHTING, MoveCategory::PHYSICAL, 15, 60, 85});
        moveList.push_back({"SAND ATTACK",   PokemonType::GROUND,   MoveCategory::STATUS,   15, 0, 100});
        moveList.push_back({"HEADBUTT",      PokemonType::NORMAL,   MoveCategory::PHYSICAL, 15, 70, 100});
        moveList.push_back({"HORN ATTACK",   PokemonType::NORMAL,   MoveCategory::PHYSICAL, 25, 65, 100});
        moveList.push_back({"FURY ATTACK",   PokemonType::NORMAL,   MoveCategory::PHYSICAL, 20, 15, 85});
        moveList.push_back({"HORN DRILL",    PokemonType::NORMAL,   MoveCategory::PHYSICAL, 5, 0, 30});
        moveList.push_back({"TACKLE",        PokemonType::NORMAL,   MoveCategory::PHYSICAL, 35, 40, 100});
        moveList.push_back({"BODY SLAM",     PokemonType::NORMAL,   MoveCategory::PHYSICAL, 15, 85, 100});
        moveList.push_back({"WRAP",          PokemonType::NORMAL,   MoveCategory::PHYSICAL, 20, 15, 90});
        moveList.push_back({"TAKE DOWN",     PokemonType::NORMAL,   MoveCategory::PHYSICAL, 20, 90, 85});
        moveList.push_back({"THRASH",        PokemonType::NORMAL,   MoveCategory::PHYSICAL, 10, 120, 100});
        moveList.push_back({"DOUBLE-EDGE",   PokemonType::NORMAL,   MoveCategory::PHYSICAL, 15, 120, 100});
        moveList.push_back({"TAIL WHIP",     PokemonType::NORMAL,   MoveCategory::STATUS,   30, 20, 100});
        moveList.push_back({"POISON STING",  PokemonType::POISON,   MoveCategory::PHYSICAL, 35, 15, 100});
        moveList.push_back({"TWINEEDLE",     PokemonType::BUG,      MoveCategory::PHYSICAL, 20, 25, 100});
        moveList.push_back({"PIN MISSILE",   PokemonType::BUG,      MoveCategory::PHYSICAL, 20, 25, 95});
        moveList.push_back({"LEER",          PokemonType::NORMAL,   MoveCategory::STATUS,   30, 0, 100});
        moveList.push_back({"BITE",          PokemonType::DARK,     MoveCategory::PHYSICAL, 25, 60, 100});
        moveList.push_back({"GROWL",         PokemonType::NORMAL,   MoveCategory::STATUS,   40, 0, 100});
        moveList.push_back({"ROAR",          PokemonType::NORMAL,   MoveCategory::STATUS,   20, 0, 0}); 
        moveList.push_back({"SING",          PokemonType::NORMAL,   MoveCategory::STATUS,   15, 0, 55}); 
        moveList.push_back({"SUPERSONIC",    PokemonType::NORMAL,   MoveCategory::STATUS,   20, 0, 55}); 
        moveList.push_back({"SONIC BOOM",    PokemonType::NORMAL,   MoveCategory::SPECIAL,  20, 0, 90});
        moveList.push_back({"DISABLE",       PokemonType::NORMAL,   MoveCategory::STATUS,   20, 0, 100}); // Move ID 50
        moveList.push_back({"ACID",          PokemonType::POISON,   MoveCategory::SPECIAL,  30, 40, 100});
        moveList.push_back({"EMBER",         PokemonType::FIRE,     MoveCategory::SPECIAL,  25, 40, 100});
        moveList.push_back({"FLAMETHROWER",  PokemonType::FIRE,     MoveCategory::SPECIAL,  15, 90, 100});
        moveList.push_back({"MIST",          PokemonType::ICE,      MoveCategory::STATUS,   30, 0, 0});
        moveList.push_back({"WATER GUN",     PokemonType::WATER,    MoveCategory::SPECIAL,  25, 40, 100});
        moveList.push_back({"HYDRO PUMP",    PokemonType::WATER,    MoveCategory::SPECIAL,  5, 110, 80});
        moveList.push_back({"SURF",          PokemonType::WATER,    MoveCategory::SPECIAL,  15, 90, 100});
        moveList.push_back({"ICE BEAM",      PokemonType::ICE,      MoveCategory::SPECIAL,  10, 90, 100});
        moveList.push_back({"BLIZZARD",      PokemonType::ICE,      MoveCategory::SPECIAL,  5, 110, 70});
        moveList.push_back({"PSYBEAM",       PokemonType::PSYCHIC,  MoveCategory::SPECIAL,  20, 65, 100}); // Move ID 60
        moveList.push_back({"BUBBLE BEAM",   PokemonType::WATER,    MoveCategory::SPECIAL,  20, 65, 100});
        moveList.push_back({"AURORA BEAM",   PokemonType::ICE,      MoveCategory::SPECIAL,  20, 65, 100});
        moveList.push_back({"HYPER BEAM",    PokemonType::NORMAL,   MoveCategory::SPECIAL,  5, 150, 90});
        moveList.push_back({"PECK",          PokemonType::FLYING,   MoveCategory::PHYSICAL, 35, 35, 100});
        moveList.push_back({"DRILL PECK",    PokemonType::FLYING,   MoveCategory::PHYSICAL, 20, 80, 100});
        moveList.push_back({"SUBMISSION",    PokemonType::FIGHTING, MoveCategory::PHYSICAL, 20, 80, 80}); 
        moveList.push_back({"LOW KICK",      PokemonType::FIGHTING, MoveCategory::PHYSICAL, 20, 50, 100});
        moveList.push_back({"STRENGTH",      PokemonType::NORMAL,   MoveCategory::PHYSICAL, 15, 80, 100});
        moveList.push_back({"ABSORB",        PokemonType::GRASS,    MoveCategory::SPECIAL,  25, 20, 100}); 
        moveList.push_back({"MEGA DRAIN",    PokemonType::GRASS,    MoveCategory::SPECIAL,  15, 40, 100}); // Move ID 70
        moveList.push_back({"RAZOR LEAF",    PokemonType::GRASS,    MoveCategory::PHYSICAL, 25, 55, 95});
        moveList.push_back({"PETAL DANCE",   PokemonType::GRASS,    MoveCategory::SPECIAL,  10, 120, 100}); 
        moveList.push_back({"FIRE SPIN",     PokemonType::FIRE,     MoveCategory::SPECIAL,  15, 35, 85});
        moveList.push_back({"THUNDER SHOCK", PokemonType::ELECTRIC, MoveCategory::SPECIAL,  30, 40, 100});
        moveList.push_back({"THUNDERBOLT",   PokemonType::ELECTRIC, MoveCategory::SPECIAL,  15, 90, 100});
        moveList.push_back({"THUNDER",       PokemonType::ELECTRIC, MoveCategory::SPECIAL,  10, 110, 70});
        moveList.push_back({"ROCK THROW",    PokemonType::ROCK,     MoveCategory::PHYSICAL, 15, 50, 90});
        moveList.push_back({"EARTHQUAKE",    PokemonType::GROUND,   MoveCategory::PHYSICAL, 10, 100, 100});
        moveList.push_back({"CONFUSION",     PokemonType::PSYCHIC,  MoveCategory::SPECIAL,  25, 50, 100}); // Move ID 80
        moveList.push_back({"PSYCHIC",       PokemonType::PSYCHIC,  MoveCategory::SPECIAL,  10, 90, 100});
        moveList.push_back({"QUICK ATTACK",  PokemonType::NORMAL,   MoveCategory::PHYSICAL, 30, 40, 100});
        moveList.push_back({"RAGE",          PokemonType::NORMAL,   MoveCategory::PHYSICAL, 20, 20, 100});
        moveList.push_back({"SELF-DESTRUCT", PokemonType::NORMAL,   MoveCategory::PHYSICAL, 5, 200, 100});
        moveList.push_back({"EGG BOMB",      PokemonType::NORMAL,   MoveCategory::PHYSICAL, 10, 100, 75});
        moveList.push_back({"LICK",          PokemonType::GHOST,    MoveCategory::PHYSICAL, 30, 30, 100});
        moveList.push_back({"SMOG",          PokemonType::POISON,   MoveCategory::SPECIAL,  20, 30, 70});
        moveList.push_back({"SLUDGE",        PokemonType::POISON,   MoveCategory::SPECIAL,  20, 65, 100});
        moveList.push_back({"BONE CLUB",     PokemonType::GROUND,   MoveCategory::PHYSICAL, 20, 65, 85});
        moveList.push_back({"FIRE BLAST",    PokemonType::FIRE,     MoveCategory::SPECIAL,  5, 110, 85}); // Move ID 90
        moveList.push_back({"WATERFALL",     PokemonType::WATER,    MoveCategory::PHYSICAL, 15, 80, 100});
        moveList.push_back({"CLAMP",         PokemonType::WATER,    MoveCategory::PHYSICAL, 15, 35, 85}); 
        moveList.push_back({"SWIFT",         PokemonType::NORMAL,   MoveCategory::SPECIAL,  20, 60, 100});
        moveList.push_back({"SPIKE CANNON",  PokemonType::NORMAL,   MoveCategory::PHYSICAL, 15, 20, 100});
        moveList.push_back({"CONSTRICT",     PokemonType::NORMAL,   MoveCategory::PHYSICAL, 35, 10, 100});
        moveList.push_back({"HIGH JUMP KICK",PokemonType::FIGHTING, MoveCategory::PHYSICAL, 10, 130, 90});
        moveList.push_back({"DREAM EATER",   PokemonType::PSYCHIC,  MoveCategory::SPECIAL,  15, 100, 100});
        moveList.push_back({"BARRAGE",       PokemonType::NORMAL,   MoveCategory::PHYSICAL, 20, 15, 85}); 
        moveList.push_back({"LEECH LIFE",    PokemonType::BUG,      MoveCategory::PHYSICAL, 10, 80, 100});
        moveList.push_back({"BUBBLE",        PokemonType::WATER,    MoveCategory::SPECIAL,  30, 40, 100}); // Move ID 100
        moveList.push_back({"DIZZY PUNCH",   PokemonType::NORMAL,   MoveCategory::PHYSICAL, 10, 70, 100});
        moveList.push_back({"CRABHAMMER",    PokemonType::WATER,    MoveCategory::PHYSICAL, 10, 100, 90});
        moveList.push_back({"EXPLOSION",     PokemonType::NORMAL,   MoveCategory::PHYSICAL, 5, 250, 100});
        moveList.push_back({"FURY SWIPES",   PokemonType::NORMAL,   MoveCategory::PHYSICAL, 15, 18, 80}); 
        moveList.push_back({"BONEMERANG",    PokemonType::GROUND,   MoveCategory::PHYSICAL, 10, 50, 90});
        moveList.push_back({"ROCK SLIDE",    PokemonType::ROCK,     MoveCategory::PHYSICAL, 10, 75, 90});
        moveList.push_back({"HYPER FANG",    PokemonType::NORMAL,   MoveCategory::PHYSICAL, 15, 80, 90});
        moveList.push_back({"TRI ATTACK",    PokemonType::NORMAL,   MoveCategory::SPECIAL,  10, 80, 100});
        moveList.push_back({"SLASH",         PokemonType::NORMAL,   MoveCategory::PHYSICAL, 20, 70, 100});
        moveList.push_back({"STRUGGLE",      PokemonType::NORMAL,   MoveCategory::PHYSICAL, 1, 50, 100}); // Move ID 109 - Used when out of PP
        
        // ========================================================================
        // GENERATION 2 MOVES (MOVE ID 110-149)
        // ========================================================================
        
        moveList.push_back({"TRIPLE KICK",   PokemonType::FIGHTING, MoveCategory::PHYSICAL, 10, 10, 90}); // Move ID 110
        moveList.push_back({"THIEF",         PokemonType::DARK,     MoveCategory::PHYSICAL, 25, 60, 100});
        moveList.push_back({"FLAME WHEEL",   PokemonType::FIRE,     MoveCategory::PHYSICAL, 25, 60, 100});
        moveList.push_back({"SNORE",         PokemonType::NORMAL,   MoveCategory::SPECIAL,  15, 50, 100});
        moveList.push_back({"AEROBLAST",     PokemonType::FLYING,   MoveCategory::SPECIAL,  5, 100, 95});
        moveList.push_back({"POWDER SNOW",   PokemonType::ICE,      MoveCategory::SPECIAL,  25, 40, 100});
        moveList.push_back({"MACH PUNCH",    PokemonType::FIGHTING, MoveCategory::PHYSICAL, 30, 40, 100}); 
        moveList.push_back({"FEINT ATTACK",  PokemonType::DARK,     MoveCategory::PHYSICAL, 20, 60, 100}); 
        moveList.push_back({"SLUDGE BOMB",   PokemonType::POISON,   MoveCategory::SPECIAL,  10, 90, 100});
        moveList.push_back({"MUD-SLAP",      PokemonType::GROUND,   MoveCategory::SPECIAL,  10, 20, 100});
        moveList.push_back({"OCTAZOOKA",     PokemonType::WATER,    MoveCategory::SPECIAL,  10, 65, 85}); // Move ID 120
        moveList.push_back({"ZAP CANNON",    PokemonType::ELECTRIC, MoveCategory::SPECIAL,  5, 120, 50});
        moveList.push_back({"ICY WIND",      PokemonType::ICE,      MoveCategory::SPECIAL,  15, 55, 95});
        moveList.push_back({"BONE RUSH",     PokemonType::GROUND,   MoveCategory::PHYSICAL, 10, 25, 90}); 
        moveList.push_back({"OUTRAGE",       PokemonType::DRAGON,   MoveCategory::PHYSICAL, 10, 120, 100});
        moveList.push_back({"GIGA DRAIN",    PokemonType::GRASS,    MoveCategory::SPECIAL,  10, 75, 100});
        moveList.push_back({"ROLLOUT",       PokemonType::ROCK,     MoveCategory::PHYSICAL, 20, 30, 90});
        moveList.push_back({"FALSE SWIPE",   PokemonType::NORMAL,   MoveCategory::PHYSICAL, 40, 40, 100});
        moveList.push_back({"SPARK",         PokemonType::ELECTRIC, MoveCategory::PHYSICAL, 20, 65, 100});
        moveList.push_back({"FURY CUTTER",   PokemonType::BUG,      MoveCategory::PHYSICAL, 20, 40, 95});
        moveList.push_back({"STEEL WING",    PokemonType::STEEL,    MoveCategory::PHYSICAL, 25, 70, 90}); // Move ID 130
        moveList.push_back({"SACRED FIRE",   PokemonType::FIRE,     MoveCategory::PHYSICAL, 5, 100, 95});
        moveList.push_back({"DYNAMIC PUNCH", PokemonType::FIGHTING, MoveCategory::PHYSICAL, 5, 100, 50});
        moveList.push_back({"MEGAHORN",      PokemonType::BUG,      MoveCategory::PHYSICAL, 10, 120, 85});
        moveList.push_back({"DRAGON BREATH", PokemonType::DRAGON,   MoveCategory::SPECIAL,  20, 60, 100});
        moveList.push_back({"PURSUIT",       PokemonType::DARK,     MoveCategory::PHYSICAL, 20, 40, 100});
        moveList.push_back({"RAPID SPIN",    PokemonType::NORMAL,   MoveCategory::PHYSICAL, 40, 50, 100});
        moveList.push_back({"IRON TAIL",     PokemonType::STEEL,    MoveCategory::PHYSICAL, 15, 100, 75});
        moveList.push_back({"METAL CLAW",    PokemonType::STEEL,    MoveCategory::PHYSICAL, 35, 50, 95});
        moveList.push_back({"VITAL THROW",   PokemonType::FIGHTING, MoveCategory::PHYSICAL, 10, 70, 100});
        moveList.push_back({"HIDDEN POWER",  PokemonType::NORMAL,   MoveCategory::SPECIAL,  15, 60, 100}); // Move ID 140
        moveList.push_back({"CROSS CHOP",    PokemonType::FIGHTING, MoveCategory::PHYSICAL, 5, 100, 80});
        moveList.push_back({"TWISTER",       PokemonType::DRAGON,   MoveCategory::SPECIAL,  20, 40, 100});
        moveList.push_back({"CRUNCH",        PokemonType::DARK,     MoveCategory::PHYSICAL, 15, 80, 100});
        moveList.push_back({"EXTREME SPEED", PokemonType::NORMAL,   MoveCategory::PHYSICAL, 5, 80, 100});
        moveList.push_back({"ANCIENT POWER", PokemonType::ROCK,     MoveCategory::SPECIAL,  5, 60, 100});
        moveList.push_back({"SHADOW BALL",   PokemonType::GHOST,    MoveCategory::SPECIAL,  15, 80, 100});
        moveList.push_back({"FUTURE SIGHT",  PokemonType::PSYCHIC,  MoveCategory::SPECIAL,  10, 120, 100});
        moveList.push_back({"ROCK SMASH",    PokemonType::FIGHTING, MoveCategory::PHYSICAL, 15, 40, 100});
        moveList.push_back({"WHIRLPOOL",     PokemonType::WATER,    MoveCategory::SPECIAL,  15, 35, 85}); // Move ID 149
        
        // ========================================================================
        // GENERATION 3 MOVES (MOVE ID 150-200)
        // ========================================================================
        
        moveList.push_back({"FAKE OUT",      PokemonType::NORMAL,   MoveCategory::PHYSICAL, 10, 40, 100}); // Move ID 150
        moveList.push_back({"UPROAR",        PokemonType::NORMAL,   MoveCategory::SPECIAL,  10, 90, 100}); 
        moveList.push_back({"HEAT WAVE",     PokemonType::FIRE,     MoveCategory::SPECIAL,  10, 95, 90});
        moveList.push_back({"FACADE",        PokemonType::NORMAL,   MoveCategory::PHYSICAL, 20, 70, 100});
        moveList.push_back({"FOCUS PUNCH",   PokemonType::FIGHTING, MoveCategory::PHYSICAL, 20, 150, 100});
        moveList.push_back({"SMELLING SALTS",PokemonType::NORMAL,   MoveCategory::PHYSICAL, 10, 70, 100});
        moveList.push_back({"SUPERPOWER",    PokemonType::FIGHTING, MoveCategory::PHYSICAL, 5, 120, 100}); 
        moveList.push_back({"REVENGE",       PokemonType::FIGHTING, MoveCategory::PHYSICAL, 10, 60, 100});
        moveList.push_back({"BRICK BREAK",   PokemonType::FIGHTING, MoveCategory::PHYSICAL, 15, 75, 100});
        moveList.push_back({"KNOCK OFF",     PokemonType::DARK,     MoveCategory::PHYSICAL, 20, 65, 100});
        moveList.push_back({"SECRET POWER",  PokemonType::NORMAL,   MoveCategory::PHYSICAL, 20, 70, 100}); // Move ID 160
        moveList.push_back({"ARM THRUST",    PokemonType::FIGHTING, MoveCategory::PHYSICAL, 20, 15, 100}); 
        moveList.push_back({"LUSTER PURGE",  PokemonType::PSYCHIC,  MoveCategory::SPECIAL,  5, 95, 100}); 
        moveList.push_back({"MIST BALL",     PokemonType::PSYCHIC,  MoveCategory::SPECIAL,  5, 95, 100}); 
        moveList.push_back({"BLAZE KICK",    PokemonType::FIRE,     MoveCategory::PHYSICAL, 10, 85, 90}); 
        moveList.push_back({"ICE BALL",      PokemonType::ICE,      MoveCategory::PHYSICAL, 20, 30, 90}); 
        moveList.push_back({"NEEDLE ARM",    PokemonType::GRASS,    MoveCategory::PHYSICAL, 15, 60, 100});
        moveList.push_back({"HYPER VOICE",   PokemonType::NORMAL,   MoveCategory::SPECIAL,  10, 90, 100});
        moveList.push_back({"POISON FANG",   PokemonType::POISON,   MoveCategory::PHYSICAL, 15, 50, 100}); 
        moveList.push_back({"CRUSH CLAW",    PokemonType::NORMAL,   MoveCategory::PHYSICAL, 10, 75, 95});
        moveList.push_back({"BLAST BURN",    PokemonType::FIRE,     MoveCategory::SPECIAL,  5, 150, 90}); // Move ID 170 
        moveList.push_back({"HYDRO CANNON",  PokemonType::WATER,    MoveCategory::SPECIAL,  5, 150, 90}); 
        moveList.push_back({"METEOR MASH",   PokemonType::STEEL,    MoveCategory::PHYSICAL, 10, 90, 90});
        moveList.push_back({"ASTONISH",      PokemonType::GHOST,    MoveCategory::PHYSICAL, 15, 30, 100});
        moveList.push_back({"WEATHER BALL",  PokemonType::NORMAL,   MoveCategory::SPECIAL,  10, 50, 100}); 
        moveList.push_back({"AIR CUTTER",    PokemonType::FLYING,   MoveCategory::SPECIAL,  25, 60, 95});
        moveList.push_back({"OVERHEAT",      PokemonType::FIRE,     MoveCategory::SPECIAL,  5, 130, 90});
        moveList.push_back({"ROCK TOMB",     PokemonType::ROCK,     MoveCategory::PHYSICAL, 15, 60, 95});
        moveList.push_back({"SILVER WIND",   PokemonType::BUG,      MoveCategory::SPECIAL,  5, 60, 100}); 
        moveList.push_back({"SIGNAL BEAM",   PokemonType::BUG,      MoveCategory::SPECIAL,  15, 75, 100});
        moveList.push_back({"SHADOW PUNCH",  PokemonType::GHOST,    MoveCategory::PHYSICAL, 20, 60, 100}); 
        moveList.push_back({"EXTRASENSORY",  PokemonType::PSYCHIC,  MoveCategory::SPECIAL,  20, 80, 100});
        moveList.push_back({"SKY UPPERCUT",  PokemonType::FIGHTING, MoveCategory::PHYSICAL, 15, 85, 90}); 
        moveList.push_back({"SAND TOMB",     PokemonType::GROUND,   MoveCategory::PHYSICAL, 15, 35, 85}); 
        moveList.push_back({"MUDDY WATER",   PokemonType::WATER,    MoveCategory::SPECIAL,  10, 90, 85});
        moveList.push_back({"BULLET SEED",   PokemonType::GRASS,    MoveCategory::PHYSICAL, 30, 25, 100}); 
        moveList.push_back({"AERIAL ACE",    PokemonType::FLYING,   MoveCategory::PHYSICAL, 20, 60, 100}); 
        moveList.push_back({"ICICLE SPEAR",  PokemonType::ICE,      MoveCategory::PHYSICAL, 30, 25, 100}); 
        moveList.push_back({"DRAGON CLAW",   PokemonType::DRAGON,   MoveCategory::PHYSICAL, 15, 80, 100});
        moveList.push_back({"FRENZY PLANT",  PokemonType::GRASS,    MoveCategory::SPECIAL,  5, 150, 90}); 
        moveList.push_back({"MUD SHOT",      PokemonType::GROUND,   MoveCategory::SPECIAL,  15, 55, 95}); 
        moveList.push_back({"POISON TAIL",   PokemonType::POISON,   MoveCategory::PHYSICAL, 25, 50, 100}); 
        moveList.push_back({"COVET",         PokemonType::NORMAL,   MoveCategory::PHYSICAL, 25, 60, 100}); 
        moveList.push_back({"VOLT TACKLE",   PokemonType::ELECTRIC, MoveCategory::PHYSICAL, 15, 120, 100});
        moveList.push_back({"MAGICAL LEAF",  PokemonType::GRASS,    MoveCategory::SPECIAL,  20, 60, 100}); 
        moveList.push_back({"LEAF BLADE",    PokemonType::GRASS,    MoveCategory::PHYSICAL, 15, 90, 100});
        moveList.push_back({"ROCK BLAST",    PokemonType::ROCK,     MoveCategory::PHYSICAL, 10, 25, 90}); 
        moveList.push_back({"SHOCK WAVE",    PokemonType::ELECTRIC, MoveCategory::SPECIAL,  20, 60, 100});
        moveList.push_back({"WATER PULSE",   PokemonType::WATER,    MoveCategory::SPECIAL,  20, 60, 100});
        moveList.push_back({"DOOM DESIRE",   PokemonType::STEEL,    MoveCategory::SPECIAL,  5, 140, 100});
        moveList.push_back({"PSYCHO BOOST",  PokemonType::PSYCHIC,  MoveCategory::SPECIAL,  5, 140, 90}); // Move ID 200
    }
    
    void initializeAbilities() {
        // ========================================================================
        // ABILITIES (ABILITY ID 1-3)
        // ========================================================================
        
        // Ability ID 1 - Grass starter ability
        abilityList.push_back(Ability("OVERGROW", "Boosts Grass-type moves by 50% when HP is below 1/3."));
        
        // Ability ID 2 - Fire starter ability
        abilityList.push_back(Ability("BLAZE",    "Boosts Fire-type moves by 50% when HP is below 1/3."));
        
        // Ability ID 3 - Water starter ability
        abilityList.push_back(Ability("TORRENT",  "Boosts Water-type moves by 50% when HP is below 1/3."));
    }
    
    void initializePokemon() {
        // ========================================================================
        // POKEMON INITIALIZATION
        // Parameters: name, type1, type2, natDex#, HP, Atk, Def, SpA, SpD, Spe, 
        //             {moveIDs}, abilityID, {evoType, evoLvl, evoName, evoItem}
        // ========================================================================
        
        // Helper lambda for cleaner Pokemon creation
        auto addPokemon = [this](const string& name, PokemonType t1, PokemonType t2, int idx,
                                  int hp, int atk, int def, int spatk, int spdef, int spd,
                                  vector<int> moves, int ability, Evolution evo) {
            pokedex.push_back({name, t1, t2, idx, hp, atk, def, spatk, spdef, spd, moves, ability, evo});
            pokedex.back().initializePP(moveList);
        };
        
        // ========================================================================
        // STARTER POKEMON (#0-2)
        // ========================================================================
        
        // Entry #0 - Treecko (Grass starter)
        addPokemon("TREECKO",  PokemonType::GRASS,  PokemonType::NONE, 252,
                   40, 45, 35, 65, 55, 70, 
                   {1, 69}, // POUND, ABSORB
                   0, // OVERGROW ability
                   {EvolutionType::LEVEL, 16, "GROVYLE", ""});
        
        // Entry #1 - Torchic (Fire starter)
        addPokemon("TORCHIC",  PokemonType::FIRE,   PokemonType::NONE, 255,
                   45, 60, 40, 70, 50, 45, 
                   {52, 10}, // EMBER, SCRATCH
                   1, // BLAZE ability
                   {EvolutionType::LEVEL, 16, "COMBUSKEN", ""});
        
        // Entry #2 - Mudkip (Water starter)
        addPokemon("MUDKIP",   PokemonType::WATER,  PokemonType::NONE, 258,
                   50, 70, 50, 50, 50, 40, 
                   {55, 33}, // WATER GUN, TACKLE
                   2, // TORRENT ability
                   {EvolutionType::LEVEL, 16, "MARSHTOMP", ""});
        
        
        // Entry #3 - Zigzagoon (Route 101 encounter)
        addPokemon("ZIGZAGOON", PokemonType::NORMAL, PokemonType::NONE, 263,
                   38, 30, 41, 30, 41, 60, 
                   {33, 39}, // TACKLE, TAIL WHIP
                   0, 
                   {EvolutionType::LEVEL, 20, "LINOONE", ""});
        
        // Entry #4 - Poochyena (Route 101 encounter)
        addPokemon("POOCHYENA", PokemonType::DARK,   PokemonType::NONE, 261,
                   35, 55, 35, 30, 30, 35, 
                   {33, 44}, // TACKLE, BITE
                   0, 
                   {EvolutionType::LEVEL, 18, "MIGHTYENA", ""});
        
        // Entry #5 - Geodude (Granite Cave)
        addPokemon("GEODUDE",   PokemonType::ROCK,   PokemonType::GROUND, 74,
                   40, 80, 100, 30, 30, 20, 
                   {33, 29}, // TACKLE, HEADBUTT
                   0, 
                   {EvolutionType::LEVEL, 25, "GRAVELER", ""});
        
        // Entry #6 - Nosepass (Granite Cave)
        addPokemon("NOSEPASS",  PokemonType::ROCK,   PokemonType::NONE, 299,
                   30, 45, 135, 45, 90, 30, 
                   {33, 77}, // TACKLE, ROCK THROW
                   0, 
                   {EvolutionType::NONE, 0, "", ""});
        
        // Entry #7 - Wurmple (Petalburg Woods)
        addPokemon("WURMPLE",   PokemonType::BUG,    PokemonType::NONE, 265,
                   45, 45, 35, 20, 30, 20, 
                   {33}, // TACKLE
                   0, 
                   {EvolutionType::LEVEL, 7, "SILCOON", ""});
        
        // Entry #8 - Taillow (Early routes)
        addPokemon("TAILLOW",   PokemonType::NORMAL, PokemonType::FLYING, 276,
                   40, 55, 30, 30, 30, 85, 
                   {64, 81}, // PECK, QUICK ATTACK
                   0, 
                   {EvolutionType::LEVEL, 22, "SWELLOW", ""});
        
        // Entry #9 - Wingull (Routes near water)
        addPokemon("WINGULL",   PokemonType::WATER,  PokemonType::FLYING, 278,
                   40, 30, 30, 55, 30, 85, 
                   {55, 16}, // WATER GUN, GUST
                   0, 
                   {EvolutionType::LEVEL, 25, "PELIPPER", ""});
        
        // Entry #10 - Ralts (Route 102)
        addPokemon("RALTS",     PokemonType::PSYCHIC, PokemonType::NONE, 280,
                   28, 25, 25, 45, 35, 40, 
                   {79}, // CONFUSION
                   0, 
                   {EvolutionType::LEVEL, 20, "KIRLIA", ""});
        
        // Entry #11 - Shroomish (Petalburg Woods)
        addPokemon("SHROOMISH", PokemonType::GRASS,  PokemonType::NONE, 285,
                   60, 40, 60, 40, 60, 35, 
                   {33, 69}, // TACKLE, ABSORB
                   0, 
                   {EvolutionType::LEVEL, 23, "BRELOOM", ""});
        
        // Entry #12 - Slakoth (Petalburg Woods)
        addPokemon("SLAKOTH",   PokemonType::NORMAL, PokemonType::NONE, 287,
                   60, 60, 60, 35, 35, 30, 
                   {10, 108}, // SCRATCH, SWIFT
                   0, 
                   {EvolutionType::LEVEL, 18, "VIGOROTH", ""});
        
        // Entry #13 - Makuhita (Granite Cave)
        addPokemon("MAKUHITA",  PokemonType::FIGHTING, PokemonType::NONE, 296,
                   72, 60, 30, 20, 30, 25, 
                   {33, 161}, // TACKLE, ARM THRUST
                   0, 
                   {EvolutionType::LEVEL, 24, "HARIYAMA", ""});
        
        // Entry #14 - Aron (Granite Cave)
        addPokemon("ARON",      PokemonType::STEEL,  PokemonType::ROCK, 304,
                   50, 70, 100, 40, 40, 30, 
                   {33, 29}, // TACKLE, HEADBUTT
                   0, 
                   {EvolutionType::LEVEL, 32, "LAIRON", ""});
        
        // Entry #15 - Electrike (Route 110)
        addPokemon("ELECTRIKE", PokemonType::ELECTRIC, PokemonType::NONE, 309,
                   40, 45, 40, 65, 40, 65, 
                   {33, 74}, // TACKLE, THUNDER SHOCK
                   0, 
                   {EvolutionType::LEVEL, 26, "MANECTRIC", ""});
        
        // Entry #16 - Lotad (Routes 102, 114)
        addPokemon("LOTAD",     PokemonType::WATER,  PokemonType::GRASS, 270,
                   40, 30, 30, 40, 50, 30, 
                   {69, 55}, // ABSORB, WATER GUN
                   0, 
                   {EvolutionType::LEVEL, 14, "LOMBRE", ""});
        
        // Entry #17 - Seedot (Routes 102, 117)
        addPokemon("SEEDOT",    PokemonType::GRASS,  PokemonType::NONE, 273,
                   40, 40, 50, 30, 30, 30, 
                   {29}, // HEADBUTT
                   0, 
                   {EvolutionType::LEVEL, 14, "NUZLEAF", ""});
        
        // Entry #18 - Surskit (Route 102)
        addPokemon("SURSKIT",   PokemonType::BUG,    PokemonType::WATER, 283,
                   40, 30, 32, 50, 52, 65, 
                   {99, 81}, // BUBBLE, QUICK ATTACK
                   0, 
                   {EvolutionType::LEVEL, 22, "MASQUERAIN", ""});
        
        // Entry #19 - Whismur (Rusturf Tunnel)
        addPokemon("WHISMUR",   PokemonType::NORMAL, PokemonType::NONE, 293,
                   64, 51, 23, 51, 23, 28, 
                   {1, 173}, // POUND, ASTONISH
                   0, 
                   {EvolutionType::LEVEL, 20, "LOUDRED", ""});
        
        // Entry #20 - Skitty (Route 116)
        addPokemon("SKITTY",    PokemonType::NORMAL, PokemonType::NONE, 300,
                   50, 45, 45, 35, 35, 50, 
                   {33, 39}, // TACKLE, TAIL WHIP
                   0, 
                   {EvolutionType::ITEM, 0, "DELCATTY", "MOON STONE"});
        
        // Entry #21 - Meditite (Mt. Pyre)
        addPokemon("MEDITITE",  PokemonType::FIGHTING, PokemonType::PSYCHIC, 307,
                   30, 40, 55, 40, 55, 60, 
                   {79, 29}, // CONFUSION, HEADBUTT
                   0, 
                   {EvolutionType::LEVEL, 37, "MEDICHAM", ""});
        
        // Entry #22 - Mawile (Granite Cave)
        addPokemon("MAWILE",    PokemonType::STEEL,  PokemonType::FAIRY, 303,
           			50, 85, 85, 55, 55, 50, 
           			{44, 10}, // BITE, SCRATCH
           			0, 
           			{EvolutionType::NONE, 0, "", ""});
        
        // Entry #23 - Sealeo (Shoal Cave)
        addPokemon("SEALEO",    PokemonType::ICE,    PokemonType::WATER, 364,
           			90, 60, 70, 75, 70, 45, 
           			{33, 115, 58, 29}, // TACKLE, POWDER SNOW, ICE BEAM, HEADBUTT
           			0, 
           			{EvolutionType::LEVEL, 44, "WALREIN", ""});
        
        // Entry #24 - Pichu (Safari Zone)
        addPokemon("PICHU",     PokemonType::ELECTRIC, PokemonType::NONE, 172,
           		  20, 40, 15, 35, 35, 60, 
           		  {74, 45}, // THUNDER SHOCK, GROWL
           		  0, 
           		  {EvolutionType::LEVEL, 15, "PIKACHU", ""});
        
        // Entry #25 - Pikachu (Safari Zone)
        addPokemon("PIKACHU",   PokemonType::ELECTRIC, PokemonType::NONE, 25,
           		  35, 55, 40, 50, 50, 90, 
           		  {74, 81, 45}, // THUNDER SHOCK, QUICK ATTACK, GROWL
           		  0, 
           		  {EvolutionType::ITEM, 0, "RAICHU", "THUNDER STONE"});
        
        // Entry #26 - Swablu (Route 114)
        addPokemon("SWABLU",    PokemonType::NORMAL, PokemonType::FLYING, 333,
                   45, 40, 60, 40, 75, 50, 
                   {64, 173}, // PECK, ASTONISH
                   0, 
                   {EvolutionType::LEVEL, 35, "ALTARIA", ""});
        
        // Entry #27 - Barboach (Underwater routes)
        addPokemon("BARBOACH",  PokemonType::WATER,  PokemonType::GROUND, 339,
                   50, 48, 43, 46, 41, 60, 
                   {119, 55}, // MUD-SLAP, WATER GUN
                   0, 
                   {EvolutionType::LEVEL, 30, "WHISCASH", ""});
        
        // Entry #28 - Corphish (Underwater routes)
        addPokemon("CORPHISH",  PokemonType::WATER,  PokemonType::NONE, 341,
                   43, 80, 65, 50, 35, 35, 
                   {99, 11}, // BUBBLE, VISE GRIP
                   0, 
                   {EvolutionType::LEVEL, 30, "CRAWDAUNT", ""});
        
        // Entry #29 - Raichu (Thunder Stone evolution)
        addPokemon("RAICHU",    PokemonType::ELECTRIC, PokemonType::NONE, 26,
           			60, 90, 55, 90, 80, 110, 
           			{74, 75, 76, 9}, // THUNDER SHOCK, THUNDERBOLT, THUNDER, THUNDER PUNCH
           			0, 
           			{EvolutionType::NONE, 0, "", ""});
        
        // Entry #30 - Spheal (Shoal Cave)
        addPokemon("SPHEAL",    PokemonType::ICE,    PokemonType::WATER, 363,
                   70, 40, 50, 55, 50, 25, 
                   {33, 115}, // TACKLE, POWDER SNOW
                   0, 
                   {EvolutionType::LEVEL, 32, "SEALEO", ""});
        
        // Entry #31 - Bagon (Meteor Falls)
        addPokemon("BAGON",     PokemonType::DRAGON, PokemonType::NONE, 371,
                   45, 75, 60, 40, 30, 50, 
                   {82, 52}, // RAGE, EMBER
                   0, 
                   {EvolutionType::LEVEL, 30, "SHELGON", ""});
        
        // Entry #32 - Absol (Route 120)
        addPokemon("ABSOL",     PokemonType::DARK,   PokemonType::NONE, 359,
                   65, 130, 60, 75, 60, 75, 
                   {10, 81}, // SCRATCH, QUICK ATTACK
                   0, 
                   {EvolutionType::NONE, 0, "", ""});
        
        // Entry #33 - PLACEHOLDER BECAUSE OF DUPED POKEMON (Mt. Pyre)
        addPokemon("",   PokemonType::GHOST,  PokemonType::NONE, 353,
                   0, 0, 0, 0, 0, 0, 
                   {0, 0}, // ASTONISH, SHADOW PUNCH
                   0, 
                   {EvolutionType::LEVEL, 0, "", ""});
        
        // Entry #34 - Snorunt (Shoal Cave)
        addPokemon("SNORUNT",   PokemonType::ICE,    PokemonType::NONE, 361,
                   50, 50, 50, 50, 50, 50, 
                   {1, 115}, // POUND, POWDER SNOW
                   0, 
                   {EvolutionType::LEVEL, 42, "GLALIE", ""});
        
        // Entry #35 - Nincada (Route 116)
        addPokemon("NINCADA",   PokemonType::BUG,    PokemonType::GROUND, 290,
                   31, 45, 90, 30, 30, 40, 
                   {10, 98}, // SCRATCH, LEECH LIFE
                   0, 
                   {EvolutionType::LEVEL, 20, "NINJASK", ""});
        
        // Entry #36 - Seviper (Route 114)
        addPokemon("SEVIPER",   PokemonType::POISON, PokemonType::NONE, 336,
           			73, 100, 60, 100, 60, 65, 
           			{40, 44}, // POISON STING, BITE
           			0, 
           			{EvolutionType::NONE, 0, "", ""});

		// Entry #37 - Wailord (Evolves from Wailmer)
		addPokemon("WAILORD",   PokemonType::WATER,  PokemonType::NONE, 321,
           		170, 90, 45, 90, 45, 60, 
           		{55, 34}, // WATER GUN, BODY SLAM
           		0, 
           		{EvolutionType::NONE, 0, "", ""});
        
        // Entry #38 - Wailmer (Underwater routes)
        addPokemon("WAILMER",   PokemonType::WATER,  PokemonType::NONE, 320,
                   130, 70, 35, 70, 35, 60, 
                   {55, 126}, // WATER GUN, GIGA DRAIN
                   0, 
                   {EvolutionType::LEVEL, 40, "WAILORD", ""});
        
        // Entry #39 - Torkoal (Fiery Path)
        addPokemon("TORKOAL",   PokemonType::FIRE,   PokemonType::NONE, 324,
                   70, 85, 140, 85, 70, 20, 
                   {52, 86}, // EMBER, LICK
                   0, 
                   {EvolutionType::NONE, 0, "", ""});
                   
        // Entry #40 - Marill (Routes near water)
        addPokemon("MARILL",    PokemonType::WATER,  PokemonType::NONE, 183,
           70, 20, 50, 20, 50, 40, 
           {33, 39, 55}, // TACKLE, TAIL WHIP, WATER GUN
           0, 
           {EvolutionType::LEVEL, 18, "AZUMARILL", ""});
           
        // Entry #41 - Silcoon (Evolves from Wurmple)
        addPokemon("SILCOON",   PokemonType::BUG,    PokemonType::NONE, 266,
           50, 35, 55, 25, 25, 15, 
           {33, 106}, // TACKLE, ROCK SLIDE
           0, 
           {EvolutionType::LEVEL, 10, "BEAUTIFLY", ""});

		// Entry #42 - Cascoon (Evolves from Wurmple)
		addPokemon("CASCOON",   PokemonType::BUG,    PokemonType::NONE, 268,
           50, 35, 55, 25, 25, 15, 
           {33, 106}, // TACKLE, ROCK SLIDE
           0, 
           {EvolutionType::LEVEL, 10, "DUSTOX", ""});
           
        // Entry #43 - Machop (Fiery Path)
        addPokemon("MACHOP",    PokemonType::FIGHTING, PokemonType::NONE, 66,
           70, 80, 50, 35, 35, 35, 
           {2, 68}, // KARATE CHOP, LOW KICK
           0, 
           {EvolutionType::LEVEL, 28, "MACHOKE", ""});
           
        // Entry #44 - Zubat (Various caves)
        addPokemon("ZUBAT",     PokemonType::POISON, PokemonType::FLYING, 41,
           40, 45, 35, 30, 40, 55, 
           {98, 48}, // LEECH LIFE, SUPERSONIC
           0, 
           {EvolutionType::LEVEL, 22, "GOLBAT", ""});

		// Entry #45 - Sableye (Granite Cave)
		addPokemon("SABLEYE",   PokemonType::DARK,   PokemonType::GHOST, 302,
           50, 75, 75, 65, 65, 50, 
           {43, 10}, // LEER, SCRATCH
           0, 
           {EvolutionType::NONE, 0, "", ""});

		// Entry #46 - Abra (Granite Cave)
		addPokemon("ABRA",      PokemonType::PSYCHIC, PokemonType::NONE, 63,
           25, 20, 15, 105, 55, 90, 
           {109}, // SLASH
           0, 
           {EvolutionType::LEVEL, 16, "KADABRA", ""});
           
        // Entry #47 - Carvanha (Underwater routes)
        addPokemon("CARVANHA",  PokemonType::WATER,  PokemonType::DARK, 318,
           45, 90, 20, 65, 20, 65, 
           {44, 82}, // BITE, RAGE
           0, 
           {EvolutionType::LEVEL, 30, "SHARPEDO", ""});

		// Entry #48 - Oddish (Safari Zone)
		addPokemon("ODDISH", PokemonType::GRASS, PokemonType::POISON, 43,
		   45, 50, 55, 75, 65, 30, 
		   {69, 51}, // ABSORB, ACID
		   0, 
		   {EvolutionType::LEVEL, 21, "GLOOM", ""});
        
        // Entry #49 - Azurill (Bred from Marill)
		addPokemon("AZURILL", PokemonType::NORMAL, PokemonType::FAIRY, 298,
           50, 20, 40, 20, 40, 20, 
           {1, 39, 55}, // POUND, TAIL WHIP, WATER GUN
           0, 
           {EvolutionType::LEVEL, 18, "MARILL", ""});
		
		// Entry #50 - Plusle (Route 110)
		addPokemon("PLUSLE", PokemonType::ELECTRIC, PokemonType::NONE, 311,
		   60, 50, 40, 85, 75, 95, 
		   {45, 81}, // GROWL, QUICK ATTACK
		   0, 
		   {EvolutionType::NONE, 0, "", ""});
		
		// Entry #51 - Minun (Route 110)
		addPokemon("MINUN", PokemonType::ELECTRIC, PokemonType::NONE, 312,
		   60, 40, 50, 75, 85, 95, 
		   {45, 81}, // GROWL, QUICK ATTACK
		   0, 
		   {EvolutionType::NONE, 0, "", ""});
		
		// Entry #52 - Gulpin (Route 110)
		addPokemon("GULPIN", PokemonType::POISON, PokemonType::NONE, 316,
		   70, 43, 53, 43, 53, 40, 
		   {1, 87}, // POUND, SMOG
		   0, 
		   {EvolutionType::LEVEL, 26, "SWALOT", ""});
		   
		// Entry #53 - Lombre (Evolves from Lotad)
		addPokemon("LOMBRE",    PokemonType::WATER,  PokemonType::GRASS, 271,
		   60, 50, 50, 60, 70, 50, 
		   {69, 55, 51, 70}, // ABSORB, WATER GUN, ACID, MEGA DRAIN
		   0, 
		   {EvolutionType::ITEM, 0, "LUDICOLO", "WATER STONE"});
		
		// Entry #54 - Nuzleaf (Evolves from Seedot)
		addPokemon("NUZLEAF",   PokemonType::GRASS,  PokemonType::DARK, 274,
		   70, 70, 40, 60, 40, 60, 
		   {1, 10, 29, 118}, // POUND, SCRATCH, HEADBUTT, SLUDGE BOMB
		   0, 
		   {EvolutionType::ITEM, 0, "SHIFTRY", "LEAF STONE"});
		
		// Entry #55 - Voltorb (New Mauville)
		addPokemon("VOLTORB",   PokemonType::ELECTRIC, PokemonType::NONE, 100,
		   40, 30, 50, 55, 55, 100, 
		   {33, 74, 48, 128}, // TACKLE, THUNDER SHOCK, SUPERSONIC, SPARK
		   0, 
		   {EvolutionType::LEVEL, 30, "ELECTRODE", ""});
		
		// Entry #56 - Magnemite (New Mauville)
		addPokemon("MAGNEMITE", PokemonType::ELECTRIC, PokemonType::STEEL, 81,
		   25, 35, 70, 95, 55, 45, 
		   {33, 74, 48, 84}, // TACKLE, THUNDER SHOCK, SUPERSONIC, SELF-DESTRUCT
		   0, 
		   {EvolutionType::LEVEL, 30, "MAGNETON", ""});
		
		// Entry #57 - Manectric (Evolves from Electrike)
		addPokemon("MANECTRIC", PokemonType::ELECTRIC, PokemonType::NONE, 310,
		   70, 75, 60, 105, 60, 105, 
		   {44, 75, 81, 76}, // BITE, THUNDERBOLT, QUICK ATTACK, THUNDER
		   0, 
		   {EvolutionType::NONE, 0, "", ""});
		
		// Entry #58 - Magneton (Evolves from Magnemite)
		addPokemon("MAGNETON",  PokemonType::ELECTRIC, PokemonType::STEEL, 82,
		   50, 60, 95, 120, 70, 70, 
		   {74, 75, 84, 197}, // THUNDER SHOCK, THUNDERBOLT, SELF-DESTRUCT, SHOCK WAVE
		   0, 
		   {EvolutionType::NONE, 0, "", ""});
		
		// Entry #59 - Sandshrew (Desert)
		addPokemon("SANDSHREW", PokemonType::GROUND, PokemonType::NONE, 27,
	   				50, 75, 85, 20, 30, 40, 
	   				{10, 28}, // SCRATCH, SAND ATTACK
	   				0, 
	   				{EvolutionType::LEVEL, 22, "SANDSLASH", ""});
	
		// Entry #60 - Sandslash (Evolves from Sandshrew)
		addPokemon("SANDSLASH", PokemonType::GROUND, PokemonType::NONE, 28,
		   			75, 100, 110, 45, 55, 65, 
		   			{10, 28, 10, 143}, // SCRATCH, SAND ATTACK, SCRATCH, CRUNCH
		   			0, 
		   			{EvolutionType::NONE, 0, "", ""});
		
		// Entry #61 - Trapinch (Desert)
		addPokemon("TRAPINCH",  PokemonType::GROUND, PokemonType::NONE, 328,
		   			45, 100, 45, 45, 45, 10, 
		   			{44, 28}, // BITE, SAND ATTACK
		   			0, 
		   			{EvolutionType::LEVEL, 35, "VIBRAVA", ""});
		
		// Entry #62 - Cacnea (Desert)
		addPokemon("CACNEA",    PokemonType::GRASS,  PokemonType::NONE, 331,
		   			50, 85, 40, 85, 40, 35, 
		   			{69, 70}, // ABSORB, MEGA DRAIN
		   			0, 
		   			{EvolutionType::LEVEL, 32, "CACTURNE", ""});
		
		// Entry #63 - Baltoy (Desert)
		addPokemon("BALTOY",    PokemonType::GROUND, PokemonType::PSYCHIC, 343,
		   			40, 40, 55, 40, 70, 55, 
		   			{79, 119}, // CONFUSION, MUD-SLAP
		   			0, 
		   			{EvolutionType::LEVEL, 36, "CLAYDOL", ""});
		
		// Entry #64 - Shelgon (Evolves from Bagon)
		addPokemon("SHELGON",   PokemonType::DRAGON, PokemonType::NONE, 372,
           			65, 95, 100, 60, 50, 50, 
           			{82, 29, 52, 143}, // RAGE, HEADBUTT, EMBER, CRUNCH
           			0, 
           			{EvolutionType::LEVEL, 50, "SALAMENCE", ""});
		
		// Entry #65 - Roselia (Various routes)
		addPokemon("ROSELIA",   PokemonType::GRASS,  PokemonType::POISON, 315,
		   			50, 60, 45, 100, 80, 65, 
		   			{69, 70}, // ABSORB, MEGA DRAIN
		   			0, 
		   			{EvolutionType::NONE, 0, "", ""});
		
		// Entry #66 - Numel (Fiery Path)
		addPokemon("NUMEL",     PokemonType::FIRE,   PokemonType::GROUND, 322,
		  			60, 60, 40, 65, 45, 35, 
		  			{33, 52}, // TACKLE, EMBER
		  			0, 
		  			{EvolutionType::LEVEL, 33, "CAMERUPT", ""});
		  			 
		// Entry #67 - Spoink (Jagged Pass)
		addPokemon("SPOINK",    PokemonType::PSYCHIC, PokemonType::NONE, 325,
           		    60, 25, 35, 70, 80, 60, 
           		    {60}, // PSYBEAM
           		    0, 
           		    {EvolutionType::LEVEL, 32, "GRUMPIG", ""});
           		    
		// Entry #68 - Slugma (Fiery Path)
		addPokemon("SLUGMA",    PokemonType::FIRE,   PokemonType::NONE, 218,
           			40, 40, 40, 70, 40, 20, 
           			{52}, // EMBER
           			0, 
           			{EvolutionType::LEVEL, 38, "MAGCARGO", ""});
           
       // Entry #69 - Magcargo (Evolves from Slugma)
       addPokemon("MAGCARGO",  PokemonType::FIRE,   PokemonType::ROCK, 219,
           			60, 50, 120, 90, 80, 30, 
           			{52, 89}, // EMBER, FIRE BLAST
           			0, 
           			{EvolutionType::NONE, 0, "", ""});

		// Entry #70 - Camerupt (Evolves from Numel)
		addPokemon("CAMERUPT",  PokemonType::FIRE,   PokemonType::GROUND, 323,
           			70, 100, 70, 105, 75, 40, 
           			{52, 78, 89}, // EMBER, EARTHQUAKE, FIRE BLAST
           			0, 
           			{EvolutionType::NONE, 0, "", ""});
           
        // Entry #71 - Spinda (Route 113)
        addPokemon("SPINDA",    PokemonType::NORMAL, PokemonType::NONE, 327,
           			60, 60, 60, 60, 60, 60, 
           			{33, 100, 29, 101}, // TACKLE, DIZZY PUNCH, HEADBUTT, CRABHAMMER
           			0, 
           			{EvolutionType::NONE, 0, "", ""});

		// Entry #72 - Vigoroth (Evolves from Slakoth)
		addPokemon("VIGOROTH",  PokemonType::NORMAL, PokemonType::NONE, 288,
		           80, 80, 80, 55, 55, 90, 
		           {10, 103, 108, 163}, // SCRATCH, EXPLOSION, SWIFT, LUSTER PURGE
		           0, 
		           {EvolutionType::LEVEL, 36, "SLAKING", ""});
		
		// Entry #73 - Linoone (Evolves from Zigzagoon)
		addPokemon("LINOONE",   PokemonType::NORMAL, PokemonType::NONE, 264,
		           78, 70, 61, 50, 61, 100, 
		           {29, 103, 10, 163}, // HEADBUTT, EXPLOSION, SCRATCH, LUSTER PURGE
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #74 - Slaking (Evolves from Vigoroth) - NERFED FOR GAMEPLAY BALANCE
		addPokemon("SLAKING",   PokemonType::NORMAL, PokemonType::NONE, 289,
		           120, 35, 80, 50, 55, 70, 
		           {108, 103, 29, 64}, // SWIFT, EXPLOSION, HEADBUTT, PECK
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
        
        // Entry #75 - Kecleon (Route 120)
		addPokemon("KECLEON",   PokemonType::NORMAL, PokemonType::NONE, 352,
		           60, 90, 70, 60, 120, 40, 
		           {10, 43, 10, 108}, // SCRATCH, LEER, SCRATCH, SWIFT
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #76 - Volbeat (Route 117)
		addPokemon("VOLBEAT",   PokemonType::BUG,    PokemonType::NONE, 313,
		           65, 73, 75, 47, 85, 85, 
		           {33, 81, 94, 163}, // TACKLE, QUICK ATTACK, SPIKE CANNON, LUSTER PURGE
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #77 - Illumise (Route 117)
		addPokemon("ILLUMISE",  PokemonType::BUG,    PokemonType::NONE, 314,
		           65, 47, 75, 73, 85, 85, 
		           {33, 81, 94, 163}, // TACKLE, QUICK ATTACK, SPIKE CANNON, LUSTER PURGE
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #78 - Beautifly (Evolves from Silcoon)
		addPokemon("BEAUTIFLY", PokemonType::BUG,    PokemonType::FLYING, 267,
		           60, 70, 50, 100, 50, 65, 
		           {16, 69, 60, 125}, // GUST, ABSORB, PSYBEAM, GIGA DRAIN
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #79 - Ninjask (Evolves from Nincada)
		addPokemon("NINJASK",   PokemonType::BUG,    PokemonType::FLYING, 291,
		           61, 90, 45, 50, 50, 160, 
		           {10, 163, 108, 106}, // SCRATCH, LUSTER PURGE, SWIFT, ROCK SLIDE
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #80 - Loudred (Evolves from Whismur)
		addPokemon("LOUDRED",   PokemonType::NORMAL, PokemonType::NONE, 294,
		           84, 71, 43, 71, 43, 48, 
		           {1, 173, 23, 167}, // POUND, ASTONISH, STOMP, HYPER VOICE
		           0, 
		           {EvolutionType::LEVEL, 40, "EXPLOUD", ""});
		
		// Entry #81 - Swellow (Evolves from Taillow)
		addPokemon("SWELLOW",   PokemonType::NORMAL, PokemonType::FLYING, 277,
		           60, 85, 60, 75, 50, 125, 
		           {64, 81, 17, 186}, // PECK, QUICK ATTACK, WING ATTACK, AERIAL ACE
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #82 - Breloom (Evolves from Shroomish)
		addPokemon("BRELOOM",   PokemonType::GRASS,  PokemonType::FIGHTING, 286,
		           60, 130, 80, 60, 60, 70, 
		           {116, 10, 68, 95}, // MACH PUNCH, SCRATCH, LOW KICK, HIGH JUMP KICK
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #83 - Grimer (Fiery Path)
		addPokemon("GRIMER",    PokemonType::POISON, PokemonType::NONE, 88,
           		   80, 80, 50, 40, 50, 25, 
           		   {1, 87, 51, 124}, // POUND, SMOG, ACID, OUTRAGE
           		   0, 
           		   {EvolutionType::LEVEL, 38, "MUK", ""});

		// Entry #84 - Koffing (Fiery Path)
		addPokemon("KOFFING",   PokemonType::POISON, PokemonType::NONE, 109,
                   40, 65, 95, 60, 45, 35, 
                   {1, 87, 86, 124}, // POUND, SMOG, LICK, OUTRAGE
                   0, 
                   {EvolutionType::LEVEL, 35, "WEEZING", ""});
           
        // Entry #85 - Tropius (Route 119)
		addPokemon("TROPIUS",   PokemonType::GRASS,  PokemonType::FLYING, 357,
                   99, 68, 83, 72, 87, 51, 
                   {16, 73, 69, 23}, // GUST, RAZOR LEAF, ABSORB, STOMP
                   0, 
                   {EvolutionType::NONE, 0, "", ""});
		
		// Entry #86 - Walrein (Evolves from Sealeo)
		addPokemon("WALREIN",   PokemonType::ICE,    PokemonType::WATER, 365,
                   110, 80, 90, 95, 90, 65, 
                   {40, 115, 58, 59}, // POISON STING, POWDER SNOW, ICE BEAM, BLIZZARD
                   0, 
                   {EvolutionType::NONE, 0, "", ""});
		
		// Entry #87 - Magikarp (All water routes)
		addPokemon("MAGIKARP",  PokemonType::WATER,  PokemonType::NONE, 129,
		           20, 10, 55, 15, 20, 80, 
		           {150, 33}, // FAKE OUT, TACKLE
		           0, 
		           {EvolutionType::LEVEL, 20, "GYARADOS", ""});
		
		// Entry #88 - Tentacool (Water routes)
		addPokemon("TENTACOOL", PokemonType::WATER,  PokemonType::POISON, 72,
		           40, 40, 35, 50, 100, 70, 
		           {51, 40, 48, 62}, // ACID, POISON STING, SUPERSONIC, AURORA BEAM
		           0, 
		           {EvolutionType::LEVEL, 30, "TENTACRUEL", ""});
		
		// Entry #89 - Feebas (Route 119)
		addPokemon("FEEBAS",    PokemonType::WATER,  PokemonType::NONE, 349,
		           20, 15, 20, 10, 55, 80, 
		           {150, 33}, // FAKE OUT, TACKLE
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #90 - Gloom (Evolves from Oddish)
		addPokemon("GLOOM",     PokemonType::GRASS,  PokemonType::POISON, 44,
		           60, 65, 70, 85, 75, 40, 
		           {69, 51, 71, 77}, // ABSORB, ACID, RAZOR LEAF, ROCK THROW
		           0, 
		           {EvolutionType::LEVEL, 21, "VILEPLUME", ""});
		
		// Entry #91 - Dustox (Evolves from Cascoon)
		addPokemon("DUSTOX",    PokemonType::BUG,    PokemonType::POISON, 269,
		           60, 50, 70, 50, 90, 65, 
		           {79, 16, 60, 85}, // CONFUSION, GUST, PSYBEAM, EGG BOMB
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #92 - Bellossom (Stone evolution from Gloom)
		addPokemon("BELLOSSOM", PokemonType::GRASS,  PokemonType::NONE, 182,
		           75, 80, 95, 90, 100, 50, 
		           {69, 195, 71, 72}, // ABSORB, LEAF BLADE, RAZOR LEAF, PETAL DANCE
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #93 - Pelipper (Evolves from Wingull)
		addPokemon("PELIPPER",  PokemonType::WATER,  PokemonType::FLYING, 279,
		           60, 50, 100, 95, 70, 65, 
		           {17, 55, 176, 57}, // WING ATTACK, WATER GUN, OVERHEAT, SURF
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #94 - Skarmory (Route 113)
		addPokemon("SKARMORY",  PokemonType::STEEL,  PokemonType::FLYING, 227,
		           65, 80, 140, 40, 70, 70, 
		           {64, 17, 130, 186}, // PECK, WING ATTACK, STEEL WING, AERIAL ACE
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #95 - Altaria (Evolves from Swablu)
		addPokemon("ALTARIA",   PokemonType::DRAGON, PokemonType::FLYING, 334,
		           75, 70, 90, 70, 105, 80, 
		           {64, 135, 188, 189}, // PECK, PURSUIT, ICICLE SPEAR, DRAGON CLAW
		           0, 
		           {EvolutionType::NONE, 0, "", ""});

		// Entry #96 - Shuppet (Mt. Pyre) - Duplicate entry
		addPokemon("SHUPPET",   PokemonType::GHOST,  PokemonType::NONE, 353,
		           44, 75, 35, 63, 33, 45, 
		           {173, 180}, // ASTONISH, SHADOW PUNCH
		           0, 
		           {EvolutionType::LEVEL, 37, "BANETTE", ""});
		
		// Entry #97 - Banette (Evolves from Shuppet)
		addPokemon("BANETTE",   PokemonType::GHOST,  PokemonType::NONE, 354,
		           64, 115, 65, 83, 63, 65, 
		           {173, 180, 146, 44}, // ASTONISH, SHADOW PUNCH, SHADOW BALL, BITE
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #98 - Duskull (Mt. Pyre)
		addPokemon("DUSKULL",   PokemonType::GHOST,  PokemonType::NONE, 355,
		           20, 40, 90, 30, 90, 25, 
		           {43, 173}, // LEER, ASTONISH
		           0, 
		           {EvolutionType::LEVEL, 37, "DUSCLOPS", ""});
		
		// Entry #99 - Dusclops (Evolves from Duskull)
		addPokemon("DUSCLOPS",  PokemonType::GHOST,  PokemonType::NONE, 356,
		           40, 70, 130, 60, 130, 25, 
		           {44, 180, 146, 154}, // BITE, SHADOW PUNCH, SHADOW BALL, FOCUS PUNCH
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #100 - Vulpix (Mt. Pyre)
		addPokemon("VULPIX",    PokemonType::FIRE,   PokemonType::NONE, 37,
		           38, 41, 40, 50, 65, 65, 
		           {52, 173}, // EMBER, ASTONISH
		           0, 
		           {EvolutionType::ITEM, 0, "NINETALES", "FIRE STONE"});
		
		// Entry #101 - Ninetales (Stone evolution from Vulpix)
		addPokemon("NINETALES", PokemonType::FIRE,   PokemonType::NONE, 38,
		           73, 76, 75, 81, 100, 100, 
		           {52, 53, 91, 89}, // EMBER, FLAMETHROWER, WATERFALL, FIRE BLAST
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #102 - Chimecho (Mt. Pyre)
		addPokemon("CHIMECHO",  PokemonType::PSYCHIC, PokemonType::NONE, 358,
		           75, 50, 80, 95, 90, 65, 
		           {173, 79, 60, 80}, // ASTONISH, CONFUSION, PSYBEAM, PSYCHIC
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #103 - Mightyena (Evolves from Poochyena)
		addPokemon("MIGHTYENA", PokemonType::DARK,   PokemonType::NONE, 262,
		           70, 90, 70, 60, 60, 70, 
		           {44, 82, 117, 143}, // BITE, RAGE, FEINT ATTACK, CRUNCH
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #104 - Graveler (Evolves from Geodude)
		addPokemon("GRAVELER",  PokemonType::ROCK,   PokemonType::GROUND, 75,
		           55, 95, 115, 45, 45, 35, 
		           {77, 29, 105, 66}, // ROCK THROW, HEADBUTT, BONEMERANG, SUBMISSION
		           0, 
		           {EvolutionType::LEVEL, 40, "GOLEM", ""});
		
		// Entry #105 - Crobat (High friendship evolution from Golbat)
		addPokemon("CROBAT",    PokemonType::POISON, PokemonType::FLYING, 169,
		           85, 90, 80, 70, 80, 130, 
		           {44, 17, 186, 141}, // BITE, WING ATTACK, AERIAL ACE, CROSS CHOP
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		           
		// Entry #106 - Golbat (Evolves from Zubat)
		addPokemon("GOLBAT",    PokemonType::POISON, PokemonType::FLYING, 42,
           			75, 80, 70, 65, 75, 90, 
           			{44, 98, 17, 141}, // BITE, LEECH LIFE, WING ATTACK, CROSS CHOP
           			0, 
           			{EvolutionType::NONE, 0, "", ""});
					   
		// Entry #107 - Sharpedo (Evolves from Carvanha)
		addPokemon("SHARPEDO",  PokemonType::WATER,  PokemonType::DARK, 319,
           			70, 120, 40, 95, 40, 95, 
           			{44, 82, 117, 143}, // BITE, RAGE, FEINT ATTACK, CRUNCH
           			0, 
           			{EvolutionType::NONE, 0, "", ""});
		
		// Entry #108 - Claydol (Evolves from Baltoy)
		addPokemon("CLAYDOL",   PokemonType::GROUND, PokemonType::PSYCHIC, 344,
		           60, 70, 105, 70, 120, 75, 
		           {79, 60, 182, 78}, // CONFUSION, PSYBEAM, SKY UPPERCUT, EARTHQUAKE
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #109 - Natu (Safari Zone)
		addPokemon("NATU",      PokemonType::PSYCHIC, PokemonType::FLYING, 177,
		           40, 50, 45, 70, 45, 70, 
		           {64, 79}, // PECK, CONFUSION
		           0, 
		           {EvolutionType::LEVEL, 25, "XATU", ""});
		
		// Entry #110 - Xatu (Evolves from Natu)
		addPokemon("XATU",      PokemonType::PSYCHIC, PokemonType::FLYING, 178,
		           65, 75, 70, 95, 70, 95, 
		           {64, 79, 17, 80}, // PECK, CONFUSION, WING ATTACK, PSYCHIC
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #111 - Lunatone (Meteor Falls)
		addPokemon("LUNATONE",  PokemonType::ROCK,    PokemonType::PSYCHIC, 337,
		           90, 55, 65, 95, 85, 70, 
		           {33, 79, 60, 80}, // TACKLE, CONFUSION, PSYBEAM, PSYCHIC
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #112 - Solrock (Meteor Falls)
		addPokemon("SOLROCK",   PokemonType::ROCK,    PokemonType::PSYCHIC, 338,
		           90, 95, 85, 55, 65, 70, 
		           {33, 29, 77, 60}, // TACKLE, HEADBUTT, ROCK THROW, PSYBEAM
		           0, 
		           {EvolutionType::NONE, 0, "", ""});	           

        
        // Entry #113 - Grovyle (Evolves from Treecko)
        addPokemon("GROVYLE",   PokemonType::GRASS,    PokemonType::NONE, 253,
                   50, 65, 45, 85, 65, 95, 
                   {22, 33, 81, 29}, // VINE WHIP, TACKLE, QUICK ATTACK, HEADBUTT
                   0, 
                   {EvolutionType::LEVEL, 36, "SCEPTILE", ""});
        
        // Entry #114 - Combusken (Evolves from Torchic)
        addPokemon("COMBUSKEN", PokemonType::FIRE,     PokemonType::FIGHTING, 256,
                   60, 85, 60, 85, 60, 55, 
                   {52, 10, 24, 64}, // EMBER, SCRATCH, DOUBLE KICK, PECK
                   1, // BLAZE ability
                   {EvolutionType::LEVEL, 36, "BLAZIKEN", ""});
        
        // Entry #115 - Marshtomp (Evolves from Mudkip)
        addPokemon("MARSHTOMP", PokemonType::WATER,    PokemonType::GROUND, 259,
                   70, 85, 70, 60, 70, 50, 
                   {55, 33, 119, 29}, // WATER GUN, TACKLE, MUD-SLAP, HEADBUTT
                   2, // TORRENT ability
                   {EvolutionType::LEVEL, 36, "SWAMPERT", ""});
                   
       // Entry #116 - Sceptile (Final Treecko evolution)
		addPokemon("SCEPTILE",  PokemonType::GRASS,  PokemonType::NONE, 254,
		           70, 85, 65, 105, 85, 120, 
		           {22, 69, 195, 185}, // VINE WHIP, ABSORB, LEAF BLADE, BULLET SEED
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #117 - Blaziken (Final Torchic evolution)
		addPokemon("BLAZIKEN",  PokemonType::FIRE,   PokemonType::FIGHTING, 257,
		           80, 120, 70, 110, 70, 80, 
		           {164, 109, 53, 182}, // BLAZE KICK, SLASH, FLAMETHROWER, SKY UPPERCUT
		           1, // BLAZE ability
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #118 - Swampert (Final Mudkip evolution)
		addPokemon("SWAMPERT",  PokemonType::WATER,  PokemonType::GROUND, 260,
		           100, 110, 90, 85, 90, 60, 
		           {55, 78, 36, 57}, // WATER GUN, EARTHQUAKE, TAKE DOWN, SURF
		           2, // TORRENT ability
		           {EvolutionType::NONE, 0, "", ""}); 
        
   		// Entry #119 - Whiscash (Evolves from Barboach)
		addPokemon("WHISCASH",  PokemonType::WATER,  PokemonType::GROUND, 340,
		           110, 78, 73, 76, 71, 60, 
		           {78, 55, 119, 57}, // EARTHQUAKE, WATER GUN, MUD-SLAP, SURF
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #120 - Crawdaunt (Evolves from Corphish)
		addPokemon("CRAWDAUNT", PokemonType::WATER,  PokemonType::DARK, 342,
		           63, 120, 85, 90, 55, 55, 
		           {99, 143, 44, 117}, // BUBBLE, CRUNCH, BITE, FEINT ATTACK
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #121 - Luvdisc (Routes near water)
		addPokemon("LUVDISC",   PokemonType::WATER,  PokemonType::NONE, 370,
		           43, 30, 55, 40, 65, 97, 
		           {33, 55, 1, 81}, // TACKLE, WATER GUN, POUND, QUICK ATTACK
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #122 - Kingdra (Trade evolution)
		addPokemon("KINGDRA",   PokemonType::WATER,  PokemonType::DRAGON, 230,
		           75, 95, 95, 95, 95, 85, 
		           {55, 99, 56, 57}, // WATER GUN, BUBBLE, HYDRO PUMP, SURF
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
        
        // Entry #123 - Hariyama (Evolves from Makuhita)
		addPokemon("HARIYAMA",  PokemonType::FIGHTING, PokemonType::NONE, 297,
		           144, 120, 60, 40, 60, 50, 
		           {161, 139, 159, 157}, // ARM THRUST, VITAL THROW, KNOCK OFF, SUPERPOWER
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #124 - Lairon (Evolves from Aron)
		addPokemon("LAIRON",    PokemonType::STEEL,  PokemonType::ROCK, 305,
		           60, 90, 140, 50, 50, 40, 
		           {33, 29, 77, 137}, // TACKLE, HEADBUTT, ROCK THROW, IRON TAIL
		           0, 
		           {EvolutionType::LEVEL, 42, "AGGRON", ""});
        
        // Entry #125 - Kirlia (Evolves from Ralts)
		addPokemon("KIRLIA",    PokemonType::PSYCHIC, PokemonType::NONE, 281,
		           38, 35, 35, 65, 55, 50, 
		           {79, 60, 80, 147}, // CONFUSION, PSYBEAM, PSYCHIC, FUTURE SIGHT
		           0, 
		           {EvolutionType::LEVEL, 30, "GARDEVOIR", ""});
		
		// Entry #126 - Gardevoir (Final Ralts evolution)
		addPokemon("GARDEVOIR", PokemonType::PSYCHIC, PokemonType::NONE, 282,
		           68, 65, 65, 125, 115, 80, 
		           {79, 80, 75, 146}, // CONFUSION, PSYCHIC, THUNDERBOLT, SHADOW BALL
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #127 - Cacturne (Evolves from Cacnea)
		addPokemon("CACTURNE",  PokemonType::GRASS,  PokemonType::DARK, 332,
		           70, 115, 60, 115, 60, 55, 
		           {69, 70, 166, 185}, // ABSORB, MEGA DRAIN, NEEDLE ARM, BULLET SEED
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #128 - Ludicolo (Stone evolution from Lombre)
		addPokemon("LUDICOLO",  PokemonType::WATER,  PokemonType::GRASS, 272,
		           80, 70, 70, 90, 100, 70, 
		           {69, 55, 70, 125}, // ABSORB, WATER GUN, MEGA DRAIN, GIGA DRAIN
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #129 - Delcatty (Stone evolution from Skitty)
		addPokemon("DELCATTY",  PokemonType::NORMAL, PokemonType::NONE, 301,
		           70, 65, 65, 55, 55, 70, 
		           {1, 173, 10, 94}, // POUND, ASTONISH, SCRATCH, SPIKE CANNON
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #130 - Vibrava (Evolves from Trapinch)
		addPokemon("VIBRAVA",   PokemonType::GROUND, PokemonType::DRAGON, 329,
		           50, 70, 50, 70, 50, 70, 
		           {44, 28, 188, 189}, // BITE, SAND ATTACK, ICICLE SPEAR, DRAGON CLAW
		           0, 
		           {EvolutionType::LEVEL, 45, "FLYGON", ""});
		
		// Entry #131 - Flygon (Final Trapinch evolution)
		addPokemon("FLYGON",    PokemonType::GROUND, PokemonType::DRAGON, 330,
		           80, 100, 80, 80, 80, 100, 
		           {44, 28, 78, 189}, // BITE, SAND ATTACK, EARTHQUAKE, DRAGON CLAW
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #132 - Salamence (Final Bagon evolution)
		addPokemon("SALAMENCE", PokemonType::DRAGON, PokemonType::FLYING, 373,
		           95, 135, 80, 110, 80, 100, 
		           {189, 19, 143, 134}, // DRAGON CLAW, FLY, CRUNCH, DRAGON BREATH
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
				   
		// Entry #133 - Shiftry (Evolution from Nuzleaf)
		addPokemon("SHIFTRY",   PokemonType::GRASS,  PokemonType::DARK, 275,
		           90, 100, 60, 90, 60, 80, 
		           {10, 73, 117, 185}, // SCRATCH, RAZOR LEAF, FEINT ATTACK, BULLET SEED
		           0, 
		           {EvolutionType::NONE, 0, "", ""});

		// Entry #134 - Glalie
		addPokemon("GLALIE",    PokemonType::ICE,    PokemonType::NONE, 362,
		           80, 80, 80, 80, 80, 80, 
		           {44, 115, 58, 59}, // BITE, POWDER SNOW, ICE BEAM, BLIZZARD
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #135 - Tentacruel (Evolves from Tentacool)
		addPokemon("TENTACRUEL", PokemonType::WATER, PokemonType::POISON, 73,
		           80, 70, 65, 80, 120, 100, 
		           {51, 40, 61, 56}, // ACID, POISON STING, BUBBLE BEAM, HYDRO PUMP
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #136 - Gyarados (Evolves from Magikarp)
		addPokemon("GYARADOS",  PokemonType::WATER,  PokemonType::FLYING, 130,
		           95, 125, 79, 60, 100, 81, 
		           {44, 17, 56, 63}, // BITE, WING ATTACK, HYDRO PUMP, HYPER BEAM
		           0, 
		           {EvolutionType::NONE, 0, "", ""});
		
		// Entry #137 - Milotic
		addPokemon("MILOTIC",   PokemonType::WATER,  PokemonType::NONE, 350,
		           95, 60, 79, 100, 125, 81, 
		           {55, 56, 57, 149}, // WATER GUN, HYDRO PUMP, SURF, WHIRLPOOL
		           0, 
		           {EvolutionType::NONE, 0, "", ""});

}
    void initializeLearnsets() {
    // ========================================================================
    // STARTER POKEMON (#0-2)
    // ========================================================================
    
    // TREECKO (#0) - Initial: POUND(1), ABSORB(69)
    pokedex[0].learnset = 
    {
        {11, 82},  // Lvl 11 - QUICK ATTACK
        {16, 135}, // Lvl 16 - PURSUIT
        {21, 71},  // Lvl 21 - RAZOR LEAF
        {26, 70},  // Lvl 26 - MEGA DRAIN
        {31, 21}   // Lvl 31 - SLAM
    };
    
    // TORCHIC (#1) - Initial: EMBER(52), SCRATCH(10)
    pokedex[1].learnset = 
    {
        {7, 45},   // Lvl 7  - GROWL
        {16, 64},  // Lvl 16 - PECK
        {21, 82},  // Lvl 21 - QUICK ATTACK
        {28, 7},   // Lvl 28 - FIRE PUNCH
        {36, 53}   // Lvl 36 - FLAMETHROWER
    };
    
    // MUDKIP (#2) - Initial: WATER GUN(55), TACKLE(33)
    pokedex[2].learnset = 
    {
        {6, 28},   // Lvl 6  - SAND ATTACK
        {10, 45},  // Lvl 10 - GROWL
        {16, 36},  // Lvl 16 - TAKE DOWN
        {24, 78},  // Lvl 24 - EARTHQUAKE
        {32, 57}   // Lvl 32 - SURF
    };
    
    // ========================================================================
    // EARLY GAME ENCOUNTERS (#3-12)
    // ========================================================================
    
    // ZIGZAGOON (#3) - Initial: TACKLE(33), TAIL WHIP(39)
    pokedex[3].learnset = 
    {
        {5, 1},    // Lvl 5  - POUND
        {9, 45},   // Lvl 9  - GROWL
        {13, 29},  // Lvl 13 - HEADBUTT
        {17, 28},  // Lvl 17 - SAND ATTACK
        {21, 36}   // Lvl 21 - TAKE DOWN
    };
    
    // POOCHYENA (#4) - Initial: TACKLE(33), BITE(44)
    pokedex[4].learnset = 
    {
        {5, 45},   // Lvl 5  - GROWL
        {9, 118},  // Lvl 9  - FEINT ATTACK
        {13, 46},  // Lvl 13 - ROAR
        {17, 82},  // Lvl 17 - QUICK ATTACK
        {25, 143}  // Lvl 25 - CRUNCH
    };
    
    // GEODUDE (#5) - Initial: TACKLE(33), HEADBUTT(29)
    pokedex[5].learnset = 
    {
        {6, 43},   // Lvl 6  - LEER
        {11, 77},  // Lvl 11 - ROCK THROW
        {16, 138}, // Lvl 16 - METAL CLAW
        {21, 78},  // Lvl 21 - EARTHQUAKE
        {29, 106}  // Lvl 29 - ROCK SLIDE
    };
    
    // NOSEPASS (#6) - Initial: TACKLE(33), ROCK THROW(77)
    pokedex[6].learnset = 
    {
        {7, 106},  // Lvl 7  - ROCK SLIDE
        {13, 28},  // Lvl 13 - SAND ATTACK
        {16, 43},  // Lvl 16 - LEER
        {22, 145}, // Lvl 22 - ANCIENT POWER
        {30, 78}   // Lvl 30 - EARTHQUAKE
    };
    
    // WURMPLE (#7) - Initial: TACKLE(33)
    pokedex[7].learnset = 
    {
        {5, 41}    // Lvl 5  - TWINEEDLE
    };
    
    // TAILLOW (#8) - Initial: PECK(64), QUICK ATTACK(82)
    pokedex[8].learnset = 
    {
        {4, 45},   // Lvl 4  - GROWL
        {8, 16},   // Lvl 8  - GUST
        {13, 17},  // Lvl 13 - WING ATTACK
        {19, 186}, // Lvl 19 - AERIAL ACE
        {26, 65}   // Lvl 26 - DRILL PECK
    };
    
    // WINGULL (#9) - Initial: WATER GUN(55), GUST(16)
    pokedex[9].learnset = 
    {
        {7, 48},   // Lvl 7  - SUPERSONIC
        {13, 17},  // Lvl 13 - WING ATTACK
        {21, 149}, // Lvl 21 - WHIRLPOOL
        {30, 57},  // Lvl 30 - SURF
        {40, 56}   // Lvl 40 - HYDRO PUMP
    };
    
    // RALTS (#10) - Initial: CONFUSION(79)
    pokedex[10].learnset = 
    {
        {6, 109},  // Lvl 6  - TELEPORT (using SLASH as placeholder)
        {11, 60},  // Lvl 11 - PSYBEAM
        {16, 79},  // Lvl 16 - CONFUSION (kept for level up)
        {21, 80},  // Lvl 21 - PSYCHIC
        {26, 181}  // Lvl 26 - SHADOW PUNCH
    };
    
    // SHROOMISH (#11) - Initial: TACKLE(33), ABSORB(69)
    pokedex[11].learnset = 
    {
        {5, 40},   // Lvl 5  - POISON STING
        {10, 70},  // Lvl 10 - MEGA DRAIN
        {16, 23},  // Lvl 16 - STOMP
        {22, 125}, // Lvl 22 - GIGA DRAIN
        {28, 88}   // Lvl 28 - SLUDGE
    };
    
    // SLAKOTH (#12) - Initial: SCRATCH(10), HYPER FANG(107)
    pokedex[12].learnset = 
    {
        {7, 45},   // Lvl 7  - GROWL
        {13, 1},   // Lvl 13 - POUND
        {19, 31},  // Lvl 19 - FURY ATTACK
        {25, 29},  // Lvl 25 - HEADBUTT
        {31, 108}  // Lvl 31 - TRI ATTACK
    };
    
    // ========================================================================
    // MID-GAME POKEMON (#13-40)
    // ========================================================================
    
    // MAKUHITA (#13) - Initial: TACKLE(33), ARM THRUST(161)
    pokedex[13].learnset = 
    {
        {7, 68},   // Lvl 7  - LOW KICK
        {13, 8},   // Lvl 13 - KARATE CHOP
        {19, 182}, // Lvl 19 - SKY UPPERCUT
        {25, 159}, // Lvl 25 - BRICK BREAK
        {31, 157}  // Lvl 31 - SUPERPOWER
    };
    
    // ARON (#14) - Initial: TACKLE(33), HEADBUTT(29)
    pokedex[14].learnset = 
    {
        {7, 138},  // Lvl 7  - METAL CLAW
        {13, 77},  // Lvl 13 - ROCK THROW
        {19, 137}, // Lvl 19 - IRON TAIL
        {25, 106}, // Lvl 25 - ROCK SLIDE
        {37, 36}   // Lvl 37 - TAKE DOWN
    };
    
    // ELECTRIKE (#15) - Initial: TACKLE(33), THUNDER SHOCK(74)
    pokedex[15].learnset = 
    {
        {8, 43},   // Lvl 8  - LEER
        {13, 128}, // Lvl 13 - SPARK
        {20, 44},  // Lvl 20 - BITE
        {26, 75},  // Lvl 26 - THUNDERBOLT
        {35, 76}   // Lvl 35 - THUNDER
    };
    
    // LOTAD (#16) - Initial: ABSORB(69), WATER GUN(55)
    pokedex[16].learnset = 
    {
        {7, 173},  // Lvl 7  - ASTONISH
        {13, 70},  // Lvl 13 - MEGA DRAIN
        {21, 61},  // Lvl 21 - BUBBLE BEAM
        {31, 125}, // Lvl 31 - GIGA DRAIN
        {37, 57}   // Lvl 37 - SURF
    };
    
    // SEEDOT (#17) - Initial: HEADBUTT(29)
    pokedex[17].learnset = 
    {
        {7, 1},    // Lvl 7  - POUND
        {13, 69},  // Lvl 13 - ABSORB
        {19, 185}, // Lvl 19 - BULLET SEED
        {25, 71},  // Lvl 25 - RAZOR LEAF
        {31, 103}  // Lvl 31 - EXPLOSION
    };
    
    // SURSKIT (#18) - Initial: BUBBLE(100), QUICK ATTACK(82)
    pokedex[18].learnset = 
    {
        {7, 61},   // Lvl 7  - BUBBLE BEAM
        {13, 16},  // Lvl 13 - GUST
        {19, 186}, // Lvl 19 - AERIAL ACE
        {25, 94},  // Lvl 25 - SPIKE CANNON
        {31, 57}   // Lvl 31 - SURF
    };
    
    // WHISMUR (#19) - Initial: POUND(1), ASTONISH(173)
    pokedex[19].learnset = 
    {
        {5, 45},   // Lvl 5  - GROWL
        {11, 151}, // Lvl 11 - UPROAR
        {15, 29},  // Lvl 15 - HEADBUTT
        {23, 167}, // Lvl 23 - HYPER VOICE
        {29, 108}  // Lvl 29 - TRI ATTACK
    };
    
    // SKITTY (#20) - Initial: TACKLE(33), TAIL WHIP(39)
    pokedex[20].learnset = 
    {
        {4, 45},   // Lvl 4  - GROWL
        {7, 1},    // Lvl 7  - POUND
        {13, 3},   // Lvl 13 - DOUBLE SLAP
        {19, 82},  // Lvl 19 - QUICK ATTACK
        {25, 108}  // Lvl 25 - TRI ATTACK
    };
    
    // MEDITITE (#21) - Initial: CONFUSION(79), HEADBUTT(29)
    pokedex[21].learnset = 
    {
        {8, 68},   // Lvl 8  - LOW KICK
        {15, 95},  // Lvl 15 - HIGH JUMP KICK
        {22, 60},  // Lvl 22 - PSYBEAM
        {29, 80},  // Lvl 29 - PSYCHIC
        {36, 182}  // Lvl 36 - SKY UPPERCUT
    };
    
    // MAWILE (#22) - Initial: BITE(44), SCRATCH(10)
    pokedex[22].learnset = 
    {
        {6, 173},  // Lvl 6  - ASTONISH
        {11, 118}, // Lvl 11 - FEINT ATTACK
        {16, 143}, // Lvl 16 - CRUNCH
        {21, 137}, // Lvl 21 - IRON TAIL
        {26, 138}  // Lvl 26 - METAL CLAW
    };
    
    // SEALEO (#23) - Initial: TACKLE(33), POWDER SNOW(115), ICE BEAM(58), HEADBUTT(29)
    pokedex[23].learnset = 
    {
        {7, 55},   // Lvl 7  - WATER GUN
        {13, 122}, // Lvl 13 - ICY WIND
        {19, 61},  // Lvl 19 - BUBBLE BEAM
        {25, 59},  // Lvl 25 - BLIZZARD
        {32, 34}   // Lvl 32 - BODY SLAM
    };
    
    // PICHU (#24) - Initial: THUNDER SHOCK(74), GROWL(45)
    pokedex[24].learnset = 
    {
        {6, 82},   // Lvl 6  - QUICK ATTACK
        {8, 39},   // Lvl 8  - TAIL WHIP
        {11, 93}   // Lvl 11 - SWIFT
    };
    
    // PIKACHU (#25) - Initial: THUNDER SHOCK(74), QUICK ATTACK(82), GROWL(45)
    pokedex[25].learnset = 
    {
        {6, 39},   // Lvl 6  - TAIL WHIP
        {8, 128},  // Lvl 8  - SPARK
        {13, 93},  // Lvl 13 - SWIFT
        {20, 75},  // Lvl 20 - THUNDERBOLT
        {26, 9},   // Lvl 26 - THUNDER PUNCH
        {33, 76}   // Lvl 33 - THUNDER
    };
    
    // SWABLU (#26) - Initial: PECK(64), ASTONISH(173)
    pokedex[26].learnset = 
    {
        {8, 16},   // Lvl 8  - GUST
        {11, 47},  // Lvl 11 - SING
        {18, 17},  // Lvl 18 - WING ATTACK
        {25, 186}, // Lvl 25 - AERIAL ACE
        {32, 135}  // Lvl 32 - PURSUIT
    };
    
    // BARBOACH (#27) - Initial: MUD-SLAP(119), WATER GUN(55)
    pokedex[27].learnset = 
    {
        {6, 61},   // Lvl 6  - BUBBLE BEAM
        {16, 198}, // Lvl 16 - WATER PULSE
        {26, 78},  // Lvl 26 - EARTHQUAKE
        {31, 184}, // Lvl 31 - MUDDY WATER
        {36, 57}   // Lvl 36 - SURF
    };
    
    // CORPHISH (#28) - Initial: BUBBLE(100), VISE GRIP(11)
    pokedex[28].learnset = 
    {
        {7, 43},   // Lvl 7  - LEER
        {10, 44},  // Lvl 10 - BITE
        {13, 159}, // Lvl 13 - KNOCK OFF
        {20, 143}, // Lvl 20 - CRUNCH
        {26, 102}, // Lvl 26 - CRABHAMMER
        {32, 100}  // Lvl 32 - DIZZY PUNCH
    };
    
    // RAICHU (#29) - Initial: THUNDER SHOCK(74), THUNDERBOLT(75), THUNDER(76), THUNDER PUNCH(9)
    pokedex[29].learnset = {};  // Fully evolved, no level-up moves needed
    
    // SPHEAL (#30) - Initial: TACKLE(33), POWDER SNOW(115)
    pokedex[30].learnset = 
    {
        {7, 55},   // Lvl 7  - WATER GUN
        {13, 61},  // Lvl 13 - BUBBLE BEAM
        {19, 58},  // Lvl 19 - ICE BEAM
        {25, 122}, // Lvl 25 - ICY WIND
        {31, 59}   // Lvl 31 - BLIZZARD
    };
    
    // BAGON (#31) - Initial: RAGE(82), EMBER(52)
    pokedex[31].learnset = 
    {
        {5, 44},   // Lvl 5  - BITE
        {9, 29},   // Lvl 9  - HEADBUTT
        {17, 189}, // Lvl 17 - DRAGON CLAW
        {21, 143}, // Lvl 21 - CRUNCH
        {25, 134}, // Lvl 25 - DRAGON BREATH
        {33, 53}   // Lvl 33 - FLAMETHROWER
    };
    
    // ABSOL (#32) - Initial: SCRATCH(10), QUICK ATTACK(82)
    pokedex[32].learnset = 
    {
        {5, 43},   // Lvl 5  - LEER
        {13, 14},  // Lvl 13 - SWORDS DANCE
        {17, 109}, // Lvl 17 - SLASH
        {21, 44},  // Lvl 21 - BITE
        {31, 106}, // Lvl 31 - ROCK SLIDE
        {41, 143}  // Lvl 41 - CRUNCH
    };
    
    // PLACEHOLDER (#33) -
    pokedex[33].learnset = 
    {
        {0, 0}
    };
    
    // SNORUNT (#34) - Initial: POUND(1), POWDER SNOW(115)
    pokedex[34].learnset = 
    {
        {4, 43},   // Lvl 4  - LEER
        {10, 8},   // Lvl 10 - ICE PUNCH
        {13, 122}, // Lvl 13 - ICY WIND
        {19, 29},  // Lvl 19 - HEADBUTT
        {22, 58},  // Lvl 22 - ICE BEAM
        {28, 143}, // Lvl 28 - CRUNCH
        {37, 59}   // Lvl 37 - BLIZZARD
    };
    
    // NINCADA (#35) - Initial: SCRATCH(10), LEECH LIFE(99)
    pokedex[35].learnset = 
    {
        {5, 28},   // Lvl 5  - SAND ATTACK
        {9, 43},   // Lvl 9  - LEER
        {14, 129}, // Lvl 14 - FURY CUTTER
        {19, 119}, // Lvl 19 - MUD-SLAP
        {25, 178}  // Lvl 25 - ROCK TOMB
    };
    
    // SEVIPER (#36) - Initial: POISON STING(40), BITE(44)
    pokedex[36].learnset = 
    {
        {7, 43},   // Lvl 7  - LEER
        {10, 118}, // Lvl 10 - FEINT ATTACK
        {16, 192}, // Lvl 16 - POISON TAIL
        {19, 87},  // Lvl 19 - SMOG
        {25, 88},  // Lvl 25 - SLUDGE
        {28, 143}, // Lvl 28 - CRUNCH
        {34, 118}, // Lvl 34 - SLUDGE BOMB
        {43, 90}   // Lvl 43 - FIRE BLAST
    };
    
    // WAILORD (#37) - Initial: WATER GUN(55), BODY SLAM(34)
    pokedex[37].learnset = 
    {
        {5, 149},  // Lvl 5  - WHIRLPOOL
        {14, 61},  // Lvl 14 - BUBBLE BEAM
        {19, 91},  // Lvl 19 - WATERFALL
        {32, 56},  // Lvl 32 - HYDRO PUMP
        {40, 57}   // Lvl 40 - SURF
    };
    
    // WAILMER (#38) - Initial: WATER GUN(55), GIGA DRAIN(126)
    pokedex[38].learnset = 
    {
        {5, 149},  // Lvl 5  - WHIRLPOOL
        {10, 173}, // Lvl 10 - ASTONISH
        {14, 61},  // Lvl 14 - BUBBLE BEAM
        {19, 91},  // Lvl 19 - WATERFALL
        {28, 34},  // Lvl 28 - BODY SLAM
        {37, 56}   // Lvl 37 - HYDRO PUMP
    };
    
    // TORKOAL (#39) - Initial: EMBER(52), LICK(86)
    pokedex[39].learnset = 
    {
        {4, 87},   // Lvl 4  - SMOG
        {7, 73},   // Lvl 7  - FIRE SPIN
        {14, 34},  // Lvl 14 - BODY SLAM
        {20, 53},  // Lvl 20 - FLAMETHROWER
        {27, 137}, // Lvl 27 - IRON TAIL
        {34, 176}, // Lvl 34 - OVERHEAT
        {40, 152}  // Lvl 40 - HEAT WAVE
    };
    
    // MARILL (#40) - Initial: TACKLE(33), TAIL WHIP(39), WATER GUN(55)
    pokedex[40].learnset = 
    {
        {3, 43},   // Lvl 3  - LEER
        {6, 99},   // Lvl 6  - BUBBLE
        {10, 61},  // Lvl 10 - BUBBLE BEAM
        {15, 91},  // Lvl 15 - WATERFALL
        {28, 34},  // Lvl 28 - BODY SLAM
        {36, 56}   // Lvl 36 - HYDRO PUMP
    };
    
    // ========================================================================
    // EVOLUTIONS & REMAINING POKEMON (#41-137)
    // ========================================================================
    
    // SILCOON (#41) - Initial: TACKLE(33), ROCK SLIDE(106)
    pokedex[41].learnset = 
    {
        {7, 106}   // Lvl 7  - ROCK SLIDE (keep as defensive shell)
    };
    
    // CASCOON (#42) - Initial: TACKLE(33), ROCK SLIDE(106)
    pokedex[42].learnset = 
    {
        {7, 106}   // Lvl 7  - ROCK SLIDE
    };
    
    // MACHOP (#43) - Initial: KARATE CHOP(2), LOW KICK(68)
    pokedex[43].learnset = 
    {
        {7, 43},   // Lvl 7  - LEER
        {13, 8},   // Lvl 13 - FOCUS ENERGY (using KARATE CHOP)
        {25, 66},  // Lvl 25 - SUBMISSION
        {31, 157}, // Lvl 31 - SUPERPOWER
        {37, 182}  // Lvl 37 - SKY UPPERCUT
    };
    
    // ZUBAT (#44) - Initial: LEECH LIFE(99), SUPERSONIC(48)
    pokedex[44].learnset = 
    {
        {6, 44},   // Lvl 6  - BITE
        {11, 173}, // Lvl 11 - ASTONISH
        {16, 17},  // Lvl 16 - WING ATTACK
        {21, 40},  // Lvl 21 - POISON STING
        {26, 88},  // Lvl 26 - SLUDGE
        {31, 146}  // Lvl 31 - SHADOW BALL
    };
    
    // SABLEYE (#45) - Initial: LEER(43), SCRATCH(10)
    pokedex[45].learnset = 
    {
        {5, 173},  // Lvl 5  - ASTONISH
        {9, 86},   // Lvl 9  - LICK
        {13, 118}, // Lvl 13 - FEINT ATTACK
        {17, 180}, // Lvl 17 - SHADOW PUNCH
        {21, 95},  // Lvl 21 - HIGH JUMP KICK
        {25, 145}, // Lvl 25 - ANCIENT POWER
        {29, 146}, // Lvl 29 - SHADOW BALL
        {37, 109}  // Lvl 37 - SLASH
    };
    
    // ABRA (#46) - Initial: SLASH(109)
    pokedex[46].learnset = {};  // Abra learns no moves by level up in Gen 3
    
    // CARVANHA (#47) - Initial: BITE(44), RAGE(83)
    pokedex[47].learnset = 
    {
        {6, 82},   // Lvl 6  - QUICK ATTACK
        {11, 118}, // Lvl 11 - FEINT ATTACK
        {16, 143}, // Lvl 16 - CRUNCH
        {21, 10},  // Lvl 21 - SCRATCH
        {26, 56}   // Lvl 26 - HYDRO PUMP
    };
    
    // ODDISH (#48) - Initial: ABSORB(69), ACID(51)
    pokedex[48].learnset = 
    {
        {7, 40},   // Lvl 7  - POISON STING
        {14, 71},  // Lvl 14 - RAZOR LEAF
        {16, 73},  // Lvl 16 - PETAL DANCE
        {18, 70},  // Lvl 18 - MEGA DRAIN
        {23, 125}, // Lvl 23 - GIGA DRAIN
        {32, 88}   // Lvl 32 - SLUDGE
    };
    
    // AZURILL (#49) - Initial: POUND(1), TAIL WHIP(39), WATER GUN(55)
    pokedex[49].learnset = 
    {
        {3, 99},   // Lvl 3  - BUBBLE
        {6, 173},  // Lvl 6  - ASTONISH
        {10, 34}   // Lvl 10 - BODY SLAM
    };
    
    // PLUSLE (#50) - Initial: GROWL(45), QUICK ATTACK(82)
    pokedex[50].learnset = 
    {
        {4, 74},   // Lvl 4  - THUNDER SHOCK
        {13, 128}, // Lvl 13 - SPARK
        {19, 93},  // Lvl 19 - SWIFT
        {22, 75},  // Lvl 22 - THUNDERBOLT
        {28, 76}   // Lvl 28 - THUNDER
    };
    
    // MINUN (#51) - Initial: GROWL(45), QUICK ATTACK(82)
    pokedex[51].learnset = 
    {
        {4, 74},   // Lvl 4  - THUNDER SHOCK
        {10, 128}, // Lvl 10 - SPARK
        {13, 93},  // Lvl 13 - SWIFT
        {19, 75},  // Lvl 19 - THUNDERBOLT
        {22, 9},   // Lvl 22 - THUNDER PUNCH
        {28, 76}   // Lvl 28 - THUNDER
    };
    
    // GULPIN (#52) - Initial: POUND(1), SMOG(87)
    pokedex[52].learnset = 
    {
        {6, 173},  // Lvl 6  - ASTONISH
        {9, 51},   // Lvl 9  - ACID
        {14, 88},  // Lvl 14 - SLUDGE
        {23, 118}, // Lvl 23 - SLUDGE BOMB
        {28, 34},  // Lvl 28 - BODY SLAM
        {44, 103}  // Lvl 44 - EXPLOSION
    };
    
    // LOMBRE (#53) - Initial: ABSORB(69), WATER GUN(55), ACID(51), MEGA DRAIN(70)
    pokedex[53].learnset = 
    {
        {13, 125}, // Lvl 13 - GIGA DRAIN
        {19, 61},  // Lvl 19 - BUBBLE BEAM
        {25, 71},  // Lvl 25 - RAZOR LEAF
        {31, 185}, // Lvl 31 - BULLET SEED
        {37, 57}   // Lvl 37 - SURF
    };
    
    // NUZLEAF (#54) - Initial: POUND(1), SCRATCH(10), HEADBUTT(29), SLUDGE BOMB(118)
    pokedex[54].learnset = 
    {
        {7, 118},  // Lvl 7  - FEINT ATTACK
        {13, 71},  // Lvl 13 - RAZOR LEAF
        {19, 173}, // Lvl 19 - ASTONISH
        {25, 181}, // Lvl 25 - EXTRASENSORY
        {31, 185}, // Lvl 31 - BULLET SEED
        {37, 143}  // Lvl 37 - CRUNCH
    };
    
    // VOLTORB (#55) - Initial: TACKLE(33), SUPERSONIC(48), THUNDER SHOCK(74), SPARK(128)
    pokedex[55].learnset = 
    {
        {4, 49},   // Lvl 4  - SONIC BOOM
        {15, 75},  // Lvl 15 - THUNDERBOLT
        {22, 84},  // Lvl 22 - SELF-DESTRUCT
        {27, 121}, // Lvl 27 - ZAP CANNON
        {33, 76}   // Lvl 33 - THUNDER
    };
    
    // MAGNEMITE (#56) - Initial: TACKLE(33), THUNDER SHOCK(74), SUPERSONIC(48), SELF-DESTRUCT(84)
    pokedex[56].learnset = 
    {
        {6, 128},  // Lvl 6  - SPARK
        {11, 49},  // Lvl 11 - SONIC BOOM
        {21, 75},  // Lvl 21 - THUNDERBOLT
        {27, 197}, // Lvl 27 - SHOCK WAVE
        {35, 138}, // Lvl 35 - METAL CLAW
        {43, 76}   // Lvl 43 - THUNDER
    };
    
    // MANECTRIC (#57) - Initial: BITE(44), LEER(43), QUICK ATTACK(82), THUNDER SHOCK(74)
    pokedex[57].learnset = 
    {
        {4, 45},   // Lvl 4  - GROWL
        {11, 128}, // Lvl 11 - SPARK
        {18, 75},  // Lvl 18 - THUNDERBOLT
        {25, 143}, // Lvl 25 - CRUNCH
        {32, 76},  // Lvl 32 - THUNDER
        {39, 77}  // Lvl 39 -  ROCK THROW
    };
    
    // MAGNETON (#58) - Initial: THUNDER SHOCK(74), THUNDERBOLT(75), SELF-DESTRUCT(84), SHOCK WAVE(197)
    pokedex[58].learnset = 
    {
        {6, 128},  // Lvl 6  - SPARK
        {11, 49},  // Lvl 11 - SONIC BOOM
        {16, 48},  // Lvl 16 - SUPERSONIC
        {30, 138}, // Lvl 30 - METAL CLAW
        {43, 76}   // Lvl 43 - THUNDER
    };
    
    // SANDSHREW (#59) - Initial: SCRATCH(10), SAND ATTACK(28)
    pokedex[59].learnset = 
    {
        {6, 40},   // Lvl 6  - POISON STING
        {11, 160}, // Lvl 11 - SECRET POWER
        {17, 106}, // Lvl 17 - ROCK SLIDE
        {23, 109}, // Lvl 23 - SLASH
        {30, 143}, // Lvl 30 - CRUNCH
        {37, 137}  // Lvl 37 - IRON TAIL
    };
    
    // SANDSLASH (#60) - Initial: SCRATCH(10), SAND ATTACK(28), SCRATCH(10), CRUNCH(143)
    pokedex[60].learnset = 
    {
        {6, 40},   // Lvl 6  - POISON STING
        {11, 163}, // Lvl 11 - SECRET POWER
        {17, 106}, // Lvl 17 - ROCK SLIDE
        {22, 109}, // Lvl 22 - SLASH
        {24, 78},  // Lvl 24 - EARTHQUAKE
        {33, 137}, // Lvl 33 - IRON TAIL
    };
    
    // TRAPINCH (#61) - Initial: BITE(44), SAND ATTACK(28)
    pokedex[61].learnset = 
    {
        {9, 129},  // Lvl 9  - FURY CUTTER
        {17, 143}, // Lvl 17 - CRUNCH
        {25, 78},  // Lvl 25 - EARTHQUAKE
        {33, 95},  // Lvl 33 - HIGH JUMP KICK
        {41, 32}   // Lvl 41 - HORN DRILL
    };
    
    // CACNEA (#62) - Initial: ABSORB(69), LEER(43), LEECH LIFE(99), MEGA DRAIN(70), POISON STING(40), SAND ATTACK(28), NEEDLE ARM(166), HYPER VOICE(167), BULLET SEED(185), PIN MISSILE(42), LEAF BLADE(195), SLUDGE BOMB(118)
    pokedex[62].learnset = 
    {
        {9, 71},   // Lvl 9  - RAZOR LEAF
        {13, 28},  // Lvl 13 - SAND ATTACK
        {17, 166}, // Lvl 17 - NEEDLE ARM 
        {21, 167}, // Lvl 21 - HYPER VOICE 
        {25, 185}, // Lvl 25 - BULLET SEED
        {29, 125}, // Lvl 29 - GIGA DRAIN
 
    };
    
    // BALTOY (#63) - Initial: CONFUSION(79), MUD-SLAP(119)
    pokedex[63].learnset = 
    {
        {3, 43},   // Lvl 3  - LEER
        {5, 77},   // Lvl 5  - ROCK THROW
        {7, 60},   // Lvl 7  - PSYBEAM
        {11, 145}, // Lvl 11 - ANCIENT POWER
        {19, 80},  // Lvl 19 - PSYCHIC
        {31, 182}, // Lvl 31 - SKY UPPERCUT
        {37, 78}   // Lvl 37 - EARTHQUAKE
    };
    
    // SHELGON (#64) - Initial: RAGE(82), EMBER(52), HEADBUTT(29), BITE(44), CRUNCH(143), DRAGON CLAW(189)
    pokedex[64].learnset = 
    {
        {42, 53},  // Lvl 42 - FLAMETHROWER
        {50, 134}, // Lvl 50 - DRAGON BREATH
        {61, 63}   // Lvl 61 - HYPER BEAM
    };
    
    // ROSELIA (#65) - Initial: ABSORB(69), GIGA DRAIN(126), RAZOR LEAF(71), ROCK THROW(77), PETAL DANCE(72), GIGA DRAIN(125), EARTHQUAKE(78), SILVER WIND(178), PIN MISSILE(42), FUTURE SIGHT(147), ANCIENT POWER(145)
    pokedex[65].learnset = 
    {
        {5, 40},   // Lvl 5  - POISON STING
        {9, 70},   // Lvl 9  - MEGA DRAIN
        {25, 88},  // Lvl 25 - SLUDGE
        {33, 118}, // Lvl 33 - SLUDGE BOMB
    };
    
    // NUMEL (#66) - Initial: TACKLE(33), EMBER(52)
    pokedex[66].learnset = 
    {
        {7, 45},   // Lvl 7  - GROWL
        {11, 73},  // Lvl 11 - FIRE SPIN
        {19, 36},  // Lvl 19 - TAKE DOWN
        {25, 78},  // Lvl 25 - EARTHQUAKE
        {31, 89},  // Lvl 31 - FIRE BLAST
        {39, 53},  // Lvl 39 - FLAMETHROWER
        {45, 176}  // Lvl 45 - OVERHEAT
    };
    
    // SPOINK (#67) - Initial: PSYBEAM(60)
    pokedex[67].learnset = 
    {
        {7, 173},  // Lvl 7  - ASTONISH
        {10, 79},  // Lvl 10 - CONFUSION
        {14, 48},  // Lvl 14 - SUPERSONIC
        {18, 80},  // Lvl 18 - PSYCHIC
        {25, 34},  // Lvl 25 - BODY SLAM
        {32, 113}, // Lvl 32 - SNORE
        {39, 147}  // Lvl 39 - FUTURE SIGHT
    };
    
    // SLUGMA (#68) - Initial: EMBER(52)
    pokedex[68].learnset = 
    {
        {8, 86},   // Lvl 8  - LICK
        {13, 113}, // Lvl 13 - SNORE
        {20, 73},  // Lvl 20 - FIRE SPIN
        {27, 89},  // Lvl 27 - FIRE BLAST
        {34, 53}   // Lvl 34 - FLAMETHROWER
    };
    
    // MAGCARGO (#69) - Initial: EMBER(52), FIRE BLAST(89)
    pokedex[69].learnset = 
    {
        {8, 86},   // Lvl 8  - LICK
        {13, 113}, // Lvl 13 - SNORE
        {20, 73},  // Lvl 20 - FIRE SPIN
        {27, 106}, // Lvl 27 - ROCK SLIDE
        {34, 53},  // Lvl 34 - FLAMETHROWER
        {38, 176}, // Lvl 38 - OVERHEAT
        {48, 145}  // Lvl 48 - ANCIENT POWER
    };
    
    // CAMERUPT (#70) - Initial: EMBER(52), EARTHQUAKE(78), FIRE BLAST(89)
    pokedex[70].learnset = 
    {
        {7, 45},   // Lvl 7  - GROWL
        {11, 73},  // Lvl 11 - FIRE SPIN
        {19, 36},  // Lvl 19 - TAKE DOWN
        {25, 119}, // Lvl 25 - MUD-SLAP
        {33, 53},  // Lvl 33 - FLAMETHROWER
        {37, 77},  // Lvl 37 - ROCK THROW
        {46, 176}  // Lvl 46 - OVERHEAT
    };
    
    // SPINDA (#71) - Initial: TACKLE(33), DIZZY PUNCH(101), HEADBUTT(29), CRABHAMMER(102)
    pokedex[71].learnset = 
    {
        {5, 82},   // Lvl 5  - QUICK ATTACK
        {12, 3},   // Lvl 12 - DOUBLE SLAP
        {16, 174}, // Lvl 16 - WEATHER BALL
        {23, 116},  // Lvl 23 - MACH PUNCH
        {27, 109}, // Lvl 27 - SLASH
        {34, 108}, // Lvl 34 - TRI ATTACK
        {38, 38},  // Lvl 38 - DOUBLE-EDGE
        {45, 143}  // Lvl 45 - CRUNCH
    };
    
    // VIGOROTH (#72) - Initial: SCRATCH(10), EXPLOSION(103), TRI ATTACK(108), LUSTER PURGE(163)
    pokedex[72].learnset = 
    {
        {25, 129}, // Lvl 25 - FURY CUTTER
        {37, 109}, // Lvl 37 - SLASH
        {49, 95}   // Lvl 49 - HIGH JUMP KICK
    };
    
    // LINOONE (#73) - Initial: HEADBUTT(29), EXPLOSION(103), SCRATCH(10), LUSTER PURGE(163)
    pokedex[73].learnset = 
    {
        {5, 1},    // Lvl 5  - POUND
        {11, 82},  // Lvl 11 - QUICK ATTACK
        {17, 39},  // Lvl 17 - TAIL WHIP
        {20, 109}, // Lvl 20 - SLASH
        {29, 21},  // Lvl 29 - SLAM
        {41, 34},  // Lvl 41 - BODY SLAM
        {49, 36}   // Lvl 49 - TAKE DOWN
    };
    
    // SLAKING (#74) - Initial: TRI ATTACK(108), EXPLOSION(103), HEADBUTT(29), PECK(64)
    pokedex[74].learnset = 
    {
        {36, 45},  // Lvl 36 - GROWL
        {37, 82},  // Lvl 37 - QUICK ATTACK
        {49, 95},  // Lvl 49 - HIGH JUMP KICK
        {63, 37}   // Lvl 63 - THRASH
    };
    
    // KECLEON (#75) - Initial: SCRATCH(10), LEER(43), SCRATCH(10), TRI ATTACK(108)
    pokedex[75].learnset = 
    {
        {4, 118},  // Lvl 4  - FEINT ATTACK
        {7, 44},   // Lvl 7  - BITE
        {12, 160}, // Lvl 12 - SECRET POWER
        {17, 180}, // Lvl 17 - SHADOW PUNCH
        {24, 109}, // Lvl 24 - SLASH
        {31, 146}, // Lvl 31 - SHADOW BALL
        {40, 145}, // Lvl 40 - ANCIENT POWER
        {49, 181}  // Lvl 49 - EXTRASENSORY
    };
    
    // VOLBEAT (#76) - Initial: TACKLE(33), QUICK ATTACK(82), SPIKE CANNON(94), LUSTER PURGE(163)
    pokedex[76].learnset = 
    {
        {5, 93},   // Lvl 5  - SWIFT
        {9, 48},   // Lvl 9  - SUPERSONIC
        {13, 173}, // Lvl 13 - ASTONISH
        {17, 128}, // Lvl 17 - SPARK
        {33, 178}, // Lvl 33 - SILVER WIND
        {41, 146}  // Lvl 41 - SHADOW BALL
    };
    
    // ILLUMISE (#77) - Initial: TACKLE(33), QUICK ATTACK(82), SPIKE CANNON(94), LUSTER PURGE(163)
    pokedex[77].learnset = 
    {
        {5, 93},   // Lvl 5  - SWIFT
        {9, 48},   // Lvl 9  - SUPERSONIC
        {13, 173}, // Lvl 13 - ASTONISH
        {17, 128}, // Lvl 17 - SPARK
        {41, 146}  // Lvl 41 - SHADOW BALL
    };
    
    // BEAUTIFLY (#78) - Initial: GUST(16), ABSORB(69), PSYBEAM(60), GIGA DRAIN(125)
    pokedex[78].learnset = 
    {
        {10, 17},  // Lvl 10 - WING ATTACK
        {13, 70},  // Lvl 13 - MEGA DRAIN
        {20, 94},  // Lvl 20 - SPIKE CANNON
        {24, 186}, // Lvl 24 - AERIAL ACE
        {31, 71},  // Lvl 31 - RAZOR LEAF
        {38, 80},  // Lvl 38 - PSYCHIC
        {45, 146}  // Lvl 45 - SHADOW BALL
    };
    
    // NINJASK (#79) - Initial: SCRATCH(10), LUSTER PURGE(163), TRI ATTACK(108), ROCK SLIDE(106)
    pokedex[79].learnset = 
    {
        {20, 82},  // Lvl 20 - QUICK ATTACK
        {25, 129}, // Lvl 25 - FURY CUTTER
        {31, 109}, // Lvl 31 - SLASH
        {38, 14},  // Lvl 38 - SWORDS DANCE
        {45, 17}   // Lvl 45 - WING ATTACK
    };
    
    // LOUDRED (#80) - Initial: POUND(1), ASTONISH(173), STOMP(23), HYPER VOICE(167)
    pokedex[80].learnset = 
    {
        {5, 45},   // Lvl 5  - GROWL
        {15, 151}, // Lvl 15 - UPROAR
        {20, 29},  // Lvl 20 - HEADBUTT
        {29, 46},  // Lvl 29 - ROAR
        {37, 192}, // Lvl 37 - POISON TAIL
        {41, 63}   // Lvl 41 - HYPER BEAM
    };
    
    // SWELLOW (#81) - Initial: PECK(64), QUICK ATTACK(82), WING ATTACK(17), AERIAL ACE(186)
    pokedex[81].learnset = 
    {
        {8, 45},   // Lvl 8  - GROWL
        {13, 16},  // Lvl 13 - GUST
        {36, 65},  // Lvl 36 - DRILL PECK
        {43, 144}  // Lvl 43 - EXTREME SPEED
    };
    
    // BRELOOM (#82) - Initial: MACH PUNCH(116), SCRATCH(10), LOW KICK(68), HIGH JUMP KICK(95)
    pokedex[82].learnset = 
    {
        {23, 69},  // Lvl 23 - ABSORB
        {28, 143}, // Lvl 28 - CRUNCH
        {36, 159}, // Lvl 36 - BRICK BREAK
        {54, 157}  // Lvl 54 - SUPERPOWER
    };
    
    // GRIMER (#83) - Initial: POUND(1), SMOG(87), ACID(51), OUTRAGE(124)
    pokedex[83].learnset = 
    {
        {4, 40},   // Lvl 4  - POISON STING
        {8, 88},   // Lvl 8  - SLUDGE
        {13, 118}, // Lvl 13 - SLUDGE BOMB
        {19, 50}  // Lvl 19 - DISABLE
    };
    
    // KOFFING (#84) - Initial: POUND(1), SMOG(87), LICK(86), OUTRAGE(124)
    pokedex[84].learnset = 
    {
        {9, 88},   // Lvl 9  - SLUDGE
        {17, 84},  // Lvl 17 - SELF-DESTRUCT
        {21, 51},  // Lvl 21 - ACID
        {25, 118}, // Lvl 25 - SLUDGE BOMB
        {33, 103}, // Lvl 33 - EXPLOSION
        {41, 121}  // Lvl 41 - ZAP CANNON
    };
    
    // TROPIUS (#85) - Initial: GUST(16), RAZOR LEAF(73), ABSORB(69), STOMP(23)
    pokedex[85].learnset = 
    {
        {7, 14},   // Lvl 7  - SWORDS DANCE
        {17, 29},  // Lvl 17 - HEADBUTT
        {21, 47},  // Lvl 21 - SING
        {27, 17},  // Lvl 27 - WING ATTACK
        {31, 195}, // Lvl 31 - LEAF BLADE
        {37, 34},  // Lvl 37 - BODY SLAM
        {41, 72},  // Lvl 41 - PETAL DANCE
        {47, 178} // Lvl 47 - SILVER WIND
    };
    
    // WALREIN (#86) - Initial: POISON STING(40), POWDER SNOW(115), ICE BEAM(58), BLIZZARD(59)
    pokedex[86].learnset = 
    {
        {7, 55},   // Lvl 7  - WATER GUN
        {13, 61},  // Lvl 13 - BUBBLE BEAM
        {19, 29},  // Lvl 19 - HEADBUTT
        {25, 122}, // Lvl 25 - ICY WIND
        {39, 34},  // Lvl 39 - BODY SLAM
        {44, 44},  // Lvl 44 - BITE
        {52, 57},  // Lvl 52 - SURF
        {65, 56}   // Lvl 65 - HYDRO PUMP
    };
    
    // MAGIKARP (#87) - Initial: FAKE OUT(150), TACKLE(33)
    pokedex[87].learnset = 
    {
        {15, 33}   // Lvl 15 - TACKLE (SPLASH NOT INCLUDED IN MOVESET CAUSE USELESS)
    };
    
    // TENTACOOL (#88) - Initial: ACID(51), POISON STING(40), SUPERSONIC(48), AURORA BEAM(62)
    pokedex[88].learnset = 
    {
        {6, 99},   // Lvl 6  - BUBBLE
        {19, 61},  // Lvl 19 - BUBBLE BEAM
        {25, 88},  // Lvl 25 - SLUDGE
        {36, 118}, // Lvl 36 - SLUDGE BOMB
        {43, 56},  // Lvl 43 - HYDRO PUMP
        {49, 86}   // Lvl 49 - LICK
    };
    
    // FEEBAS (#89) - Initial: FAKE OUT(150), TACKLE(33)
    pokedex[89].learnset = 
    {
        {15, 33}    // Lvl 15 - TACKLE (SPLASH NOT INCLUDED IN MOVESET CAUSE USELESS)
    };
    
    // GLOOM (#90) - Initial: ABSORB(69), POISON STING(40), ACID(51), RAZOR LEAF(71), CONFUSION(79), GIGA DRAIN(125), LICK(86), ROCK THROW(77), PETAL DANCE(72)
    pokedex[90].learnset = 
    {
        {7, 40},   // Lvl 7  - POISON STING
        {14, 70},  // Lvl 14 - MEGA DRAIN
        {23, 88},  // Lvl 23 - SLUDGE
    };
    
    // DUSTOX (#91) - Initial: CONFUSION(79), GUST(16), PSYBEAM(60), EGG BOMB(85)
    pokedex[91].learnset = 
    {
        {10, 173}, // Lvl 10 - ASTONISH
        {13, 79},  // Lvl 13 - CONFUSION
        {17, 16},  // Lvl 17 - GUST
        {20, 40},  // Lvl 20 - POISON STING
        {24, 60},  // Lvl 24 - PSYBEAM
        {27, 86},  // Lvl 27 - LICK
        {31, 178}, // Lvl 31 - SILVER WIND
        {34, 149}, // Lvl 34 - WHIRLPOOL
        {38, 118}  // Lvl 38 - SLUDGE BOMB
    };
    
    // BELLOSSOM (#92) - Initial: ABSORB(69), MAGICAL LEAF(195), RAZOR LEAF(71), PETAL DANCE(72)
    pokedex[92].learnset = 
    {
        {23, 70},  // Lvl 23 - MEGA DRAIN
        {44, 72},  // Lvl 44 - PETAL DANCE
        {53, 125}  // Lvl 53 - GIGA DRAIN
    };
    
    // PELIPPER (#93) - Initial: WING ATTACK(17), WATER GUN(55), OVERHEAT(176), SURF(57)
    pokedex[93].learnset = 
    {
        {7, 48},   // Lvl 7  - SUPERSONIC
        {13, 149}, // Lvl 13 - WHIRLPOOL
        {25, 46},  // Lvl 25 - ROAR
        {30, 61},  // Lvl 30 - BUBBLE BEAM
        {40, 56}   // Lvl 40 - HYDRO PUMP
    };
    
    // SKARMORY (#94) - Initial: PECK(64), WING ATTACK(17), STEEL WING(130), AERIAL ACE(186)
    pokedex[94].learnset = 
    {
        {10, 43},  // Lvl 10 - LEER
        {13, 104}, // Lvl 13 - FURY SWIPES
        {16, 93},  // Lvl 16 - SWIFT
        {26, 28},  // Lvl 26 - SAND ATTACK
        {29, 138}, // Lvl 29 - METAL CLAW
        {32, 21},  // Lvl 32 - SLAM
        {42, 14}   // Lvl 42 - SWORDS DANCE
    };
    
    // ALTARIA (#95) - Initial: PECK(64), PURSUIT(135), ICICLE SPEAR(188), DRAGON CLAW(189)
    pokedex[95].learnset = 
    {
        {8, 16},   // Lvl 8  - GUST
        {18, 47},  // Lvl 18 - SING
        {35, 17},  // Lvl 35 - WING ATTACK
        {40, 125}, // Lvl 40 - GIGA DRAIN
        {45, 34},  // Lvl 45 - BODY SLAM
        {54, 124}  // Lvl 54 - OUTRAGE
    };
    
    // SHUPPET (#96) - Initial: ASTONISH(173), SHADOW PUNCH(180)
    pokedex[96].learnset = 
    {
        {8, 48},   // Lvl 8  - SUPERSONIC
        {13, 86},  // Lvl 13 - LICK
        {20, 118}, // Lvl 20 - FEINT ATTACK
        {28, 181}, // Lvl 28 - EXTRASENSORY
        {37, 146}  // Lvl 37 - SHADOW BALL
    };
    
    // BANETTE (#97) - Initial: ASTONISH(173), SHADOW PUNCH(180), SHADOW BALL(146), BITE(44)
    pokedex[97].learnset = 
    {
        {8, 48},   // Lvl 8  - SUPERSONIC
        {13, 86},  // Lvl 13 - LICK
        {20, 118}, // Lvl 20 - FEINT ATTACK
        {28, 181}, // Lvl 28 - EXTRASENSORY
        {37, 143}, // Lvl 37 - CRUNCH
        {49, 109}  // Lvl 49 - SLASH
    };
    
    // DUSKULL (#98) - Initial: LEER(43), ASTONISH(173)
    pokedex[98].learnset = 
    {
        {9, 50},   // Lvl 9  - DISABLE
        {17, 118}, // Lvl 17 - FEINT ATTACK
        {25, 135}, // Lvl 25 - PURSUIT
        {33, 143}, // Lvl 33 - CRUNCH
        {41, 147}  // Lvl 41 - FUTURE SIGHT
    };
    
    // DUSCLOPS (#99) - Initial: BITE(44), SHADOW PUNCH(180), SHADOW BALL(146), FOCUS PUNCH(154)
    pokedex[99].learnset = 
    {
        {9, 50},   // Lvl 9  - DISABLE
        {17, 118}, // Lvl 17 - FEINT ATTACK
        {25, 135}, // Lvl 25 - PURSUIT
        {33, 143}, // Lvl 33 - CRUNCH
        {37, 60},  // Lvl 37 - PSYBEAM
        {49, 147}  // Lvl 49 - FUTURE SIGHT
    };
    
    // VULPIX (#100) - Initial: EMBER(52), ASTONISH(173)
    pokedex[100].learnset = 
    {
        {5, 39},   // Lvl 5  - TAIL WHIP
        {9, 46},   // Lvl 9  - ROAR
        {13, 82},  // Lvl 13 - QUICK ATTACK
        {17, 95},  // Lvl 17 - CONFUSE RAY (using HIGH JUMP KICK as placeholder)
        {21, 118}, // Lvl 21 - FEINT ATTACK
        {25, 53},  // Lvl 25 - FLAMETHROWER
        {29, 89}   // Lvl 29 - FIRE BLAST
    };
    
    // NINETALES (#101) - Initial: EMBER(52), FLAMETHROWER(53), WATERFALL(91), FIRE BLAST(89)
    pokedex[101].learnset = {};  // Fully evolved, learns moves via TM/stone evolution
    
    // CHIMECHO (#102) - Initial: ASTONISH(173), CONFUSION(79), PSYBEAM(60), PSYCHIC(80)
    pokedex[102].learnset = 
    {
        {6, 35},   // Lvl 6  - WRAP
        {9, 45},   // Lvl 9  - GROWL
        {14, 93},  // Lvl 14 - SWIFT
        {17, 39},  // Lvl 17 - TAIL WHIP
        {22, 36},  // Lvl 22 - TAKE DOWN
        {27, 181}, // Lvl 27 - EXTRASENSORY
        {33, 97}   // Lvl 33 - DREAM EATER
    };
    
    // MIGHTYENA (#103) - Initial: BITE(44), RAGE(83), FEINT ATTACK(117), CRUNCH(143)
    pokedex[103].learnset = 
    {
        {5, 45},   // Lvl 5  - GROWL
        {13, 46},  // Lvl 13 - ROAR
        {18, 82}  // Lvl 18 - QUICK ATTACK
    };
    
    // GRAVELER (#104) - Initial: ROCK THROW(77), HEADBUTT(29), BONEMERANG(105), SUBMISSION(66)
    pokedex[104].learnset = 
    {
        {11, 43},  // Lvl 11 - LEER
        {16, 138}, // Lvl 16 - METAL CLAW
        {21, 106}, // Lvl 21 - ROCK SLIDE
        {29, 78},  // Lvl 29 - EARTHQUAKE
        {37, 38}   // Lvl 37 - DOUBLE-EDGE
    };
    
    // CROBAT (#105) - Initial: BITE(44), WING ATTACK(17), AERIAL ACE(186), CROSS CHOP(141)
    pokedex[105].learnset = 
    {
        {6, 48},   // Lvl 6  - SUPERSONIC
        {11, 173}, // Lvl 11 - ASTONISH
        {16, 98},  // Lvl 16 - LEECH LIFE
        {21, 40},  // Lvl 21 - POISON STING
        {28, 88},  // Lvl 28 - SLUDGE
        {36, 118}, // Lvl 36 - SLUDGE BOMB
        {42, 152}  // Lvl 42 - HEAT WAVE
    };
    
    // GOLBAT (#106) - Initial: BITE(44), LEECH LIFE(98), WING ATTACK(17), CROSS CHOP(141)
    pokedex[106].learnset = 
    {
        {6, 48},   // Lvl 6  - SUPERSONIC
        {11, 173}, // Lvl 11 - ASTONISH
        {16, 40},  // Lvl 16 - POISON STING
        {27, 88},  // Lvl 27 - SLUDGE
        {32, 146}  // Lvl 32 - SHADOW BALL
    };
    
    // SHARPEDO (#107) - Initial: BITE(44), RAGE(83), FEINT ATTACK(117), CRUNCH(143)
    pokedex[107].learnset = 
    {
        {6, 82},   // Lvl 6  - QUICK ATTACK
        {11, 118}, // Lvl 11 - FEINT ATTACK
        {21, 109}, // Lvl 21 - SLASH
        {30, 10},  // Lvl 30 - SCRATCH
        {40, 56}   // Lvl 40 - HYDRO PUMP
    };
    
    // CLAYDOL (#108) - Initial: CONFUSION(79), PSYBEAM(60), SKY UPPERCUT(182), EARTHQUAKE(78)
    pokedex[108].learnset = 
    {
        {5, 77},   // Lvl 5  - ROCK THROW
        {7, 119},  // Lvl 7  - MUD-SLAP
        {11, 145}, // Lvl 11 - ANCIENT POWER
        {19, 80},  // Lvl 19 - PSYCHIC
        {36, 63}   // Lvl 36 - HYPER BEAM
    };
    
    // NATU (#109) - Initial: PECK(64), CONFUSION(79)
    pokedex[109].learnset = 
    {
        {10, 43},  // Lvl 10 - LEER
        {20, 173}, // Lvl 20 - ASTONISH
        {30, 60},  // Lvl 30 - PSYBEAM
        {40, 80},  // Lvl 40 - PSYCHIC
        {50, 147}  // Lvl 50 - FUTURE SIGHT
    };
    
    // XATU (#110) - Initial: PECK(64), CONFUSION(79), WING ATTACK(17), PSYCHIC(80)
    pokedex[110].learnset = 
    {
        {10, 43},  // Lvl 10 - LEER
        {20, 173}, // Lvl 20 - ASTONISH
        {25, 60},  // Lvl 25 - PSYBEAM
        {35, 147}  // Lvl 35 - FUTURE SIGHT
    };
    
    // LUNATONE (#111) - Initial: TACKLE(33), CONFUSION(79), PSYBEAM(60), PSYCHIC(80)
    pokedex[111].learnset = 
    {
        {9, 77},   // Lvl 9  - ROCK THROW
        {17, 145}, // Lvl 17 - ANCIENT POWER
        {25, 80},  // Lvl 25 - PSYCHIC
        {33, 147}, // Lvl 33 - FUTURE SIGHT
        {41, 103}  // Lvl 41 - EXPLOSION
    };
    
    // SOLROCK (#112) - Initial: TACKLE(33), HEADBUTT(29), ROCK THROW(77), PSYBEAM(60)
    pokedex[112].learnset = 
    {
        {9, 79},   // Lvl 9  - CONFUSION
        {17, 145}, // Lvl 17 - ANCIENT POWER
        {25, 80},  // Lvl 25 - PSYCHIC
        {33, 53},  // Lvl 33 - FLAMETHROWER
        {41, 103}  // Lvl 41 - EXPLOSION
    };
    
    // GROVYLE (#113) - Initial: VINE WHIP(22), TACKLE(33), QUICK ATTACK(82), HEADBUTT(29)
    pokedex[113].learnset = 
    {
        {16, 69},  // Lvl 16 - ABSORB
        {17, 135}, // Lvl 17 - PURSUIT
        {23, 129}, // Lvl 23 - FURY CUTTER
        {29, 109}, // Lvl 29 - SLASH
        {35, 21}   // Lvl 35 - SLAM
    };
    
    // COMBUSKEN (#114) - Initial: EMBER(52), SCRATCH(10), DOUBLE KICK(24), PECK(64)
    pokedex[114].learnset = 
    {
        {7, 45},   // Lvl 7  - GROWL
        {13, 82},  // Lvl 13 - QUICK ATTACK
        {21, 21},  // Lvl 21 - SLAM
        {32, 53}   // Lvl 32 - FLAMETHROWER
    };
    
    // MARSHTOMP (#115) - Initial: WATER GUN(55), TACKLE(33), SAND ATTACK(28), MUD-SLAP(119)
    pokedex[115].learnset = 
    {
        {16, 36},  // Lvl 16 - TAKE DOWN
        {20, 61},  // Lvl 20 - BUBBLE BEAM
        {31, 78},  // Lvl 31 - EARTHQUAKE
        {42, 57}   // Lvl 42 - SURF
    };
    
    // SCEPTILE (#116) - Initial: VINE WHIP(22), ABSORB(69), LEAF BLADE(195), ROCK BLAST(196)
    pokedex[116].learnset = 
    {
        {11, 82},  // Lvl 11 - QUICK ATTACK
        {16, 135}, // Lvl 16 - PURSUIT
        {17, 71},  // Lvl 17 - RAZOR LEAF
        {23, 129}, // Lvl 23 - FURY CUTTER
        {29, 109}, // Lvl 29 - SLASH
        {35, 21},  // Lvl 35 - SLAM
        {43, 127}, // Lvl 43 - ROLLOUT
        {51, 118}  // Lvl 51 - FEINT ATTACK
    };
    
    // BLAZIKEN (#117) - Initial: BLAZE KICK(164), FIRE PUNCH(7), FLAMETHROWER(53), MUDDY WATER(184)
    pokedex[117].learnset = 
    {
        {7, 45},   // Lvl 7  - GROWL
        {13, 82},  // Lvl 13 - QUICK ATTACK
        {16, 24},  // Lvl 16 - DOUBLE KICK
        {21, 21},  // Lvl 21 - SLAM
        {28, 185}, // Lvl 28 - BULLET SEED
        {32, 159}, // Lvl 32 - BRICK BREAK
        {36, 182}, // Lvl 36 - SKY UPPERCUT
        {42, 176}  // Lvl 42 - OVERHEAT
    };
    
    // SWAMPERT (#118) - Initial: WATER GUN(55), EARTHQUAKE(78), TAKE DOWN(36), SURF (57)
    pokedex[118].learnset = 
    {
        {16, 28},  // Lvl 16 - SAND ATTACK
        {20, 61},  // Lvl 20 - BUBBLE BEAM
        {25, 119}, // Lvl 25 - MUD-SLAP
        {31, 29},  // Lvl 31 - HEADBUTT
        {46, 56}   // Lvl 46 - HYDRO PUMP
    };
    
    // WHISCASH (#119) - Initial: EARTHQUAKE(78), WATER GUN(55), MUD-SLAP(119), FIRE BLAST(90)
    pokedex[119].learnset = 
    {
        {6, 61},   // Lvl 6  - BUBBLE BEAM
        {16, 198}, // Lvl 16 - WATER PULSE
        {21, 191}, // Lvl 21 - MUD SHOT
        {26, 73},  // Lvl 26 - FIRE SPIN
        {36, 57},  // Lvl 36 - SURF
        {46, 147}  // Lvl 46 - FUTURE SIGHT
    };
    
    // CRAWDAUNT (#120) - Initial: BUBBLE(99), CRUNCH(143), BITE(44), FEINT ATTACK(117)
    pokedex[120].learnset = 
    {
        {7, 43},   // Lvl 7  - LEER
        {10, 159}, // Lvl 10 - KNOCK OFF
        {20, 109}, // Lvl 20 - SLASH
        {26, 14},  // Lvl 26 - SWORDS DANCE
        {34, 12},  // Lvl 34 - GUILLOTINE
        {42, 100}  // Lvl 42 - DIZZY PUNCH
    };
    
    // LUVDISC (#121) - Initial: TACKLE(33), WATER GUN(55), POUND(1), QUICK ATTACK(82)
    pokedex[121].learnset = 
    {
        {4, 45},   // Lvl 4  - GROWL
        {12, 173}, // Lvl 12 - ASTONISH
        {16, 39},  // Lvl 16 - TAIL WHIP
        {24, 36},  // Lvl 24 - TAKE DOWN
        {37, 198}  // Lvl 37 - WATER PULSE
    };
    
    // KINGDRA (#122) - Initial: WATER GUN(55), BUBBLE(99), HYDRO PUMP(56), SURF(57)
    pokedex[122].learnset = {};  // Fully evolved via trade
    
    // HARIYAMA (#123) - Initial: ARM THRUST(161), VITAL THROW(139), KNOCK OFF(159), SUPERPOWER(157)
    pokedex[123].learnset = 
    {
        {7, 68},   // Lvl 7  - LOW KICK
        {13, 2},   // Lvl 13 - KARATE CHOP
        {19, 182}, // Lvl 19 - SKY UPPERCUT
        {27, 21},  // Lvl 27 - SLAM
        {33, 38}   // Lvl 33 - DOUBLE-EDGE
    };
    
    // LAIRON (#124) - Initial: TACKLE(33), HEADBUTT(29), ROCK THROW(77), IRON TAIL(137)
    pokedex[124].learnset = 
    {
        {7, 138},  // Lvl 7  - METAL CLAW
        {13, 106}, // Lvl 13 - ROCK SLIDE
        {19, 143}, // Lvl 19 - CRUNCH
        {25, 78},  // Lvl 25 - EARTHQUAKE
        {37, 36}   // Lvl 37 - TAKE DOWN
    };
    
    // KIRLIA (#125) - Initial: CONFUSION(79), PSYBEAM(60), PSYCHIC(80), FUTURE SIGHT(147)
    pokedex[125].learnset = 
    {
        {6, 45},   // Lvl 6  - GROWL
        {16, 181}, // Lvl 16 - EXTRASENSORY
        {21, 118}, // Lvl 21 - FEINT ATTACK
    };
    
    // GARDEVOIR (#126) - Initial: CONFUSION(79), PSYCHIC(80), THUNDERBOLT(75), SHADOW BALL(146)
    pokedex[126].learnset = 
    {
        {6, 45},   // Lvl 6  - GROWL
        {11, 79},  // Lvl 11 - CONFUSION
        {16, 181}, // Lvl 16 - EXTRASENSORY
        {21, 118}, // Lvl 21 - FEINT ATTACK
        {26, 147}, // Lvl 26 - FUTURE SIGHT
        {33, 97}   // Lvl 33 - DREAM EATER
    };
    
    // CACTURNE (#127) - Initial: ABSORB(69), MEGA DRAIN(70), NEEDLE ARM(166), BULLET SEED(185)
    pokedex[127].learnset = 
    {
        {9, 71},   // Lvl 9  - RAZOR LEAF
        {13, 28},  // Lvl 13 - SAND ATTACK
        {17, 118}, // Lvl 17 - FEINT ATTACK
        {21, 167}, // Lvl 21 - HYPER VOICE
        {25, 143}, // Lvl 25 - CRUNCH
        {29, 125}, // Lvl 29 - GIGA DRAIN
        {35, 106}, // Lvl 35 - ROCK SLIDE
        {41, 88}   // Lvl 41 - SLUDGE
    };
    
    // LUDICOLO (#128) - Initial: ABSORB(69), WATER GUN(55), MEGA DRAIN(70), GIGA DRAIN(125)
    pokedex[128].learnset = 
    {
        {13, 71},  // Lvl 13 - RAZOR LEAF
        {19, 61},  // Lvl 19 - BUBBLE BEAM
        {25, 185}, // Lvl 25 - BULLET SEED
        {31, 57}   // Lvl 31 - SURF
    };

    // DELCATTY (#129) - Initial: POUND(1), ASTONISH(173), SCRATCH(10), SPIKE CANNON(94)
    pokedex[129].learnset = 
    {
        {4, 118},  // Lvl 4  - FEINT ATTACK
        {13, 108}, // Lvl 13 - TRI ATTACK
        {17, 82}   // Lvl 17 - QUICK ATTACK
    };
        
    
    
    // VIBRAVA (#130) - Initial: BITE(44), SAND ATTACK(28), ICICLE SPEAR(188), DRAGON CLAW(189)
    pokedex[130].learnset = 
    {
        {9, 129},  // Lvl 9  - FURY CUTTER
        {17, 143}, // Lvl 17 - CRUNCH
        {25, 78},  // Lvl 25 - EARTHQUAKE
        {33, 134}, // Lvl 33 - DRAGON BREATH
        {41, 63}   // Lvl 41 - HYPER BEAM
    };
    
    // FLYGON (#131) - Initial: BITE(44), SAND ATTACK(28), EARTHQUAKE(78), DRAGON CLAW(189)
    pokedex[131].learnset = 
    {
        {9, 129},  // Lvl 9  - FURY CUTTER
        {17, 143}, // Lvl 17 - CRUNCH
        {25, 19},  // Lvl 25 - FLY
        {33, 134}, // Lvl 33 - DRAGON BREATH
        {41, 124}, // Lvl 41 - OUTRAGE
        {49, 63}   // Lvl 49 - HYPER BEAM
    };
    
    // SALAMENCE (#132) - Initial: DRAGON CLAW(189), FLY(19), CRUNCH(143), DRAGON BREATH(134)
    pokedex[132].learnset = 
    {
        {5, 82},   // Lvl 5  - RAGE
        {9, 44},   // Lvl 9  - BITE
        {17, 29},  // Lvl 17 - HEADBUTT
        {30, 53},  // Lvl 30 - FLAMETHROWER
        {50, 124}, // Lvl 50 - OUTRAGE
        {61, 63}   // Lvl 61 - HYPER BEAM
    };
    
    // SHIFTRY (#133) - Initial: SCRATCH(10), RAZOR LEAF(73), FEINT ATTACK(117), BULLET SEED(185)
    pokedex[133].learnset = 
    {
        {13, 143}, // Lvl 13 - CRUNCH  
        {19, 173}, // Lvl 19 - ASTONISH
        {25, 181}, // Lvl 25 - EXTRASENSORY
        {31, 14}   // Lvl 31 - SWORDS DANCE
    };

    // GLALIE (#134) - Initial: BITE(44), POWDER SNOW(115), ICE BEAM(58), BLIZZARD(59)
    pokedex[134].learnset = 
    {
        {4, 43},   // Lvl 4  - LEER
        {10, 8},   // Lvl 10 - ICE PUNCH
        {13, 122}, // Lvl 13 - ICY WIND
        {19, 29},  // Lvl 19 - HEADBUTT
        {28, 143}, // Lvl 28 - CRUNCH
        {37, 146}, // Lvl 37 - SHADOW BALL
        {42, 109}, // Lvl 42 - SLASH
        {51, 103}  // Lvl 51 - EXPLOSION
    };
    
    // TENTACRUEL (#135) - Initial: ACID(51), POISON STING(40), BUBBLE BEAM(61), HYDRO PUMP(56)
    pokedex[135].learnset = 
    {
        {6, 99},   // Lvl 6  - BUBBLE
        {19, 88},  // Lvl 19 - SLUDGE
        {25, 48},  // Lvl 25 - SUPERSONIC
        {30, 62},  // Lvl 30 - AURORA BEAM
        {38, 118}, // Lvl 38 - SLUDGE BOMB
        {49, 86}   // Lvl 49 - LICK
    };
    
    // GYARADOS (#136) - Initial: BITE(44), WING ATTACK(17), HYDRO PUMP(56), HYPER BEAM(63)
    pokedex[136].learnset = 
    {
        {20, 143}, // Lvl 20 - CRUNCH
        {25, 142}, // Lvl 25 - TWISTER
        {30, 106}, // Lvl 30 - ROCK SLIDE
        {35, 82},  // Lvl 35 - RAGE
        {40, 57},  // Lvl 40 - SURF
        {50, 124}  // Lvl 50 - OUTRAGE
    };
    
    // MILOTIC (#137) - Initial: WATER GUN(55), HYDRO PUMP(56), SURF(57), WHIRLPOOL(149)
    pokedex[137].learnset = 
    {
        {5, 35},   // Lvl 5  - WRAP
        {10, 198}, // Lvl 10 - WATER PULSE
        {15, 142}, // Lvl 15 - TWISTER
        {20, 91},  // Lvl 20 - WATERFALL
        {30, 181}, // Lvl 30 - EXTRASENSORY
        {40, 57},  // Lvl 40 - SURF
        {50, 63}   // Lvl 50 - HYPER BEAM
    };
}

public:
    const Pokemon* findPokemon(const string& name) const {
        for (const auto& p : pokedex) {
            if (p.pokemonName == name) return &p;
        }
        return nullptr;
    }
    
    const Ability& getAbility(int id) const {
        static Ability emptyAbility = {"Unknown", "No description available."};
        
        if (id <= 0 || id > (int)abilityList.size()) {
            return emptyAbility;
        }
        return abilityList[id - 1];
    }
};

#endif
 