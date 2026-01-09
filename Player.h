#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <iostream>

#include "PokemonDatabase.h"
#include "ItemDatabase.h"

using namespace std;

/*****************************************************************************
 * CONSTANTS
 *****************************************************************************/
namespace PlayerConfig {
    constexpr int MAX_PARTY_SIZE = 6;
    constexpr int TOTAL_BADGES = 8;
    constexpr int STARTING_MONEY = 3000;
}
namespace BadgeIndex {
    constexpr int STONE_BADGE = 0;    // Rustboro - Roxanne
    constexpr int KNUCKLE_BADGE = 1;  // Dewford - Brawly
    constexpr int DYNAMO_BADGE = 2;   // Mauville - Wattson
    constexpr int HEAT_BADGE = 3;     // Lavaridge - Flannery
    constexpr int BALANCE_BADGE = 4;  // Petalburg - Norman
    constexpr int FEATHER_BADGE = 5;  // Fortree - Winona
    constexpr int MIND_BADGE = 6;     // Mossdeep - Tate & Liza
    constexpr int RAIN_BADGE = 7;     // Sootopolis - Juan
}
/*****************************************************************************
 * ENUMS
 *****************************************************************************/
enum Gender { 
    BOY, 
    GIRL 
};

/*****************************************************************************
 * PLAYER STRUCTURE
 *****************************************************************************/
struct Player {
    string username;
    Gender gender;
    vector<Pokemon> currentPokemon;
    vector<Item> items;
    vector<string> encounteredPokemon;
    int money;
    bool badges[PlayerConfig::TOTAL_BADGES] = {false};

    Player() 
        : username("DEFAULT")
        , gender(BOY)
        , money(PlayerConfig::STARTING_MONEY) 
    {}

    /******************
     * BADGE SYSTEM
     ******************/
    int getBadgeCount() const {
        int count = 0;
        for (int i = 0; i < PlayerConfig::TOTAL_BADGES; ++i) {
            if (badges[i]) ++count;
        }
        return count;
    }

    bool hasBadge(int badgeIndex) const {
        if (badgeIndex < 0 || badgeIndex >= PlayerConfig::TOTAL_BADGES) return false;
        return badges[badgeIndex];
    }

    void earnBadge(int badgeIndex) {
        if (badgeIndex >= 0 && badgeIndex < PlayerConfig::TOTAL_BADGES) {
            badges[badgeIndex] = true;
        }
    }

    /******************
     * PARTY MANAGEMENT
     ******************/
    void addPokemon(const Pokemon& newPokemon) {
        if ((int)currentPokemon.size() < PlayerConfig::MAX_PARTY_SIZE) {
            currentPokemon.push_back(newPokemon);
            return;
        }
        
        cout << "Your party is full! " 
             << newPokemon.pokemonName 
             << " was sent to storage." << endl;
    }
    
    bool hasAlivePokemon() const {
        for (const auto& p : currentPokemon) {
            if (p.currentHP > 0) return true;
        }
        return false;
    }
    
    Pokemon* getFirstAlivePokemon() {
        for (auto& p : currentPokemon) {
            if (p.currentHP > 0) return &p;
        }
        return nullptr;
    }
    /************
    for POKEDEX ENTRIES
    *************/
    void encounterPokemon(const string& name) {
        if (find(encounteredPokemon.begin(), encounteredPokemon.end(), name) == encounteredPokemon.end()) {
            encounteredPokemon.push_back(name);
        }
    }
    
};

/*****************************************************************************
 * PLAYER CREATION HELPER
 *****************************************************************************/
inline Player createNewPlayer(const PokemonDatabase& db, const string& starterName) {
    Player player;

    // Starter Pokemon
    const Pokemon* starter = db.findPokemon(starterName);
    if (starter) {
        player.currentPokemon.push_back(*starter);
        player.currentPokemon.back().initializePP(db.moveList);
    }

    // Default inventory
    player.items = Item::createDefaultItemSet();

    return player;
}

#endif // PLAYER_H