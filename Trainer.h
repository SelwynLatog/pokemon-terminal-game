#ifndef TRAINER_H
#define TRAINER_H

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

#include "Player.h"
#include "PokemonDatabase.h"
#include "ItemDatabase.h"

using namespace std;

/*****************************************************************************
 * CONSTANTS
 *****************************************************************************/
namespace TrainerAIConfig {
    constexpr double HEAL_THRESHOLD_EASY = 0.25;
    constexpr double HEAL_THRESHOLD_MEDIUM = 0.50;
    constexpr double HEAL_THRESHOLD_HARD = 0.75;
    constexpr double TYPE_MATCH_BONUS = 1.5;
    constexpr double STATUS_MOVE_BASE_SCORE = 1.0;
}

/*****************************************************************************
 * ENUMS
 *****************************************************************************/
enum Role { 
    RIVAL, 
    GYM_LEADER, 
    TRAINER 
};

enum TrainerAI {
    AI_EASY = 1,
    AI_MEDIUM = 2,
    AI_HARD = 3
};

/*****************************************************************************
 * TRAINER STRUCTURE
 *****************************************************************************/
struct Trainer : public Player {
    string trainerName;
    Role role;
    TrainerAI trainerAIlevel;
    string preBattleLine;
    string postBattleLine;

    Trainer() = default;

    Trainer(const string& name, Role r, TrainerAI aiLevel = AI_EASY)
        : trainerName(name)
        , role(r)
        , trainerAIlevel(aiLevel)
    {}

    /******************
     * POKEMON MANAGEMENT
     ******************/
    void addTrainerPokemon(const Pokemon& p, const vector<Move>& moveList) {
        currentPokemon.push_back(p);
        currentPokemon.back().initializePP(moveList);
        currentPokemon.back().currentHP = currentPokemon.back().baseHP;
    }

    /******************
     * ITEM MANAGEMENT
     ******************/
    void addItem(const Item& it) {
        // Check if item already exists
        for (auto& item : items) {
            if (item.itemName == it.itemName) {
                item.quantity += it.quantity;
                return;
            }
        }
        items.push_back(it);
    }

    /******************
     * BATTLE STATE
     ******************/
    bool hasAlivePokemon() const {
        for (const auto& p : currentPokemon) {
            if (p.currentHP > 0) return true;
        }
        return false;
    }

    int firstAliveIndex() const {
        for (int i = 0; i < (int)currentPokemon.size(); ++i) {
            if (currentPokemon[i].currentHP > 0) return i;
        }
        return -1;
    }
};

/*****************************************************************************
 * TRAINER BUILDER
 *****************************************************************************/
inline Trainer buildTrainer(const string& name,
                            Role role,
                            TrainerAI aiLevel,
                            const vector<pair<string, int>>& teamWithLevels,
                            const vector<Item>& itemsToGive,
                            const string& preBattleLine,
                            const string& postBattleLine,
                            const PokemonDatabase& db) {
    Trainer t(name, role, aiLevel);
    t.preBattleLine = preBattleLine;
    t.postBattleLine = postBattleLine;

    // Add items
    for (const auto& item : itemsToGive) {
        t.addItem(item);
    }

    // Add Pokemon with specified levels
    for (const auto& entry : teamWithLevels) {
        const string& pokeName = entry.first;
        int lvl = entry.second;

        const Pokemon* found = db.findPokemon(pokeName);
        if (!found) {
            cout << "Warning: Trainer " << name 
                 << " - Pokemon \"" << pokeName << "\" not found in DB.\n";
            continue;
        }

        Pokemon p = *found;
        p.level = lvl;
        p.scaleStatsToLevel();
        p.initializePP(db.moveList);
        t.addTrainerPokemon(p, db.moveList);
    }

    return t;
}

/*****************************************************************************
 * RIVAL CREATION
 *****************************************************************************/
inline Trainer createRival(const string& rivalName, const Player& player, const PokemonDatabase& db) {
    Trainer rival;
    rival.role = TRAINER;
    rival.trainerAIlevel = AI_EASY;
    rival.trainerName = rivalName;
    rival.preBattleLine = "Let's battle!";
    rival.postBattleLine = "You're pretty strong!";

    if (player.currentPokemon.empty()) {
        cout << "Error: Player has no starter Pokemon!\n";
        return rival;
    }

    const string& playerStarter = player.currentPokemon[0].pokemonName;
    string rivalStarterName = "MUDKIP";

    if (playerStarter == "TREECKO")      rivalStarterName = "TORCHIC";
    else if (playerStarter == "MUDKIP")  rivalStarterName = "TREECKO";
    else if (playerStarter == "TORCHIC") rivalStarterName = "MUDKIP";

    const Pokemon* found = db.findPokemon(rivalStarterName);
    if (found) {
        Pokemon starter = *found;
        starter.level = LevelConfig::STARTING_LEVEL;
        starter.scaleStatsToLevel();
        starter.initializePP(db.moveList);
        rival.addTrainerPokemon(starter, db.moveList);
    }

    return rival;
}

// Rustboro rival battle (after first badge)
inline Trainer createRustboroRival(const string& rivalName, const Player& player, const PokemonDatabase& db) {
    // Determine rival's team based on player's starter
    vector<pair<string, int>> rivalTeam;
    
    if (!player.currentPokemon.empty()) {
        const string& playerStarter = player.currentPokemon[0].pokemonName;
        
        if (playerStarter == "TREECKO" || playerStarter == "GROVYLE" || playerStarter == "SCEPTILE") {
            rivalTeam = {{"COMBUSKEN", 16}, {"WINGULL", 14}};
        } else if (playerStarter == "MUDKIP" || playerStarter == "MARSHTOMP" || playerStarter == "SWAMPERT") {
            rivalTeam = {{"GROVYLE", 16}, {"WINGULL", 14}};
        } else if (playerStarter == "TORCHIC" || playerStarter == "COMBUSKEN" || playerStarter == "BLAZIKEN") {
            rivalTeam = {{"MARSHTOMP", 16}, {"WINGULL", 14}};
        } else {
            rivalTeam = {{"MARSHTOMP", 16}, {"WINGULL", 14}};
        }
    } else {
        rivalTeam = {{"MARSHTOMP", 16}, {"WINGULL", 14}};
    }
    
    return buildTrainer(
        rivalName,
        TRAINER,
        AI_MEDIUM,
        rivalTeam,
        {Item("POTION", ItemType::HEALING, ItemConfig::POTION_HEAL, 2)},
        "So you're ready to challenge me? Let's battle!",
        "You've gotten even stronger!",
        db
    );
}
/*****************************************************************************
 * RUSTBORO RIVAL BATTLE
 *****************************************************************************/
inline Trainer createRustboroRival(const Player& player, const PokemonDatabase& db) {
    Trainer rival;
    rival.role = RIVAL;
    rival.trainerAIlevel = AI_MEDIUM;  // Harder than first battle
    
    // Assign name based on player gender
    if (player.gender == BOY) {
        rival.trainerName = "MAY";
        rival.preBattleLine = "So you're ready to challenge me " + player.username + "? Let's battle!";
        rival.postBattleLine = "You've gotten even stronger!";
    } else {
        rival.trainerName = "BRENDAN";
        rival.preBattleLine = "So you're ready to challenge me " + player.username + "? Let's battle!";
        rival.postBattleLine = "You've gotten even stronger!I need to improve more";
    }
    
    if (player.currentPokemon.empty()) {
        cout << "Error: Player has no starter Pokemon!\n";
        return rival;
    }
    
    // Rival has evolved starter + additional Pokemon
    const string& playerStarter = player.currentPokemon[0].pokemonName;
    string rivalStarterName = "MARSHTOMP";  // Evolved forms
    string rivalSecondPokemon = "WINGULL";
    
    if (playerStarter == "TREECKO" || playerStarter == "GROVYLE" || playerStarter == "SCEPTILE") {
        rivalStarterName = "COMBUSKEN";
    } else if (playerStarter == "MUDKIP" || playerStarter == "MARSHTOMP" || playerStarter == "SWAMPERT") {
        rivalStarterName = "GROVYLE";
    } else if (playerStarter == "TORCHIC" || playerStarter == "COMBUSKEN" || playerStarter == "BLAZIKEN") {
        rivalStarterName = "MARSHTOMP";
    }
    
    // Add evolved starter (level 16)
    const Pokemon* starter = db.findPokemon(rivalStarterName);
    if (starter) {
        Pokemon p = *starter;
        p.level = 16;
        p.scaleStatsToLevel();
        p.initializePP(db.moveList);
        rival.addTrainerPokemon(p, db.moveList);
    }
    
    // Add second Pokemon (level 14)
    const Pokemon* second = db.findPokemon(rivalSecondPokemon);
    if (second) {
        Pokemon p = *second;
        p.level = 14;
        p.scaleStatsToLevel();
        p.initializePP(db.moveList);
        rival.addTrainerPokemon(p, db.moveList);
    }
    
    // Give rival some healing items
    rival.addItem(Item("POTION", ItemType::HEALING, ItemConfig::POTION_HEAL, 2));
    
    return rival;
}

/*****************************************************************************
 * ROUTE110 RIVAL BATTLE
 *****************************************************************************/
inline Trainer createRoute110Rival(const string& rivalName, const Player& player, const PokemonDatabase& db) {
    Trainer rival;
    rival.role = RIVAL;
    rival.trainerAIlevel = AI_MEDIUM;
    rival.trainerName = rivalName;
    
    rival.preBattleLine = "I've even gotten stronger than before, " + player.username + "! Let's battle!";
    rival.postBattleLine = "Damn it! I gotta switch it up next time...";
    
    if (player.currentPokemon.empty()) {
        cout << "Error: Player has no starter Pokemon!\n";
        return rival;
    }
    
    const string& playerStarter = player.currentPokemon[0].pokemonName;
    string rivalStarterName = "MARSHTOMP";
    string rivalSecondPokemon = "WINGULL";
    string rivalThirdPokemon = "LOMBRE";
    
    if (playerStarter == "TREECKO" || playerStarter == "GROVYLE" || playerStarter == "SCEPTILE") {
        rivalStarterName = "COMBUSKEN";
    } else if (playerStarter == "MUDKIP" || playerStarter == "MARSHTOMP" || playerStarter == "SWAMPERT") {
        rivalStarterName = "GROVYLE";
    } else if (playerStarter == "TORCHIC" || playerStarter == "COMBUSKEN" || playerStarter == "BLAZIKEN") {
        rivalStarterName = "MARSHTOMP";
    }
    
    // Add starter
    const Pokemon* starter = db.findPokemon(rivalStarterName);
    if (starter) {
        Pokemon p = *starter;
        p.level = 20;
        p.scaleStatsToLevel();
        p.initializePP(db.moveList);
        rival.addTrainerPokemon(p, db.moveList);
    }
    
    // Add second Pokemon
    const Pokemon* second = db.findPokemon(rivalSecondPokemon);
    if (second) {
        Pokemon p = *second;
        p.level = 18;
        p.scaleStatsToLevel();
        p.initializePP(db.moveList);
        rival.addTrainerPokemon(p, db.moveList);
    }
    
	//Add third Pokemonn
    const Pokemon* third = db.findPokemon(rivalThirdPokemon);
    if (third) {
        Pokemon p = *third;
        p.level = 18;
        p.scaleStatsToLevel();
        p.initializePP(db.moveList);
        rival.addTrainerPokemon(p, db.moveList);
    }
    
    rival.addItem(Item("POTION", ItemType::HEALING, ItemConfig::POTION_HEAL, 1));
    
    return rival;
}

/*****************************************************************************
 * ROUTE 119 RIVAL BATTLE
 *****************************************************************************/
inline Trainer createRoute119Rival(const string& rivalName, const Player& player, const PokemonDatabase& db) {
    Trainer rival;
    rival.role = RIVAL;
    rival.trainerAIlevel = AI_MEDIUM;
    rival.trainerName = rivalName;
    
    rival.preBattleLine = "You've made it this far, " + player.username + "! But I've gotten even stronger!";
    rival.postBattleLine = "You're really something else... I need to train harder!";
    
    if (player.currentPokemon.empty()) {
        cout << "Error: Player has no starter Pokemon!\n";
        return rival;
    }
    
    const string& playerStarter = player.currentPokemon[0].pokemonName;
    string rivalStarterName = "MARSHTOMP";
    
    if (playerStarter == "TREECKO" || playerStarter == "GROVYLE" || playerStarter == "SCEPTILE") {
        rivalStarterName = "COMBUSKEN";
    } else if (playerStarter == "MUDKIP" || playerStarter == "MARSHTOMP" || playerStarter == "SWAMPERT") {
        rivalStarterName = "GROVYLE";
    } else if (playerStarter == "TORCHIC" || playerStarter == "COMBUSKEN" || playerStarter == "BLAZIKEN") {
        rivalStarterName = "MARSHTOMP";
    }
    
    // Add Numel (level 29)
    const Pokemon* numel = db.findPokemon("NUMEL");
    if (numel) {
        Pokemon p = *numel;
        p.level = 29;
        p.scaleStatsToLevel();
        p.initializePP(db.moveList);
        rival.addTrainerPokemon(p, db.moveList);
    }
    
    // Add Shroomish (level 29)
    const Pokemon* shroomish = db.findPokemon("SHROOMISH");
    if (shroomish) {
        Pokemon p = *shroomish;
        p.level = 29;
        p.scaleStatsToLevel();
        p.initializePP(db.moveList);
        rival.addTrainerPokemon(p, db.moveList);
    }
    
    // Add evolved starter (level 31) - ace Pokemon
    const Pokemon* starter = db.findPokemon(rivalStarterName);
    if (starter) {
        Pokemon p = *starter;
        p.level = 31;
        p.scaleStatsToLevel();
        p.initializePP(db.moveList);
        rival.addTrainerPokemon(p, db.moveList);
    }
    
    rival.addItem(Item("SUPER POTION", ItemType::HEALING, ItemConfig::SUPER_POTION_HEAL, 2));
    
    return rival;
}

/*****************************************************************************
 * GYM LEADER FACTORIES
 *****************************************************************************/
inline Trainer createGymLeaderRoxanne(const PokemonDatabase& db) {
    return buildTrainer(
        "GYM LEADER ROXANNE", 
        GYM_LEADER, 
        AI_HARD,
        {{"GEODUDE", 12},{"GEODUDE", 12},{"NOSEPASS", 15}},
        {
            Item("POTION", ItemType::HEALING, ItemConfig::POTION_HEAL, 2)
        },
        "I'm pretty hard headed, I do not accept defeat, Show me what you got!",
        "A leader of a big Gym bears a lot of responsibility. You are strong. I have more to learn.",
        db
    );
}

inline Trainer createGymLeaderBrawley(const PokemonDatabase& db) {
    return buildTrainer(
        "GYM LEADER BRAWLEY", 
        GYM_LEADER, 
        AI_HARD,
        {{"MACHOP", 16},{"MEDITITE", 16},{"MAKUHITA", 19}},
        {
            Item("SUPER POTION", ItemType::HEALING, ItemConfig::POTION_HEAL, 2)
        },
        "You outscore everyone and still rank lower? Brutal. Step into the ring, I only judge by SKILLS ALONE.",
        "Dang... you beat me straight up. No bias. No extras. Just pure strength. Wish the world worked like that.",
        db
    );
}

inline Trainer createGymLeaderWattson(const PokemonDatabase& db) {
    return buildTrainer(
        "GYM LEADER WATTSON", 
        GYM_LEADER, 
        AI_HARD,
        {{"VOLTORB", 20},{"ELECTRIKE", 20},{"MAGNETON", 22}, {"MANECTRIC", 24}},
        {
            Item("SUPER POTION", ItemType::HEALING, ItemConfig::POTION_HEAL, 2)
        },
        "I've been waiting for a trainer with a shocking desire for battle!",
        "WAHAHAHA! That was an electrifying battle!",
        db
    );
}

inline Trainer createGymLeaderFlannery(const PokemonDatabase& db) {
    return buildTrainer(
        "GYM LEADER FLANNERY", 
        GYM_LEADER, 
        AI_HARD,
        {{"NUMEL", 24},{"SLUGMA", 24},{"CAMERUPT", 26}, {"TORKOAL", 29}},
        {
            Item("SUPER POTION", ItemType::HEALING, ItemConfig::POTION_HEAL, 4)
        },
        "I can feel the heat! I'm Flannery, the Lavaridge Town Gym Leader! Do you feel the heat? Well, get ready, because it's gonna burn!",
    	"Oh! You're really hot stuff! but the flames of Lavaridge will always burn!",
        db
    );
}

inline Trainer createGymLeaderNorman(const PokemonDatabase& db) {
    return buildTrainer(
        "GYM LEADER NORMAN", 
        GYM_LEADER, 
        AI_HARD,
        {{"SPINDA", 27}, {"VIGOROTH", 27}, {"LINOONE", 29}, {"SLAKING", 31}},
        {
            Item("SUPER POTION", ItemType::HEALING, ItemConfig::SUPER_POTION_HEAL, 2)
        },
        "I've been waiting for you. I've longed for this day. My own child, battling me as a Trainer. But a battle is a battle! I will show you no mercy!",
        "I can't believe it. I lost to my own child... But that's fine. No, it's even better! You have become a strong Trainer, haven't you?",
        db
    );
}

inline Trainer createGymLeaderWinona(const PokemonDatabase& db) {
    return buildTrainer(
        "GYM LEADER WINONA", 
        GYM_LEADER, 
        AI_HARD,
        {{"SWELLOW", 31}, {"PELIPPER", 30}, {"SKARMORY", 32}, {"ALTARIA", 33}},
        {
            Item("SUPER POTION", ItemType::HEALING, ItemConfig::SUPER_POTION_HEAL, 4)
        },
        "I am Winona. I am the Leader of the FORTREE POKEMON GYM. I have become one with BIRD POKEMON and have soared the skies... However grueling the battle, we will triumph with grace.",
        "Never before have I seen a Trainer command POKEMON with more grace than I... In recognition of your prowess, I present you with this...",
        db
    );
}

inline Trainer createGymLeaderTateLiza(const PokemonDatabase& db) {
    return buildTrainer(
        "GYM LEADERS TATE AND LIZA", 
        GYM_LEADER, 
        AI_HARD,
        {{"CLAYDOL", 41}, {"XATU", 41}, {"LUNATONE", 42}, {"SOLROCK", 42}},
        {
            Item("SUPER POTION", ItemType::HEALING, ItemConfig::SUPER_POTION_HEAL, 6)
        },
        "Hehehe...Fufufu.. Were you surprised? There are TWO GYM LEADERS! We can each determine what the other is thinking, all in our minds! This combination of ours, can you beat it?",
        "What?! Our combination....was shattered!...",
        db
    );
}

inline Trainer createGymLeaderJuan(const PokemonDatabase& db){
	return buildTrainer(
        "GYM LEADER JUAN", 
        GYM_LEADER, 
        AI_HARD,
        {{"LUVDISC", 41}, {"WHISCASH", 41}, {"SEALEO", 43}, {"CRAWDAUNT", 43}, {"KINGDRA",46}},
        {
            Item("SUPER POTION", ItemType::HEALING, ItemConfig::SUPER_POTION_HEAL, 8)
        },
        "I taught WALLACE everything there is to know about POKEMON, as his mentor, you shall bear witness to the grand illusion of water sculpted by POKEMON and myself!",
        "Excellent! from you, I sense the brilliant shine of skill that will overcome all.",
        db
    );
}

/*****************************************************************************
 * POKEMON LEAGUE FACTORIES
 *****************************************************************************/
inline Trainer createElite4Sidney(const PokemonDatabase& db){
 	return buildTrainer(
        "ELITE 4 SIDNEY", 
        TRAINER, 
        AI_HARD,
        {{"MIGHTYENA", 46}, {"SHIFTRY", 48}, {"CRAWDAUNT", 48}, {"CACTURNE", 46}, {"ABSOL",49}},
        {
            Item("SUPER POTION", ItemType::HEALING, ItemConfig::SUPER_POTION_HEAL, 8)
        },
        "I'm SIDNEY of the ELITE FOUR. I like the look you're giving me. That's good. Let's have a battle that can only be staged here in the LEAGUE!",
        "Listen to what this loser has to say, you've got what it takes to go far. Now, go on to the next room and enjoy the battle.",
        db
    );
}
 
inline Trainer createElite4Phoebe(const PokemonDatabase& db){
 	return buildTrainer(
        "ELITE 4 PHOEBE", 
        TRAINER, 
        AI_HARD,
        {{"DUSCLOPS", 48}, {"DUSCLOPS", 51}, {"BANETTE", 49}, {"BANETTE", 46}, {"SABLEYE",50}},
        {
            Item("SUPER POTION", ItemType::HEALING, ItemConfig::SUPER_POTION_HEAL, 8)
        },
        "I'm PHOEBE of the ELITE FOUR. While I trained, I gained the ability to commune with GHOST-type POKEMON. Our bond between flesh and spirit is strong. See if you can even inflict damage!",
        "There's a definite bond between you and your POKEMON, too. I'd like to see how far your bond will carry you. Go ahead, move to the next room.",
        db
    );
}
 
 inline Trainer createElite4Glacia(const PokemonDatabase& db){
 	return buildTrainer(
        "ELITE 4 GLACIA", 
        TRAINER, 
        AI_HARD,
        {{"SEALEO", 50}, {"GLALIE", 50}, {"SEALEO", 52}, {"GLALIE", 52}, {"WALREIN",53}},
        {
            Item("SUPER POTION", ItemType::HEALING, ItemConfig::SUPER_POTION_HEAL, 8)
        },
        "Welcome. I'm GLACIA of the ELITE FOUR. I've travelled from afar to HOENN so I can hone my ICE-skills. But all I've seen are weak TRAINERS. I would like to finally get some challenge!",
        "It's been a while since I faced a truly strong TRAINER. Advance to the next room, and there, comes a truly fearsome one of the ELITE 4...",
        db
    );
}

inline Trainer createElite4Drake(const PokemonDatabase& db){
 	return buildTrainer(
        "ELITE 4 DRAKE", 
        TRAINER, 
        AI_HARD,
        {{"SHELGON", 52}, {"KINGDRA", 53}, {"FLYGON", 53}, {"ALTARIA", 54}, {"SALAMENCE",55}},
        {
            Item("SUPER POTION", ItemType::HEALING, ItemConfig::SUPER_POTION_HEAL, 8)
        },
        "I am the last of the ELITE FOUR, DRAKE the DRAGON master! For us to battle between TRAINER and POKEMON. Do you know what is needed? Do you know what it takes?",
        "Superb. You deserve every credit for coming this far as a TRAINER. You have heart! Now go on! The CHAMPION is waiting!",
        db
    );
}

inline Trainer createChampionWallace(const PokemonDatabase& db){
 	return buildTrainer(
        "CHAMPION WALLACE", 
        TRAINER, 
        AI_HARD,
        {{"WAILORD", 57}, {"TENTACRUEL", 55}, {"LUDICOLO", 56}, {"WHISCASH", 56}, {"GYARADOS",56}, {"MILOTIC", 58}},
        {
            Item("SUPER POTION", ItemType::HEALING, ItemConfig::SUPER_POTION_HEAL, 8)
        },
        "TRAINERS and POKEMON are one. They benefit from one another, creating a harmonious bond. This is where that bond is put to the test. Now, who can most elegantly dance with their POKEMON in HOENN?",
        "I, the CHAMPION, fall in defeat...That was wonderful work. You were elegant, infuriatingly so.",
        db
    );
}  
/*****************************************************************************
 * ROUTE TRAINER FACTORIES
 *****************************************************************************/

// Route 102
inline Trainer createYoungsterCalvin(const PokemonDatabase& db) {
    return buildTrainer(
        "YOUNGSTER CALVIN",
        TRAINER,
        AI_EASY,
        {{"POOCHYENA", 5}},
        {},
        "Huwaw bag ong trainer! Let's battle!",
        "Wow, you're strong!",
        db
    );
}

inline Trainer createBugCatcherRick(const PokemonDatabase& db) {
    return buildTrainer(
        "BUG CATCHER RICK",
        TRAINER,
        AI_EASY,
        {{"WURMPLE", 4}, {"WURMPLE", 4}},
        {},
        "My bug Pokemon are the best!",
        "Aww, my Pokemon...",
        db
    );
}

//Petalburg Woods 1
inline Trainer createBugCatcherLyle(const PokemonDatabase& db) {
    return buildTrainer(
        "BUG CATCHER LYLE",
        TRAINER,
        AI_EASY,
        {{"WURMPLE", 3}, {"WURMPLE", 3},{"WURMPLE", 3},{"WURMPLE", 3}},
        {},
        "Stop debugging and face my bugs!",
        "I got a compilation error....",
        db
    );
}

inline Trainer createBugCatcherJames(const PokemonDatabase& db) {
    return buildTrainer(
        "BUG CATCHER JAMES",
        TRAINER,
        AI_EASY,
        {{"NINCADA",6},{"NINCADA",6} },
        {},
        "My classmates say they're ahead of me, but my BUG POKEMON outscore them!",
        "Looks like I need to get more EXTRACURRICULARS...",
        db
    );
}

//Petalburg Woods 2
inline Trainer createTeamAquaGrunt(const PokemonDatabase& db) {
    return buildTrainer(
        "TEAM AQUA GRUNT",
        TRAINER,
        AI_EASY,
        {{"POOCHYENA", 9}}, 
        {},
        "Hey kid, being only 3rd placer while acing the exam and laboratories by miles is insane.",
        "Here take your reward, It's the only place your EFFORTS and RESULTS are JUSTIFIED.",
        db
    );
}

//Oceanic Museum
inline Trainer createTeamAquaGruntOM1(const PokemonDatabase& db) {
    return buildTrainer(
        "TEAM AQUA GRUNT",
        TRAINER,
        AI_EASY,
        {{"ZUBAT", 14}}, 
        {},
        "You're not getting past us!",
        "Ugh... We failed...",
        db
    );
}

inline Trainer createTeamAquaGruntOM2(const PokemonDatabase& db) {
    return buildTrainer(
        "TEAM AQUA GRUNT",
        TRAINER,
        AI_EASY,
        {{"ZUBAT", 14},{"CARVANHA", 14}}, 
        {},
        "You're not getting past my fisssh!!!",
        "Ugh... We failed...",
        db
    );
}

//Route 110
inline Trainer createPokefanIsabel(const PokemonDatabase& db){
	return buildTrainer(
        "POKEFAN ISABEL",
        TRAINER,
        AI_EASY,
        {{"PLUSLE", 14},{"MINUN", 14}}, 
        {},
        "I'm a big Pokenerd especially when it comes to electric types",
        "It seems like you're a better Pokenerd than I am...",
        db
    );
}

inline Trainer createCollectorEdwin(const PokemonDatabase& db){
	return buildTrainer(
        "COLLECTOR EDWIN",
        TRAINER,
        AI_EASY,
        {{"LOMBRE", 14},{"NUZLEAF", 14}}, 
        {},
        "See my fine collection of rare Pokemon!",
        "It seems like you're a better 	Pokemon Collector than I am...",
        db
    );
}

//Route 111 A (Southern)
inline Trainer createAromaLadyCelina(const PokemonDatabase& db){
	return buildTrainer(
        "AROMA LADY CELINA",
        TRAINER,
        AI_EASY,
        {{"ROSELIA", 18}}, 
        {},
        "I love flowers, can you help me pick some?",
        "It seems that I need to grow a more beautiful garden next time...",
        db
    );
}

inline Trainer createPicnickerBianca(const PokemonDatabase& db){
	return buildTrainer(
        "PICNICKER BAINCA",
        TRAINER,
        AI_EASY,
        {{"SHROOMISH", 18}}, 
        {},
        "I'll give you some sandwiches if you beat me!",
        "Take these, it's pb and j, enjoy trainer...",
        db
    );
}

inline Trainer createKindlerHayden(const PokemonDatabase& db){
	return buildTrainer(
        "KINDLER HAYDEN",
        TRAINER,
        AI_EASY,
        {{"NUMEL", 18}}, 
        {},
        "I recently discovered that I'm really good at BBQs!",
        "Up north makes the best BBQs if you're hungry...",
        db
    );
}

inline Trainer createCamperTravis(const PokemonDatabase& db){
	return buildTrainer(
        "CAMPER TRAVIS",
        TRAINER,
        AI_EASY,
        {{"SANDSHREW", 18}}, 
        {},
        "I'm a big fan of the Outdoor boys, anyway lets battle!",
        "You should really give the Outdoor boys a watch!....",
        db
    );
}

//Route 111 B (Middle)
inline Trainer createCoolTrainerBrooke(const PokemonDatabase& db){
	return buildTrainer(
        "COOL TRAINER BROOKE",
        TRAINER,
        AI_EASY,
        {{"WINGULL", 17},{"NUMEL", 17},{"ROSELIA", 17}}, 
        {},
        "I see a trainer with styke, let's see who can out-style each other...",
        "Even you're too cool for me...",
        db
    );
}

inline Trainer createBlackBeltDaisuke(const PokemonDatabase& db){
	return buildTrainer(
        "BLACK BELT DAISUKE",
        TRAINER,
        AI_EASY,
        {{"MACHOP", 19}}, 
        {},
        "Self defense through the arts of combat is a must don't you think so?...",
        "It seems that you have great technique...",
        db
    );
}

inline Trainer createInterviewerGabbyAndTy(const PokemonDatabase& db){
	return buildTrainer(
        "INTERVIEWER GABY AND TY",
        TRAINER,
        AI_EASY,
        {{"MAGNEMITE", 19},{"WHISMUR", 19}}, 
        {},
        "Excuse me, but would you like to have an interview with us?...",
        "Oh...you're truly a worthy trainer. Have an interview with us any time!...",
        db
    );
}

//Route 111 B (Desert)
inline Trainer createCamperCliff(const PokemonDatabase& db){
	return buildTrainer(
        "CAMPER CLIFF",
        TRAINER,
        AI_EASY,
        {{"BALTOY", 22},{"SANDSHREW", 22},{"BALTOY", 22}}, 
        {},
        "The desert is harsh, but my Pokemon are tougher!",
        "The heat must've gotten to me...",
        db
    );
}

inline Trainer createPicnickerHeidi(const PokemonDatabase& db){
	return buildTrainer(
        "PICNICKER HEIDI",
        TRAINER,
        AI_EASY,
        {{"SANDSHREW", 23},{"BALTOY", 23}}, 
        {},
        "I brought plenty of water for this desert trek!",
        "Looks like I should've trained more instead...",
        db
    );
}

inline Trainer createCamperDrew(const PokemonDatabase& db){
	return buildTrainer(
        "CAMPER DREW",
        TRAINER,
        AI_EASY,
        {{"SANDSHREW", 24}}, 
        {},
        "Sand Pokemon are perfectly adapted to this environment!",
        "Your Pokemon are even more adapted than mine!",
        db
    );
}

inline Trainer createPicnickerBecky(const PokemonDatabase& db){
	return buildTrainer(
        "PICNICKER BECKY",
        TRAINER,
        AI_EASY,
        {{"SANDSHREW", 24}}, 
        {},
        "This desert is beautiful, don't you think?",
        "You're as strong as this desert is vast...",
        db
    );
}

//Route 112 A
inline Trainer createHikerBrice(const PokemonDatabase& db){
	return buildTrainer(
        "HIKER BRICE",
        TRAINER,
        AI_EASY,
        {{"NUMEL", 17},{"MACHOP", 17}}, 
        {},
        "I've been keeping fit by hiking. Power, I have in spades!",
        "I hear there are some seriously tpugh TRAINERS in MT. CHIMNEY...Goodluck...",
        db
    );
}

inline Trainer createHikerTrent(const PokemonDatabase& db){
	return buildTrainer(
        "HIKER TRENT",
        TRAINER,
        AI_EASY,
        {{"GEODUDE", 16},{"GEODUDE", 16},{"GEODUDE", 17}}, 
        {},
        "Hiking is seriously fun isn't it? You can just feel the adrenaline!",
        "Hahahahaha! Something flew up my nose! Hahahaha-hatchoo!",
        db
    );
}

inline Trainer createCamperLarry(const PokemonDatabase& db){
	return buildTrainer(
        "CAMPER LARRY",
        TRAINER,
        AI_EASY,
        {{"NUZLEAF", 18}}, 
        {},
        "It's my first time camping! I'm on my way to the mountains!",
        "I'm not crying because I miss my mommy! Snivel...",
        db
    );
}

//Mt Chimney Summit
inline Trainer createTeamMagmaAdmin(const PokemonDatabase& db){
	return buildTrainer(
        "TEAM MAGMA ADMIN TABITHA",
        TRAINER,
        AI_MEDIUM,
        {{"NUMEL", 22},{"NUMEL", 18},{"POOCHYENA", 22},{"ZUBAT", 22}}, 
        {},
        "We're using the volcanic energy for our plan to expand the land. Hope you brought some suncreen!",
        "You have to get past our leader MAXIE, you're not gonna stop us!",
        db
    );
}

inline Trainer createTeamMagmaGruntSummit1(const PokemonDatabase& db){
	return buildTrainer(
        "TEAM MAGMA GRUNT",
        TRAINER,
        AI_EASY,
        {{"ZUBAT", 20}}, 
        {},
        "TEAM MAGMA is working here, you're not allowed to interfere!",
        "I can't believe I lost...leader MAXIE won't be pleased...",
        db
    );
}

inline Trainer createTeamMagmaGruntSummit2(const PokemonDatabase& db){
	return buildTrainer(
        "TEAM MAGMA GRUNT",
        TRAINER,
        AI_EASY,
        {{"NUMEL", 20}}, 
        {},
        "We won't let anyone near our leader! You have to get through me first!",
        "How? A kid was able to beat me....",
        db
    );
}

//Jagged Pass
inline Trainer createTeamMagmaGruntJagged(const PokemonDatabase& db){
	return buildTrainer(
        "TEAM MAGMA GRUNT",
        TRAINER,
        AI_EASY,
        {{"NUMEL", 22},{"POOCHYENA", 22}}, 
        {},
        "Hey! how did a kid get here!",
        "What? How did I lose to a kid!....",
        db
    );
}

inline Trainer createHikerEric(const PokemonDatabase& db){
	return buildTrainer(
        "HIKER ERIC",
        TRAINER,
        AI_EASY,
        {{"GEODUDE", 20},{"BALTOY", 20}}, 
        {},
        "My dad said if you're going hiking, don't do it on a volcano, Well look at me now dad..",
        "What's all that rumbling? probably my belly....",
        db
    );
}

inline Trainer createTriathleteJulio(const PokemonDatabase& db){
	return buildTrainer(
        "TRIATHLETE JULIO",
        TRAINER,
        AI_EASY,
        {{"MAGNEMITE", 21}}, 
        {},
        "Not only did I get strong on altitude training, I also just recently got diagnosed with stage 4 lung cancer!",
        "I guess those volcanic ashes finally got me...",
        db
    );
}

inline Trainer createPicnickerAutumn(const PokemonDatabase& db){
	return buildTrainer(
        "PICNICKER AUTUMN",
        TRAINER,
        AI_EASY,
        {{"SHROOMISH", 21}}, 
        {},
        "They say this is a stupid idea, but having a picnic at the side of an erupting volcano makes your food warm without a heater!",
        "I guess this was indeed a stupid idea...",
        db
    );
}

//Route 118
inline Trainer createInterviewerGabbyAndTyRoute118(const PokemonDatabase& db){
	return buildTrainer(
        "INTERVIEWER GABY AND TY",
        TRAINER,
        AI_EASY,
        {{"MAGNEMITE", 27},{"LOUDRED", 27}}, 
        {},
        "Excuse me, but would you like to have an interview with us?...",
        "Oh...you've gotten really strong. Have an interview with us any time!...",
        db
    );
}

inline Trainer createBirdKeeperChester(const PokemonDatabase& db){
	return buildTrainer(
        "BIRD KEEPER CHESTER",
        TRAINER,
        AI_EASY,
        {{"TAILLOW", 25},{"SWELLOW", 25}}, 
        {},
        "My bird Pokemon soar through the skies with grace!",
        "Your Pokemon fly even higher than mine...",
        db
    );
}

inline Trainer createGuitaristDalton(const PokemonDatabase& db){
	return buildTrainer(
        "GUITARIST DALTON",
        TRAINER,
        AI_EASY,
        {{"MAGNEMITE", 15},{"WHISMUR", 15}}, 
        {},
        "Let me play you the song of battle!",
        "That was quite the performance you put on...",
        db
    );
}

//Route 119
inline Trainer createBugManiacTaylor(const PokemonDatabase& db){
	return buildTrainer(
        "BUG MANIAC TAYLOR",
        TRAINER,
        AI_EASY,
        {{"WURMPLE", 25},{"CASCOON", 25},{"DUSTOX", 25}}, 
        {},
        "Bugs are the strongest Pokemon type! Let me show you!",
        "My bugs weren't strong enough...",
        db
    );
}

inline Trainer createNinjaBoyTakashi(const PokemonDatabase& db){
	return buildTrainer(
        "NINJA BOY TAKASHI",
        TRAINER,
        AI_EASY,
        {{"NINCADA", 25},{"KOFFING", 25},{"NINJASK", 25}}, 
        {},
        "Silent as the wind, swift as a shadow!",
        "You have bested this ninja...",
        db
    );
}


//Mt Pyre Interior
inline Trainer createHexManiacTasha(const PokemonDatabase& db){
	return buildTrainer(
        "HEX MANIAC TASHA",
        TRAINER,
        AI_EASY,
        {{"SHUPPET", 32}}, 
        {},
        "See the shadows within! Prepare to be hexed!",
        "You have bested me with better magic...",
        db
    );
}

inline Trainer createHexManiacValerie(const PokemonDatabase& db){
	return buildTrainer(
        "HEX MANIAC VALERIE",
        TRAINER,
        AI_EASY,
        {{"SABLEYE", 32}}, 
        {},
        "I've always been fond of darkness and ghosts, my mom always tells me its just a phase, but it's not!",
        "It's not a phase, I won't grow out of this I say!...",
        db
    );
}

//Mt Pyre Exterior
inline Trainer createTeamAquaGruntMP1(const PokemonDatabase& db) {
    return buildTrainer(
        "TEAM AQUA GRUNT",
        TRAINER,
        AI_EASY,
        {{"ZUBAT", 32}}, 
        {},
        "Hey! We're in a middle of a very secret operation!",
        "Heh! Doesn't matter that should've bought enough time for the BOSS to snatch the item...",
        db
    );
}

inline Trainer createTeamAquaGruntMP2(const PokemonDatabase& db) {
    return buildTrainer(
        "TEAM AQUA GRUNT",
        TRAINER,
        AI_EASY,
        {{"CARVANHA", 32}}, 
        {},
        "No entries allowed! Beat it!",
        "Heh! Doesn't matter that should've bought enough time for the BOSS to snatch the item...",
        db
    );
}

//Magma Hideout
inline Trainer createTeamMagmaGruntMH1(const PokemonDatabase& db){
	return buildTrainer(
        "TEAM MAGMA GRUNT",
        TRAINER,
        AI_EASY,
        {{"MIGHTYENA", 29}}, 
        {},
        "What, what, what?! Only TEAM MAGMA members are supposed to be in here! You rouse my suspicion!",
        "Dang... BOSS MAXIE will not be happy about this...",
        db
    );
}

inline Trainer createTeamMagmaGruntMH2(const PokemonDatabase& db){
	return buildTrainer(
        "TEAM MAGMA GRUNT",
        TRAINER,
        AI_EASY,
        {{"ZUBAT", 29}}, 
        {},
        "You can hear tremor here sometimes. Could it be the volcano rattling or is it GROU... Whoops! never mind!",
        "It doesn't matter if you have beaten me, TEAM MAGMA will have its way!",
        db
    );
}

inline Trainer createTeamMagmaGruntMH3(const PokemonDatabase& db){
	return buildTrainer(
        "TEAM MAGMA GRUNT",
        TRAINER,
        AI_EASY,
        {{"ZUBAT", 29}}, 
        {},
        "I understand everything our leader does, but digging up an ANCIENT POKEMON and ripping off someone's METEORITE is a bit too far",
        "I really should've thought about my life choices....",
        db
    );
}

inline Trainer createTeamMagmaGruntMH4(const PokemonDatabase& db){
	return buildTrainer(
        "TEAM MAGMA GRUNT",
        TRAINER,
        AI_EASY,
        {{"NUMEL", 29}}, 
        {},
        "I don't really have a bone to pick with you, I'm just following orders....",
        "I really need a raise...",
        db
    );
}

inline Trainer createTeamMagmaLeaderMaxie(const PokemonDatabase& db){
	return buildTrainer(
        "TEAM MAGMA LEADER MAXIE",
        TRAINER,
        AI_HARD,
        {{"MIGHTYENA", 37}, {"CROBAT", 38},{"NUMEL", 39}}, 
        {},
        "Oh so it was you?! I've seen you poking around here and there. I get it now. You must have pulled a cheap stunt!",
        "What makes you so adept at handling POKEMON?",
        db
    );
}

//Aqua Hideout
inline Trainer createTeamAquaGruntAH1(const PokemonDatabase& db) {
    return buildTrainer(
        "TEAM AQUA GRUNT",
        TRAINER,
        AI_EASY,
        {{"POOCHYENA", 32}}, 
        {},
        "Hey! We're in a middle of a very secret operation!",
        "It doesn't matter if you beat me, We will have our plan completed!",
        db
    );
}

inline Trainer createTeamAquaGruntAH2(const PokemonDatabase& db) {
    return buildTrainer(
        "TEAM AQUA GRUNT",
        TRAINER,
        AI_EASY,
        {{"CARVANHA", 32}}, 
        {},
        "Which do you think is cooler? TEAM AQUA's or TEAM MAGMA's uniform",
        "I lost in a cool way...",
        db
    );
}

inline Trainer createTeamAquaGruntAH3(const PokemonDatabase& db) {
    return buildTrainer(
        "TEAM AQUA GRUNT",
        TRAINER,
        AI_EASY,
        {{"ZUBAT", 31},{"CARVANHA", 31}}, 
        {},
        "Wahahaha! I grew weary of waiting. You owe me a battle",
        "BOSS... Is this good enough to buy you some time?...",
        db
    );
}

inline Trainer createTeamAquaGruntAH4(const PokemonDatabase& db) {
    return buildTrainer(
        "TEAM AQUA GRUNT",
        TRAINER,
        AI_EASY,
        {{"CARVANHA", 32}}, 
        {},
        "Fuel Control? A-OK, On-board snacks? A-OK, Nothing left to do but KO a pesky meddler!",
        "Hmph! This wasn't supposed to happen!",
        db
    );
}

inline Trainer createTeamAquaGruntAH5(const PokemonDatabase& db) {
    return buildTrainer(
        "TEAM AQUA GRUNT",
        TRAINER,
        AI_EASY,
        {{"ZUBAT", 32}}, 
        {},
        "Portals, Our pride and joy. You're clueless about where you are aren't you?",
        "What's wrong with you? You aren't tired at all...",
        db
    );
}

inline Trainer createTeamAquaGruntAH6(const PokemonDatabase& db) {
    return buildTrainer(
        "TEAM AQUA GRUNT",
        TRAINER,
        AI_EASY,
        {{"ZUBAT", 31},{"POOCHYENA", 31}}, 
        {},
        "The plan is almost complete, there is no stopping us now!",
        "We will have our way whether some kid likes it or not!",
        db
    );
}

inline Trainer createTeamAquaAdminMatt(const PokemonDatabase& db){
	return buildTrainer(
        "TEAM AQUA ADMIN MATT",
        TRAINER,
        AI_MEDIUM,
        {{"MIGHTYENA", 34},{"GOLBAT", 34}}, 
        {},
        "Got here already, did you? I'm a step above all grunts. I'm not stalling for time, I'm going to pulverize you!",
        "We will have our way whether some kid likes it or not!",
        db
    );
}

//Seafloor Cavern
inline Trainer createTeamAquaLeaderArchie(const PokemonDatabase& db){
	return buildTrainer(
        "TEAM AQUA LEADER ARCHIE",
        TRAINER,
        AI_HARD,
        {{"MIGHTYENA", 41}, {"CROBAT", 41},{"SHARPEDO", 43}}, 
        {},
        "My dream must be fulfilled, I can't stop now!",
        "What?! I lost to a mere child like you?!",
        db
    );
}

//Mossdeep 3 - Space Center
inline Trainer createTeamMagmaLeaderMaxieMD(const PokemonDatabase& db){
	return buildTrainer(
        "TEAM MAGMA LEADER MAXIE",
        TRAINER,
        AI_HARD,
        {{"MIGHTYENA", 42}, {"CROBAT", 43},{"CAMERUPT", 44}}, 
        {},
        "We will nuke that volcano whether you like it or not!",
        "Maybe this wasn't the right goal, maybe I was wrong...",
        db
    );
}

//Victory Road
inline Trainer createCoolTrainerHalle(const PokemonDatabase& db){
	return buildTrainer(
        "COOL TRAINER HALLEE",
        TRAINER,
        AI_EASY,
        {{"SABLEYE", 43}, {"ABSOL", 43}}, 
        {},
        "I'm a big fan of the ELITE 4, Especially PHOEBE. I want to get as strong as her someday...",
        "I've got a lot of training to do especially when I get beaten by someone like you...",
        db
    );
}

inline Trainer createCoolTrainerEdgar(const PokemonDatabase& db){
	return buildTrainer(
        "COOL TRAINER EDGAR",
        TRAINER,
        AI_EASY,
        {{"CACTURNE", 43}, {"PELIPPER", 43}}, 
        {},
        "I'm pretty confident I will be the next CHAMPION. Slow but firm steps. Hoo-ha!",
        "Hoo-ha.. My POKEMON gassed out...",
        db
    );
}

inline Trainer createTrainerWally(const PokemonDatabase& db){
	return buildTrainer(
        "TRAINER WALLY",
        TRAINER,
        AI_EASY,
        {{"ALTARIA", 43}, {"DELCATTY", 43}, {"ROSELIA",44}, {"MAGNETON",44},{"GARDEVOIR",44}}, 
        {},
        "WALLY: I'm WALLY, I'm the closest you can get to an ELITE 4, just you wait, I'm almost there. I can't stop now.",
        "WALLY: You have so much potential. I was wrong. I have much more to learn.",
        db
    );
}

inline Trainer createProgrammerSelwyn(const PokemonDatabase& db){
	return buildTrainer(
        "PROGRAMMER SELWYN",
        TRAINER,
        AI_HARD,
        {{"SWAMPERT", 40}, {"SALAMENCE", 40}, {"FLYGON",40}, {"MAGNETON",40},{"GARDEVOIR",42}}, 
        {},
        "I'm so angry at this project that I added myself. You're up for a CHALLENGE? GOODLUCK! POWER in the palm of my hands!",
        "I accept my defeat. I'll buff myself real soon...fufufu...",
        db
    );
}
/*****************************************************************************
 * TRAINER LOADER
 *****************************************************************************/
inline vector<Trainer> loadAllTrainers(const PokemonDatabase& db) {
    return {
        createGymLeaderRoxanne(db)
        // Add more trainers here
    };
}

/*****************************************************************************
 * TRAINER AI HELPERS
 *****************************************************************************/

// Attempt to use a healing item. Returns true if item was used.
inline bool tryUseHealingItem(Trainer& trainer, Pokemon& active, int maxHP) {
    if (trainer.items.empty()) return false;

    // Determine heal threshold based on AI level
    double threshold = TrainerAIConfig::HEAL_THRESHOLD_MEDIUM;
    switch (trainer.trainerAIlevel) {
        case AI_EASY:   threshold = TrainerAIConfig::HEAL_THRESHOLD_EASY;   break;
        case AI_MEDIUM: threshold = TrainerAIConfig::HEAL_THRESHOLD_MEDIUM; break;
        case AI_HARD:   threshold = TrainerAIConfig::HEAL_THRESHOLD_HARD;   break;
    }

    // Don't heal if above threshold
    if ((double)active.currentHP > (double)maxHP * threshold) return false;

    // Find and use first available healing item
    for (auto& item : trainer.items) {
        if (item.itemType != ItemType::HEALING || item.quantity <= 0) continue;
        
        active.currentHP += item.healAmount;
        if (active.currentHP > maxHP) active.currentHP = maxHP;
        item.quantity--;

        cout << trainer.trainerName << " used " << item.itemName 
             << " on " << active.pokemonName << "!\n";
        return true;
    }

    return false;
}

// Choose move index based on AI level. Returns index into Pokemon::moveID.
inline int chooseMoveAI(const Trainer& trainer, const Pokemon& opponent, const PokemonDatabase& db) {
    int activeIdx = trainer.firstAliveIndex();
    if (activeIdx == -1) return 0;
    
    const Pokemon& active = trainer.currentPokemon[activeIdx];
    int moveCount = (int)active.moveID.size();
    if (moveCount == 0) return 0;

    // AI_EASY: Random move
    if (trainer.trainerAIlevel == AI_EASY) {
        return rand() % moveCount;
    }

    // AI_MEDIUM: Highest power non-status move
    if (trainer.trainerAIlevel == AI_MEDIUM) {
        int bestIdx = 0;
        int bestPower = -1;
        
        for (int i = 0; i < moveCount; ++i) {
            if (active.currentPP[i] <= 0) continue;

            int moveID = active.moveID[i];
            if (moveID <= 0 || moveID > (int)db.moveList.size()) continue;
            
            const Move& m = db.moveList[moveID - MoveConfig::MOVE_ID_OFFSET];
            if (m.category == MoveCategory::STATUS) continue;
            
            if (m.power > bestPower) {
                bestPower = m.power;
                bestIdx = i;
            }
        }
        
        return (bestPower >= 0) ? bestIdx : rand() % moveCount;
    }

    // AI_HARD: Best score considering type matchup
    double bestScore = -1.0;
    int bestMove = 0;
    
    for (int i = 0; i < moveCount; ++i) {
        if (active.currentPP[i] <= 0) continue;

        int moveID = active.moveID[i];
        if (moveID <= 0 || moveID > (int)db.moveList.size()) continue;
        
        const Move& m = db.moveList[moveID - MoveConfig::MOVE_ID_OFFSET];

        double score = (m.category == MoveCategory::STATUS) 
            ? TrainerAIConfig::STATUS_MOVE_BASE_SCORE 
            : (double)m.power;

        // Type advantage bonus
        if (m.type == opponent.primaryType || m.type == opponent.secondaryType) {
            score *= TrainerAIConfig::TYPE_MATCH_BONUS;
        }

        if (score > bestScore) {
            bestScore = score;
            bestMove = i;
        }
    }

    return bestMove;
}

#endif // TRAINER_H