#ifndef POKEMON_UTILITY_H
#define POKEMON_UTILITY_H

#include <iostream>
#include <vector>
#include <string>
#include <conio.h> 
#include <cstdlib>
#include <iomanip>

#include "PokemonDatabase.h"
#include "Player.h"
#include "PokemonDisplay.h"
#include "ItemDatabase.h"

using namespace std;

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/
void chooseGender(Player &player);
void enterPlayerName(Player &player, const char *birchIntro[], int numLines);
void drawAlphabetGrid(int cursorRow, int cursorCol, const string &currentName);

/*****************************************************************************
 * CONFIGURATION CONSTANTS
 *****************************************************************************/
namespace UIConfig {
    // Grid dimensions for name entry
    constexpr unsigned short GRID_ROWS = 6;
    constexpr unsigned short GRID_COLS = 6;
    
    // UI layout widths
    constexpr int STANDARD_BOX_WIDTH = 50;
    constexpr int WIDE_BOX_WIDTH = 60;
    constexpr int ACTION_MENU_COL_WIDTH = 22;
    constexpr int MOVE_MENU_COL_WIDTH = 28;
    constexpr int BATTLE_BOX_WIDTH = 70;
    
    // Text animation delays (milliseconds)
    constexpr int DIALOGUE_DELAY = 200;
    constexpr int POKEBALL_SHAKE_DELAY = 700;
    constexpr int POKEBALL_FINAL_DELAY = 900;
    constexpr int HEALING_DOT_DELAY = 500;
    constexpr int BRIEF_PAUSE = 300;
    constexpr int SHORT_PAUSE = 500;
    constexpr int MEDIUM_PAUSE = 600;
    constexpr int LONG_PAUSE = 800;
}

namespace GameplayConfig {
    // PC storage limits
    constexpr int PC_BOX_MAX = 30;
    
    // Party limits
    constexpr int PARTY_MAX = 6;
    constexpr int PARTY_MIN = 1;
    
    // Pokedex pagination
    constexpr int POKEDEX_ENTRIES_PER_PAGE = 15;
    
    // Catching mechanics
    constexpr int CATCH_CHANCE_CRITICAL = 70;  // HP <= 25%
    constexpr int CATCH_CHANCE_HALF = 45;      // HP <= 50%
    constexpr int CATCH_CHANCE_WEAKENED = 25;  // HP <= 75%
    constexpr int CATCH_CHANCE_HEALTHY = 100;   // HP > 75%
    constexpr int CATCH_ALMOST_BONUS = 15;
    
    // HP thresholds for catching
    constexpr double HP_CRITICAL_THRESHOLD = 0.25;
    constexpr double HP_HALF_THRESHOLD = 0.5;
    constexpr double HP_WEAKENED_THRESHOLD = 0.75;
    
    // Name constraints
    constexpr int PLAYER_NAME_MIN_LENGTH = 2;
    constexpr int PLAYER_NAME_MAX_LENGTH = 7;
    
    // Item limits
    constexpr int MAX_ITEM_QUANTITY = 99;
    
    // Badge count
    constexpr int TOTAL_BADGES = 8;
}

namespace KeyCodes {
    // Arrow keys (extended codes)
    constexpr int ARROW_PREFIX_1 = 224;
    constexpr int ARROW_PREFIX_2 = 0;
    constexpr int ARROW_UP = 72;
    constexpr int ARROW_DOWN = 80;
    constexpr int ARROW_LEFT = 75;
    constexpr int ARROW_RIGHT = 77;
    
    // Action keys
    constexpr int KEY_ENTER = 13;
    constexpr int KEY_ESC = 27;
    constexpr int KEY_BACKSPACE = 8;
    constexpr int KEY_M_LOWER = 'm';
    constexpr int KEY_M_UPPER = 'M';
}

namespace CatchResult {
    constexpr int FAILED = 0;
    constexpr int ALMOST = 1;
    constexpr int SUCCESS = 2;
}

/*****************************************************************************
 * ALPHABET GRID DATA
 *****************************************************************************/
const vector<string> ALPHABET = {
    "A", "B", "C", "D", "E", "F", "G", "H", "I",
    "J", "K", "L", "M", "N", "O", "P", "Q", "R",
    "S", "T", "U", "V", "W", "X", "Y", "Z", "OK"
};

/*****************************************************************************
 * PC BOX STORAGE
 *****************************************************************************/
static vector<Pokemon> pcBox;

/*****************************************************************************
 * POKEDEX ENTRY STRUCTURE
 *****************************************************************************/
struct PokedexEntry {
    int pokedexNum;
    string pokemonName;
    bool encountered;
    bool owned;
};

/*****************************************************************************
 * TYPE NAME HELPER
 *****************************************************************************/
string getTypeName(PokemonType type) {
    switch (type) {
        case PokemonType::NORMAL:   return "NORMAL";
        case PokemonType::FIRE:     return "FIRE";
        case PokemonType::WATER:    return "WATER";
        case PokemonType::GRASS:    return "GRASS";
        case PokemonType::ELECTRIC: return "ELECTRIC";
        case PokemonType::ICE:      return "ICE";
        case PokemonType::FIGHTING: return "FIGHTING";
        case PokemonType::POISON:   return "POISON";
        case PokemonType::GROUND:   return "GROUND";
        case PokemonType::FLYING:   return "FLYING";
        case PokemonType::PSYCHIC:  return "PSYCHIC";
        case PokemonType::BUG:      return "BUG";
        case PokemonType::ROCK:     return "ROCK";
        case PokemonType::GHOST:    return "GHOST";
        case PokemonType::DRAGON:   return "DRAGON";
        case PokemonType::DARK:     return "DARK";
        case PokemonType::STEEL:    return "STEEL";
        case PokemonType::FAIRY:    return "FAIRY";
        case PokemonType::NONE:     return "";
        default:                    return "UNKNOWN";
    }
}

/*****************************************************************************
 * GENERAL UTILITY FUNCTIONS
 *****************************************************************************/
void pressAnyKey() {
    cout << "Press Any Key To Continue:" << endl;
    _getch();
}

void startKey(){
	_getch();
}
void clearScreen() {
    system("cls");
}

/*****************************************************************************
 * MAIN BATTLE ACTION MENU (FIGHT/BAG/POKEMON/RUN)
 *****************************************************************************/
int selectionMenu(Pokemon& player) {
    const vector<string> options = {"FIGHT", "BAG", "POKEMON", "RUN"};
    int cursor = 0;
    
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    while (true) {
        clearScreen();

        // Draw 2x2 grid layout
        for (int i = 0; i < 4; ++i) {
            drawHighlight(h, options[i], cursor == i);

            int spaces = UIConfig::ACTION_MENU_COL_WIDTH - (int)options[i].size();
            cout << string(max(1, spaces), ' ');

            if (i % 2 == 1) cout << "\n\n";
        }

        cout << "\nARROW KEYS  -  MOVE\n";
        cout << "ENTER       -  SELECT\n";

        int key = _getch();

        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();

            switch (key) {
                case KeyCodes::ARROW_UP:
                    if (cursor == 2) cursor = 0;
                    else if (cursor == 3) cursor = 1;
                    break;

                case KeyCodes::ARROW_DOWN:
                    if (cursor == 0) cursor = 2;
                    else if (cursor == 1) cursor = 3;
                    break;

                case KeyCodes::ARROW_LEFT:
                    if (cursor == 1) cursor = 0;
                    else if (cursor == 3) cursor = 2;
                    break;

                case KeyCodes::ARROW_RIGHT:
                    if (cursor == 0) cursor = 1;
                    else if (cursor == 2) cursor = 3;
                    break;
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            return cursor;
        }
    }

    return -1;
}

/*****************************************************************************
 * MOVE SELECTION MENU
 *****************************************************************************/
int moveSelectionMenu(const vector<Move>& moveList,
                      const vector<int>& moveIDs,
                      const vector<int>& currentPP)
{
    if (moveIDs.empty()) return -1;

    int cursor = 0;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    // Flush leftover keypresses
    while (_kbhit()) _getch();

    while (true) {
        clearScreen();

        // Header
        cout << "+" << string(UIConfig::BATTLE_BOX_WIDTH, '-') << "+\n";
        cout << "| CHOOSE A MOVE:" << string(UIConfig::BATTLE_BOX_WIDTH - 14, ' ') << "|\n";
        cout << "+" << string(UIConfig::BATTLE_BOX_WIDTH, '-') << "+\n\n";

        // Display moves in 2x2 grid
        for (int i = 0; i < 4; ++i) {
            string display;

            if (i < (int)moveIDs.size()) {
                const Move& m = moveList.at(moveIDs[i] - 1);
                display = m.moveName + " (" + to_string(currentPP[i]) + "/" + to_string(m.pp) + ")";
            } else {
                display = "-";
            }

            drawHighlight(h, display, cursor == i);

            int spaces = UIConfig::MOVE_MENU_COL_WIDTH - (int)display.size();
            cout << string(max(1, spaces), ' ');

            if (i % 2 == 1) cout << "\n\n";
        }

        cout << "ARROW KEYS  -  MOVE\n";
        cout << "ENTER       -  SELECT\n";
        cout << "ESC         -  BACK\n";

        int key = _getch();

        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();

            switch (key) {
                case KeyCodes::ARROW_UP:
                    if (cursor >= 2) cursor -= 2;
                    break;

                case KeyCodes::ARROW_DOWN:
                    if (cursor + 2 < (int)moveIDs.size()) cursor += 2;
                    break;

                case KeyCodes::ARROW_LEFT:
                    if (cursor % 2 == 1) cursor -= 1;
                    break;

                case KeyCodes::ARROW_RIGHT:
                    if (cursor % 2 == 0 && cursor + 1 < (int)moveIDs.size()) cursor += 1;
                    break;
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            if (cursor < (int)moveIDs.size()) return cursor;
        }
        else if (key == KeyCodes::KEY_ESC) {
            return -1;
        }

        // Safety clamp for empty slots
        if (cursor >= (int)moveIDs.size()) {
            cursor = (int)moveIDs.size() - 1;
        }
    }

    return -1;
}

/*****************************************************************************
 * YES / NO MENU
 *****************************************************************************/
int yesNoMenu(const string& prompt, bool inlineMode) {
    int cursor = 0;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    
    const int boxWidth = (prompt.length() > 50) ? prompt.length() + 4 : 50;
    
    while (true) {
        clearScreen();
        
        // Header box
        cout << "+" << string(boxWidth, '-') << "+\n";
        cout << "| " << prompt;
        
        int padding = boxWidth - prompt.length() - 1;
        if (padding > 0) cout << string(padding, ' ');
        cout << "|\n";
        cout << "+" << string(boxWidth, '-') << "+\n\n";
        
        // Options
        drawHighlight(h, "YES", cursor == 0);
        cout << "\n";
        drawHighlight(h, "NO", cursor == 1);
        cout << "\n\n";
        
        // Controls
        cout << "UP/DOWN  -  NAVIGATE\n";
        cout << "ENTER    -  SELECT\n";
        
        int key = _getch();
        
        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            if (key == KeyCodes::ARROW_UP || key == KeyCodes::ARROW_DOWN) {
                cursor = (cursor == 0 ? 1 : 0);
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            return cursor == 0;
        }
    }
    
    return 1;
}

/*****************************************************************************
 * STORY DIALOGUES
 *****************************************************************************/
void drBirchIntro(Player &player) {
    const char* birchIntro[] = {
        "PROF BIRCH: Hi! Sorry to keep you waiting!",
        "Welcome to the world of POKEMON!",
        "My name is BIRCH.",
        "But everyone calls me the POKEMON PROFESSOR.",
        "This is what we call a POKEMON.",
        "This world is widely inhabited by creatures known as POKEMON.",
        "We humans live alongside POKEMON, at times as friendly playmates, and",
        "at times as cooperative workmates.",
        "But despite our closeness, we don't know everything about POKEMON.",
        "To unravel their mysteries, I've been undertaking research.",
        "That's what I do!",
        "And you are?",
        "Your very own adventure is about to unfold!",
        "Come join into the world of POKEMON where adventure awaits!",
        "Well, I'll be expecting you later. Come see me in my POKEMON LAB."
    };

    const int numLines = sizeof(birchIntro) / sizeof(birchIntro[0]);

    clearScreen();

    // First segment (0-4)
    for (int i = 0; i < 5; i++) {
        showDialogue(birchIntro[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
    
    pokemonSampleArt();
    Sleep(UIConfig::MEDIUM_PAUSE);
    pressAnyKey();
    clearScreen();
    
    // Second segment (5-11)
    for (int i = 5; i < 12; i++) {
        showDialogue(birchIntro[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }

    chooseGender(player);
    enterPlayerName(player, birchIntro, numLines);
    
    // Final segment (12+)
    for (int i = 12; i < numLines; i++) {
        showDialogue(birchIntro[i], 40, 25, true);
        clearScreen();
    }
}

void momStarter(Player &player) {
    const char* momStarter[] = {
        "MOM: You must be tired after moving all those boxes Honey",
        "I'll prepare all your things for you",
		"I heard PROF BIRCH is looking for you at his POKEMON LAB",
		"Go and see him, i'll take care of your stuff",
        "Take care now, I love you!"
    };
    
    const int numLines = sizeof(momStarter) / sizeof(momStarter[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(momStarter[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void drBirchStarter(Player &player) {
    const char* birchStarter[] = {
        "PROF BIRCH: Ahh It's good that you're here!",
        "As you can see I'm in a bit of a scuffle",
		"I'm being attacked by a ZIGZAGOON",
		"Over there is my bag which contains POKEBALLS",
		"Inside it are POKEMON, CHOOSE one and...",
		"Fend off this ZIGZAGOON for me"
    };
    
    const int numLines = sizeof(birchStarter) / sizeof(birchStarter[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(birchStarter[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void drBirchStarter2(Player &player) {
    clearScreen();
    
    string thankPlayer = "Phew.. I owe you there " + player.username;
    showDialogue(thankPlayer);
    clearScreen();

    const char* birchStarter2[] = {
        "As a reward, I will give you that POKEMON",
        "I entrust you to take care of it",
        "And oh by the way",
        "Here's a little thank you",
        "These are POKEBALLS...",
        "You will use this to CATCH well...",
        "POKEMON, Of your choice that is",
        "A tip is a weaker a POKEMON gets....",
        "The higher the chance for them to get caught!",
        "Now go on and start your journey",
        "And experience the world for yourself"
    };

    const int numLines = sizeof(birchStarter2) / sizeof(birchStarter2[0]);

    for (int i = 0; i < numLines; i++) {
        showDialogue(birchStarter2[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();

        // After "These are POKEBALLS...", give player 5 Pokéballs
        if (i == 6) {
            const int pokeballGiftQuantity = 5;
            bool found = false;
            
            for (auto &item : player.items) {
                if (item.itemName == "POKEBALL") {
                    item.quantity += pokeballGiftQuantity; 
                    found = true;
                    break;
                }
            }

            if (!found) {
                player.items.push_back(Item("POKEBALL", ItemType::POKEBALL, 0, pokeballGiftQuantity));
            }

            showDialogue("Obtained POKEBALLS X5!");
            Sleep(UIConfig::DIALOGUE_DELAY);
            clearScreen();
        }
    }
}

void aquaArchieOrb() {
    const char* archieOrb[] = {
       "TEAM AQUA LEADER ARCHIE: TEAM MAGMA's MAXIE got ahead of us, but we also got what we wanted...",
		"The RED ORB preserved at MT. PYRE... I, ARCHIE, now have it in my posession!",
		"Fufufu... Now we can bring our ultimate objective to fruition!",
		"Okay, TEAM! We're pulling out!"
    };
    
    const int numLines = sizeof(archieOrb) / sizeof(archieOrb[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(archieOrb[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void oldGrandmaOrb() {
    const char* grandmaOrb[] = {
       "OLD LADY, THE ORB KEEPER: Oh no that cannot happen...",
		"Not only the BLUE ORB, but even the RED ORB has been taken...",
		"The BLUE ORB and RED ORB must never be separated. They belong together...",
		"What are those men trying to do with the two ORBS?",
		"TEAM MAGMA also left a clue about using the BLUE ORB inside a VOLCANO!",
		"I fear something bad may happen if those two orbs are separated...",
		"We must stop those sinister men from whatever they are about to do!..."
    };
    
    const int numLines = sizeof(grandmaOrb) / sizeof(grandmaOrb[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(grandmaOrb[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void maxiePreGroudon() {
    const char* maxiePre[] = {
        "TEAM MAGMA LEADER MAXIE: GROUDON!",
		"Nothing could awaken you from your sleep bathed in magma...",
		"This BLUE ORB is what you sought. Wasn't it?",
		"I have brought you the BLUE ORB. Let its shine awaken you!",
		"And show me... Show me the full extent of your power!..."
    };
    
    const int numLines = sizeof(maxiePre) / sizeof(maxiePre[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(maxiePre[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void maxiePostGroudon() {
    const char* maxiePost[] = {
        "TEAM MAGMA LEADER MAXIE: GROUDON! What's wrong?!",
		"Wasn't the BLUE ORB the key?",
		"GROUDON! Where have you gone..."
    };
    
    const int numLines = sizeof(maxiePost) / sizeof(maxiePost[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(maxiePost[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void maxiePostDefeat() {
    const char* maxiePostDef[] = {
        "TEAM MAGMA LEADER MAXIE: There has to be some reason why GROUDON fled...",
		"That's what you're trying to say, arent't you?",
		"With GROUDON gone there is no more reason to say in this blasted VOLCANO.",
		"I'm going after GROUDON, so this is where we part!"
    };
    
    const int numLines = sizeof(maxiePostDef) / sizeof(maxiePostDef[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(maxiePostDef[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void mattPostDefeat(){
	const char* mattPostDef[]={
		"TEAM AQUA ADMIN MATT: Hehehe....",
		"While I was toying with you, our BOSS got through his preparations!",
		"He has already DIVED through a CAVERN underneath this very spot!",
		"By the time you get to him, he should have awakened the thing!...",
		"Goodluck... You'll need it!"
	};
	
	const int numLines = sizeof(mattPostDef) / sizeof(mattPostDef[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(mattPostDef[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void archiePreKyogre() {
    const char* archiePre[] = {
        "TEAM AQUA LEADER ARCHIE: Hold it right there.",
		"Fufufu... So it was you, after all.",
		"Behold!...",
		"See how beautiful it is, the sleeping form of the ancient POKEMON KYOGRE!",
		"I have waited so long for this day to come.",
		"It surprises me how you've managed to chase me here.",
		"But that's all over now, for the realization of my dream, you must dissapear now!"
    };
    
    const int numLines = sizeof(archiePre) / sizeof(archiePre[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(archiePre[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void archiePostDefeat(){
	const char* archiePostDef[]={
		"TEAM AQUA LEADER ARCHIE: Fufufu....",
		"I must recognize that you are truly gifted.",
		"But! I have this in my possession!",
		"With this RED ORB I can make KYOGRE...",
		"...",
		"The RED ORB suddenly began shining by itself!"
	};
	
	const int numLines = sizeof(archiePostDef) / sizeof(archiePostDef[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(archiePostDef[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void archiePostKyogre(){
	const char* archiePost[] = {
        "TEAM AQUA LEADER ARCHIE: What?!",
		"I didn't do anything, why did the RED ORB...",
		"Where did KYOGRE go?",
		"Hmmph? It's a message from my TEAM AQUA members...",
		"Yes? what is it? It's raining heavily outside?",
		"Good..That should have happened. This is why we awakened KYOGRE,",
		"To realize TEAM AQUA's vision of expanding the sea.",
		"What?! It's raining far harden than we envisioned? You're in danger?",
		"This can't be, that's impossible! Hold your position and monitor the situation!",
		"The RED ORB was supposed to control KYOGRE. Why?!"	
    };
    
    const int numLines = sizeof(archiePost) / sizeof(archiePost[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(archiePost[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void maxieToArchie(){
	const char* maxieArchie[] = {
        "TEAM MAGMA LEADER MAXIE: You awakened KYOGRE did you?",
		"The world's landmass will drown in deepening sea.",
		"Let's go outside and see if what we've wrought is the world that we desired!",
		"Hey you, kid. You have to leave, now!"
    };
    
    const int numLines = sizeof(maxieArchie) / sizeof(maxieArchie[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(maxieArchie[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void stevenToMaxie(Player &player){
	const char* stevenMaxie[] = {
        "STEVEN: TEAM MAGMA...What's the point of stealing the rocket fuel?",
		"TEAM MAGMA LEADER MAXIE : Fufufu... Since you're so curious, you deserve an explanation.",
		"We're going to jettison the entire load into MT CHIMNEY!",
		"With GROUDON gone, we have no need for that slag heap of a mountain!",
		"So we'll use the fuel's power to make the volcano erupt!",
		"STEVEN: We won't let you do that!..."
    };
    
    const int numLines = sizeof(stevenMaxie) / sizeof(stevenMaxie[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(stevenMaxie[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
    
    string helpStevenMagma= player.username + " stop TEAM MAGMA, brace yourself!";
    showDialogue(helpStevenMagma);
}

void maxiePostDefeatRocket(Player &player){
	const char* maxiePostDefRocket[] = {
        "TEAM MAGMA LEADER MAXIE: We failed to control GROUDON, we failed to make the volcano erupt...",
		"Is our goal to expand the land misguided?",
		"All right... We will give up on the fuel...",
		"There appears to be more important matters I must examine...",
		"Hmph...? What's that? There is what in SOOTOPOLIS now?!",
		"GROUDON and KYOGRE are WHAT?!",
		"TEAM MAGMA! Cancel the plans. We need to go. NOW!",
    };
    
    const int numLines = sizeof(maxiePostDefRocket) / sizeof(maxiePostDefRocket[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(maxiePostDefRocket[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void stevenPostRocket(Player &player){
	string warnPlayer= "STEVEN: "+ player.username + " this is really bad!";
	showDialogue(warnPlayer);
	const char* stevenPostRocket[] = {
        "SOOTOPOLIS is in shambles!",
		"No time to explain, we must head there, NOW!",
		"With TATE and LIZA's psychic prowess, Mr Briney's S.S. Tidal should be able to DIVE towards the city.",
		"Meet me there."
    };
    
    const int numLines = sizeof(stevenPostRocket) / sizeof(stevenPostRocket[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(stevenPostRocket[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void stevenIntroduceWallace(Player &player){
	string bracePlayer= "STEVEN: "+ player.username+ ",";
	showDialogue(bracePlayer);
	
	const char* stevenIntroWallace[]={
		"You being here now, I'll take to mean that you're prepared to become involved in this crisis.",
		"Well, then. There's someone I'd like you to meet...",
		"Come with me..."
	};
	
	const int numLines = sizeof(stevenIntroWallace) / sizeof(stevenIntroWallace[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(stevenIntroWallace[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
	
}

void wallaceIntro(Player &player){
	string metPlayer= "WALLACE: Ah so you're "+ player.username+ "? I heard tales of your exploits.";
	showDialogue(metPlayer);
	
	const char* wallaceIntr[]={
		"My name is WALLACE.",
		"I was once the GYM LEADER of SOOTOPOLIS, but something came up.",
		"So now I've entrusted my mentor, JUAN, with the GYM's operation.",
		"GROUDON and KYOGRE wreaking havoc, the two ANCIENT POKEMON must be stopped.",
		"There is only ONE WAY to stop both of them.",
		"You see there isn't only TWO ANCIENT POKEMON.",
		"There is ANOTHER ONE.",
		"In the SKY PILLAR where he sleeps.",
		"There a SUPER ANCIENT POKEMON, one that rules above LAND and WATER, once that rules the SKIES.",
		"That POKEMON is RAYQUAZA.",
		"It is said that only RAYQUAZA has the power to calm the two storms.",
		"There is not a moment to lose, HOENN is in your hands.",
		"Take Briney's boat and head over to the SKY PILLAR."
	};
	
	const int numLines = sizeof(wallaceIntr) / sizeof(wallaceIntr[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(wallaceIntr[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void AwakenRayquazaTxt(){
	const char* rayTxt[]={
		"You feel the tower starting to rumble...",
		".....",
		"The ANCIENT DRAGON OF THE SKIES opens its eyes...",
		"The SKY PILLAR starts shaking!",
		"The awakened RAYQUAZA flew off!"
	};
	
	const int numLines = sizeof(rayTxt) / sizeof(rayTxt[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(rayTxt[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

void wallacePostDefeat (Player &player){
	string kudosPlayer= "WALLACE: Kudos to you, "+ player.username;
	showDialogue(kudosPlayer);
	const char* wallacePostDef[]={
		"WALLACE: The POKEMON you sent into battle...",
		"At times they danced like a spring breeze, and at times they struck like lightning.",
		"It was with light, yet surefooted, elegance that you led your POKEMON.",
		"Even I fel bedazzled by your masterful performance!",
		"You now stand at the glorious peak of the POKEMON LEAGUE. I now proclaim you to be HOENN's new CHAMPION."
	};
	
	const int numLines = sizeof(wallacePostDef) / sizeof(wallacePostDef[0]);
    
    for (int i = 0; i < numLines; i++) {
        showDialogue(wallacePostDef[i]);
        Sleep(UIConfig::DIALOGUE_DELAY);
        clearScreen();
    }
}

/*****************************************************************************
 * PLAYER CREATION: CHOOSE GENDER
 *****************************************************************************/
void chooseGender(Player &player) {
    const vector<string> genders = {"BOY", "GIRL"};
    int cursor = 0;

    size_t maxLen = 0;
    for (const auto& g : genders) {
        if (g.length() > maxLen) maxLen = g.length();
    }

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    while (true) {
        clearScreen();

        // Prompt box
        string prompt = "ARE YOU A BOY OR A GIRL?";
        cout << "+" << string(prompt.length() + 2, '-') << "+\n";
        cout << "| " << prompt << " |\n";
        cout << "+" << string(prompt.length() + 2, '-') << "+\n\n";

        // Options
        for (int i = 0; i < (int)genders.size(); i++) {
            string text = genders[i] + string(maxLen - genders[i].length(), ' ');
            drawHighlight(h, text, cursor == i);
            cout << "\n";
        }

        int key = _getch();

        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            if (key == KeyCodes::ARROW_UP || key == KeyCodes::ARROW_DOWN) {
                cursor = 1 - cursor;
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            player.gender = (cursor == 0 ? BOY : GIRL);
            clearScreen();
            return;
        }
    }
}

/*****************************************************************************
 * PLAYER CREATION: ENTER NAME
 *****************************************************************************/
void enterPlayerName(Player &player, const char* birchIntro[], int numLines) {
    int cursorRow = 0;
    int cursorCol = 0;
    string currentName = "";
    bool finished = false;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    size_t maxLen = 0;
    for (const auto &a : ALPHABET) {
        if (a.length() > maxLen) maxLen = a.length();
    }

    auto clampCursorToValid = [&](int &r, int &c) {
        int idx = r * UIConfig::GRID_COLS + c;
        if (idx < (int)ALPHABET.size()) return;

        while (c > 0 && (r * UIConfig::GRID_COLS + c) >= (int)ALPHABET.size()) {
            c--;
        }

        while ((r * UIConfig::GRID_COLS) >= (int)ALPHABET.size() && r > 0) {
            r--;
            while (c > 0 && (r * UIConfig::GRID_COLS + c) >= (int)ALPHABET.size()) {
                c--;
            }
        }
    };

    while (!finished) {
        clearScreen();

        string prompt = "YOUR NAME?";
        cout << "+" << string(prompt.length() + 2, '-') << "+\n";
        cout << "| " << prompt << " |\n";
        cout << "+" << string(prompt.length() + 2, '-') << "+\n\n";

        // Draw alphabet grid
        for (int r = 0; r < UIConfig::GRID_ROWS; r++) {
            for (int c = 0; c < UIConfig::GRID_COLS; c++) {
                int index = r * UIConfig::GRID_COLS + c;
                if (index >= (int)ALPHABET.size()) break;

                string text = ALPHABET[index] + string(maxLen - ALPHABET[index].length(), ' ');
                drawHighlight(h, text, (r == cursorRow && c == cursorCol));
                cout << "\t";
            }
            cout << "\n";
        }

        cout << "\nARROW KEYS - Move\n";
        cout << "ENTER      - Select Letter\n";
        cout << "BACKSPACE  - Delete\n";
        cout << "OK         - Set Name\n\n";
        cout << "CURRENT NAME: " << currentName << "\n";

        int key = _getch();

        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();

            if (key == KeyCodes::ARROW_UP) {
                if (cursorRow > 0) {
                    int nr = cursorRow - 1, nc = cursorCol;
                    clampCursorToValid(nr, nc);
                    cursorRow = nr; 
                    cursorCol = nc;
                }
            }
            else if (key == KeyCodes::ARROW_DOWN) {
                if (cursorRow < UIConfig::GRID_ROWS - 1) {
                    int nr = cursorRow + 1, nc = cursorCol;
                    clampCursorToValid(nr, nc);
                    if (nr * UIConfig::GRID_COLS + nc < (int)ALPHABET.size()) {
                        cursorRow = nr; 
                        cursorCol = nc;
                    }
                }
            }
            else if (key == KeyCodes::ARROW_LEFT) {
                if (cursorCol > 0) {
                    cursorCol--;
                } else if (cursorRow > 0) {
                    cursorRow--;
                    cursorCol = UIConfig::GRID_COLS - 1;
                    clampCursorToValid(cursorRow, cursorCol);
                }
            }
            else if (key == KeyCodes::ARROW_RIGHT) {
                int nextIndex = cursorRow * UIConfig::GRID_COLS + (cursorCol + 1);
                if (nextIndex < (int)ALPHABET.size()) {
                    cursorCol++;
                } else if (cursorRow < UIConfig::GRID_ROWS - 1) {
                    int nr = cursorRow + 1, nc = 0;
                    clampCursorToValid(nr, nc);
                    if (nr * UIConfig::GRID_COLS + nc < (int)ALPHABET.size()) {
                        cursorRow = nr; 
                        cursorCol = nc;
                    }
                }
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            int index = cursorRow * UIConfig::GRID_COLS + cursorCol;

            if (index < (int)ALPHABET.size()) {
                string choice = ALPHABET[index];

                if (choice == "OK") {
                    int nameLen = currentName.length();
                    if (nameLen >= GameplayConfig::PLAYER_NAME_MIN_LENGTH && 
                        nameLen <= GameplayConfig::PLAYER_NAME_MAX_LENGTH) {
                        finished = true;
                    } else {
                        cout << "\nName must be 2-7 letters long!";
                        Sleep(UIConfig::LONG_PAUSE);
                    }
                }
                else if ((int)currentName.length() < GameplayConfig::PLAYER_NAME_MAX_LENGTH) {
                    currentName += choice;
                }
            }
        }
        else if (key == KeyCodes::KEY_BACKSPACE && !currentName.empty()) {
            currentName.pop_back();
        }
    }

    player.username = currentName;
    clearScreen();

    string confirmMsg = "So it's '" + player.username + "'!";
    cout << "+" << string(confirmMsg.length() + 2, '-') << "+\n";
    cout << "| " << confirmMsg << " |\n";
    cout << "+" << string(confirmMsg.length() + 2, '-') << "+\n\n";
    pressAnyKey();
    clearScreen();

    string nextMsg = "So you're " + player.username + " who's moving to LITTLEROOT!";
    cout << "+" << string(nextMsg.length() + 2, '-') << "+\n";
    cout << "| ";
    typeText(nextMsg, 25);
    cout << " |\n";
    cout << "+" << string(nextMsg.length() + 2, '-') << "+\n\n";
    pressAnyKey();
    clearScreen();

    for (int i = 15; i < numLines; i++) {
        showDialogue(birchIntro[i], 40, 0, false);
        pressAnyKey();
        clearScreen();
    }
}

void drawAlphabetGrid(int cursorRow, int cursorCol, const string &currentName) {      
    // Draw alphabet grid
    for (int r = 0; r < UIConfig::GRID_ROWS; r++) {
        for (int c = 0; c < UIConfig::GRID_COLS; c++) {
            int index = r * UIConfig::GRID_COLS + c;
            if (index >= (int)ALPHABET.size()) break;
            
            if (r == cursorRow && c == cursorCol) {
                cout << ">" << ALPHABET[index] << "\t";
            } else {
                cout << " " << ALPHABET[index] << "\t";
            }
        }
        cout << endl;
    }
    
    cout << "ARROW KEYS - Move" << endl;
    cout << "ENTER      - Select Letter" << endl;
    cout << "BACKSPACE  - Delete Selected Letter" << endl;
    cout << "OK         - Set Name" << endl;
    cout << "YOUR NAME: " << currentName << endl;
}

/*****************************************************************************
 * CHOOSE STARTER MENU
 *****************************************************************************/
int chooseStarterMenu(const vector<Pokemon>& starters) {
    int cursor = 0;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    size_t maxLen = 0;
    for (const auto &p : starters) {
        if (p.pokemonName.length() > maxLen) maxLen = p.pokemonName.length();
    }

    while (true) {
        clearScreen();

        // Header
        cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH + 2, '-') << "+\n";
        string prompt = "CHOOSE YOUR STARTER POKEMON:";
        cout << "| " << prompt << string(UIConfig::STANDARD_BOX_WIDTH - prompt.length(), ' ') << " |\n";
        cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH + 2, '-') << "+\n\n";

        // List starters
        for (int i = 0; i < (int)starters.size(); i++) {
            string text = starters[i].pokemonName + string(maxLen - starters[i].pokemonName.length(), ' ');
            drawHighlight(h, text, (cursor == i));

            int pad = UIConfig::STANDARD_BOX_WIDTH - maxLen;
            cout << string(max(1, pad), ' ') << "\n";
        }

        int key = _getch();

        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            if (key == KeyCodes::ARROW_UP) {
                cursor = (cursor - 1 + starters.size()) % starters.size();
            }
            else if (key == KeyCodes::ARROW_DOWN) {
                cursor = (cursor + 1) % starters.size();
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            return cursor;
        }
    }
}

/*****************************************************************************
 * POKEBALL CATCHING MECHANICS
 *****************************************************************************/
int throwPokeball(const Pokemon& target) {
    double hpRatio = static_cast<double>(target.currentHP) / target.baseHP;
    int catchChance = 0;
    
    // Determine catch chance based on HP
    if (hpRatio <= GameplayConfig::HP_CRITICAL_THRESHOLD) {
        catchChance = GameplayConfig::CATCH_CHANCE_CRITICAL;
    }
    else if (hpRatio <= GameplayConfig::HP_HALF_THRESHOLD) {
        catchChance = GameplayConfig::CATCH_CHANCE_HALF;
    }
    else if (hpRatio <= GameplayConfig::HP_WEAKENED_THRESHOLD) {
        catchChance = GameplayConfig::CATCH_CHANCE_WEAKENED;
    }
    else {
        catchChance = GameplayConfig::CATCH_CHANCE_HEALTHY;
    }

    int roll = rand() % 100 + 1;

    if (roll <= catchChance) {
        return CatchResult::SUCCESS;
    } 
    else if (roll <= catchChance + GameplayConfig::CATCH_ALMOST_BONUS) {
        return CatchResult::ALMOST;
    } 
    else {
        return CatchResult::FAILED;
    }
}

void displayThrowPokeball(Player& player, Pokemon& playerPokemon, Pokemon& target, int result) {
    clearScreen();
    drawBattleFrame(playerPokemon, target);
    showBattleDialogue(player.username + " threw a POKEBALL!");
    Sleep(UIConfig::LONG_PAUSE);

    cout << "\nThe ball shakes once...\n";
    Sleep(UIConfig::POKEBALL_SHAKE_DELAY);
    cout << "The ball shakes twice...\n";
    Sleep(UIConfig::POKEBALL_SHAKE_DELAY);
    cout << "The ball shakes three times...\n";
    Sleep(UIConfig::POKEBALL_FINAL_DELAY);

    clearScreen();
    drawBattleFrame(playerPokemon, target);

    switch (result) {
        case CatchResult::FAILED:
            showBattleDialogue("Aww, I thought I had it...");
            break;
        case CatchResult::ALMOST:
            showBattleDialogue("Shoot! Almost got it!");
            break;
        case CatchResult::SUCCESS: {
            string caughtText = "Gotcha! " + target.pokemonName + " was caught!";
            showBattleDialogue(caughtText);
            break;
        }
    }

    pressAnyKey();
}

/*****************************************************************************
 * POKEMON STAT SUMMARY
 *****************************************************************************/
void displayPokemonSummary(const Pokemon& pkmn) {
    clearScreen();

    string header = "POKEMON SUMMARY";

    // Draw header box
    cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n";
    cout << "| " << header << string(UIConfig::STANDARD_BOX_WIDTH - (int)header.size(), ' ') << "|\n";
    cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n\n";

    // Core summary
    cout << "NAME:       " << pkmn.pokemonName << "\n";
    
    // Type display
    cout << "TYPE:       " << getTypeName(pkmn.primaryType);
    if (pkmn.secondaryType != PokemonType::NONE) {
        cout << " / " << getTypeName(pkmn.secondaryType);
    }
    cout << "\n";
    
    cout << "HP:         " << pkmn.currentHP << " / " << pkmn.baseHP << "\n";
    cout << "ATTACK:     " << pkmn.baseAttack << "\n";
    cout << "DEFENSE:    " << pkmn.baseDefense << "\n";
    cout << "SP. ATTACK: " << pkmn.baseSPAttack << "\n";
    cout << "SP. DEFENSE:" << pkmn.baseSPDefense << "\n";
    cout << "SPEED:      " << pkmn.baseSpeed << "\n\n";

    cout << "Press any key to return...\n";
    _getch();
}

/*****************************************************************************
 * POKEMON ACTION MENU (SWITCH / SUMMARY)
 *****************************************************************************/
int pokemonActionMenu(const Pokemon &selected) {
    const vector<string> options = {"SWITCH", "SUMMARY"};
    int cursor = 0;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    size_t maxLen = 0;
    for (const auto &o : options) {
        if (o.length() > maxLen) maxLen = o.length();
    }

    while (true) {
        clearScreen();

        // Header
        string header = "WHAT WILL " + selected.pokemonName + " DO?";
        cout << "+" << string(header.length() + 2, '-') << "+\n";
        cout << "| " << header << " |\n";
        cout << "+" << string(header.length() + 2, '-') << "+\n\n";

        // Draw options
        for (int i = 0; i < (int)options.size(); ++i) {
            string text = options[i] + string(maxLen - options[i].length(), ' ');
            drawHighlight(h, text, (cursor == i));
            cout << "\n";
        }

        // Controls
        cout << "\nARROW KEYS  -  MOVE\n";
        cout << "ENTER       -  SELECT\n";
        cout << "ESC         -  BACK\n";

        int key = _getch();
        
        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            if (key == KeyCodes::ARROW_UP && cursor > 0) {
                cursor--;
            }
            else if (key == KeyCodes::ARROW_DOWN && cursor < (int)options.size() - 1) {
                cursor++;
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            return cursor;
        }
        else if (key == KeyCodes::KEY_ESC) {
            return -1;
        }
    }
}

/*****************************************************************************
 * POKEMON PARTY SELECTION MENU
 *****************************************************************************/
int pokemonSelectionMenu(Player &player, bool inBattle) {
    int cursor = 0;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    while (true) {
        clearScreen();

        // Header
        cout << "+" << string(40, '-') << "+\n";
        cout << "| CHOOSE A POKEMON:" << string(22, ' ') << "|\n";
        cout << "+" << string(40, '-') << "+\n\n";

        // Compute max display length
        size_t maxLen = 0;
        for (const auto &p : player.currentPokemon) {
            string disp = p.pokemonName + " (Lv." + to_string(p.level) + ")";
            if (p.currentHP <= 0) disp += " (FAINTED)";
            if (disp.length() > maxLen) maxLen = disp.length();
        }

        // List Pokémon
        for (int i = 0; i < (int)player.currentPokemon.size(); ++i) {
            const Pokemon &p = player.currentPokemon[i];

            string display = p.pokemonName + " (Lv." + to_string(p.level) + ")";
            if (p.currentHP <= 0) display += " (FAINTED)";
            display += string(maxLen - display.length(), ' ');

            drawHighlight(h, display, (cursor == i));
            cout << "\n";
        }

        int key = _getch();

        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            if (key == KeyCodes::ARROW_UP && cursor > 0) {
                cursor--;
            }
            else if (key == KeyCodes::ARROW_DOWN && cursor < (int)player.currentPokemon.size() - 1) {
                cursor++;
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            const Pokemon &selected = player.currentPokemon[cursor];

            // Cannot switch to fainted mon in battle
            if (inBattle && selected.currentHP <= 0) {
                showBattleDialogue(selected.pokemonName + " has fainted! Pick another.");
                pressAnyKey();
                continue;
            }

            int action = pokemonActionMenu(selected);

            // SWITCH
            if (action == 0) {
                if (inBattle) {
                    // Prevent switching to the same mon (index 0 is active)
                    if (cursor == 0) {
                        showBattleDialogue("It's already in battle!");
                        pressAnyKey();
                        continue;
                    }
                    return cursor;
                }

                // Out-of-battle switching
                clearScreen();
                showDialogue("Select a POKEMON to switch with:");
                int switchIndex = 0;

                while (true) {
                    clearScreen();

                    size_t maxLen2 = 0;
                    for (const auto &p2 : player.currentPokemon) {
                        string d2 = p2.pokemonName + " (Lv." + to_string(p2.level) + ")";
                        if (p2.currentHP <= 0) d2 += " (FAINTED)";
                        if (d2.length() > maxLen2) maxLen2 = d2.length();
                    }

                    for (int i = 0; i < (int)player.currentPokemon.size(); ++i) {
                        const Pokemon &p2 = player.currentPokemon[i];

                        string display2 = p2.pokemonName + " (Lv." + to_string(p2.level) + ")";
                        if (p2.currentHP <= 0) display2 += " (FAINTED)";
                        display2 += string(maxLen2 - display2.length(), ' ');

                        drawHighlight(h, display2, (i == switchIndex));
                        cout << "\n";
                    }

                    int key2 = _getch();

                    if (key2 == KeyCodes::ARROW_PREFIX_1 || key2 == KeyCodes::ARROW_PREFIX_2) {
                        key2 = _getch();
                        if (key2 == KeyCodes::ARROW_UP && switchIndex > 0) {
                            switchIndex--;
                        }
                        else if (key2 == KeyCodes::ARROW_DOWN && switchIndex < (int)player.currentPokemon.size() - 1) {
                            switchIndex++;
                        }
                    }
                    else if (key2 == KeyCodes::KEY_ENTER) {
                        if (switchIndex == cursor) {
                            showBattleDialogue("That's already in position!");
                            pressAnyKey();
                        } else {
                            swap(player.currentPokemon[cursor], player.currentPokemon[switchIndex]);
                            clearScreen();
                            showBattleDialogue("Switched POKEMON positions!");
                            pressAnyKey();
                        }
                        break;
                    }
                    else if (key2 == KeyCodes::KEY_ESC) {
                        break;
                    }
                }
            }
            // SUMMARY
            else if (action == 1) {
                displayPokemonSummary(player.currentPokemon[cursor]);
            }
        }
        // ESC: go back
        else if (key == KeyCodes::KEY_ESC) {
            return -1;
        }
    }
}

/*****************************************************************************
 * QUANTITY SELECTION MENU
 *****************************************************************************/
int selectItemQuantity(const ShopItem& item, int playerMoney) {
    int quantity = 1;
    int maxAffordable = playerMoney / item.price;
    
    if (maxAffordable <= 0) {
        showDialogue("You don't have enough money!");
        pressAnyKey();
        return 0;
    }
    
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    
    while (true) {
        clearScreen();
        
        // Header
        cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n";
        string header = "HOW MANY " + item.name + "?";
        cout << "| " << header << string(UIConfig::STANDARD_BOX_WIDTH - header.length(), ' ') << "|\n";
        cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n\n";
        
        // Item info
        cout << "ITEM:       " << item.name << "\n";
        cout << "PRICE:      " << item.price << " POKEDOLLARS\n";
        cout << "YOUR MONEY: " << playerMoney << " POKEDOLLARS\n\n";
        
        // Quantity display
        string qtyDisplay = item.name + " X" + to_string(quantity);
        drawHighlight(h, qtyDisplay, true);
        cout << "\n\n";
        
        // Cost calculation
        int totalCost = item.price * quantity;
        cout << "TOTAL COST: " << totalCost << " POKEDOLLARS\n";
        
        if (totalCost > playerMoney) {
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "NOT ENOUGH MONEY!\n";
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        
        cout << "\n";
        cout << "UP/DOWN  -  ADJUST QUANTITY\n";
        cout << "ENTER    -  CONFIRM\n";
        cout << "ESC      -  CANCEL\n";
        
        int key = _getch();
        
        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            
            if (key == KeyCodes::ARROW_UP) {
                if (quantity < maxAffordable && quantity < GameplayConfig::MAX_ITEM_QUANTITY) {
                    quantity++;
                }
            }
            else if (key == KeyCodes::ARROW_DOWN) {
                if (quantity > 1) {
                    quantity--;
                }
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            int totalCost = item.price * quantity;
            if (totalCost <= playerMoney) {
                return quantity;
            }
        }
        else if (key == KeyCodes::KEY_ESC) {
            return 0;
        }
    }
}

/*****************************************************************************
 * POKEMART SHOP MENU
 *****************************************************************************/
void openPokemart(Player& player) {
    vector<ShopItem> shopInventory = getPokemartInventory();
    int cursor = 0;
    
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Find max item name length for padding
    size_t maxLen = 0;
    for (const auto& item : shopInventory) {
        if (item.name.length() > maxLen) {
            maxLen = item.name.length();
        }
    }
    
    while (true) {
        clearScreen();
        
        // Header
        cout << "+" << string(UIConfig::WIDE_BOX_WIDTH, '-') << "+\n";
        cout << "| WELCOME TO THE POKEMART!" << string(UIConfig::WIDE_BOX_WIDTH - 25, ' ') << "|\n";
        cout << "+" << string(UIConfig::WIDE_BOX_WIDTH, '-') << "+\n\n";
        
        // Money display
        cout << "YOUR MONEY: " << player.money << " POKEDOLLARS\n\n";
        
        // Shop items
        for (int i = 0; i < (int)shopInventory.size(); ++i) {
            const ShopItem& item = shopInventory[i];
            
            string display = item.name;
            display += string(maxLen - item.name.length() + 3, ' ');
            display += to_string(item.price) + " P";
            
            drawHighlight(h, display, cursor == i);
            cout << "\n";
        }
        
        // Exit option
        cout << "\n";
        drawHighlight(h, "EXIT", cursor == (int)shopInventory.size());
        cout << "\n\n";
        
        cout << "UP/DOWN  -  NAVIGATE\n";
        cout << "ENTER    -  SELECT\n";
        
        int key = _getch();
        
        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            
            if (key == KeyCodes::ARROW_UP) {
                cursor = (cursor - 1 + shopInventory.size() + 1) % (shopInventory.size() + 1);
            }
            else if (key == KeyCodes::ARROW_DOWN) {
                cursor = (cursor + 1) % (shopInventory.size() + 1);
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            // Exit option
            if (cursor == (int)shopInventory.size()) {
                showDialogue("Thank you! Please come again!");
                pressAnyKey();
                return;
            }
            
            // Item selected
            const ShopItem& selectedItem = shopInventory[cursor];
            
            // Select quantity
            int quantity = selectItemQuantity(selectedItem, player.money);
            
            if (quantity > 0) {
                // Confirm purchase
                clearScreen();
                int totalCost = selectedItem.price * quantity;
                
                string confirmMsg = "Buy " + to_string(quantity) + " " + 
                                   selectedItem.name + " for " + 
                                   to_string(totalCost) + " POKEDOLLARS?";
                
                if (yesNoMenu(confirmMsg, true)) {
                    // Process purchase
                    player.money -= totalCost;
                    
                    // Add to inventory
                    bool found = false;
                    for (auto& item : player.items) {
                        if (item.itemName == selectedItem.name) {
                            item.quantity += quantity;
                            found = true;
                            break;
                        }
                    }
                    
                    if (!found) {
                        player.items.push_back(Item(
                            selectedItem.name,
                            selectedItem.type,
                            selectedItem.healAmount,
                            quantity
                        ));
                    }
                    
                    clearScreen();
                    string successMsg = "Obtained " + selectedItem.name + " X" + to_string(quantity) + "!";
                    showDialogue(successMsg);
                    pressAnyKey();
                }
            }
        }
        else if (key == KeyCodes::KEY_ESC) {
            showDialogue("Thank you! Please come again!");
            pressAnyKey();
            return;
        }
    }
}

/*****************************************************************************
 * POKECENTER HEALING
 *****************************************************************************/
void healAtPokecenter(Player& player, const vector<Move>& moveList) {
    // Healing animation
    clearScreen();
    cout << "\n\n";
    cout << "       Healing your POKEMON";
    
    for (int i = 0; i < 3; ++i) {
        cout << ".";
        cout.flush();
        Sleep(UIConfig::HEALING_DOT_DELAY);
    }
    
    cout << "\n\n";
    Sleep(UIConfig::SHORT_PAUSE);
    
    // Heal all Pokemon (HP + PP)
    for (auto& pokemon : player.currentPokemon) {
        // Restore HP
        pokemon.currentHP = pokemon.baseHP;
        
        // Restore PP for all moves
        for (int i = 0; i < (int)pokemon.moveID.size(); ++i) {
            int moveID = pokemon.moveID[i];
            if (moveID > 0 && moveID <= (int)moveList.size()) {
                const Move& move = moveList[moveID - 1];
                pokemon.currentPP[i] = move.pp;
            }
        }
    }
    
    clearScreen();
    showDialogue("Your POKEMON are fully healed!");
    pressAnyKey();
    clearScreen();
    showDialogue("We hope to see you again!");
    pressAnyKey();
}

/*****************************************************************************
 * POKEDEX: GENERATE ENTRIES
 *****************************************************************************/
vector<PokedexEntry> generatePokedex(const Player& player, const PokemonDatabase& db) {
    vector<PokedexEntry> pokedexList;
    
    for (const auto& pkmn : db.pokedex) {
        PokedexEntry entry;
        entry.pokedexNum = pkmn.pokemonIndex;
        entry.pokemonName = pkmn.pokemonName;
        
        // Check if encountered
        entry.encountered = (find(player.encounteredPokemon.begin(), 
                                  player.encounteredPokemon.end(), 
                                  pkmn.pokemonName) != player.encounteredPokemon.end());
        
        // Check if owned
        entry.owned = false;
        for (const auto& playerPkmn : player.currentPokemon) {
            if (playerPkmn.pokemonName == pkmn.pokemonName) {
                entry.owned = true;
                break;
            }
        }
        
        pokedexList.push_back(entry);
    }
    
    sort(pokedexList.begin(), pokedexList.end(), 
         [](const PokedexEntry& a, const PokedexEntry& b) {
             return a.pokedexNum < b.pokedexNum;
         });
    
    return pokedexList;
}

/*****************************************************************************
 * POKEDEX: DISPLAY PAGE
 *****************************************************************************/
void displayPokedexPage(const vector<PokedexEntry>& pokedexList, int page, int entriesPerPage, int cursor) {
    clearScreen();
    
    cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n";
    cout << "| POKEDEX" << string(UIConfig::STANDARD_BOX_WIDTH - 8, ' ') << "|\n";
    cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n\n";
    
    int startIdx = page * entriesPerPage;
    int endIdx = min(startIdx + entriesPerPage, (int)pokedexList.size());
    
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    
    for (int i = startIdx; i < endIdx; ++i) {
        const PokedexEntry& entry = pokedexList[i];
        
        string display;
        if (entry.encountered || entry.owned) {
            display = to_string(entry.pokedexNum) + ". " + entry.pokemonName;
            if (entry.owned) {
                display += " (OWNED)";
            }
        } else {
            display = to_string(entry.pokedexNum) + ". ??? (UNDISCOVERED)";
        }
        
        bool isHighlighted = (i == startIdx + cursor);
        drawHighlight(h, display, isHighlighted);
        cout << "\n";
    }
    
    cout << "\n";
    
    int totalPages = (pokedexList.size() + entriesPerPage - 1) / entriesPerPage;
    cout << "PAGE " << (page + 1) << " / " << totalPages << "\n\n";
    
    cout << "UP/DOWN     -  NAVIGATE\n";
    cout << "LEFT/RIGHT  -  CHANGE PAGE\n";
    cout << "ENTER       -  VIEW DETAILS\n";
    cout << "ESC         -  BACK\n";
}
/*****************************************************************************
 * POKEDEX: MAIN INTERFACE
 *****************************************************************************/
void openPokedex(Player& player, const PokemonDatabase& db) {
    vector<PokedexEntry> pokedexList = generatePokedex(player, db);
    
    if (pokedexList.empty()) {
        showDialogue("POKEDEX is empty!");
        pressAnyKey();
        return;
    }
    
    int totalPages = (pokedexList.size() + GameplayConfig::POKEDEX_ENTRIES_PER_PAGE - 1) / GameplayConfig::POKEDEX_ENTRIES_PER_PAGE;
    int currentPage = 0;
    int cursor = 0;
    
    while (true) {
        displayPokedexPage(pokedexList, currentPage, GameplayConfig::POKEDEX_ENTRIES_PER_PAGE, cursor);
        
        int key = _getch();
        
        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            
            int startIdx = currentPage * GameplayConfig::POKEDEX_ENTRIES_PER_PAGE;
            int endIdx = min(startIdx + GameplayConfig::POKEDEX_ENTRIES_PER_PAGE, (int)pokedexList.size());
            int pageSize = endIdx - startIdx;
            
            switch (key) {
                case KeyCodes::ARROW_UP:
                    if (cursor > 0) cursor--;
                    break;
                case KeyCodes::ARROW_DOWN:
                    if (cursor < pageSize - 1) cursor++;
                    break;
                case KeyCodes::ARROW_LEFT:
                    if (currentPage > 0) {
                        currentPage--;
                        cursor = 0;
                    }
                    break;
                case KeyCodes::ARROW_RIGHT:
                    if (currentPage < totalPages - 1) {
                        currentPage++;
                        cursor = 0;
                    }
                    break;
            }
        }
        
        else if (key == KeyCodes::KEY_ENTER) {
		    int selectedIdx = currentPage * GameplayConfig::POKEDEX_ENTRIES_PER_PAGE + cursor;
		    const PokedexEntry& entry = pokedexList[selectedIdx];
		    
		    if (!entry.encountered && !entry.owned) {
		        clearScreen();
		        showDialogue("This POKEMON has not been encountered!");
		        pressAnyKey();
		    } else if (entry.encountered && !entry.owned) {
		        clearScreen();
		        showDialogue("You haven't caught this POKEMON yet!");
		        pressAnyKey();
		    } else {
		        // entry.owned == true
		        bool found = false;
		        for (const auto& pkmn : player.currentPokemon) {
		            if (pkmn.pokemonName == entry.pokemonName) {
		                displayPokemonSummary(pkmn);
		                found = true;
		                break;
		            }
		        }
		        if (!found) {
		            clearScreen();
		            showDialogue("POKEMON currently not in your party!");
		            pressAnyKey();
		        }
		    }
		}
        
        else if (key == KeyCodes::KEY_ESC) {
            return;
        }
    }
}

/*****************************************************************************
 * BAG MENU (OUT OF BATTLE)
 *****************************************************************************/
void openBagMenuFromMain(Player& player) {
    if (player.items.empty()) {
        showDialogue("Your BAG is empty!");
        pressAnyKey();
        return;
    }
    
    int cursor = 0;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    
    size_t maxLen = 0;
    for (const auto& item : player.items) {
        if (item.itemName.length() > maxLen) {
            maxLen = item.itemName.length();
        }
    }
    
    while (true) {
        clearScreen();
        
        cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n";
        cout << "| BAG" << string(UIConfig::STANDARD_BOX_WIDTH - 4, ' ') << "|\n";
        cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n\n";
        
        for (int i = 0; i < (int)player.items.size(); ++i) {
            const Item& item = player.items[i];
            
            string display = item.itemName;
            display += string(maxLen - item.itemName.length() + 3, ' ');
            display += "X" + to_string(item.quantity);
            
            drawHighlight(h, display, cursor == i);
            cout << "\n";
        }
        
        cout << "\n";
        cout << "UP/DOWN  -  NAVIGATE\n";
        cout << "ENTER    -  USE ITEM\n";
        cout << "ESC      -  BACK\n";
        
        int key = _getch();
        
        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            
            if (key == KeyCodes::ARROW_UP && cursor > 0) {
                cursor--;
            }
            else if (key == KeyCodes::ARROW_DOWN && cursor < (int)player.items.size() - 1) {
                cursor++;
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            clearScreen();
            showDialogue("There's a better time and place to do that!");
            pressAnyKey();
        }
        else if (key == KeyCodes::KEY_ESC) {
            return;
        }
    }
}

/*****************************************************************************
 * PROFILE / TRAINER CARD
 *****************************************************************************/
void displayProfile(const Player& player, const PokemonDatabase& db) {
    clearScreen();
    
    cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n";
    cout << "| TRAINER CARD" << string(UIConfig::STANDARD_BOX_WIDTH - 13, ' ') << "|\n";
    cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n\n";
    
    cout << "NAME:     " << player.username << "\n";
    
    /*// Champion status
    if (flagWallaceDefeated) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "TITLE:    POKEMON CHAMPION"<<endl;
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }*/
    
    int discovered = 0;
    for (const auto& dbPkmn : db.pokedex) {
        for (const auto& playerPkmn : player.currentPokemon) {
            if (playerPkmn.pokemonName == dbPkmn.pokemonName) {
                discovered++;
                break;
            }
        }
    }
    
    cout << "POKEDEX:  " << discovered << " / " << db.pokedex.size() << "\n";
    cout << "MONEY:    " << player.money << " P\n";
    cout << "BADGES:   " << player.getBadgeCount() << " / " << GameplayConfig::TOTAL_BADGES << "\n\n";
    
    const char* badgeNames[GameplayConfig::TOTAL_BADGES] = {
        "STONE BADGE",
        "KNUCKLE BADGE",
        "DYNAMO BADGE",
        "HEAT BADGE",
        "BALANCE BADGE",
        "FEATHER BADGE",
        "MIND BADGE",
        "RAIN BADGE"
    };
    
    for (int i = 0; i < GameplayConfig::TOTAL_BADGES; i++) {
        cout << "  " << badgeNames[i] << ":";
        int pad = 16 - strlen(badgeNames[i]);
        cout << string(pad, ' ');
        cout << (player.badges[i] ? "[OBTAINED]" : "[        ]") << "\n";
    }
    
    pressAnyKey();
}

/*****************************************************************************
 * CONTROLS SCREEN
 *****************************************************************************/
void displayControls() {
    clearScreen();
    
    cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n";
    cout << "| CONTROLS" << string(UIConfig::STANDARD_BOX_WIDTH - 9, ' ') << "|\n";
    cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n\n";
    
    cout << "MENU NAVIGATION:\n";
    cout << "  ARROW KEYS  -  NAVIGATE MENUS\n";
    cout << "  ENTER       -  SELECT / CONFIRM\n";
    cout << "  ESC         -  BACK / CANCEL\n\n";
    
    cout << "MAP CONTROLS:\n";
    cout << "  W       -  MOVE UP\n";
    cout << "  S       -  MOVE DOWN\n";
    cout << "  A       -  MOVE LEFT\n";
    cout << "  D       -  MOVE RIGHT\n";
    cout << "  M       -  OPEN MAIN MENU\n\n";
    
    cout << "BATTLE CONTROLS:\n";
    cout << "  ARROW KEYS  -  NAVIGATE OPTIONS\n";
    cout << "  ENTER       -  SELECT ACTION\n";
    cout << "  ESC         -  BACK (MOVE SELECTION)\n\n";
    
    pressAnyKey();
}

/*****************************************************************************
 * MAIN MENU
 *****************************************************************************/
bool openMainMenu(Player& player, const PokemonDatabase& db) {
    const vector<string> options = {
        "POKEDEX",
        "POKEMON",
        "BAG",
        "PROFILE",
        "CONTROLS",
        "EXIT"
    };
    
    int cursor = 0;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    
    size_t maxLen = 0;
    for (const auto& opt : options) {
        if (opt.length() > maxLen) {
            maxLen = opt.length();
        }
    }
    
    while (true) {
        clearScreen();
        
        cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n";
        cout << "| MAIN MENU" << string(UIConfig::STANDARD_BOX_WIDTH - 10, ' ') << "|\n";
        cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n\n";
        
        for (int i = 0; i < (int)options.size(); ++i) {
            string text = options[i] + string(maxLen - options[i].length(), ' ');
            drawHighlight(h, text, cursor == i);
            cout << "\n";
        }
        
        cout << "\n";
        cout << "UP/DOWN  -  NAVIGATE\n";
        cout << "ENTER    -  SELECT\n";
        cout << "ESC/M    -  CLOSE MENU\n";
        
        int key = _getch();
        
        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            
            if (key == KeyCodes::ARROW_UP && cursor > 0) {
                cursor--;
            }
            else if (key == KeyCodes::ARROW_DOWN && cursor < (int)options.size() - 1) {
                cursor++;
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            switch (cursor) {
                case 0: // POKEDEX
                    openPokedex(player, db);
                    break;
                case 1: // POKEMON
                    pokemonSelectionMenu(player, false);
                    break;
                case 2: // BAG
                    openBagMenuFromMain(player);
                    break;
                case 3: // PROFILE
                    displayProfile(player, db);
                    break;
                case 4: // CONTROLS
                    displayControls();
                    break;
                case 5: // EXIT
                    clearScreen();
                    if (yesNoMenu("Are you sure you want to exit?", false)) {
                        return true;
                    }
                    break;
            }
        }
        else if (key == KeyCodes::KEY_ESC || key == KeyCodes::KEY_M_LOWER || key == KeyCodes::KEY_M_UPPER) {
            return false;
        }
    }
}

/*****************************************************************************
 * PC ACTION MENU
 *****************************************************************************/
int pcActionMenu() {
    const vector<string> options = {"WITHDRAW POKEMON", "DEPOSIT POKEMON", "MOVE POKEMON", "SEE YA!"};
    int cursor = 0;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    
    size_t maxLen = 0;
    for (const auto& opt : options) {
        if (opt.length() > maxLen) maxLen = opt.length();
    }
    
    while (true) {
        clearScreen();
        
        cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n";
        cout << "| SOMEONE'S PC" << string(UIConfig::STANDARD_BOX_WIDTH - 13, ' ') << "|\n";
        cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n\n";
        
        for (int i = 0; i < (int)options.size(); ++i) {
            string text = options[i] + string(maxLen - options[i].length(), ' ');
            drawHighlight(h, text, cursor == i);
            cout << "\n";
        }
        
        cout << "\n";
        cout << "UP/DOWN  -  NAVIGATE\n";
        cout << "ENTER    -  SELECT\n";
        cout << "ESC      -  BACK\n";
        
        int key = _getch();
        
        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            if (key == KeyCodes::ARROW_UP && cursor > 0) {
                cursor--;
            }
            else if (key == KeyCodes::ARROW_DOWN && cursor < (int)options.size() - 1) {
                cursor++;
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            return cursor;
        }
        else if (key == KeyCodes::KEY_ESC) {
            return 3;
        }
    }
}

/*****************************************************************************
 * DISPLAY BOX POKEMON LIST
 *****************************************************************************/
void displayBoxList(int cursor, bool forWithdraw) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    
    clearScreen();
    
    cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n";
    string header = forWithdraw ? "WITHDRAW FROM BOX 1" : "BOX 1";
    cout << "| " << header << string(UIConfig::STANDARD_BOX_WIDTH - header.length() - 1, ' ') << "|\n";
    cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n\n";
    
    if (pcBox.empty()) {
        cout << "  (Empty)\n\n";
    } else {
        size_t maxLen = 0;
        for (const auto& p : pcBox) {
            string disp = p.pokemonName + " (Lv." + to_string(p.level) + ")";
            if (disp.length() > maxLen) maxLen = disp.length();
        }
        
        for (int i = 0; i < (int)pcBox.size(); ++i) {
            const Pokemon& p = pcBox[i];
            string display = p.pokemonName + " (Lv." + to_string(p.level) + ")";
            display += string(maxLen - display.length(), ' ');
            
            drawHighlight(h, display, cursor == i);
            cout << "\n";
        }
        cout << "\n";
    }
    
    cout << "(" << pcBox.size() << "/" << GameplayConfig::PC_BOX_MAX << ")\n\n";
    cout << "UP/DOWN  -  NAVIGATE\n";
    cout << "ENTER    -  SELECT\n";
    cout << "ESC      -  BACK\n";
}

/*****************************************************************************
 * DISPLAY PARTY FOR DEPOSIT/MOVE
 *****************************************************************************/
void displayPartyForPC(Player& player, int cursor, const string& action) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    
    clearScreen();
    
    cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n";
    string header = action + " - SELECT POKEMON";
    cout << "| " << header << string(UIConfig::STANDARD_BOX_WIDTH - header.length() - 1, ' ') << "|\n";
    cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n\n";
    
    size_t maxLen = 0;
    for (const auto& p : player.currentPokemon) {
        string disp = p.pokemonName + " (Lv." + to_string(p.level) + ")";
        if (disp.length() > maxLen) maxLen = disp.length();
    }
    
    for (int i = 0; i < (int)player.currentPokemon.size(); ++i) {
        const Pokemon& p = player.currentPokemon[i];
        string display = p.pokemonName + " (Lv." + to_string(p.level) + ")";
        display += string(maxLen - display.length(), ' ');
        
        drawHighlight(h, display, cursor == i);
        cout << "\n";
    }
    
    cout << "\n";
    cout << "PARTY: " << player.currentPokemon.size() << "/" << GameplayConfig::PARTY_MAX << "\n";
    cout << "BOX:   " << pcBox.size() << "/" << GameplayConfig::PC_BOX_MAX << "\n\n";
    cout << "UP/DOWN  -  NAVIGATE\n";
    cout << "ENTER    -  SELECT\n";
    cout << "ESC      -  BACK\n";
}

/*****************************************************************************
 * WITHDRAW POKEMON
 *****************************************************************************/
void withdrawPokemon(Player& player) {
    if (pcBox.empty()) {
        clearScreen();
        showDialogue("The box is empty!");
        pressAnyKey();
        return;
    }
    
    if (player.currentPokemon.size() >= GameplayConfig::PARTY_MAX) {
        clearScreen();
        showDialogue("Your party is full! Deposit a POKEMON first.");
        pressAnyKey();
        return;
    }
    
    int cursor = 0;
    
    while (true) {
        displayBoxList(cursor, true);
        
        int key = _getch();
        
        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            if (key == KeyCodes::ARROW_UP && cursor > 0) {
                cursor--;
            }
            else if (key == KeyCodes::ARROW_DOWN && cursor < (int)pcBox.size() - 1) {
                cursor++;
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            if (player.currentPokemon.size() >= GameplayConfig::PARTY_MAX) {
                clearScreen();
                showDialogue("Your party is full!");
                pressAnyKey();
                continue;
            }
            
            Pokemon selected = pcBox[cursor];
            player.currentPokemon.push_back(selected);
            pcBox.erase(pcBox.begin() + cursor);
            
            clearScreen();
            showDialogue("Withdrew " + selected.pokemonName + "!");
            pressAnyKey();
            
            if (cursor >= (int)pcBox.size()) {
                cursor = max(0, (int)pcBox.size() - 1);
            }
            
            if (pcBox.empty()) return;
        }
        else if (key == KeyCodes::KEY_ESC) {
            return;
        }
    }
}

/*****************************************************************************
 * DEPOSIT POKEMON
 *****************************************************************************/
void depositPokemon(Player& player) {
    if (player.currentPokemon.size() <= GameplayConfig::PARTY_MIN) {
        clearScreen();
        showDialogue("You can't deposit your last POKEMON!");
        pressAnyKey();
        return;
    }
    
    if ((int)pcBox.size() >= GameplayConfig::PC_BOX_MAX) {
        clearScreen();
        showDialogue("The box is full!");
        pressAnyKey();
        return;
    }
    
    int cursor = 0;
    
    while (true) {
        displayPartyForPC(player, cursor, "DEPOSIT");
        
        int key = _getch();
        
        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            if (key == KeyCodes::ARROW_UP && cursor > 0) {
                cursor--;
            }
            else if (key == KeyCodes::ARROW_DOWN && cursor < (int)player.currentPokemon.size() - 1) {
                cursor++;
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            if (player.currentPokemon.size() <= GameplayConfig::PARTY_MIN) {
                clearScreen();
                showDialogue("You can't deposit your last POKEMON!");
                pressAnyKey();
                continue;
            }
            
            if ((int)pcBox.size() >= GameplayConfig::PC_BOX_MAX) {
                clearScreen();
                showDialogue("The box is full!");
                pressAnyKey();
                continue;
            }
            
            Pokemon selected = player.currentPokemon[cursor];
            pcBox.push_back(selected);
            player.currentPokemon.erase(player.currentPokemon.begin() + cursor);
            
            clearScreen();
            showDialogue("Deposited " + selected.pokemonName + "!");
            pressAnyKey();
            
            if (cursor >= (int)player.currentPokemon.size()) {
                cursor = max(0, (int)player.currentPokemon.size() - 1);
            }
            
            if (player.currentPokemon.size() <= GameplayConfig::PARTY_MIN) return;
            if ((int)pcBox.size() >= GameplayConfig::PC_BOX_MAX) return;
        }
        else if (key == KeyCodes::KEY_ESC) {
            return;
        }
    }
}

/*****************************************************************************
 * MOVE POKEMON (SWAP BETWEEN PARTY AND BOX)
 *****************************************************************************/
void movePokemon(Player& player) {
    if (pcBox.empty() && player.currentPokemon.size() <= GameplayConfig::PARTY_MIN) {
        clearScreen();
        showDialogue("Nothing to move!");
        pressAnyKey();
        return;
    }
    
    int cursor = 0;
    bool inParty = true;
    Pokemon* selectedPokemon = nullptr;
    int selectedIndex = -1;
    bool selectedFromParty = false;
    
    while (true) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        const int boxWidth = 55;
        
        clearScreen();
        
        cout << "+" << string(boxWidth, '-') << "+\n";
        string header = selectedPokemon ? "SELECT DESTINATION" : "MOVE POKEMON";
        cout << "| " << header << string(boxWidth - header.length() - 1, ' ') << "|\n";
        cout << "+" << string(boxWidth, '-') << "+\n\n";
        
        if (selectedPokemon) {
            cout << "SELECTED: " << selectedPokemon->pokemonName << "\n\n";
        }
        
        // Party section
        cout << "--- PARTY (" << player.currentPokemon.size() << "/" << GameplayConfig::PARTY_MAX << ") ---\n";
        for (int i = 0; i < (int)player.currentPokemon.size(); ++i) {
            const Pokemon& p = player.currentPokemon[i];
            string display = p.pokemonName + " (Lv." + to_string(p.level) + ")";
            
            bool highlight = (inParty && cursor == i);
            bool isSelected = (selectedFromParty && selectedIndex == i);
            
            if (isSelected) display = "[" + display + "]";
            
            drawHighlight(h, display, highlight);
            cout << "\n";
        }
        
        cout << "\n--- BOX 1 (" << pcBox.size() << "/" << GameplayConfig::PC_BOX_MAX << ") ---\n";
        if (pcBox.empty()) {
            cout << "  (Empty)\n";
        } else {
            for (int i = 0; i < (int)pcBox.size(); ++i) {
                const Pokemon& p = pcBox[i];
                string display = p.pokemonName + " (Lv." + to_string(p.level) + ")";
                
                bool highlight = (!inParty && cursor == i);
                bool isSelected = (!selectedFromParty && selectedIndex == i);
                
                if (isSelected) display = "[" + display + "]";
                
                drawHighlight(h, display, highlight);
                cout << "\n";
            }
        }
        
        cout << "\n";
        cout << "UP/DOWN     -  NAVIGATE\n";
        cout << "LEFT/RIGHT  -  SWITCH LIST\n";
        cout << "ENTER       -  SELECT/SWAP\n";
        cout << "ESC         -  BACK\n";
        
        int key = _getch();
        
        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            
            if (key == KeyCodes::ARROW_UP) {
                if (inParty && cursor > 0) cursor--;
                else if (!inParty && cursor > 0) cursor--;
            }
            else if (key == KeyCodes::ARROW_DOWN) {
                if (inParty && cursor < (int)player.currentPokemon.size() - 1) cursor++;
                else if (!inParty && cursor < (int)pcBox.size() - 1) cursor++;
            }
            else if (key == KeyCodes::ARROW_LEFT || key == KeyCodes::ARROW_RIGHT) {
                inParty = !inParty;
                cursor = 0;
                
                if (!inParty && pcBox.empty()) {
                    inParty = true;
                }
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            if (!selectedPokemon) {
                // First selection
                if (inParty) {
                    if (player.currentPokemon.size() <= GameplayConfig::PARTY_MIN) {
                        clearScreen();
                        showDialogue("You can't move your last POKEMON!");
                        pressAnyKey();
                        continue;
                    }
                    selectedPokemon = &player.currentPokemon[cursor];
                    selectedIndex = cursor;
                    selectedFromParty = true;
                } else {
                    if (pcBox.empty()) continue;
                    selectedPokemon = &pcBox[cursor];
                    selectedIndex = cursor;
                    selectedFromParty = false;
                }
            } else {
                // Second selection - perform swap/move
                if (selectedFromParty && !inParty) {
                    // Move from party to box
                    if ((int)pcBox.size() >= GameplayConfig::PC_BOX_MAX) {
                        clearScreen();
                        showDialogue("The box is full!");
                        pressAnyKey();
                    } else {
                        Pokemon temp = player.currentPokemon[selectedIndex];
                        player.currentPokemon.erase(player.currentPokemon.begin() + selectedIndex);
                        pcBox.push_back(temp);
                        
                        clearScreen();
                        showDialogue("Moved " + temp.pokemonName + " to BOX!");
                        pressAnyKey();
                    }
                }
                else if (!selectedFromParty && inParty) {
                    // Move from box to party
                    if (player.currentPokemon.size() >= GameplayConfig::PARTY_MAX) {
                        clearScreen();
                        showDialogue("Your party is full!");
                        pressAnyKey();
                    } else {
                        Pokemon temp = pcBox[selectedIndex];
                        pcBox.erase(pcBox.begin() + selectedIndex);
                        player.currentPokemon.push_back(temp);
                        
                        clearScreen();
                        showDialogue("Moved " + temp.pokemonName + " to PARTY!");
                        pressAnyKey();
                    }
                }
                else if (selectedFromParty && inParty) {
                    // Swap within party
                    if (selectedIndex != cursor) {
                        swap(player.currentPokemon[selectedIndex], player.currentPokemon[cursor]);
                        clearScreen();
                        showDialogue("Swapped positions!");
                        pressAnyKey();
                    }
                }
                else if (!selectedFromParty && !inParty) {
                    // Swap within box
                    if (selectedIndex != cursor) {
                        swap(pcBox[selectedIndex], pcBox[cursor]);
                        clearScreen();
                        showDialogue("Swapped positions!");
                        pressAnyKey();
                    }
                }
                
                // Reset selection
                selectedPokemon = nullptr;
                selectedIndex = -1;
                cursor = 0;
            }
        }
        else if (key == KeyCodes::KEY_ESC) {
            if (selectedPokemon) {
                selectedPokemon = nullptr;
                selectedIndex = -1;
            } else {
                return;
            }
        }
    }
}

/*****************************************************************************
 * MAIN PC INTERFACE
 *****************************************************************************/
void openPC(Player& player) {
    clearScreen();
    showDialogue("Accessed SOMEONE'S PC...");
    Sleep(UIConfig::SHORT_PAUSE);
    
    while (true) {
        int choice = pcActionMenu();
        
        switch (choice) {
            case 0:
                withdrawPokemon(player);
                break;
            case 1:
                depositPokemon(player);
                break;
            case 2:
                movePokemon(player);
                break;
            case 3:
                clearScreen();
                showDialogue("Logged off...");
                Sleep(UIConfig::BRIEF_PAUSE);
                return;
        }
    }
}

/*****************************************************************************
 * BOAT TRAVEL MENU (MR. BRINEY)
 *****************************************************************************/
int boatTravelMenu(const vector<string>& destinations) {
    int cursor = 0;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    
    size_t maxLen = 0;
    for (const auto& dest : destinations) {
        if (dest.length() > maxLen) {
            maxLen = dest.length();
        }
    }
    
    while (true) {
        clearScreen();
        
        cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n";
        cout << "| MR. BRINEY: Where would you like to go?" << string(UIConfig::STANDARD_BOX_WIDTH - 40, ' ') << "|\n";
        cout << "+" << string(UIConfig::STANDARD_BOX_WIDTH, '-') << "+\n\n";
        
        for (int i = 0; i < (int)destinations.size(); ++i) {
            string text = destinations[i] + string(maxLen - destinations[i].length(), ' ');
            drawHighlight(h, text, cursor == i);
            cout << "\n";
        }
        
        cout << "\n";
        cout << "UP/DOWN  -  NAVIGATE\n";
        cout << "ENTER    -  SELECT\n";
        cout << "ESC      -  CANCEL\n";
        
        int key = _getch();
        
        if (key == KeyCodes::ARROW_PREFIX_1 || key == KeyCodes::ARROW_PREFIX_2) {
            key = _getch();
            
            if (key == KeyCodes::ARROW_UP && cursor > 0) {
                cursor--;
            }
            else if (key == KeyCodes::ARROW_DOWN && cursor < (int)destinations.size() - 1) {
                cursor++;
            }
        }
        else if (key == KeyCodes::KEY_ENTER) {
            return cursor;
        }
        else if (key == KeyCodes::KEY_ESC) {
            return (int)destinations.size() - 1;  // Return last item (CANCEL)
        }
    }
}

static void hallOfFameSequence(Player &player) {
    clearScreen();
    
    // Intro animation
    for (int i = 0; i < 3; i++) {
        clearScreen();
        cout << "Recording your victory";
        for (int j = 0; j <= i; j++) {
            cout << ".";
            cout.flush();
        }
        Sleep(600);
    }
    
    clearScreen();
    Sleep(500);
    
    // Hall of Fame display
    cout << "+---------------------------------------+\n";
    cout << "|                                       |\n";
    cout << "|          HALL OF FAME                 |\n";
    cout << "|                                       |\n";
    cout << "+---------------------------------------+\n";
    Sleep(1500);
    
    
    cout << "TRAINER: " << player.username << "\n\n";
    Sleep(1000);
    
    // Display all Pokemon with Pokedex numbers
    for (const auto& pokemon : player.currentPokemon) {
        // Format: No. XXX - Name - Lv. XX
        cout << "No. " 
             << setw(3) << setfill('0') << pokemon.pokemonIndex 
             << " - " << pokemon.pokemonName 
             << " - Lv. " << pokemon.level << "\n";
        Sleep(800);
    }
    
    Sleep(1500);
    clearScreen();
    
    cout << "+---------------------------------------+\n";
    cout << "|                                       |\n";
    cout << "|            Welcome to the             |\n";
    cout << "|             HALL OF FAME!             |\n";
    cout << "|                                       |\n";
    cout << "+---------------------------------------+\n";
    
    Sleep(2000);
    pressAnyKey();
}

#endif //POKEMON_UTILITY_H