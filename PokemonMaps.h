#ifndef POKEMON_MAPS_H
#define POKEMON_MAPS_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <cctype>

#include "PokemonUtility.h"
#include "Player.h"
#include "Trainer.h"
#include "PokemonDisplay.h"
#include "PokemonDatabase.h"

using namespace std;
extern PokemonDatabase db;

// Forward declarations
void firstBattle(Player &player, const PokemonDatabase &db, const std::string& wildName, int minLevel, int maxLevel);
void chooseStarter(Player &player, const PokemonDatabase &db);
void startBattleLoop(Player &player, Pokemon &playerPokemon, Pokemon &enemyPokemon, const PokemonDatabase &db, Trainer* trainer);
Pokemon getWildPokemonRoute101(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonRoute102(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonRoute104(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonPetalburgWoods(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonGraniteCave(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonRoute110(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonRoute111Desert(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonRoute112(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonJaggedPass(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonRoute118(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonFieryPath(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonRoute119(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonMtPyreInterior(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonMtPyreExterior(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonMtPyreSummit(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonMagmaHideout(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonSeafloorCavern(const PokemonDatabase& db, int minLevel, int maxLevel);
Pokemon getWildPokemonVictoryRoad(const PokemonDatabase& db, int minLevel, int maxLevel);

void playBoatAnimation(const string& destination);
int boatTravelMenu(const vector<string>& destinations);
void playCableCarAnimation(bool goingUp = true);


/*****************************************************************************
 * CONSTANTS
 *****************************************************************************/
namespace MapConfig {
    constexpr int WILD_ENCOUNTER_CHANCE = 2;      // 10% chance per step in grass
    constexpr int WILD_LEVEL_MIN = 2;              // Default wild Pokemon level range
    constexpr int WILD_LEVEL_MAX = 3;
    constexpr int WHITEOUT_MONEY_DIVISOR = 2;      // Lose half money on whiteout
    constexpr int EDGE_THRESHOLD = 1;              // Rows/cols from edge to trigger transition
    constexpr int EDGE_THRESHOLD_WIDE = 2;         // For wider edge detection (routes)
    constexpr int DOOR_EXIT_ROW_OFFSET = 1;        // Spawn 1 row below door when exiting
}

namespace TileChars {
    constexpr char SPAWN_MARKER = 'X';
    constexpr char BOAT_MARKER = 'B';
    constexpr char ROUTE_TRANSITION = '=';
    constexpr char NPC_MARKER = '@';
    constexpr char PC_TILE = 'c';
    constexpr char GROUND = '.';
    constexpr char PLAYER_CHAR = '0';
}

/*****************************************************************************
 * ENUMS
 *****************************************************************************/
enum MapID {
    MAP_TRUCK,
    MAP_LITTLEROOT,
    MAP_MOMHOUSE,
    MAP_RIVALHOUSE,
    MAP_BIRCHLAB,
    MAP_ROUTE101,
    MAP_OLDALE,
    MAP_POKECENTER,
    MAP_POKEMART,
    MAP_ROUTE102,
    MAP_PETALBURG1,
    MAP_PETALBURG2,
    MAP_PETALBURGGYM,
    MAP_ROUTE104,
    MAP_PETALBURGWOODS1,
    MAP_PETALBURGWOODS2,
    MAP_RUSTBORO1,
    MAP_RUSTBORO2,
    MAP_RUSTBOROGYM,
    MAP_DEWFORD1,
    MAP_DEWFORD2,
    MAP_DEWFORDGYM,
    MAP_GRANITECAVE,
    MAP_SLATEPORT1,
    MAP_SLATEPORT2,
    MAP_ROUTE110_1,
    MAP_OCEANIC_MUSEUM,
    MAP_ROUTE110_2,
    MAP_MAUVILLE,
    MAP_MAUVILLEGYM,
    MAP_ROUTE111_1,
    MAP_ROUTE111_2,
    MAP_ROUTE111_3,
    MAP_ROUTE111_DESERT,
    MAP_ROUTE112_1,
    MAP_ROUTE112_2,
    MAP_FIERYPATH,
    MAP_CABLECAR,
    MAP_MTCHIMNEY,
    MAP_MTCHIMNEY_SUMMIT,
    MAP_JAGGEDPASS,
    MAP_LAVARIDGE,
    MAP_LAVARIDGEGYM,
    MAP_HARBOR,
    MAP_ROUTE118,
    MAP_ROUTE119,
    MAP_FORTREE,
    MAP_FORTREEGYM,
    MAP_LILYCOVE1,
    MAP_LILYCOVE2,
    MAP_LILYCOVE3,
    MAP_LILYCOVE4,
    MAP_AQUA_ENTRANCE,
    MAP_MTPYRE_ENTRANCE,
    MAP_MTPYRE1,
    MAP_MTPYRE2,
    MAP_MTPYRE_SUMMIT,
    MAP_MAGMA_HIDEOUT_1,
    MAP_MAGMA_HIDEOUT_2,
    MAP_MAGMA_HIDEOUT_3,
    MAP_MAGMA_HIDEOUT_4,
	MAP_AQUA_HIDEOUT_1,
	MAP_AQUA_HIDEOUT_2,
	MAP_AQUA_HIDEOUT_3,
	MAP_AQUA_HIDEOUT_4,
	MAP_AQUA_HIDEOUT_5,
	MAP_SEAFLOOR_CAVERN_1,
	MAP_SEAFLOOR_CAVERN_2,
	MAP_MOSSDEEP1,
	MAP_MOSSDEEP2,
	MAP_MOSSDEEPGYM,
	MAP_MOSSDEEP3,
	MAP_SOOTOPOLIS1,  
	MAP_SOOTOPOLIS2,
	MAP_SOOTOPOLISGYM,
	MAP_CAVE_OF_ORIGIN,
	MAP_SKY_PILLAR_ENTRANCE,
	MAP_SKY_PILLAR,
	MAP_EVER_GRANDE_CENTER,
	MAP_VICTORY_ROAD1,
	MAP_VICTORY_ROAD2,
	MAP_EVER_GRANDE_LEAGUE,
	MAP_LEAGUE_ENTRANCE,
	MAP_LEAGUE_RM1,
	MAP_LEAGUE_RM2,
	MAP_LEAGUE_RM3,
	MAP_LEAGUE_RM4,
	MAP_LEAGUE_RM5,        
    MAP_COUNT  //represents total count
};

static void enterMap(MapID id, char entryMarker, int rowOffset = 0, int colOffset = 0);
static void enterMap(MapID id, const string& fromEdge);
static void enterMap(MapID id);

enum StoryState {
    INTRO_TRUCK,
    INTRO_MOM_DIALOGUE,
    INTRO_GO_TO_LAB,
    INTRO_PICKED_STARTER,
    STORY_FREE_ROAM
};

enum TileColor {
    COLOR_GREY       = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    COLOR_DARK_GREY  = FOREGROUND_INTENSITY,
    COLOR_DARK_GREEN = FOREGROUND_GREEN,
    COLOR_GREEN      = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    COLOR_LIGHT_GREY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    COLOR_LIGHT_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    COLOR_RED        = FOREGROUND_RED | FOREGROUND_INTENSITY,
    COLOR_BLUE       = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    COLOR_YELLOW     = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    COLOR_BROWN      = FOREGROUND_RED | FOREGROUND_GREEN,
    COLOR_PURPLE = FOREGROUND_RED | FOREGROUND_BLUE,
    COLOR_LIGHT_PURPLE = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	COLOR_CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
	COLOR_LIGHT_CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    COLOR_WHITE      = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
};

/*****************************************************************************
 * DATA STRUCTURES
 *****************************************************************************/
struct Tile {
    bool passable;
    int color;
    bool wildEncounter;
};

struct Position {
    int row;
    int col;
};

struct MapData {
    MapID id;
    vector<string> layout;
    Position spawn;
    string name;
    Pokemon (*wildEncounterFunc)(const PokemonDatabase&, int, int) = nullptr;
    int wildMinLevel = 2;
    int wildMaxLevel = 3; 
};

struct NPCPos {
    int row;
    int col;
    int id;
};

struct TrainerNPC {
    int row;
    int col;
    int trainerId;
    bool defeated;
};

struct RouteTrainerData {
    vector<Trainer> trainers;
    vector<int> prizeMoney;
    bool initialized = false;
};

/*****************************************************************************
 * GLOBAL STATE
 *****************************************************************************/
static StoryState storyState = INTRO_TRUCK;
static bool flagVisitedMom = false;
static bool flagHasStarter = false;

//Dewford
static bool flagMetSteven = false;

//Slateport
static bool flagRescueStern = false;
static bool flagCheckBlockRoute110 = false;
static bool flagUnblockRoute110 = false;

//Mt Chimney Magma Machine Encounter (Unlock Jagged Pass after)
static bool flagCheckMagmaMachine= false;
static bool flagEncounterMachine = false;
static bool flagMtSummitCleared= false;

//Aqua Entrance check pre orb event
//Route 121 -> Mt Pyre locked until Aqua entrance checked
static bool flagCheckAquaEntrance= false;

//Post orb event
static bool flagOrbStolen= false;

//Magma Hideout Groudon Awakening
static bool flagShowMagmaEntranceOnce= false;
static bool flagUnlockMagmaHideout= false;
static bool flagMaxieDefeated= false;
static bool flagAwakenGroudon= false; 

//Aqua Hideout Kyogre Awakening
static bool flagUnlockSeafloorCavern= false;
static bool flagArchieDefeated= false;
static bool flagAwakenKyogre= false;

//Mossdeep rocket event
static bool flagRocketEvent= false;

//Sootopolis trio event
static bool flagGroudonKyogreClash = false;
static bool flagSootopolisTalkToSteven=false;
static bool flagMetWallace= false;
static bool flagAwakenRayquaza= false; 
static bool flagTrioEvent= false;

//League End
static bool flagWallaceDefeated= false;

static string rivalName = ""; 

static MapID lastPokecenterMap = MAP_MOMHOUSE;
static bool hasVisitedPokecenter = false;
static MapID lastBuildingOriginMap = MAP_MOMHOUSE; //track shared interiors

static map<MapID, MapData> worldMaps;
static MapData* currentMap = nullptr;
static MapID currentMapID = MAP_COUNT;
static Position playerPos = {0, 0};

static vector<NPCPos> currentNPCs;
static map<MapID, RouteTrainerData> allTrainers;
static vector<TrainerNPC> currentTrainers;
static vector<pair<MapID, int>> defeatedTrainers;


/***DEV MODE FOR TESTING***/
static bool DEBUG_MODE = false;  // Set to false for normal gameplay
static MapID DEBUG_SPAWN_MAP = MAP_MOSSDEEP1;


/*****************************************************************************
 * TILE DEFINITIONS
 *****************************************************************************/
static map<char, Tile> tileMap = {
    // Walls & obstacles (non-passable)
    {'#',  {false, COLOR_DARK_GREY, false}},
    {'|',  {false, COLOR_DARK_GREY,  false}},
    {'/',  {false, COLOR_DARK_GREY,  false}},
    {'\\', {false, COLOR_DARK_GREY,  false}},
    {'-',  {false, COLOR_DARK_GREY,  false}},
    {'Y',  {false, COLOR_DARK_GREEN, false}},
    {'T',  {false, COLOR_DARK_GREY,  false}},
    {'~',  {false, COLOR_LIGHT_BLUE, false}},
    {'`',  {false, COLOR_LIGHT_CYAN, false}},
    {'%',  {false, COLOR_RED, false}},
    {'D',  {false, COLOR_DARK_GREY,  false}},
    {'_',  {false, COLOR_DARK_GREY,  false}},
    {'[',  {false, COLOR_DARK_GREY,  false}},
    {']',  {false, COLOR_DARK_GREY,  false}},
    {'c',  {false, COLOR_GREY,       false}},
    {'&',  {false, COLOR_BROWN,       false}},
    
    // NPCs & special markers (non-passable)
    {'@',  {false, COLOR_RED,        false}},
    {'P',  {false, COLOR_RED,        false}},
    {'C',  {false, COLOR_RED,        false}},
    {'M',  {false, COLOR_LIGHT_BLUE, false}},
    {'A',  {false, COLOR_LIGHT_BLUE, false}},
    {'R',  {false, COLOR_LIGHT_BLUE, false}},
    {'T',  {false, COLOR_LIGHT_BLUE, false}},
	{'G',  {false, COLOR_RED,        false}},
	{'y',  {false, COLOR_RED,        false}},
	{'m',  {false, COLOR_RED,        false}},
	{'+',  {false, COLOR_RED,        false}},
	{'0',  {false, COLOR_RED,        false}},
	
    // Walkable ground (no encounters)
    {'.',  {true,  COLOR_YELLOW,     false}},
    {':',  {true,  COLOR_YELLOW,     false}},
    {'\'', {true,  COLOR_GREEN,      false}},
    {' ',  {true,  COLOR_YELLOW,     false}},

    // Wild encounter tiles (passable + encounters)
    {'^',  {true,  COLOR_GREEN,      true}},
    {'V',  {true,  COLOR_GREEN,      true}},
    {'"',  {true,  COLOR_DARK_GREEN, true}},
    {'*',  {true,  COLOR_BROWN, true}},
    {',',  {true,  COLOR_YELLOW, true}},

    // Transitions & spawns (passable)
    {'X',  {true,  COLOR_WHITE,      false}},
    {'B',  {true,  COLOR_YELLOW,     false}},
    {'=',  {true,  COLOR_YELLOW,     false}},
    {'1',  {true,  COLOR_YELLOW,     false}},
    {'2',  {true,  COLOR_YELLOW,     false}},
    {'3',  {true,  COLOR_YELLOW,     false}},
    {'4',  {true,  COLOR_YELLOW,     false}},
    {'5',  {true,  COLOR_YELLOW,     false}},
    {'6',  {true,  COLOR_YELLOW,     false}},
    {'7',  {true,  COLOR_YELLOW,     false}},
    {'8',  {true,  COLOR_YELLOW,     false}},
    {'9',  {true,  COLOR_YELLOW,     false}},
    {'v',  {true,  COLOR_YELLOW,     false}},
    {'K',  {true,  COLOR_YELLOW,     false}},
    {'F',  {true,  COLOR_YELLOW,     false}},
    {'Q',  {true,  COLOR_YELLOW,     false}},
    {'E',  {true,  COLOR_YELLOW,     false}},
    {'F',  {true,  COLOR_YELLOW,     false}},
    {'t',  {true,  COLOR_YELLOW,     false}},
    {'a',  {true,  COLOR_YELLOW,     false}},
    {'J',  {true,  COLOR_YELLOW,     false}},
    {'p',  {true,  COLOR_YELLOW,     false}},
    {'h',  {true,  COLOR_YELLOW,     false}},
    {'l',  {true,  COLOR_YELLOW,     false}},
    {'s',  {true,  COLOR_YELLOW,     false}},
    {'r',  {true,  COLOR_YELLOW,     false}},
    {'g',  {true,  COLOR_YELLOW,     false}},
    {'L',  {true,  COLOR_YELLOW,     false}},
    {'w',  {true,  COLOR_YELLOW,     false}},
    {'W',  {true,  COLOR_YELLOW,     false}}
};

/*****************************************************************************
 * MAP LAYOUTS
 *****************************************************************************/
static vector<string> truck_map = {
    "#################",
    "####....#####...#",
    "#####...#####...#",
    "###.............=",
    "###.......X.....=",
    "######.....###..#",
    "#################"
};

static vector<string> littleroot_map = {
    "YYYYYYYYYYYYYYYYYYYYYYYYYYYY======YYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYY......YYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYY'''''''''''''''''''''''''''''''''''''''''''''''''''''''YYYY",
	"YY'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''YY",
	"YY''''''----------'''''''''''''''''''''''''''----------''''''YY",
	"YY'''--|||||||||||--''''''''''''''''''''''--|||||||||||--''''YY",
	"YY'''||||||||||||||||'''''''''''''''''''''|||||||||||||||'''''YY",
	"YY''''|  |||   |___|'''''''''''''''''''''''|__|   |||  |'''''YY",
	"YY''''|  |-|---|   |'''''''''''''''''''''''|  |---|-|  |'''''YY",
	"YY''''##########222#'''''''''''''''''''''''#111#########'''''YY",
	"YY'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''YY",
	"YY'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''YY",
	"YY'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''YY",
	"YY''''-------------------------''''''''''''''''''''''''''''''YY",
	"YY''''| |###| | | | | | | | | |''''''''''''''''''''''''''''''YY",
	"YY''''-------------------------''''''''''''''''''''''''''''''YY",
	"YY''''| |----| | | |____| | | |''''''''''''''''''''''''''''''YY",
	"YY''''| |----| | | |    | | | |''''''''''''''''''''''''''''''YY",
	"YY''''##############3333#######''''''''''''''''''''''''''''''YY",
	"YY'''''''''''''''''''''''''''''''''''''''''''''''''''''YYYYYYYY",
	"YYYY'''''''''''''''''''''''''''''''''''''''''''''''YYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"	
};

static vector<string> mom_house_map = {
    "#############################||||||||||",
    "#...#:::::::::::###:::::::::#||||||||||",
    "###################:::::::::###########",
    "#:::::::::::::::::::::::::::::::::::::#",
    "#:::::::::::::::::::::::::::::::::::::#",
    "#:::::::----------::::::::::::::::::::#",
    "#:::::::|--------|::::::::::::::::::::#",
    "#::::@::|--------|::::::::::::::::::::#",
    "#:::::::|--------|::::::::::::::::::::#",
    "#:::::::----------::::::::::::::::::::#",
    "#::::::::::::::::::::::::::::::::::X::#",
    "#:::::::::::::::::::::::::::::::::::::#",
    "##################################====#"
};

static vector<string> rival_house_map = {
    "||||||||||#############################",
    "||||||||||#:::::::::###:::::::::#...###",
    "###########:::::::::###################",
    "#:::::::::::::::::::::::::::::::::::::#",
    "#:::::::::::::::::::::::::::::::::::::#",
    "#::::::::::::::::::::----------:::::::#",
    "#::::::::::::::::::::|--------|:::::::#",
    "#::::::::::::::::::::|--------|::@::::#",
    "#::::::::::::::::::::|--------|:::::::#",
    "#::::::::::::::::::::----------:::::::#",
    "#::X::::::::::::::::::::::::::::::::::#",
    "#:::::::::::::::::::::::::::::::::::::#",
    "#====##################################"
};

static vector<string> birch_lab_map = {
    "########################################",
    "#----#::::::::::::::::::::::::::::#----#",
    "#----#::::::::::::::::::::::::::::#----#",
    "######::::::::::::::@:::::::::::::######",
    "#::::::::::::::::::::::::::::::::::::::#",
    "####:::::::::::::::::::::::##::::#######",
    "#::########::::::::::::::##::#####-----#",
    "#::#------#::::::::::::::##::#####-----#",
    "####------#::::::::::::::::##::::#######",
    "#---------#::::::::::::::::::::::::::::#",
    "#---------#::::::::::::::::::::::::::::#",
    "###########::::::::::::::::::::::::::::#",
    "#::::::::::::::::::::::::::::::::::::::#",
    "#----------|:::::::::::::::::::::::::::#",
    "#----------|:::::::::::::::::X:::::::::#",
    "#----------|:::::::::::::::::::::::::::#",
    "############################===#########"
};

static vector<string> route101_map = {
    "YYYYYYYYYYYYYYYYYY=====YYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
    "YYYYYYYYYYYYYYYYYY.....YYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
    "YYY^^^^^^'''''''''.....'''''''^^^^^^^^^^^^^^^^YYYYYY",
    "YY^^^^^^^^^'''''''......''''^^^^^^^^^^^^^^^^^^^^^YYY",
    "^^^^^^^^^^^'''''............^^^^^^^^^^^^^^^^^^^^^YYY",
    "^^^^^^^^^^^'''''.............''^^^^^^^^^^^^^^^^^^YYY",
    "YY'''''''''''################YY^^^^^^^^^^^^^^^^^^YYY",
    "YYY###########'''''''''''''''YYYY'''''''''.......YYY",
    "YYY''''''''''''''''........''YYYYYY'''''.........YYY",
    "YYYYY'''''''.................YYY^^^^^''.............",
    "YYYYY'''''''.................^^^^^^^^''.@...........",
    "YY^^'''''''''''..............^^^^^^^^^^'''''........",
    "^^^^^^^^'''''''''............^^^^^^^^^^^^^^^''''''''",
    "^^^^^^^^^^'''''''''''''''''''^^^^^^^^^^^^^^^^^''''''",
    "^^^^^^^^^^^^'''''''''''''''''^^^^^^^^^^^^^^'''''''''",
    "^^^^^^^^^^^^''''@'''''#########YYY^^^^^........''''",
    "YY^^^^^^^^''''''''''''''''''''''YYY^^^^^........''''",
    "YYY^^^^^^^''''''''''''''''''''''YYYYYYYYYYYYYYYYYYYY",
    "YYYYYYYYYYYYYYYYYYYY'''.'....'''YYYYYYYYYYYYYYYYYYYY",
    "YYYYYYYYYYYYYYYYYYYY...=====....YYYYYYYYYYYYYYYYYYYYY"
};

static vector<string> oldale_map = {
    "YYYYYYYYYYYYYYYYYYYYYYYYYYYY======YYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
    "YYYYYYYYYYYYYYYYYYYYYYYYYYYY......YYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
    "YYYYYY'''''''''''''''''''''.......''''''''''''''''''''''''YYYYY",
    "YYY''''''----------------...........''''''||||||||||||||||''YYY",
    "YYY''''''||||||||||||||||...........''''''||||||||||||||||''YYY",
    "YYY''''''||||||||||||||||...........''''''||||||||||||||||''YYY",
    "YYY''''''||||||||||||||||.................|| ||   |MART| |''YYY",
    "YYY''''''||--|----|--|--|.................|| ||   |    | |''YYY",
    "YYY''''''||--|----|--|--|.................#####555########''YYY",
    "YYY''''''################...............@...................YYY",
    "YYY''''''''''...............................................YYY",
    "=.........................'''''''''''''.....................YYY",
    "=.........................'''''''''''''.....................YYY",
    "=.........................'''''''''''''.....................YYY",
    "YYY''''''.................'''''''''''''..................'''YYY",
    "YYY'''''''''''''....................................''''''''YYY",
    "YYY''''''''''''''...............................''''''''''''YYY",
    "YYY''''''''''''''...............................''''''''''''YYY",
    "YYY''''''''''''''...............................''''''''''''YYY",
    "YYY''''''''---------------.@...............-----------------YYY",
    "YYY''''''''||  |||||  || |.................|||||||||||||||||YYY",
    "YYY''''''''||  |||||  || |.................|||||||||||||||||YYY",
    "YYY''''''''---------------.................-----------------YYY",
    "YYY''''''''|| ||   |  PC |.................|---|   |---|---|YYY",
    "YYY''''''''|| ||   |     |.................|---|   |---|---|YYY",
    "YYY''''''''#####444#######.................#################YYY",
    "YYYYYY''''''''''''........................''''''''''''''''''YYY",
    "YYYYYY''''''''''''........................''''''''''''''''''YYY",
    "YYYYYY''''''''''''........................''''''''''''''''YYYYY",
    "YYYYYYYYYYYYYYYYYYYYYYYYYYYYY======YYYYYYYYYYYYYYYYYYYYYYYYYYYY",
    "YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"
};

static vector<string> pokecenter_map = {
    "###########################################################",
    "###########################################################",
    "##....---------|  | |[][]|...........|  |[##]............##",
    "##....|[][][][]|  | |[][]|...........|  |#cc#............##",
    "##....---------|  --------------------  |................##",
    "##.............---------------@----------................##",
    "##.......................................................##",
    "##.......................................................##",
    "##.......................................................##",
    "##.......................................................##",
    "##.......................................................##",
    "##...............................................--------##",
    "##...............................................|######|##",
    "##..........................X....................|######|##",
    "##########################=====############################",
    "###########################################################"
};

static vector<string> pokemart_map = {
    "###########################################################",
    "###########################################################",
    "##----|  |......................||[]|[]][]|[]||[][][]||..##",
    "##----|  |......................||[]|[]][]|[]||[][][]||..##",
    "##----|  @......................||------------------ ||..##",
    "##____|  |...............................................##",
    "##_______|...............................................##",
    "##.......................................................##",
    "##..............................--------..............---##",
    "##..............................|[]||[]|..............|[]##",
    "##..............................|[]||[]|..............|[]##",
    "##.....@........................|[]||[]|..............|[]##",
    "##..............................--------..............---##",
    "##...........X...........................................##",
    "###########======##########################################",
    "###########################################################"
};

static vector<string> route102_map = {
    "YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
    "YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
    "YYYYYY^^^^^^^^^^'''''''''''''''''''''''YYYYYYY^^^^^~~~~~~~YYYYY",
    "YY^^^^^^^^^^^^^^........@......'''''''''''''&&^^^^^~~~~~~~~~YYY",
    "YY^^^^^^^^^.........................''''''''&&'^^^^~~~~~~~~~YYY",
    "=''^^^^^^^^...........YY&&....&&&&&&&&&.....&&'^^^^~~~~~~~~~YYY",
    "=''''''''''''''''''''YYYYY.....^^^^^^^^YYYYYYY''''''''''''''YYY",
    "='''''''''''''''''''YYYYYY....^^^^^^^^^YYYYYYY''''''''''''''''=",
    "YYYYYYYYYYYY'''''''YYYYYYY...^^^^^^^^^^YYYYYYY''''''..........=",
    "YYYYYYY^^^^^'''''YYYYYYYYY.^^^^^^^^^^^^YY^^YY'''''............=",
    "YYYYYY^^^^^^&&&&&&&&YYYYYY^^^^^^^^^^'''''^^^^^^..............YY",
    "YYYYYY^^^^^^^^''''''YYYYYY^^^^^^^''''''''^^^^^^..............YY",
    "YYYYYY^^^^^^^^''''''YYYYYYYYYY'''''''''''''^^^^..............YY",
    "YYYYYY^^^^^^^^''''''YYYYYYYYYY''''''''''@'''''''''''''''''YYYYY",
    "YYYYYY^^^^^^^^^^YYYYYYYYYYYYYY'''''YYY''''''''''''''''''''YYYYY",
    "YYYYYY^^^^^^^^^^YYYYYYYYYYYYY''''^^^^YYY'''''''''''''''YYYYYYYY",
    "YYYYYYYYYYYYYYYYYYYYYYYYYYYYY''^^^^^^^^^''''''''''''YYYYYYYYYYY",
    "YYYYYYYYYYYYYYYYYYYYYYYYYYYYY^^^^^^^^^^^^^'''''YYYYYYYYYYYYYYYY",
    "YYYYYYYYYYYYYYYYYYYYYYYYYYYYY^^^^^^^^^^^^^^^^^^YYYYYYYYYYYYYYYY",
    "YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"
};

static vector <string> petalburg1_map = {
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"'''''''''''''''''''''''''''''''''''''''''''''||||||||||||||||YY",
	"'''''''''''''''''''''''''''''''''''''''''''''||||||||||||||||YY",
	"=..................@.''''''''''''''''''''''''||||||||||||||||YY",
	"=....................''''''''''''''''''''''''|---|   |MART|||YY",
	"''''''''''''''''.....''''''''''''''''''''''''|---|   |----|||YY",
	"''''''''''''''''.....''---------------'''''''|####555########YY",
	"YYY'''''''''''''.....''||  |||||  || |'''''''''''.....'''''''YY",
	"YYY-------------.....''||  |||||  || |'''''''''''.....'''''''YY",
	"YYY|||||||||||||.....''---------------'''''''''''.....'''''''YY",
	"YYY|||||||||||||.....''|--|   |PC|---|'''''''''''.....'''''''YY",
	"YYY|---|___|---|.....''|--|   |  |---|'''''''''''.....'''''''YY",
	"YYY|---|___|---|.....''####444########'''''''''''.............=",
	"YYY#############..............................................=",
	"YYY'''''''''''''..............................................=",
	"YYY'''''''''''''....................@........................YY",
	"YYY~~~~~~~~~~~''.....''''''''''''''''''''''''''''''''''''''''YY",
	"YYY~~~~~~~~~~~''.....''''''''''''''''''''''''''''''''''''''''YY",
	"YYY~~~~~~~~~~~''.....''-------------'''''''''''''''''''''''''YY",
	"YYY~~~~~~~~~~~''.....''|||||||||||||'''''''''''''''''''''''''YY",
	"YYY~~~~~~~~~~~''.....''|||||||||||||'''''''''''''''''''''''''YY",
	"YYY~~~~~~~~~~~''.....''|---|___|---|'''''''''''''''''''''''''YY",
	"YYY~~~~~~~~~~~''.....''|---|___|---|'''''''''''''''''''''''''YY",
	"YYY~~~~~~~~~~~''.....''#############'''''''''''''''''''''''''YY",
	"YYY~~~~~~~~~~~YY....................'''''''''''''''''''''''''YY",
	"YYY~~~~~~~~~~~YY....................'''''''''''''''''''''''''YY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"
};

static vector <string> petalburg2_map= {
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYY-------------YYYYYYYYYYYYYYYYYYYYYYYY'''~~~~~~~~~~~~~~~~~~~~",
	"YYY|||||||||||||YYYYYYYYYYYYYYYYYYYYYYYY~~~~~~~~~~~~~~~~~~~~~~~",
	"YYY|||||||||||||YYY---------------------~~~~~~~~~~~~~~~~~~~~~~~",
	"YYY|---|___|---|YYY|||||||||||||||||||||~~~~~~~~~~~~~~~~~~~~~~~",
	"YYY|---|___|---|YYY|||||||||||||||||||||~~~~~~~~~~~~~~~~~~~~~~~",
	"YYY#############YYY|||||||||||||||||||||~~~~~~~~~~~~~~~~~~~~~~~",
	"YYY''''.....'''''''|[]|Gym|[]|---|  |[]|~~~~~~~~~~~~~~~~~~~~~~~",
	"YYY''''.....'''''''|  |   |  |---|  |  |~~~~~~~~~~~~~~~~~~~~~~~",
	"YYY''''.....'''''''###########666#######'''''''''''''''''''''''",
	"YYY''''.....'''''''''''''''''.....'''''''''''''''''''''''@'''''",
	"YYY''''.....''''''''''''''''@.....'''''''''''''''''''''''''''''",
	"=.................................'''''''''''''''''''''''''''''",
	"=.................................'''''''''''''''''''''''''''''",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYY=====YYYYYYYYYYYYYYYYYYYYYYYYYYYYY"
};

static vector <string> petalburggym_map{
	"###############################################",
	"##|||||||[]||[]||||||||||||||||[]||[]||||||||##",
	"##|||||||||||||||||||||||||||||||||||||||||||##",
	"##-------------------------------------------##",
	"##...........||........@......||.............##",
	"##...........|____........_____|.............##",
	"##...........................................##",
	"##...........................................##",
	"##...........................................##",
	"##.....................X.....................##",
	"#####################=====#####################"
};

static vector <string> route104_map={
	"YYYYYYYYYYYYYYYYYYYYYYYYYYY====YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"~~~~~~~~^^^^^^^^^^^^^^^^^^^.....^^^^^^^^^^^^^^^^YYYYYYYYYYYYYYYYYY",
	"~~~~~~~~~~~^^^^^^^^^^^^^^^^.....^^^^^^^^^^^^^^^^YYYYYYYYYYYYYYYYYY",
	"~~~~~~~~~~~^^^^^^^^^^^^^^^^......^^^^^^^^^^^^^^^^^YYYYYYYYYYYYYYYY",
	"~~~~~~~~~~~~~^^^^^^^^^^^^^^^......^^^^^^^^^^^^^^^^YYYYYYYYYYYYYYYY",
	"~~~~~~~~~~~~~^^^^^^^^^^^^^^^......................'''''YYYYYYYYYYY",
	"~~~~~~~~~~~~~~~^^^^^^^^^^^^^..........................'''''''''YYY",
	"~~~~~~~~~~~~~~'-------------'''.......................'''''''''YYY",
	"~~~~~~~~~~~~~~'|||||||||||||'''''''''''''''''''......''''''''''YYY",
	"~~~~~~~~~~~~~~'|||||||||||||'''''''''''''''''''......''''''''''YYY",
	"~~~~~~~~~~~~~~'|---|   |---|'''''''''''''''''''......''''''''''YYY",
	"~~~~~~~~~~~~~~'|---|   |---|'''''''''''''''''''.......'''''''''YYY",
	"~~~~~~~~~~~~~~'#############''''''''''''''''''''''''^^^^^''''''YYY",
	".....................@.B....'''''''''''''''''''^^^^^^^^^^^^^^^^YYY",
	"............................'''''''''''''^^^^^^^^^^^^^^^^^^^^^^YYY",
	"~~~~~~~~~~~~~~~~~~~~~~~~~&&&&&&&&&&&&&&&^^^^^^^^^^^^^^^^^^^^^^^YYY",
	"~~~~~~~~~~~~~~~~~~~~~~~~~.............&&^^^^^^^^^^^^^^'''''''''YYY",
	"~~~~~~~~~~~~~~~~~~~~~~~~~.............&&^^^^^^'''''''''''''''''YYY",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~..........&&&&^^^''''.......'''''''YYY",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~............&&....................YYY",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.........&&'''''.................=",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.........&&'''''...''''''........=",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.........&&''''...''''''.........=",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.....&&&&&&&&&.............YYY",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~...........&&&&&&&&&&&&&&&&&&"
};

static vector <string> petalburgwoods1_map={
	"YYYYYYYYYYYYYYYYYYYYYYY###########YYYYYYYYYYYYYYYYYYYYYYYY=====YY",
	"YYYYYYYYY''^^^''''''''''''''''''''''^^^^^^^^^^^^^^^^^^^^^^.....YY",
	"YYY''^^^^^^^^^^''''''''''''''''''''''''^^^^^^^^^^^^^^^'''''''''YY",
	"YYY^^^^^^^^^^^^^^^^^^^^^^^'''''''''''''''''''''''''''''''''''''YY",
	"YYY^^^^^^^^^^^^^^^^^^^^^^^^^'''''''''''''''''''''''''''''''''''YY",
	"YYY^^^^^^^^^^^^^^YYYYYYYYYYY''''''''@''''''''''''''''''''''''''YY",
	"YY^^^^^^^^^^^^^^^YYYYYYYYYYY'''''''''''''''''YYYYYYYYYYYYYYYYYYYY",
	"YY^^^^^^^^^^^^^^^YYYYYYYYYYY&&&&&&&&&&&&&&&&&YYYYYYYYYYYYYYYYYYYY",
	"YY^^^^^^^^^^^^^^^^^^^^^^^^'''''''''''''''''''YYYYYYYYYYYYYYYYYYYY",
	"YY'''^^^^^^^^@^^^^^^^''''''''''''''''''''''''^^^^^^^^^^^YYYYYYYYY",
	"YY''''''^^^^^^^^^^^^^'''''''''''''''''''''^^^^^^^^^^^^^^YYYYYYYYY",
	"YY''''''''''''''''''''''''''''''''''''''''^^^^^^^^^^^^^^YYYYYYYYY",
	"YYYYYYYY''''''''''''''''''''''''''''''''''^^^^^^^^^^^^^^YYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYY..........YYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYY............YYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYY..........YYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY=====YYYYYYYYYYYYYYYYYYYYYYYYYYY"
};

static vector <string> petalburgwoods2_map={
	"YYYYYYYYYYYYYYYYYYYYYY======YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYY......YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYY......YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYY..@...YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYY........YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YY'''''''^^^^^^^^''''........'''''''YYYYYYYYYYYYYYY''YYYYYYYYYYYY",
	"YY'''^^^^^^^^^^^^'''''''''''''''''''YYYYYYYYYYYY^^^^^^^''''''''YY",
	"YY^^^^^^^^^^^^^^^'''''''''''''''''''''^^^^^^^^^^^^^^^^^^^^YYYYYYY",
	"YY^^^^^^^^^^^^^^^'''''''''''''''''''''''''^^^^^^^^^^^^^^^^YYYYYYY",
	"YY'''^^^^^YYYYYYYYYYYYYYYY''''''''''''YYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YY''''''''YYYYYYYYYYYYYYYYY'''''''''''YYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YY''''''^^YYYYYYYYYYYYYYYYY&&&&&&&&&&&&&&YYYYYYYYYYYYYYYYYYYYYYYY",
	"YY''^^^^^^^^^^^^^^'''''''''''''''''''''''''''''''''''''''''''''YY",
	"YY^^^^^^^^^^^^^^^^^^'''''''''''''''''^^^^^^^^^'''''''''''''''''YY",
	"YY^^^^^^^^^^^^^^^^^^''''''''''''''^^^^^^^^^^^^^^^^^^^^^^^^'''''YY",
	"YY''^^^^^^^^^^^^^^''''''''''''''''^^^^^^^^^^^^^^^^^^^^^^^^.....YY",
	"YYYYYYYYYYYYYYYYYYYYYYY'''''''''''YYYYYYYYYYYYYYYYYYYYYYYY.....YY",
	"YYYYYYYYYYYYYYYYYYYYYYY&&&&&&&&&&&YYYYYYYYYYYYYYYYYYYYYYYY=====YY"
};

static vector <string> rustboro1_map={
	
	"~~~~''|_____________________:::::=====:::::::::::::::::::::::::|",
	"~~~'''|:::::::::::::::::::::::::|.....|::::::::::------------::|",
	"~~''YY|:::--------------------::|.....|:::-------|||||||||||||:|",
	"~~''YY|:::||||||||||||||||||||:::...@.::::|''''''|||||||||||||:|",
	"~~''YY|:::||||||||||||||||||||:::.....::::|''''''| []  |  [] |:|",
	"~~''YY|:::|  []  []  []  []  |:::.....::::|------|-----|-----|:|",
	"~~''YY|:::|  []  []  []  []  |:::.....::::||||||||||||||:::::--|",
	"~~''YY|:::|                  |:::.....::::||||||||||||||:::::::|",
	"~''YYY|:::####################::|.....|:::| []  |  []  |:::::::|",
	"~''YYY|:::::::::::::::::::::::::|.....|...|     |      |:::::::|",
	"~''YYY|::::::::::::::::::::::::::@....::::##############:::::::|",
	"~''YYY|---------::::-------------.....:::::::::::::::::::::::::|",
	"~YYYYY||||||||||::::|   || ||   |.....|::::::::::::::::::::::::|",
	"~YYYYY||||||||||::::|   || ||   |.....|::::::::@:::::::::::::::|",
	"~YYYYY|[] |  []|::::|-|  | |PC|-|.....:::::::::::::::::::::::::|",
	"~YYYYY|   |    |::::| |  | |  | |.....::::::::::::#####::::::::|",
	"YYYYYY|########|::::########444#|.....:::::::::::#~~~~~#:::::::|",
	"YYYYYY|:::............................::::::::::::#####|||||||:|",
	"YYYYYY|:::............................:--------------::|||||||:|",
	"YYYYYY|:::::::::::::-------------.....:||||||||||||||::|  [] |:|",
	"-------:::::::::::::|  ||  ||   |.....:||||||||||||||::|[]   |:|",
	"|||||||||||||||:::::|  ||  ||   |.....:| []   |  [] |::|   []|:|",
	"|||||||||||||||:::::|-| |  |MART|.....:|      |     |::|[]   |:|",
	"| []  []  []  |:::::| | |  |    |......##############::#######:|",
	"| []  []  []  |:::::########555#|...................:::::::::::|",
	"| []  []  []  |:::::...............@................:::::::::::|",
	"| []  []  []  |:::::................................:::::::::::|",
	"###############:::::@....:::::::::::::::::::::::::::############",
	"|:::::::::::::::::::....###########################|YYYYYYYYYYYY",
	"###################|....|'''YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYY|====|YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"
};

static vector <string> rustboro2_map={
	"~~~~~~~YYYYYYYYYYYYYYYYYYYYYYYY======YYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"~~~~~~~YYYYYYYYYYYYYYYYYYYYYYYY''''''YYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"~~~~~~~YYYYYYYYYYYYYYYYYYYYYYYY''''''YYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"~~'''''''''YYYYYYYYYYYYYYYYYYYY'''''''''YYYYYYYYYYYYYYYYYYYYYYYY",
	"~~'''''''''''''''YYYYYYYYYYYYYY''''''###########################",
	"~~''|||||||||||||||||||||||||:|:::::::::::::::::::::::::::::::::",
	"~~''|||||||||||||||||||||||||:|:::::::::::::::::::::::::::::::::",
	"~~''|[] ||||||||||||||||| []|:|................................=",
	"~~''|[] |[][]|[]|[]|[][]| []|:|........@.......................=",
	"~~''|[] |[][]|[]|[]|[][]| []|:|................................=",
	"~~''|[] |[][]|[]|[]|[][]| []|:|.....::::::::::::::::::::::::::::",
	"~~''|[] |[][]|[]|[]|[][]| []|:|.....::::::::::::::::::::::::::::",
	"~~''|---|----|_____|----|---|:|.....::::::::::::::::::::::::::::",
	"~~''|########|  |  |#########:|.....############################",
	"~~''|:::::::#########:::::::::|.....|:::::::::::::::::::::::::::",
	"~~''|:::::::::::::::::::::::::|.....----------------:-----------",
	"~~''|:::::::::::::::::::::::::|.....||||||||||||||||:|||||||||||",
	"~~''|:::::::::::::::::::::::::|.....||||||||||||||||:|||||||||||",
	"~~''|:::::::::::::::::::::::::|.....| Gym|---| | []|:|[] |  []||",
	"~~''|:::::::::::::::::::::::::|.....|  | |---| |   |:|   |    ||",
	"~~''|:::::::::::::::::::::::::|.....######777#######:##########|",
	"~~''|::::|:::::::::::|::::::::|.....:::::::::::::::::::::::::::|",
	"~~''|::::|:::::::::::|::######|.....:::::::::::::::::::::::::::|",
	"~~''|::::::|:::::::|::::::@..........................@.........|",
	"~~''|::::::|:::::::|::::::.....................................|",
	"~~''|:::::::::::::::::::::.....................................|",
	"~~''|_____________________:::::=====:::::::::::::::::::::::::::|"
};

static vector <string> rustborogym_map={
	"###############################################",
	"##|||||||[]||[]||||||||||||||||[]||[]||||||||##",
	"##|||||||||||||||||||||||||||||||||||||||||||##",
	"##-------------------------------------------##",
	"##...........||........@......||.............##",
	"##...........|____........_____|.............##",
	"##...........................................##",
	"##############.................#######......###",
	"#######........................#######......###",
	"#######...............X........#######......###",
	"#####################=====#####################"
};

static vector <string> dewford1_map={
	"YY||||||||||||||||.......=====.....~~~~~~YYYYYYYYYYYYYYYYYYYYYY",
	"YY||||||||||||||||.................~~~~~~YYYYYYYYYYYYYYYYY..YYY",
	"..| ||   |  | || |.................~~~~~~YYYYYYYYYYYYYYYYYYYYYY",
	"YY| ||   |  |[]| |.................~~~~~~YYYY..YYYYYYYYYYYYYYYY",
	"YY################...@.............~~~~~~YYYYYYYYYYYYYYYYYYYYYY",
	"YY...........................YYYY..~~~~~~YYYYYYYYYY..YYYYYYYYYY",
	".....................||||||||||||||~~~~~~YYYYYYYYYYYYYYYYYYYYYY",
	"..||  |||||  || |....||||||||||||||~~~~~~~~~~~YYYYYYYYYYYYYYYYY",
	"..||  |||||  || |....|  ||  |[]|  |~~~~~~~~~~~~~~~~~~YYYYYYYYYY",
	"..---------------....|  ||  |[]|  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	"..|--|   |PC|---|....##############~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	"..|--|   |  |---|..................::::::::~~~~~~~~~~~~~~~~~~~~",
	"..####444########..................:::B::@:~~~~~~~~~~~~~~~~~~~~",
	"...................................~~~~~~~~....................",
	"...............................................................",
	".............@.................................................",
	"..............................................|||||||||||||||||",
	"..............................................|||||||||||||||||",
	"..............................................| ||  |[][]| || |",
	"YYY..................---------------------....| ||  |    | || |",
	"YYY..................|||||||||||||||||||||....#################",
	"YYY..................|||||||||||||||||||||..................YYY",
	"..YYY................|||||||||||||||||||||..................YYY",
	"YYYYYYY..............|[]|Gym|[]|---|  |[]|.........YYYYYYYYYYYY",
	"YYYYYYY..............|  |   |  |   |  |  |.....&YYY..YYYYYYY&&&",
	"YYYYYYYYYYY..........###########888#######....&&YYYYYYYYYYYYYYY",
	"YYYYYYYYYYY.....................................YYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"
};

static vector <string> dewford2_map={
	"............................~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	".............................~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	"&&&&&&&&&&............................~~~~~~~~~~~~~~~~~~~~~~~~",
	"&&&&&&&&&&.......................................~~~~~~~~~~~~~",
	"&&&&&&&&&&.......................................~~~~~~~~~~~~~",
	"&&&&&&&&&&....................................@..~~~~~~~~~~~~~",
	"&&&&&&&&............................................~~~~~~~~~~",
	"=&&&&&..............................................~~~~~~~~~~",
	"..............YY....................................~~~~~~~~~~",
	".............YY.YYY.................................~~~~~~~~~~",
	"..........YYY.YYY..YYY..................................~~~~~~",
	"YYYYYYYYYYYYYYYYYYYYYY...................=====..........~~~~~~"			
};

static vector<string> dewfordgym_map = {
    "###############################################",
    "##|||||||[]||[]||||||||||||||||[]||[]||||||||##",
    "##|||||||||||||||||||||||||||||||||||||||||||##",
    "##-------------------------------------------##",
    "##...........||........@......||.............##",
    "##...........|____........_____|.............##",
    "##...........................................##",
    "##...........................................##",
    "##...........................................##",
    "##.....................X.....................##",
    "#####################=====#####################"
};

static vector <string> granitecave_map={
	"&&&&&&&&&&&&#######################################&&&&&&&&&&&",
	"&&&&&&&&&###&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&###########",
	"#########&&&#######################################&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&##########################&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&########*************************###########################",
	"&&#***********************************************************",
	"&&#***********************************************************",
	"&&#***********************************************************",
	"&&#***********************############************************",
	"&&#*****************######&&&&&&&&&&&&#################*******",
	"&&##*******@*******##&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&#*******",
	"&&####************##&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&##......",
	"&&&&&&############&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&=====&"
};

static vector <string> slateport1_map={
	".=====''''''''''''||||||||||||||||||||||||||:::::::::::::::~~~",
	"......''''''''''''||||||||||||||||||||||||||:::::::::::::::~~~",
	"......''''''''''''|------------------------|~~~~~~~~[~~~~~~~~~",
	"......''''''''''''|------------------------|~~~~~[||[||]~~~~~~",
	"......''''''''''''|------------------------|~~~~~[|||||]~~~~~~",
	"......''''''''''''######################################~~~~~~",
	"......''''''''''''...@.................................#~~~~~~",
	".......................................................#~~~~~~",
	"...............................................#########~~~~~~",
	"......||||||||||||||||||||||||||...............#~~~~~~~~~~~~~~",
	"......||||||||||||||||||||||||||...............#~~~~~~~~~~~~~~",
	"......|[]  ||  []  |  []  || []|...............#~~~~~~~~~~~~~~",
	"......|    ||      |      ||   |...............#~~~~~~~~~~~~~~",
	"......##########################...............#~~~~~~~~~~~~~~",
	"...............................................#~~~~~~~~~~~~~~",
	"..@............................................#~~~~~~~~~~~~~~",
	"...............................................#~~~~~~~~~~~~~~",
	".......#########################################~~[~~~~~~~~~~~",
	".......#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[|||||]~~~~~~~~",
	".......#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[~~~~~~~~",
	".......#~~~~~~~~~~~~~~~~~~#########################[||||]~~~~~",
	".......####################.............................#~~~~~",
	".....................................''''##'''''........#~~~~~",
	".....................................'''####''''........#~~~~~",
	".....................................''''##'''''........#~~~~~",
	".....................................'''####''''..@.....#~~~~~",
	".....''###################..............................#~~~~~",
	".....''#.................#..............................#~~~~~",
	".....''#.................#######...............##########~~~~~",
	".....''#.......................#...............#.........~~~~~",
	".....''#.......................######.....######.........~~~~~",
	".....................................................~~~~~~~~~",
	"..B..................................................~~~~~~~~~"
};

static vector <string> slateport2_map{
	"''''''''====='''''''''|||||||||||||||||||||||~~~~~~~~~~~~~~~~~",
	"''''''''.....'''''''''|||||||||||||||||||||||~~~~~~~~~~~~~~~~~",
	"''''''''.....'''''''''|||||||||||||||||||||||~~~~~~~~~~~~~~~~~",
	"''''''''.....'''''''''|   []||   |   | []|| |~~~~~~~~~~~~~~~~~",
	"''''''''.....'''''''''|     ||   |   |   || |~~~~~~~~~~~~~~~~~",
	"''''''''.....'''''''''############QQQ########''~~~~~~~~~~~~~~~",
	"........................................'''''''~~~~~~~~~~~~~~~",
	"........................................'''''''''''''''''''#~~",
	"........................................'''''''''''''''''''#~~",
	"'''''''''''''|||||||||||||'''''''''''''''''''''''''''''''''#~~",
	"'''''''''''''|||||||||||||'''''''''''''''''''''''''''''''''#~~",
	"'''''''''''''||  || | PC||'''''''''''''''''''''''''''''''''#~~",
	"'''''''''''''||  || |   ||'''###############################~~",
	"'''''''''''''#####444#########~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~",
	".....@................'''#~~~~~~~~~~~~~~~~~~~~~~~&&&~~~~~~~~~~",
	"......................####~~~~~~~~~~~~~~~~~~~~~&&&&~~~~~~~~~~~",
	".................'''''#~~~~~~~~~~~~~~~~~~~~~~~&&&~~~~~~~~~~~~~",
	"''''''''''''.....'''''#~~~~~~~~~~~~~~~~~~~~~~~~~&&&~~~~~~~~~~~",
	"''''''''''''.....'''''#~~~~~################################~~",
	"''''''''''''.....'''''#~~~~~#'|||||||||||||||||||||'.......#~~",
	"||||||||||||.....'''''#~~~~~#'|||||||||||||||||||||'.......#~~",
	"||||||||||||.....'''''#~~####'|||||||||||||||||||||'.......#~~",
	"||  || MART|.....'''''#~~#...'| || ||     ||  ||  |'.......#~~",
	"||  || |   |.....'''''#~~#...'| || ||     ||  ||  |'.......#~~",
	"#####555####..........####...'#######99999#########'.......#~~",
	"................................@@@@@''''''''''''''........#~~",
	"................................''''''''''''''''''.........#~~",
	"'''''''''''................................................#~~",
	"'''''''''''.....''''#######................................#~~",
	"'''''''''''.....''''#~~~~~##################################~~",
	"'''''''''''=====''''#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
};

static vector <string> route110_1_map{
	"~~~~~~~~~~&&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#======#~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~&&&&~~~~~~~~~~~~~~~~~~~~~#'^^^''#~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&&~~~~#^^^^^'#~~~~",
	"||||||||||||~~~&&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#'''^^^#~~~~",
	"||||||||||||~~~~&&~~~~~~~~~~~~~~~~~&~~~~~~~~~~~#^^^''^#~~~~",
	"|  |  [][] |~~~~~~~~~~~~~~~~~~~~~~&&&~~~~~~~####^''^^'####~",
	"|  |       |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#'^^^^'^^^^'^#~",
	"############~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#''^^^^^^^^^^#~",
	"=..........#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#^^^^''^^^^^'#~",
	"=..........####################....##########''^^^^'^^^'^#~",
	"=..........''''''''''''''''''^^^'^^^^^^^^^'''^^^^^^^''^^'#~",
	"'''''......'''''''''''''''''''^^^^^''''^^^^^^^^^^^^^^''''#~",
	"YYY''.....''''''''''''''''^''''^''''^^''''''''^^^^^^^''''#~",
	"YYYYY@@.@@YYYY############################################~",
	"YYY''.....''''#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	"'''''=====''''#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
};

static vector <string> oceanic_museum_map{
	"###########|  |#####################|___|####|____|",
	"----.......|__|...........................[]........",
	"   |...................[   ].....[   ]....[].......",
	"___|...............................................",
	"...................................................",
	"----.......................@..............[].......",
	"   |.............@.....[   ]......[   ]...[].......",
	"___|....@..........................................",
	"...X...............................................",
	"##===######|  |#####################|___|####|____|"
};

static vector <string> route110_2_map{
	"#======#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	"#^^^^''###################################################~",
	"#^^^^^^^'''^^'''''''''''''^^''''^^^^^^'''^'''^''^''''^^^^#~",
	"#'^''^'@''^^''''''''''^^^^'''''''^^''''^^^^^^^^^^^^^@^^^^#~",
	"#############################################^^^^'^^^^^'^#~",
	"~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#''^^^^^^^^^^#~",
	"~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#''^^^^^^^^''#~",
	"~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#^^^^''''^^^^#~",
	"~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~''''''~~`#'''^^^^^''^^#~",
	"~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~''''''~~~#^^^^^^^^^^^^#~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~''''~~~#'''^^^^^^^^'#~",
	"~~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~#^^^''''^^^^^#~",
	"~~~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~####'''^''####~",
	"~~~~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~~#^^^'''#~~~~",	
	"~~~~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~~#^^^^^^#~~~~",	
	"~~~~~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~#''^^^^#~~~~",	
	"~~~~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~~#^^^^^^#~~~~",	
	"~~~~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~~#''''''#~~~~",	
	"~~~~~~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~#''''''#~~~~",	
	"~~~~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~~#''^^^^#~~~~",	
	"~~~~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~~#^^^^^'#~~~~",	
	"~~~~~~~~~~~~~~~~~~~~~~&&&~~~~~~~~~~~~~~~~~~~~~~#''@^^^#~~~~",	
	"~~~~~~~~~~~~~~~~~~~~&&&~~~~~~~~~~~~~~~~~~~~~~~~#^^^^^^#~~~~",	
	"~~~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~~~#^^^^^^#~~~~",	
	"~~~~~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~#'^^^^^#~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~&&&~~~~~~~~~~~~~~~~~~~~~~#======#~~~~",
};

static vector <string> mauville_map={
	"'''''''''''''''''''''''=====''''&''YYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"||||||||||||||||''&''''.....''''&''''''''&&&&&&&&&&&&&||||||||||",
	"||||||||||||||||''&''''.....|||||||||||||&''@''''''@'&||||||||||",
	"||||||||||||||||&&&''''.....|||||||||||||&'''''''''''&|| ||  []|",
	"|[]|Gym|[]|-|[]|'''''''.....|| ||___PC__|&'''''''''''&|| ||    |",
	"|  |   |  |-|  |'''''''.....|| ||   ||  |'''''''''''''##########",
	"####vvv#########'''''''.....#444#########''''''''''''''.....''''",
	"''''.....''''''''''''''.....''.....''''''''''''''''''''.....''''",
	"''''.....''''''''''''''.....''.....''''''''''''''''''''.....''''",
	"=..............................................................=",
	"=...........@............................@.....................=",
	"=..............................................................=",
	".'''|||||||||||||||||...''''''''''''''''''''.....'''''''''''''''",
	".'''|||||||||||||||||...''''''''''''''''''''.....'''''''''''''''",
	"''''||  ||#####||  ||...'''|||||||||||||''''.....''|||||||||||||",
	"''''||  ||#####||  ||...'&&|||||||||||||''''.....''|||||||||||||",
	"''''####||#####||####...'&'|| ||_MART__|''''.....''|| ||[][] |||",
	"''''''''#########''''...'&'|| ||  ||   |''''.....''|| ||     |||",
	"&&&&&&&&'''''''''''''...'&'#555#########''''.@...''#############",
	"YYY''''&&&&&&&&&&&&&&&&&&&.......................''''''''''''YYY",
	"YYYYYY''''''''''''''''''''.......................''''''''''''YYY",
	"YYYYYYY'''''''''''''''''''.......................''''''''''YYYYY",
	"YYYYYYYYYYYYYYYYYYY'''''''.....''''''''''''''''''''''''''''YYYYY",
	"YYYYYYYYYYYYYYYYYYY'''''''=====&&&&&&&&&&&&&&&&&&&&&&&'''''YYYYY"
};

static vector <string> mauvillegym_map={
	"###############################################",
    "#############[]|||||||||||||||[]###############",
    "#############[]|||||||||||||||[]###############",
    "#############[]---------------[]###############",
    "##...........||........@......||.............##",
    "##...........|____........_____|.............##",
    "##...........................................##",
    "##...........................................##",
    "##...........................................##",
    "##.....................X.....................##",
    "#####################=====#####################"
};

static vector <string> route111_1_map={
	"&&&&&&........''''''''====='''....&&&&&&&|||||[][]||||||&&&&&&&&",
	"~~~~~~~~~~'''''''''''''''''''''.....&&&&&|||||[][]||||||&&&&&&&&",
	"~~~~~~~~~~YYYYY''''''''''''''''''YYY&&&&&|||||||||||||||&:&&&&&&",
	"''''''''''YYYYY''''''''''''''''''YYY&:&&&|  ||[][]||   |&:&&&&&&",
	"''''''''''YYYYYYY'''''''''''''''''''&:&&&|  ||[][]||   |&:&&&&&&",
	"''''''''''''''''''''''''''''''''''''&:&&&|  ||[][]||   |&:&&&&&&",
	"||||||||||||####YYYY''''''''''''''''&:&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"|----------|''''YYYY''''''''''''''''&:&:::::::::::::::::::&&&&&&",
	"|[] |  | []|''''YYY'''''''''''''YYYY&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"|   |  |   |''''YY''''''''''''YYYYYYY&&&&&&&&&&###&&&&&#########",
	"############'''''''''''YYYYY''''Y''Y..................&#&&&&&&&&",
	"~~~~~~''''''''''''''@'''YYYY''''......................&#&&&&&&&&",
	"~~~~~~~~~YYYYY''''''''''''''''''''''''''''''''''''''''&#&&&&&&&&",
	"'''''''''YYYYYYY''''''''''''''''@''''YYY#&&&&&&&&&&&&&&#&&&&&&&&",
	"&&&&&&&&&&YYYYYYYYYYY'''''''.....'''YYYY#&&&&&&&&&&&&&&#&&&&&&&&",
	"&&&&&&&&&&&&''YYYYYYYYYYY''''''''YYYYYYY###&&&&&&&&&&&&#&&&&&&&&",
	"&&&&&&&&&&&&&&YYYYY'''''&&&&'''''&YYYYYYYY##&&&&&&&&&&&###&&&&&&",
	"&&&&&&&&&&&&&&&&#YYYYY''&'''.....&YYYYYYYYY######&&&&&&&&#&&&&&&",
	"&&&&&&&&&&&&&&&&#''''''Y&Y''.....&'YYYYYYYYYYYYY#&&&&&&&&#&&&&&&",
	"&&&&&&&&&&&&&&&&#''''''&&Y''.....&'YYYYYYYYYYYYY#&&&&&&&&#&&&&&&",
	"&&&&&&&&&&&######''''''&YY'''''''&&&&YYYYYYYYYYY#&&&&&&&&#&&&&&&",
	"&&&&&&&&&###YYYYYY'&&&&&YY''.....'''&&&&&&&''''''#########&&&&&&",
	"&&&&######'''&&&&&&&YYYY''''....@'''''''''&&&&&YYYYY''&&&##&&&&&",
	"#####YYYYYYYY&&YYYYY''''''''.....'''''''''''''&&&&&&&YY'&&##&&&&",
	"YYYYYYY&&&&&&&''''''''''''''.....'''''''''''''YYYYYY&Y'''&&###&&",
	"YYYYYYY&&'''''''''''''''''''.....'''''''''''''YYYYYY&&YY''&&&#&&",
	"''''''&&'........................''''YYY'''''''''''''&YYY'''&###",
	"&&&&&&&''........................''''YYY'''''''''''''&''''''&&&&",
	"'''''''''@.......................''''YYYY''''''''''''&&&&&&&&'''",
	"'''''''''.....''''''''''''''''''''''''''''''''''''''''YYY''''&&&",
	"'''''''''.....'''&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&YY",
	"'''''''''.....'''&''YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"'''''''''====='''&''YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"
};

static vector <string> route111_2_map={
	"''''''''''''''''=====''''#&&&&&&&&&&&&&&&&&#&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&#'''''''.....#&&&#&&&&&&&&&&&&&&&&&#&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&#'''''''.....#&&&#&&&&&&&&&&&&&&&&&#&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&#'''''''.....#&&&#&&&&&&&&&&&&&&&&&#&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&#'''''''.....#&&&#&&&&&&&&&&&&&&&&&#&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&#YY'''''.....#&&&#&&&&&&&&&&&&&&&&&###&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&#YYY''''.....#&&&###########&&&&&&&&&#&&&&&&&&&&&&&&&&&&",
	"&&&&&####'''''''...@.#&&&&&&&&&&&&&#&&&&&&&&&#&&&&&&&&&&&&&&&&&&",
	"&&&&&#''''''''''.....######&&&&&&&&#&&&&&&&&&#&&&&&&&&&&&&&&&&&&",
	"&&&&&#''''''''''.....'''''#&&&&&&&&#&&&&&&&&&####&&&&&&&&&&&&&&&",
	"&&&&&#''''''''''.....'''''#&&&&&&&&###&&&&&&&&&&#&&&&&&&&&&&&&&&",
	"&&####''''''''''.....'''''#####&&&&&&#&&&&&&&&&&#&&&&&&&&&&&&&&&",
	"&##Y''''''''''''.....'''''''''#&&&&&&#&&&&&&&&&&#&&&&&&&&&&&&&&&",
	"&#YY''''''''''''''''''''''''''#&&&&&&#&&&&&&&&&&#&&&&&&&&&&&&&&&",
	"##YY''''''''''''''''''''''''''#&&&&&&###&&&&&&&&#&&&&&&&&&&&&&&&",
	"YYYY'''''@''''''''""'YYYYYYYYY#&&&&&&&&#&&&&&&&&#&&&&&&&&&&&&&&&",
	"YY'''''''''''''''''''YYYYYYYYY#&&&&&&&&#&&&&&&&&#&&&&&&&&&&&&&&&",
	"YY'''''''''''''''''''''YYYYYYY##&&&&&&&####&&&&&####&&&&&&&&&&&&",
	"'''''''''''''''''''''''''''''''#&&&&&&&&&&#&&&&&&&&#&&&&&&&&&&&&",
	"YYY''''''''''''''''''''''''....#&&&&&&&&&&#&&&&&&&&#&&&&&&&&&&&&",
	"YYYYY''''''''''''''''''''''....#&&&&&&&&&&#&&&&&&&&#&&&&&&&&&&&&",
	"&&&&&&&&&&&&'''''''''''''''....##&&&&&&&&&###&&&&&&##&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&.......'''''.....#&&&&&&&&&&&#&&&&&&&#&&&&&&&&&&&",
	"&&&&&&&&&&&&&#&&&&&#..''''''''''########&&&&#&&&&&&&#&&&&&&&&&&&",
	"&&&&&&&&&&&&&#&&&&&#....''''''''..&&&.#&&&&&#&&&&&&&#&&&&&&&&&&&",
	"&&&&&&&&&&&&&#&&&&&#....''''''''.&&&&.#&&&&&#&&&&&&&#&&&&&&&&&&&",
	"&&&&&&&&&&&###&&&&&#...&&&''''''......###&&&###&&&&&##&&&&&&&&&&",
	"&&&&&&&&&&#&&&&&&###..&&&&&'''''...&&&..#&&&&&#&&&&&&#&&&&&&&&&&",
	"###########&&&&&&#.....'''''''''......@.#&&&&&#&&&&&&#&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&##.....'''''''''........#&&&&&##&&&&&###&&&&&&&&",
	"&&&&&&&&&&&&&&&##....''''''''''''''.....###&&&&###&&&&&###&&&&&&",
	"################...'''====='''''''''......##&&&&&##&&&&&&##&&&&&"
};

static vector <string> route111_3_map={
	"&&&&&&&&&&&&&&&&&&&####...=====..#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&###.............#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"##################...............#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"YYYYYY''''''''''''''''''.........#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"YYYYYY''''''''''''''''''''''.....#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"YYYY....'''''''''......''''''''''#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"'''....'''''''''''.....''''''''..#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"'''''''''''''''''''''''''''''''''#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"''''''''''''''''''''........''''.##&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"=.................................#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"=.................................#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"=..............................####&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"''''''''''''''''.....''''''''''#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"''''''''''''''''.....'''''''####&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"''''''''''''''''====='''''''#&&&&&&&&&&&&&&#&&&&&&&&&&&&&&&&&&&&"
};

static vector <string> route111_desert_map={
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.........&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.......,,,,&&&&",
	"...................,,,,,.........,,,,,,,,,,,,,,,,..........&&&&",
	".................,,,,,,,........,,,,,,,,,,,,,,,,,..........&&&&",
	".................,,,,,,,.......,,,,,,,.....,,,,,,...........&&&",
	"................,,,,,,,,,,,,,,,,,,,,,.......................&&&",
	"................,,,,,,,,,,,,,,,,,,,,,.......................&&&",
	"..............,,,,,,,,,,,,,,,,,,&&,,,,,,,,,.............&&&&&&&",
	"..............,,,,,,,,&&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&",
	"...........,,,,,,,,,,&&&&,,,,,,,,,&&,,,,,,,,,,,,&&&&&&&&&&&&&&&",
	"...........,,,,&&,,,,,,,,,,,,,,,,,,,,,@,,,,,,,,,&&&&&&&&&&&&&&&",
	"...........................,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&",
	"....................................,,,,,,,,,,,,,,,,&&&&&&&&&&&",
	"....&&&,,,,,,,,.....................,,,,,,,,,,,&&&,,&&&&&&&&&&&",
	"&&,&&&&,,,,,,,,,,,,,,,,,,,,,,,,.............,&&&&&,,,,&&&&&&&&&",
	"&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,.............,,,,,,,,,,&&&&&&&&&",
	"&&,,,,,,,,,,,,@,,,,,,,,,,,,,,,,,,,..............,,,,,,,,&&&&&&&",
	"&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,..............,,,,,,,,&&&&&&&",
	"&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,...........,,,,,,,,&&&&&&&",
	"&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,...............,,,,,,&&&&&",
	"&&,,,,,,,,,,,,,,,,,-----,,,,,,,,,,,,,...............,,,,,,,,,&&",
	"&&,,,,,,,,,,,,,,,,,|   |,,,,,,,,,,,,,...............,,,,,,,,,&&",
	"&&,,,,&&,,,,,,,,,--|---|--,,,,,,,,,,,,,,,,..........,,,,,,,,,&&",
	"&&,,,&&&&,,&&,,,,| |' '| |,,,,,,,,,,,,,,,,,,........,,,,,,,,,,,",
	"&&,,,,,,,,,,,,,,,| |___| |,,,,,,,,,,,,,,,,,,.....,,,,,,,,,&&&&,",
	"&&,&&,,,,,,,,,,,,| |' '| |,,,,,,@,,,,,...........,,,,,,,&&&&&&,",
	"&&,,,,,,,,,,,,,,,| |###| |.......................,,,,,,,,,,,,,,",
	"&&,&&,,,.........................................,,,,,,,,......",
	"&&,,,,,,.................,,,,,,,.................,,,,,,,,......",
	"&&,,,,,......&&&&&.......,,,,,,,.................,@,,,,,,......",
	"&&...=====..&&&&&&&......,,,,,,,.................,,,,,,,,......"
};

static vector <string> route112_1_map={
	"&&&&...YYYY&&&&&&&&&&&&&&&&&&YYY&&YYYYYYY########=====######YYYYYY",
	"&&&&&...YYY&&&&&&&...YYYYY&&&&&&&&YYYYYY.....................YYYY&",
	"&&&&&&&&&&&&...YYYY&&&&&&&&&&&&&&YYYYYY.....................YYYYY&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&..YYY.......................YY&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&...YY..................@....YY&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&................................YY&&&&&&",
	"&&&&FFFFF&&&&&&&&&&&......................................YY&&&&&&",
	"..........................................................YY&&&&&&",
	".......................................................YYYYY&&&&&&",
	"..................................................YYYYY&&&&&&&&&&&",
	"..................@...............................&&&&&&&&&&&&&&&&",
	"&&&&&&&.....................:::::.......&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&:::::&&&&&&&&&&&&&&&&&&&&&&&&&&YYYYYYY",
	"YYYYY#'#''''''#&&&&&&&&&&&&&:::::&&&&&&&&&&&&&&^^^^^^^^^YYYYYYYYYY",
	"YYYYY#'#''''###'#'''''''''''.....'''''''''''''^^^^^^^^^^^^^^^YYYYY",
	"YYY###'#''''#'''#'''''''''''.....'''''''''''''^^^^^^^^^^^^^^^''YYY",
	"YYY#'''#''''#'''#'''''''''''.....''''''''''&&&&&&&&&&&&^^^^^^'''''",
	"YYY#'''#''''#'''#'''''''''''.....''''''''''&&&&&&&&&&&&&&'''''''''",
	"YYY#'''#''''#'''#'''''''''''.....'''''''''''''&&&&&&&&&&&'''''''''",
	"YYY#'''#''''#'''#'''''''''''.....'''''''''''''''''''''&&&''''.....",
	"'''#'''#''''#'''#'''''''''''.......................@.............=",
	"'''#'''#''''#'''#'''''''''''.....................................=",
	"'''#'''#''''#'''#'''''''''''.....................................=",
	"''YYYYYYYYYYYYY'YY######################YYYYYYYYYY###########&&&&&",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY&&&&&&",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY&&&&&&"
};

static vector <string> fierypath_map={
	"&&&&&&&&&&***********&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&*************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&********************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&********************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&*************************&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&*************************&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&*************************&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&**************************************&&&&&&&&&&&&&&&",
	"&&&&&&&&&&**************************************&&&&&&&&&&&&&&&",
	"&&&&&&&&&&**************************************&&&&&&&&&&&&&&&",
	"&&&&&&&&&&**************************************&&&&&&&&&&&&&&&",
	"&&&&&&&&&&**************************************&&&&&&&&&&&&&&&",
	"&&&&&&&&&&**************************************&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&***********&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&***********&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&***********&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&***********&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&***********&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&***********&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&***********&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&***********&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&***********&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&*****X*****&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&***=====***&&&&&&&&&&&&&&&&&&&&&&&&&&",
};

static vector <string> route112_2_map={
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"---------...&&&....&&&&&&&&&&&&......&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&.--------.&&&&&&&&&&&&......YYYYYYYYYYYYYYYYYY......&&&&&",
	"&&&&&&&&&...&&&...----------.........YYYYYYYYYYYYYYYYYY......&&&&&",
	"&&&&&&&&&...&&&....&&&&&&&&&-------##.YYY.....YYYYYYYYY......YY&&&",
	"&&&&&&&&&...&&&....&&&&&&&&&&&&....##.......|||||||||||||||.....YY",
	"&&&&&&&&&...&&&....&&&&&&&&&&&&....##_______||||||||||||||||||..YY",
	"&&&&&&&&&...&&&....&&&&&&&&&&&&....##.......|    ||||||    |||||YY",
	"&&&&&&&&...&&&....&&&&&&&&&&&&&..Y.##......Y|    |____|       ||YY",
	"&&&&&&&&...&&&...&&&&&&&&&&&&&&..YY###YYY..Y|    |    |        |YY",
	"&&&&&&&....&&&..&&&&&&&&&&&&&&&&&&&YYYYYYYYY#####|KKKK|#########YY",
	"&&&&&&YY...&&YY.&&&&&&&&&&&&&&&&&&&YYYYYYY....................YYYY",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&YYY&&YYYYYY....................YYYYY",
	"&&&&...YYYY&&&&&&&&&&&&&&&&&&YYY&&YYYYYYY########=====######YYYYYY"
};

static vector <string> cablecar_map={
	"###################################",
	"|__|||___[][]___|||___[][]___|||__|",
	"|__|||__________|||__________|||__|",
	"|############| [   ] |############|",
	"|############| [   ] |############|",
	"|____________---------____________|",
	"|::::::::::::::::@::::::::::::::::|",
	"|:::::::::::::::::::::::::::::::::|",
	"|:::::::::::::::::::::::::::::::::|",
	"|::::::::::::::::X::::::::::::::::|",
	"|###############=====##############"
};

static vector <string> mtchimney_1_map={
	"&&....&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&=====&&&.....&&&...&&&...&&&&",
	"&&..........&&&&,,,&&&&&&&&&&&&&&&&&&.....&&......&&&...&&&...&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&......................&&&...&&&...&&&&",
	"...........&&&&&&&&&&&&&&&&&................&&&&&&&&&...&&&...&&&&",
	"##########################&&...|.....:::::&&&&........&&&&&...&&&&",
	"#.....|||||||||||||||__________#&&&&&:::::&&&&....&&&&&&&.....&&&&",
	"#..||||||||||||||||||..........#&&&.......&&&&....&&&......&&&&&&&",
	"#||||        |||||| |..........#..........&&&&....&&&...&&&&&&&&&&",
	"#|           |    | |..........#..........&&&&....&&&...&&&&&&&&&&",
	".#############KKKK###...................&&&&&&....&&&...&&&&&&&&&&",
	"#...............X.......................&&&&......&&&...&&&&&&&&&&",
	"#.....................................&&&&......&&&&&...&&&&&&&&&&",
	"#.....................................&&&&......&&......&&&&&&&&&&",
	"#....................................&&&&......&&......&&&&&&&&&&&",
	"########################.......##########..&&&&&&..&&&&&&&&&&&&&&&",
	"##&&..##...#####....####=====########&&&&&&....&&&&&&&...&&&&&&&&&"
};

static vector <string> mtchimney_summit_map={
	"....&&&.&&&.&&&.&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&...&&&.&&&&&",
	"....&&&.&&&.&&&.&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&...&&&.&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%&...&...&&&&&",
	"......................[+]&%%%%%%%%%%%%%%%%%%%%%%%%%%%&...&.&&&&&&&",
	"..................@.....|&%%%%%%%%%%%%%%%%%%%%%%%%%%%&..&&.&&&&&&&",
	"........&&&&&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%&..&&.&&&&&&&",
	"........&&&.&&&.&&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%&..&&.&&&&&&&",
	"&&&.......&&&&&&&.............&%%%%%%%%%%%%%%%%%%%%%%&..&&.&&&&&&&",
	"&&&&.............&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%&&&&&&&&&&&&&&&&",
	"&&&&&&.......&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&...........&&&&&",
	"&&&&&&&.......&&...................................&&&&&&&&&&&&&&&",
	"&&&&&&&&.......&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&............&&",
	"&&&&&&&&&........&&&&&&&.......&&&&&&&&&&&....&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&........................................................",
	"&&&&&&&&&&........@...............................................",
	"&&&&&&&&&&&&......................................................",
	"&&&&&&&&&&&&......................................................",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&........@...................",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&..............&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&..=====...&&&&&&&&&&&&&&"
};

static vector <string> jagged_pass_map={
	"&&&&&&&TTTTTTTTTT......=====..........TTTTTTTTTTTT.&&.TTT.&&.TTT.",
	"TTT..&&&TTTTTTTTTT...........@......TTTTTTTTTTTTT.&&&.TTT&&&&&&&&",
	".TTT..&&TTTTTTTTTT..................TTTTTTTTTTTTT.&&&....&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&.:::::....&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"...TTTTTTTTTTT......&............&..&TTTTTTTTTTTTT..&&&TTTTTT&&&&",
	"&&&&TTTTTTTT&&&&&&&&...:::::....&&&&&&&TTTTTTTTT&&&&&&&&&&&&&&&&&",
	"&&&&&TTTTTTT&&&&&&&&.....................TTTTTTT.............&&&&",
	"&TTT&&&&&&&&&&&&&&&::::::::::::::::::::::TT&&&&&&&&&&&&&&&&&TTT&&",
	"TT............&&&&&...........:::::.......T...........TTTT...TTT.",
	"&&&TTTT&&&&&&&&&&&&.......................TTT&&&&&&&&&&&&&TTTT&&&",
	"&&&TTTTTTTT^^^^^^^:.....@................:.........TTT........&&&",
	"......TTTTT^^^^^^^::::::::::::::::::::::::........TTTTT.........&",
	"&&&&&&&TTTT^^^^^^^^.......:::::............&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&......&&&&&&&&&........................&&&.....TTTTTT..TTT&&",
	"&&&&&&&&&&&&&TTTTTTTT::::::::::::::::::....::&&&&&&&&&TTTT&&&&&&&",
	".....TTTT......TTTTTT...............@....:::......TTTTTT...TTTTTT",
	"&&&&&&&&&TTTTTTT:::::::::::::::..............:::::&&&&&&&&&&&&&&&",
	"&&&&&&&&&&TTTTTTT.................&&&...^^^^^^^^^^TTTTT....TTTTTT",
	"TTTTT......TTTTTTT::::::::::::::&&###&&^^^^^^^^^^^TTT&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&..............&&JJJ&&^^^^^^^^^^^TTT&&&&&&&&&&&&",
	"&&&&&&&&TTTTT::::::::::::.....::::::::&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"TTTTTT.............................&&&&&TTTTT........TTTTTT....TT",
	"&&&&&&&&&&&:::::::::::::::::::::::&&&&&&&TTTT&&&&&&&&&&TTTTTT&&&&",
	"&&&&&&&&&&&&&&&&...@.............&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"YYYYYYYYYYYYYYYYY...............YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYY.............YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYY..=====....YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"
};

static vector <string> lavaridge_map ={
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&YYYYY=====YYYY",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&YYYYY'''''YYYY",
	"&&.&############''|||||||||||||''''|||||||||||||YYYYYYYYY'''''YYYY",
	"&&.&#~~~~~~~~~~#''|||||||||||||''''|||||||||||||YYYYYYY''''''''YYY",
	"&&.&#~~~~~~~~~~#''| |  | | PC |'YY'| | | | MART|''''''''''''''''''",
	"&&.&#~~~~~~~~~~#''| |  | |    |'YY'| | | |     |''''''''''''''''''",
	"&&.&############''###444#######''''##555########''''''''''''''''''",
	"&&.&......@@....''''.....''''''''''''.....''''''''''''''''''''''''",
	"&&.&............''''.....''''''''''''.....''''''''''''''''''''''''",
	"&&.&.........'''''''..............................................",
	"&&.&..............................................................",
	"&&.&.........................................@....................",
	"&&.&''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''",
	"&&.&||||||||||||||||''''''''''''''''''''''''''''''''''''''''''''''",
	"&&.&||||||||||||||||'''''''''|||||||||||'||||||||||||'''''''''''''",
	"&&.&||||||||||||||||'''''''''|||||||||||'||||||||||||YY'''''YYYYYY",
	"&&.&|[]|Gym|[]|---||'''''''''| | |[]   |'| | | [] []|YY'''''YYYYYY",
	"&&.&|  |   |  |   ||'''''''''| | |     |'| | |      |YY'''''YYYYYY",
	"&&.&####EEE#########'''''''''###########'############YY'''''YYYYYY",
	"&&.&''''''''''''''''''''''''''''''''''''''''''''''YYYYY'''''YYYYYY",
	"&&.&''''''''''''''@''''''''''''''''''''''''''''''''YYYYY''''YYYYYY",
	"&&.&''''''''''''''''''''''''''''''''''''''''''''''''''''''''YYYYYY",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"
};

static vector <string> lavaridgegym_map={
	"#########################################",
    "##########[]|||||||||||||||[]############",
    "##########[]|||||||||||||||[]############",
    "##########[]---------------[]############",
    "##........||........@......||..........##",
    "##........|____........_____|..........##",
    "##.....................................##",
    "##.....................................##",
    "##.....................................##",
    "##..................X..................##",
    "##################=====##################"
};

static vector <string> harbor_map={
	"......##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	"......##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	"......##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	"......##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	"......##~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~~~~~~",
	"......##~~~________[:|::::]___~~~~~~~~~~~~~",
	"......##~~[::[  ]||[:|::::]:  |~~~~~~~~~~~~",
	"......##~~[::[  ]||[------]:  |~~~~~~~~~~~~",
	"......##~~[::[  ]::- - - - - ]~~~~~~~~~~~~~",
	"......##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	"......########...##########################",
	".............@.............................",
	"...............................X...........",
	".............................=====........."
};

static vector <string> route118_map={
	".....#''YYYYYYYYYYY&&&...........&&&...&&&YY=====YY&&&&&&&&&&&&",
	".....#'''YYYYYYYYYY&&&.....&&&&&&&&&...&&&YY.....YY&&&&&&&&&&&&",
	".....#'''YYYYYYYYYY&&&.....&&&&&&&&....&&&YY.....YY&&&&&&&&&&&&",
	"~~&&&####YYYYYYYYYY&&&.....&&&&&&&&&&&&&&&YY.....YY&&&&&&&&&&&&",
	"~~.&&...#YY''......&&&&&&&&&&YYYYYYYYYYYYY^'....@.^^^^^^^^^^'''",
	"~~......#'Y''''''''''''''''''YYYY^^^^^^^^^^'.....^^^^^^^^^^^^''",
	"=.......#''''''''''''''''''''''''''^^^^^^^^''''''^^^^^^^^^^^^''",
	"=.......#YY'''#############################''''''##############",
	"=.......####'''''''''''''''''^^^^^^^^^''''''''''''''''^^^^^^^^^",
	"~~~~.....@.#'''''''''''''''''^^^^^^^^^^^^'''''''''''''''^^^^^^^",
	"~~~~.......#''''''''''''''''''^^^^^^^^^^^^'''''''''@''''^^^^^^^",
	"~~~~.......#########'''''''&&&&''YY^^^^^^^'''''''''YYYYYY''''''",
	"~~~~......................&&&&&&'YYYY^^^^^''YYYYYYYYYYYYYYY''''",
	"~~~~......................&&&&&..YYYYYYYYYYYYYYYYYYYYYYYYYYYY''",
	"~~~~.............................YYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"~~~~~~~..........................YYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"~~~~~~~..........................YYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"
};

static vector <string> route119_map={
	"YYYYYYY''''''''''''''''&~~~~~~~~~~~~~&YYYYYYYYYY''=====''VVVVVV",
	"YYYYYYY''''''''''''''''&~~~~~~~~~~~~~&YYYYYYYYYY''.....VVVV'VVV",
	"YYYYYYY''''''''&&''''''&''~~~~~~~~~~~&YYYYYY''''''....@VVVVVVVV",
	"&&&&&&&&&&&&&&&&&::::::&''~~~~~~~~~~~&YYYYYY''''''.....'VV'VVVV",
	"&&&&&&&&&&&&&&&&&'''''''''~~~~~~~~~~~&&&&&&&&&&'''.....'VVVVVVV",
	"&&&&&&&&&&&&&&&&&'''''''''~~~~~~~~~~~&&&&&&&&&&'''.....'VV'VVVV",
	"|||||||||||||||||||||&&&&&```````````&&&&&&&&&&&&':::::''VV'VV'",
	"|[###]_________[###]|&&&&&```````````&&&&&&&&&&&&::::::::&&&&&&",
	"|[###]_________[###]|''&&&```````````&&&&&&&&&&&&::::::::&&&&&&",
	"|[][][][][] [][][][]|''&&&''~~~~~~~''&&&'''''....@.......''&&&&",
	"|[][][][][] [][][][]|''&&&''~~~~~~~''&&&'''''............''&&&&",
	"|[][][][][] [][][][]|''&&&~~~~~~~~~~~&&&'''...........'''''&&&&",
	"#####################''&&&~~~~~~~~~~~&&&'''...........'''''&&&&",
	"YYYYYY'''..............::::::::::::::::...............'''''&&&&",
	"YYYYYY'''..............::::::::::::::::..............@''''''&&&",
	"YYYYYY'''.....'''''''''&&&~~~~~~~~~~~&&&'''::::::'''VVV'VVV&&&&",
	"'''''''''.....'''''''''&&&~~~~~~~~~~~&&&'''......''VVV'VVVV&&&&",
	"'''''''''.....'''''''''&&&~~~~~~~~~~~&&&'''......''VVVVVVVV&&&&",
	"'''''''''====='''''''''&&&~~~~~~~~~~~&&&'''......''VVVV'VVV&&&&"
};

static vector <string> fortree_map={
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY'''''''YYYYY",
	"YYYYY'''''''''''''YYY''''''''YYY''''''''YYY'''''''YY| | | |YYYYY",
	"YYYYY|||||||||||||...| |  | |YYY| |  | |YYY| | | |YY| | | |YYYYY",
	"YYYYY|||||||||||||...| |  | |YYY| |  | |YYY| | | |YY#######YYYYY",
	"'YYYY| |  | | PC |.@.########YYY########YYY#######YY....@..YYYYY",
	"'YYYY| |  | |    |...........:::........:::.......::.......YYYYY",
	"'YYYY###444#######YYYYYY:::YYYYYYYYYYYYYYYYYY::YYYYYYYY::YYYYYYY",
	"='''''''''''''''''YYY''''''YY||||||||||||||||''YYYYYYYY''YYYYY''",
	"=''''''''''''''''''''''''''YY||||||||||||||||'''YYYYYYY'''''''''",
	"=''''''''''''''''''''''''''YY||||||||||||||||'''YYYYYYY'''''''''",
	"'''''''''''''''''''''''''''YY||[] [] |   |[]|'''YYYYYYY'''''''''",
	"'''''''''''''''''''''''''''YY|| Gym  |   |  |''YYYYYYYY'''YYYYYY",
	"YY'''''''''''''''''''''''''YY#########ttt####''YYYYYYYY'YY''''''",
	"YY|||||||||||||''''''''''''YY'''''''''''''''''YYYYYYYYY'YY| || |",
	"YY|||||||||||||''''''''''''YYY@'''''''''''''''YYYYYYYYY'YY######",
	"YY| |  | |MART|'''''''''YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY'YY::::::",
	"YY| |  | |    |'''''''YY..''''''''..YYYYYYYYYYYYYYYYYYY'YYYY::YY",
	"YY##555########'''''''YY..| |  | |..YYYYYYYYYYYYYYYYYYY'YYYY::YY",
	"YY''''''''''''''''''''YY..| |  | |..YYYYYYYYYYYYYYYYYYY'YYYY::YY",
	"YY'''''''''''''@''''''YY..########..YYYYYYYYYYYYYYYYYYY'YYYY::YY",
	"YYYYYY''''''''''''''''YY.....:::::::::::::::::::::::::::::::::YY",
	"YYYYYYYYY'''''''''''''YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"
};

static vector <string> fortreegym_map={
	"###################################",
	"|__###___[][]___|||___[][]___|||__|",
	"|__|||__________|||__________|||__|",
	"|:::::::::|             |::::::::::",
	"|:::::::::|       @     |::::::::::",
	"|_________|__...........|_________|",
	"|:::::::::::::::::::::::::::[][][]|",
	"|:::::::::::::::::::::::::::[][][]|",
	"|[][][][][]:::::::::::::::::[][][]|",
	"|[][][][][]::::::X::::::::::[][][]|",
	"|###############=====##############"
};

static vector <string> lilycove1_map={
	"YYY'''''''''''''''''''''''''''''''''''''''''''''''''''''====='''",
	"YYYYYYYYYYYY########################################''''.....'''",
	"&&&&&YYYYYYY'''||||||||||''''''|||||||||||||||||''''''''.....'''",
	"~~~&&YYY'''''''||||||||||''''''|||||||||||||||||''''''''.....'''",
	"~~~&&YYY'''''..| | | [] |''''YY|-----|  |------|YY''''''.....'''",
	"~~~&&YYY''''...| | |    |''''YY|-----|  |------|YY''''''.....'''",
	"~~~&&YYY''.....##########''''YY#################''''''''.....'''",
	"~~~&&YYY'..................@...................................=",
	"~~~&&YYY'......................................................=",
	"~~~&&YYY'......................................................=",
	"~~~&&YYY.....||||||||||||||||'''''''''''''''''''''''''''''''''''",
	"~~~&&YYY.....||||||||||||||||&&&&&&&&&&&&&&&&&&&&&&&&'''''''''''",
	"~~~~&&Y'.....|---[]----[]---|~~~~~~~~~~~~~~~~~~~~~~~&'''''''''''",
	"~~~~&&''.....|----|    |----|~~~~~~~~~~~~~~~~~~~~~~~&'''''''''''",
	"~~~~&&''.....######QQQ#######~~~~~~~~~~~~~~~~~~~~~~~&'''''''''''",
	"~~~~&&''............B..'''''''''&&~~~~~~~~~~~~~~~~~~&&&&''''''''",
	"~~~~&&''...............'''''@'''&&~~~~~~~~~~~~~~~~~~~~&&''''''''",
	"~~~~&&'''.............''''''@'''&&~~~~~~~~~~~~~~~~~~~~&&&&''''''",
	"~~~~&&''''''''''''''''''''''''''&&~~~~~~~~~~~~~~~~~~~~~~~&''''''",
	"~~~~~&&&&&&&&&&&&&&&&&&&&&&&&&&&~~~~~~~~~~~~~~~~~~~~~~~~~&''''''",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&''''''",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&&&&&&",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
};

static vector <string> lilycove2_map={
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYY||||||||||||||||||||YYYYYYYYYYYYYY",
	"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYY||||||||||||||||||||YYYYYYYYYYYYYY",
	"YYYYY'''|||||||'''YYYYYYYYYYYY|  []    []    []  |YYYYYYYYYYYYYY",
	"--------|[] []|--------&YYYYYY|                  |YYYYYYYYYYYYYY",
	"|||||||||||||||||||||||&YYYYYY|  []    []    []  |YYYYYYYYYYYYYY",    
	"-----------------------&YYYYYY|                  |YYYYYYYYYYYYYY",
	"||  ||  |-----|  ||  ||&YYYYYY|  [] | 000 |  []  |YYYYYYYYYYYYYY",
	"|||||||||-----|||||||||&______|     |-----|      |______________",
	"&&&&&&&&&:::::&&&&&&&&&&''''''########555#########''''''''''''''",
	"='''''''............................@..........................=",
	"='''''''.......................................................=",
	"='''''''.......................................................=",
	"'''''''''''''''''''''''''''''''''''''|||||||||||||||''''.....'''",
	"&&&&&&&&&&&&&&||||||||||||||||'''''''|||||||||||||||''''.....'''",
	"YYY&&&&&&&&&&&||||||||||||||||'''''''|||||||||||||||'''#:::::'''",
	"YYY&&&&&&&&&&&|| ||   |[][]| |'''''''|| ||   | PC  |'''#:::::'''",
	"YYYYYY&&&&&&&&|| ||   |    | |'''''''|| ||   |     |'''#.....'''",
	"YYYYYYYYYYYYYY################_______#####444#######___#.....'''",
	"''''''''''''''..........@..............@.....................'''",
	"''''''''''''''..............................................@'''",
	"''''''''''''''...............................................'''",
	"YYY'''''''''''''''''''''''''''''''''''''''''''''''''''''====='''"
};

static vector <string> lilycove3_map={
	"&&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~......&&&&&&&",
	"&&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~...........&&&&&&",
	"YY'''''''''''''''''''''''''''''''''~~~~~~~........&&&.......&aaa&",
	"Y||||||||||||YYY||||||||||||''''''''''''''&.......&&&&..........~",
	"Y||||||||||||YYY||||||||||||''''''''''''''&&&.............~~~~~~~",
	"'| | | [][] |YYY| | | [] []|''''''''''''''&&&..........~~~~~~~~~~",
	"'| | |      |YYY| | |      |''''''''''''''&&&.......~~~~~~~~~~~~~",
	"'############YYY############''''''''''''''&&&.......~~~~~~~~~~~~~",
	"=....................@.'''''''''''''''''''&&&.......~~~~~~~~~~~~~",
	"=......................'''''''''''''''''''&&&.......~~~~~~~~~~~~~",
	"=......................'''''''''''''''''''&&&,,,,,...~~~~~~~~~~~~",
	"&&&&&&&&&&&&&&&&&&&&&&&''''''&&&&&&&&&&&&&&&&&&&&&&&....~~~~~~~~~",
	"&&|||||||||||||&'''''''''''''''''''''''||||||||||||'&,,..~~~~~~~~",
	"&&|||||||||||||&'''''''''''''''''''''''||||||||||||'&,,,,,,~~~~~~",
	"''| [] | | [] |&'''''''''''''''''''''''| |  |  [] |'&,,,,,,,~~~~~", 
	"''|    | |    |&'''''''''''''''''''''''| |  |     |'&,,,,,,,,~~~~",  
	"''############&''''''''''''''''''''''''############'&,,,,,,,,~~~~",  
	"..........'''''&&&&&&&&&&&&&'''''''''''''''''''''''''&,,,,,,,,,~~",
	"..........'''''''''''''''''&'''''''''''''''''''''''''&,,,,,,,,,~~",
	"'''''''''''''''''''''''''''&''''''''''''''''''''''''@&,,,,,,,,,~~", 
	"'''''''''''''''''''''''''''&'''''''''''''''''''''''''&,,,,,,,,,~~",
	"'''''''''''''''''''''''''''&''''''''''''''''''''''''':,,,,,,,,,~~",
	"'''''''''''''''''''''''''''&'''''''''''''':::::'''''':,,,,,,,,,~~",
	"'''''''''''''''''''''''''''&&&&&&&&&&&&&&&=====&&&&&&&,,,,,,,,,~~"
};

static vector <string> lilycove4_map={
	"'''''''||||||||''''''''''''''''''''''&&&&&=====&&&&&&&,,,,,,,,,.~",
	"'''''||||||||||||YYYYY'''''''''''''''''''&,,,,,,,,,,,,,.........~",
	"'''''||  ||[] []|YYYYY'''''''''''''''''''&,,,,,,,,,,,..........~~",
	"'''''||  ||     |YYYYY'''''''''''''''''''&,,,,,,,,.............~~",
	"'''''############YYYYY'''''''''''''''''''&,,,,,,,,,,,,,.......~~~",
	"=........................................:,,,,,,,,,,,.........~~~",
	"=........................................:,,,,,..............~~~~",
	"'''''''''''''''''''''''''''''''''''&&&&&&&,,,,.............~~~~~~",
	"'''''''''''''''''''''''''''''''''''&,,,,,,,,,,,,,,,,,...~~~~~~~~~",
	"'''''''''''''''''''''''&&&&&&&&&&&&&,,,,,,,.............~~~~~~~~~",
	"''''''''''&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,.............~~~~~~~~~~",
	"''&&&&&&&&,,,,,,,,,,,,,,,,,&&&,,,,,,,,,,,,,,,....,,,...~~~~~~~~~~",
	"''&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,........,,,,,...~~~~~~~~~~",
	"''&,,&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,..........,,,,,,,..~~~~~~~~~~",
	"''&,,&&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,.........,,,,,,,,..~~~~~~~~~~",
	"''&,,,,,,,,,,,,,,,,,,,,,,,&&&..........................~~~~~~~~~~",
	"''&&&&&&&,,,,,,,,,,,,,,,,,,,,,,...................&&&&&&&&&&~~~~~",
	"'''''''''&&&&&&&&&&&&&&,,,,,,,,,,,,...............&''''''''&~~~~~",
	"'''''''''''''''''''''''&&&&&&&&&&&&&&&&&&&&&&&&&&&'''''''''&~~~~~",
	"'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''&~~~~~",
	"'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''&~~~~~"	
};

static vector <string> aqua_entrance_map ={
	"#####################@=====@######################",
	"&&&&&&&&&&&&&&&&&&&&&:::::::&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&~~~~~~~~~~~~~~~~~~:::~~~~~~~~~~~~~~~~~~~&&&&&",
	"&&&&&~~~~~~~~~~~~~~~~~~:::~~~~~~~~~~~~~~~~~~~&&&&&",
	"&&&&&~~~~~~~~~~~~~~~~~~:::~~~~~~~~~~~~~~~~~~~&&&&&",
	"&&&&&~~~~~~~~~~~~~~~~~~:::~~~~~~~~~~~~~~~~~~~&&&&&",
	"&&&&&~~~~~~~~~~~~~~~~~~:::~~~~~~~~~~~~~~~~~~~&&&&&",
	"&&&&&~~~~~~~~~~~~~~~~~~:::~~~~~~~~~~~~~~~~~~~&&&&&",
	"&&&&&~~~~~~~~~~~~~~~~~~:::~~~~~~~~~~~~~~~~~~~&&&&&",
	"&&&&&~~~~~~~~~~~~~~~~~~:::~~~~~~~~~~~~~~~~~~~&&&&&",
	"&&&&&~~~~~~~~~~~~~~~~~~:::~~~~~~~~~~~~~~~~~~~&&&&&",
	"&&&&&~~~~~~~~~~~~~~~~~~:::~~~~~~~~~~~~~~~~~~~&&&&&",
	"&&&&&~~~~~~~~~~~~~~~~~~:::~~~~~~~~~~~~~~~~~~~&&&&&",
	"&&&&&~~~~~~~~~~~~~~~~~~:::~~~~~~~~~~~~~~~~~~~&&&&&",
	"&&&&&&&&&&&~~~~~~~~~~~~:::~~~~~~~~~~~~~&&&&&&&&&&&",
	"&&&&&&&&&&&&&~~~~~~~~~~:X:~~~~~~~~~~~&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&======&&&&&&&&&&&&&&&&&&&&&&"
};

static vector <string> mtpyre_entrance_map={
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&&&&&&&&&&&&&&&&&&&~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~&&&&&&&&&&&&&&&&&&&.&&&&&&&&&&&&&&.....&&&&~~~~~~~~",
	"~~~~~~~~~~~~&&&.......&&&&&&&&&&............&&&&&&&....&&&&~~~~~~~",
	"~~~~~~~~~~&&..&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.&&&~~~~~~",
	"~~~~~~~~&&...&&&..............&&&&&&&&&&&&&&&&&&&....&&&&..&&&~~~~",
	"~~~~~~~~&&.&&&&...&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.....&&&&&..&&~~~~",
	"~~~~~~~~&&.&&.&..&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&...&&&&..&&~~~~",
	"~~~~~~~~&&.&&.&..&................&&&&&&&&&&&&&...&&.&&&&&..&&~~~~",
	"~~~~~~~~&&.&&.&..&&&&&&&&..........&&&&&&&&&&&&&..&&.&&&&&..&&~~~~",
	"~~~~~~~~&&.&&.&..&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.&&&&&..&&~~~~",
	"~~~~~~~~&&.&&.&..&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.&&&&&..&&~~~~",
	"~~~~~~~~&&.&&.&..&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.&&&&&..&&~~~~",
	"~~~~~~~~&&.&&.&..&&&&&&&&&&&&&&&&&&&........&&&&&&&&.&&&&&..&&~~~~",
	"~~~~~~~~&&.&&.&..&&&&&&&&&&.................&&&&&&&&.&&&&&..&&~~~~",
	"~~~~~~~~&&.&&.&..&&&&&&&....................&&&&&&&&.&&&&&..&&~~~~",
	"~~~~~~~~&&.&&.&..&&&&&&&....................&&&&&&&&.&&&&&..&&~~~~",
	"~~~~~~~~&&.&&.&..&&&&&&&&&&........&&&&&&&&&&....&&&.&&&&..&~~~~~~",
	"~~~~~~~~&&.&&.&..&&&&&&&&&&&.....&&&&&&&&&.........&&...&&&~~~~~~~",
	"~~~~~~~~~&&.&&.&..&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.....&&&~~~~~~~",
	"~~~~~~~~~&&.&&.&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.....&&&~~~~~~~",
	"~~~~~~~~~~&&.&&.&&&&&...&&&&&&&&&&&&&......&&&&&&&&&&&&&&~~~~~~~~~",
	"~~~~~~~~~~~~&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&ppp&&&&~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~&&&&&&&&&&&&&&&&&&&&~~~~~~~''''''''''~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~&&&&&&&&&&&&&&&&&~~~~~~~~~''''B'@'''~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~________[:|::::]___~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[::[  ]||[:|::::]:  |~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[::[  ]||[------]:  |~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[::[  ]::- - - - - ]~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
};

static vector <string> mtpyre1_map ={
	"######################################################",
	"##########:::::::::|------|::::::::::::::::::::::::::#",
	"##########:::::::::|------|::::::::::::::::::::::::::#",
	"######################################################",
	":::::::::#*****************&&**&&*********************",
	":::::::::#*****************&&**&&*****&&&&&&&****&&&&&",
	"##########*****************&&**&&*****&&&&&&&****&&&&&",
	"=**********@***************&&**&&*********************",
	"=*****************************************************",
	"=*************************************&&&&&&&****&&&&&",
	"##########****************************&&&&&&&****&&&&&",
	"##########&&&&****************************************",
	"##########&&&&****************************@***********",
	"##########****&&&&************************************",
	"##########****&&&&************************************",
	"##########&&&&****&&&&****************************&&&&",
	"##########&&&&****&&&&****************************&&&&",
	"##############################*****###################",
	"##############################*****###################",
	"##############################*****###################",
	"##############################*****###################",
	"##############################*****###################",
	"##############################*****###################",
	"##############################*****###################",
	"##############################**X**###################",
	"##############################=====###################"
};

static vector <string> mtpyre2_map={
	"''&&'''''''''''''''''''''''''====='''''''''''''''''''''&&&&&&'&",
	"&&&&&''''''''''''''''''''''''.....''&&&''''''''''''''&&&&&&&&&&",
	"'&'&&''''&&&&&&&&&&&&&&&&&&''.....''&&&'''&&&'''''''''''''&&''&",
	"'&'&&'''''&&''&&&&&&'''&&&&''....@''&&&'''&&&'''''''''''''&&''&",
	"'&'&&'''''&&''&&&&&&'''&&&&''.....''&&&''''''''''^^^^^^^^^&&''&",
	"'&&&&'''''&&'''''''''''&&&&''.....''&&&''''''''^^^^^^^^^^^&&''&",
	"''&&&'''''&&'''''''''''&&&&''.....''&&&'''&&&^^^^^^^^^^^^^&&&&&",
	"''&&&'''''&&''&&&&&&'''&&&&''.....''&&&'''&&&^^^^^^^^^^^^^&&&''",
	"''&&&'''''&&''&&&&&&'''&&&&''.....''&&&''''''&&&&&&&&&&&&&&&&''",
	"''&&&&&&&&&&'''''''''''&&&&''@....''&&&''''''&&&&&&&&&&&&&&&&''",
	"''&&''''''''''''''''''''''''''''''''''''''''''''''''''''''''&''",
	"'''&&&&&&&&''''''''''''''''''''''''''''''''''''''''''''&&&&&'''",
	"&&'''''''''&&&&&&&&&&&&&&&&&&:::::&&&&&&&&&&&&&&&&&&&&&'''''''&",
	".&''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''=",
	".&''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''=",
	".&''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''=",
	".&&''''''''''''''''''''''''''''''''''''''''''''''''''''''''&&&&",
	"...&&&&''''''''''''''''''''''.....''''''''''''''''''''''......."
};

static vector <string> mtpyre_summit_map={
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~&&''''''''''''''''''''''''''&&~~~~~~~~~~~~~~~~~",
	"&&&&&&&&&&&&&&&&&&''''''''''''''''''''''''''&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&''''''''''''''''''''''''''&&&&&&&&&&&&&&&&&&&",
	"&&^^^^^'''''''''&&'''''''''@''''''@'''''''''&&'''''^^^^^^^^^^&&",
	"&&^^^^^^^'''''''&&''''''''''''''''''''''''''&&'''''^^^^^^^^^^&&",
	"&&&&&&&&&&&&''''&&''''''''''''''''''''''''''&&''''&&&&&&&^^&&&&",
	"&&&&&&&&&&&&''''&&''''''''''''''''''''''''''&&''''&&&&&&&&&&&&&",
	"&'''''&&''&&''''&&&&&&&&&&&&&:::::&&&&&&&&&&&&''''&&'''''''''&&",
	"&'&&&'&&''&&''''''''''''''''''''''''''''''''''''''&&'&&&&&&''&&",
	"&'&&&'&&''&&''''''''''''''''''''''''''''''''''''''&&'&&''&&''&&",
	"&'''''&&''&&&&&&&&&&&&&&&&&&&:::::&&&&&&&&&&&&&&&&&&'&&''&&''&&",
	"&&&&&&&&'''''''''''''''''''''''''''''''''''''''''''''&&''&&''&&",
	"''''&&&&&&&&&&&&&&&&&&&&&&&''''''''''&&&&&&&&&&&&&&&&&&&&''''&&",
	"''''&&&&&&&&&&&&&&&&&&&&&&&&&:::::&&&&&&&&&&&&&&&&&&&&&&&&&''&&",
	"&&&&'''''''''''''''''''''''''.....'''''''''''''''''''''''''''&&",
	"''&&'''''''''''''''''''''''''====='''''''''''''''''''''&&&&&&'&"
};

static vector <string> magma_hideout_1_map={
	"&&...&&&..&&&&&&&&&&&&&&&&&&&=====&&&&&&&&&&&",
	"&&...&&....&&&&&&&&&&&&&&&&&&..X.............",
	"&&...&&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,,,,,,",
	"&..............&&&&&&........................",
	"&&&.......&&&&&&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,",
	"&&&.......&&&&&&&............................",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&::::::&&&&&&&&&&",
	".....&&&&&&&&&&&&&&&&&&&&&&&&::::::&&&&&&&&&&",
	".....&&&&&&&&[------][------]::::::[      ]&&",
	"&&&&&&&&&&&&&[--||--][--[]--],,,,,,,||..||,,,",
	"&&&&&&&&&&&&&................................",
	".....&&&&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,",
	".....&&&&&...................................",
	"&&&&&&&&&,,,,,,,,,,,,,,,,,,,,,,,,,,,[--[]--],",
	"&&&&&&&&&...[--[]--].................[]--[]..",
	"&&&&&&&&,,,,,[]--[],,,,,,,,,,,,,,,,,,,,,,,,,,",
	"&&&&&&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,",
	"=,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&",
	"=,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&",
	"=,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"
};

static vector <string> magma_hideout_2_map={
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"..@....&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"=,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"=.........&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"=,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"..............&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&................&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&...............[____][-----]&&&&&&&&&&&&&",
	"&&&&...............[__|_][-----]&&&&&&&&&&&&&",
	"&&&&&&&,,,,,,,,,,,,[----][-----]&&&&&&&&&&&&&",
	"&&&&&&&.........................&&&&&&&&&&&&&",
	"&&&&&&&&&...@...............@...&&&&&&&&&&&&&",
	"&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&",
	"&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&",
	"&&&&&&&&&&&&&&.....................&&&&&&&&&&",
	"&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&.................&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&..................&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&,,,,,,,,,,,,,&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&.............&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&...............&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&,,,,,,,,,,,,&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&..............=",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.@............=",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&..............=",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"
};

static vector <string> magma_hideout_3_map={
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&[_______]&&&&&&&&&&[  []  ][  []  ]&&&&&&",
	"&&&&&&&&[---|---][  []  ]&&.[]..[]..[]..[]...&&&&",
	"[     ]&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,=",
	"[[] []].........................................=",
	",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,=",
	".......[  []  ]..................................",
	",,,,,,,,[],,[],,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,",
	",,,,,,,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&",
	"........................&&&&&&&&&&&&&&&&&&&&&&&&&",
	",,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	",,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"....=====.......&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"
};

static vector <string> magma_hideout_4_map={
	"&&&&.=====......&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&.........&&&&&&&&&&&&&&&&&&&&&&&&%%%&&&&&&&&&&&",
	"&&&&.........&&&&&&&&&&&&&&&&&%%%%%%%%%%%%%%%&&&&&&",
	"&&&&.........&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%&&&",
	"&&&&.........&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%&&&",
	"&&&&.........&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%&",
	"&&&&.........&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%&",
	"&&&&.........&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"&&&&.........&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"&&&&.........&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"&&&&............&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"&&&&..............&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%%%",
	"&&&&................&&&&&&&%%%%%%%%%%%%%%%%%%%%&&&&",
	"&[____]................&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&[____][___].....................@.................",
	"&&.................................................",
	"&&&&...............................................",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"
};

static vector <string> aqua_hideout_1_map={
	"#######################################=====######",
	"#| | | | | | | | | | | | | | | | | | |#:::::#| | #",
	"#######################################:::::######",
	"#::::::::::::::::::::::::::::::::::::::@:::::::::#",
	"#::::::::::::::::::::::::::::::::::::::::::::::::#",
	"#:::##########################################:::#",
	"#:::#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#:::#",
	"#:::#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#:::#",
	"#:::#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#:::#",
	"#:::#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#:::#",
	"#:::#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#:::#",
	"#:::#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#:::#",
	"#:::##########################################:::#",
	"#::::::::::::::::::::::::::::::::::::::::::::::::#",
	"#::::::::::::::::::::::::::::::::::::::::::::::::#",
	"######################=====#######################"
};

static vector <string> aqua_hideout_2_map={
	"###########################################======#",
	"#[] []  []  []  | || | [] |  [] [] []  |  #::::::#",
	"#############[--|--]#######################:::@::#",
	"#:::::::::::::::::................:::::::::::::::#",
	"#:::::::::::::::::[   ]....@......:::::::::::::::#",
	"#:::::::::::::::::................:::::::::::::::#",
	"#:::::::::::::::::...........[   ]:::::::::::::::#",
	"#:::::::::::::::::................:::::::::::::::#",
	"#::::::::::::::::::::::::::::::::::::::::::::::::#",
	"#######################################=====######",
};

static vector <string> aqua_hideout_3_map={
	"#######################################",
	"#[---][ || ]#  |  |  |  |  |  |  |  | #",
	"#[---][----]###########################",
	"#::::::::::::::::::::::::::|||||::|||||",
	"#::::::::::::::::::::::::::|||||::|||||",
	"#:::::::::::::::::::::::::::::::@:::::=",
	"#:::::::::::::::::::::::::::::::::::::=",
	"############################=====######",
};

static vector <string> aqua_hideout_4_map={
	"#################################",
	"#| | | | | | |#&&&&&&#| | | | | #",
	"#| | | | | | |#&&&&&&#| | | | | #",
	"#################################",
	"#:::::::::::::::::::::::::::::::#",
	"#:::::::::::::::::::::::::::::::#",
	"=:::::::::@:::::::::::::::::@:::#",
	"=:::::::::::::::::::::::::::::::#",
	"#:::::::::::::::::::::::::::::::#",
	"################=====############"
};

static vector <string> aqua_hideout_5_map={
	"##############################################=====############",
	":::::::::::::::::::::::::::::::::::::::::::###:::::############",
	"|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|###:::::############",
	"|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|###:::::############",
	"|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~h::::::::::::::::::::#",
	"|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~h:@::::::::::::::::::#",
	"|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~h::::::::::::::::::::#",
	"|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|:::::::::::::::::::#",
	"|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|:::::::::::::::::::#",
	"::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#",
	"###############################################################"
};

static vector <string> seafloor_cavern_1_map={
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"=,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"=X,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&",
	"=,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&:::::::::::::::::::::&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&:::::::::::::::::::::&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&:::::::::::::::::::&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&:::::::::::::::::::&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&:::::::::::::::&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&:::::::::::::::&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&......................&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&.......=====..........&&&&&&&&&&&&&&&&&&&&&&&"
};

static vector <string> seafloor_cavern_2_map={
	"&&&&&&&&&&&&&&&&&&.......=====...........&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&...............................&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&...........................&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&...................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&...................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&....................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&....................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&...................&&&&&&&&&&&&&&&&~~~~~~~~~~&&&&&&&&&&",
	"&&&&&&&&...................&&&&&&&&&&&&&~~~~~~~~~~~~~~~~&&&&&&&",
	"&&&&&&&&...................&&&&&&&&&&~~~~~~~~~~~~~~~~~~~~~&&&&&",
	"&&&&&&&&...................&&&&&&&~~~~~~~~~~~~~~~~~~~~~~~~~~~&&",
	"&&&&&&&&...................&&&&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&",
	"&&&&&&&&...................&&&&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&",
	"&&&&&&.....................&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&",
	"&&&&&&.....................&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&",
	"&&&&&&.....................&&&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&",
	"&&&&&&.....................&&&&&~~~~~~~~~~~~~~~~~~~~~~~~~&&&&&&",
	"&&&&&&.....................&&&&&&&&~~~~~~~~~~~~~~~~~~~~&&&&&&&&",
	"&&&&&&&&&&&..................&&&&&&&&&~~~~~~~~~~~~~~~~&&&&&&&&&",
	"&&&&&&&&&&&&&&.................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&...................&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&...........................................&&&&",
	"&&&&&&&&&&&&&&&&..............................@...........&&&&&",
	"&&&&&&&&&&&&&&&&&..........................................&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"
};

static vector <string> mossdeep_1_map={
	"YYY'''''''====='''''''''''''..........................&&&",
	"&'''''''''.....'''''''''''''..........................&&&",
	"&'''''''''.....'''''''''''''.....''||||||||||||||.....'''",
	"&&&&&&''''.....''''''YYYY'''.....''||||||||||||||.....'''",
	"&&&&&&''''.....''''YYY''''''.....''| | |  [][]  |.....'''",
	"....&&&&&&:::::&&&&&&&&&&&''.....''| | |        |.....'''",
	"....&&&&&&:::::&&&&&&&&&&&''.....''##############.....'''",
	"........................&&''.....''''''''''''''''.......=",
	"........................&&''.....''''''''''''''''.......=",
	"........................&&''.....''''''''''''''''.......=",
	"........................&&&&:::::&&&&&&&&&&&&&&&&&&&&&&&&",
	"........................&&&&:::::&&&&&&&&&&&&&&&&&&&&&&&&",
	"....................................&''YY.YYY.YYY.YYY.YYY",
	"....................................&&&YY.YYYYY.YYYY.YY&Y",
	".....................................YYY.YYYY...YYY.YYY.Y",
	"~~~~~~~~~~~~~~~...................YYY.YYYY.....YYY..YYY.Y",
	"~~~~~~~~~~~~~~~~~~~..................YYYYY.YYYYY.YYYY.YYY",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~....B.@.....YYY.YYYYY.YYYYY.YY",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~..........YYY.YYY.YYYY.YYYY",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~________[:|::::]___~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[::[  ]||[:|::::]:  |~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[::[  ]||[------]:  |~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[::[  ]::- - - - - ]~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
};

static vector <string> mossdeep_2_map={
	".......&''''''''''''''''''''''''''''''''''''''''''''''''''''''''''&&",
	".......&''''''''''''''''''''YYYYYYYYYY|||||||||||||||||||||'''''''&&",
	".......&'''''''''''''''''''''YYYYY'YYY|||||||||||||||||||||'''''''&&",
	".....&''''''''''''''''''''''''YYYY''YY|||||||||||||||||||||'''''''&&",
	".....&'''''''''''''''''''''''''''YYY''|[][] [][]|| | |[][]|''''YYY&&",
	"....&''''''''''||||||||||||||||'''''''|  Gym    || | |    |''YYY&&&&",
	"YYYYY''''''''''||||||||||||||||'''''''############lll######YYY&&&''",
	"YYYYYYY''''''''||||||||||||||||''...........@..............'''&&&''",
	"YYYYYYYYYY'''''| |  | |  [  ] |''..........................'''&&&''",
	"YYYYYYYYYYYYYYY| |  | |       |''..........................'''&&&''",
	"YYYY''YYYY'YYYY################''.....'''''''''''''''''''''&&&&&'''",
	"'''''''''''''''''''''''''''''''''.....''''''''''''''''''&&&''''''''",
	"'''''''''''''''''''''''''''''''''.....''''''''''''''''''&&&''''''''",
	"'''''''''''''''''''''''''''''''''.....''''''''''''''''''&&&''''''''",
	"'''''''''''''''||||||||||||||||''.....''''''''''''''''''&&&''''''''",
	"'''''''''''''''||||||||||||||||''.....''''''''''''''''''&&&'YYYY'''",
	"''''YYY''''''''||||||||||||||||''...@.''''''''''''''''''&&&''YY''''",
	"'YYYY''YY''''''|| ||   | PC | |''.....''||||||||||||||||&&&'YYYY'''",
	"YYYY''YYYY'''''|| ||   |    | |''.....''||||||||||||||||&&&''''''''",
	"'''''''''''''''#####444########''.....''||||||||||||||||&&&''''''''",
	"''''''''''............................''|| ||   |MART| |&&&&'''''''",
	"''''''''''............................''|| ||   |    | |&&&&'''''''",
	"''''''''''............................''#####555########''&&'''''''",
	"YYY'''''''====='''''''''''''''''''''''''''''''''''''''''''&&'''''''"
};

static vector <string> mossdeepgym_map={
	"##################################",
	"#    #############################",
	"#:[]:#############################",
	"#::::#  |  |  |  |  |  |  |  |  |#",
	"#::::#############################",
	"#::::#.............@.............#",
	"#::::########.....:::.....########",
	"#::::::::::::::::::::::::::::::::#",
	"#::X:::::::::::::::::::::::::::::#",
	"##===#############################"
};

static vector <string> mossdeep_3_map={
	"~~~~||||||||||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~~~~~",
	"~~&&||||||||||''''''''''YYY''~~~~~~~~~~~~##__~|~__##~~~~~~~~~~~~~~~",
	"~~&&||  | [] |'''''''''YYYYY''~~~~~~~~~~~##~~| |~~##~~~~~~~~~~~~~~~",
	"''&&||  |    |''''''''''''''&&'''''''''''##~~| |~~##'''''''''''''''",
	"'''''''''''''''''''''''''''&&''''''''''''##__| |__##'''''''''''''''",
	"'''''''''''''''''''''''''''&&''''''''''''##''| |''##'''''''''''''''",
	"''''''.......''''''''''''&&''''''''''''''###'| |'###'''''''''''''''",
	"'''''........''''''''''''&&''''''''''''''###|| ||###'''''''''''''''",
	"''''.....'''''''''''''''&&''''''||||||||||||||||||||||||||||||'''''",
	"'''......'''''''''''''''&&''''''|  |[  ]|    |-|    |[  ]|   |'''''",
	"'''......'''''''''''''''&&''''''|  |[  ]|    |-|    |[  ]|   |'''''",
	"''.......'''''''''''''&&''''''''|[][]|[][]|__|-|___|[][]|[][]|'''''",
	"''.......'''''''''''''&&''''''''|[][]|[][]|__|-|___|[][]|[][]|'''''",
	"''''.....'''''''''''''&&''''''''|    |    |  | |   |    |    |'''''",
	"''''''...''''''''''''''&&'''''''||||||||||||||||||||||||||||||'''''",
	"'''''''''''''''''''''''&&'''''''|||||||||||||[][]|||||||||||||'''''",
	"''''''''''''''''''''''''&&''''''##############################'''''",
	"''''''''''''''''''''''''&&&&&&&&'''''''''''''......''''''''&&&&&&&&",
	"&&&&''''''''''''''''''''&&''''&&&&&&&&&&&&&&&::::::&&&&&&&&''''''''",
	"''''&&&&&&&&&&&&'''''''''&&&&&&&&&&&&&&&&&&&&::::::&&&&&&&&&&&&&&&&",
	"=.....''''''''''&&&:::::::&&&&&&&&&&&&&&&&&&&''''''&&&&&&&&&&&&&&&&",
	"=.....''''''''''&&&:::::::&&&&&&&&&&&&&&&'''''''''''''''&&&&&&&&&&&",
	"=.........................''''''''''''''''''@'@''''''''''''''''''''",
	"..........................''''''''''''''''''''''''''''''''''''''YYY",
	"'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''YYYY",
	"'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''YYYY"
};

static vector <string> sootopolis_1_map={
	"........&,,,,,&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&====&&&&...........&",
	"|||||||||||||,&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&,,,,&&&&&&&&&&&&&&&&",
	"|||||||||||||,&~~~~~~||||||||||||||||||||~~~~~~&,,,,&&.|||||||||||||",
	"||  || MART |,&~~~~~~||||||||||||||||||||~~~~~~&,,,,&&.|||||||||||||",
	"||  ||      |,&~~~~~~||||||||||||||||||||~~~~~~&,,,,&&.|| ||[] PC  |",
	"##555########,&~~~~~~|[][] [][] |  |[][]|~~~~~~&,,,,&&.|| ||       |",
	".............&,~~~~~~| Gym      |  |    |~~~~~~&,,,,&&.##444########",
	"............&,,~~~~''###########sss######''~~~~&,,,,&&..............",
	"......:::::&&,,::::'''''''''''''''''''''''':::::,,,,&&..............",
	"&&&&&&:::::,,,,::::'''''''''''''''''''''''':::::,,,,&&&&&&:::::&&&&&",
	"'''''''''''''''~~~~''''@'''''''''@''@''''''~~~~~''''''''''''''''''''",
	"'''''''''''''''~~~~''''''''''''''''''''''''~~~~~''''''''''''''''''''",
	"'''''''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~''''''''''''''''''''",
	"'''''''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'''''''''''''''",
	"'''''''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'''''''''''''''",
	"''''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'''''''''''''''",
	"''''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'''''''''''''''",
	"''''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'''''''''''''''",
	"''''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'B'''@'''''''''",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~________[:|::::]___~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[::[  ]||[:|::::]:  |~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[::[  ]||[------]:  |~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[::[  ]::- - - - - ]~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
};

static vector <string> sootopolis_2_map={
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&===&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&.......&&&&&&&&,,,&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&........&&&&&&&&,,,&&&&&&&&&&&&........&&&&&&&&&&&&&&&&",
	".....................&&&&&&&,,,,,,&&&&&&&&&&&.......&&&&&&&&&&&&&&&&",
	".....................&&&,,,,,,,,,,,,,,,,,,,&&................&&&&&&&",
	".........&&&&&&&&&&&&&&&,,,,,,,,,,,,,,,,,,,&&................&&&&&&&",
	".........&&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&&&&........&&&&&",
	"&&&&&&...&&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&......&&&&",
	"&&&&&&:::&&&,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,&&&&&&&&&......&&&",
	"&&&&&&...&&&,,,,&&&&&&&&&&&&&&&&&&&&&&&&&&&&,,,,,,,,&&&&&&&&......&&",
	"&&&&&....&&,,,,&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&,,,,&&&&&&&&......&&",
	"&&&&.....&&,,,,&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&,,,,&&&&&&&.......&&",
	"&&&.....&&,,,,&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&,,,,&&&&&&........&&",
	".......&&,,,,&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&,,,,&&&&..........&&",
	"........&,,,,,&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&====&&&&...........&"
};

static vector <string> cave_of_origin_map={
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&...&&&&&&&&&&&&&&&&&&...................&&&&&&&&&&&&&&&&&&...&&&",
	"&&&..&&..........&&&&&&.....................&&&&&...........&&..&&&",
	"&&..&&&........&&&&&&.........@.B.............&&&&&.........&&&..&&",
	"&&..&&&........&&&&&&.........................&&&&&.........&&&..&&",
	"&&..&&&........&&................................&&.........&&&..&&",
	"&&..&&&........&&................................&&.........&&&..&&",
	"&&..&&&........&&................................&&.........&&&..&&",
	"&&..&&&........&&................................&&.........&&&..&&",
	"&&..&&&........&&................................&&.........&&&..&&",
	"&&..&&&........&&................................&&.........&&&..&&",
	"&&..&&&........&&&&&&.........................&&&&&.........&&&..&&",
	"&&..&&&........&&&&&&.........................&&&&&.........&&&..&&",
	"&&&..&&..........&&&&&&.....................&&&&&...........&&..&&&",
	"&&&...&&&&&&&&&&&&&&&&&&...................&&&&&&&&&&&&&&&&&&...&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&......&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&======&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"
};

static vector <string> sky_pillar_entrance_map={
	"~~~~~~~~~~~~~~~~~~~~.....|  [] []  |........~~~~~~~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~........|         |...............&&&~~~~~~~~~~~~~",
	"~~~~~~~~~~~~.............|  [] []  |...............&&&&&&~~~~~~~~~~",
	"~~~~~~~~~&&&.............|         |...............&&&&&&&~~~~~~~~~",
	"~~~~~~~~&&&&.............|  [] []  |...............&&&&&&&~~~~~~~~~",
	"~~~~~~~&&&&&.............|         |...............&&&&&&&~~~~~~~~~",
	"~~~~~~~&&&&&.............#####rrr###...............&&&&&&&~~~~~~~~~",
	"~~~~~~~&&&&&&&.....................................&&&&&&&~~~~~~~~~",
	"~~~~~~~~&&&&&&...................................&&&&&&&~~~~~~~~~~~",
	"~~~~~~~~&&&&&&...................................&&&&&&&~~~~~~~~~~~",
	"~~~~~~~~&&&&&&...................................&&&&&&&~~~~~~~~~~~",
	"~~~~~~~~~&&&&&&.................................&&&&&&&~~~~~~~~~~~~",
	"~~~~~~~~~~&&&&&&................................&&&&&&&~~~~~~~~~~~~",
	"~~~~~~~~~~&&&&&&&.......................B.@.....&&&&&&&~~~~~~~~~~~~",
	"~~~~~~~~~~~&&&&&&&............................&&&&&&&&&~~~~~~~~~~~~",
	"~~~~~~~~~~~~&&&&&&&..........................&&&&&&&&~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~&&&&&&&........................&&&&&&&&&~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~&&&&&&&&&&&&.............&&&&&&&&&&&&~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&~~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~&&&&&&&&&&&&&&&####&&&&&&&&&&&&~~~~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~&&&&&&&&&.........&&&&&~~~~~~~~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.........|~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~________[:|::::]___~~~~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~[::[  ]||[:|::::]:  |~~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~[::[  ]||[------]:  |~~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~[::[  ]::- - - - - ]~~~~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
};

static vector <string> sky_pillar={
	"...................&&&&&&&&&&&&&&&&&&&&&&&&&&&.....................",
	"........................&&&&.........&&&&..........................",
	".........................&&...........&&...##.##&&&........##......",
	"##...&&...........##..&&&...............&&&...##.........##..##....",
	"########&&............&&&...............&&&.....##...##....##......",
	"##........&&.............&&&..........&&&.......##.................",
	"##..........&&............&&..........&&&#########..........####&&&",
	"##########&&..&&......................&&&&#########################",
	".................&&.....................&&&&&&&&&&&&&&&&...........",
	"&&&..............&&&&&&&&&&&&&.......&&&&&&&&&&......&&&&&&&&&&&&&&",
	"&&&&..........&&..........................&&.....&&................",
	"......&&&...................&&..................&&&................",
	".......&&&.........&&&&..........X......................&&&&&&.....",
	".........&&&.........&&&.......=====......&&......................."
};

static vector <string> sootopolisgym_map={
	"---###########################################---",
	"####______|````|________________|````|_______####",
	"___|______|````|________________|````|_______|___",
	"---#......[````].......@........[````].......#---",
	"---##################:::::####################---",
	"---#________________#:::::#__________________#---",
	"---#.....&&&&................................#---",
	"---#.........................&&&&......&&&&..#---",
	"---#....&&&&.....&&&&&.......................#---",
	"---#...................X........&&&&.........#---",
	"---###################===#####################---"
};

static vector <string> ever_grande_center_map={
	"~~~~~~~~~~~&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&~~~~~~~~~~~~~~~",
	"~~~~~~~&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&~~~~~~~~~~~~~",
	"~~~~~~&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&~~~~~~~~~",
	"~~~&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&~~~~~",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&~~~",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&ggg&&&&&&&&&&&&&&'''''''''''''''''''&&&&",
	"&&&&&&&&&&&''''''''''''''''''''''''''&&&'||||||||||||||||'''YYY&&&&&",
	"&&&&&&&''''''''''''''''''''''''''''''&&&'||||||||||||||||''YYY&&&&&~",
	"&&&&&&&'''''''''''''''''''''''''''''&&&''||||||||||||||||''YYY&&&&&~",
	"&&&&&&&'''''''''''''''''''''''''''''&&&''|| ||   | PC | |'YYY&&&&&~~",
	"&&&&&&&&'''''''''''''''''''''''''''&&&'''|| ||   |    | |'YYY&&&&~~~",
	"&&&&&&&&&&''''''''''''''''''''&&&&&&'''''##444###########'YY&&&&&~~~",
	"&&&&&&&&&&''''''&&&&&&&&&:::::&'''''''''''''''''''''''''''YY&&&&&~~~",
	"&&&&&&&&&&''''''&'''''''''''''''''''''''''''''''''''''''''YY&&&&&~~~",
	"&&&&&&&&&&&'''''&''''''''''''''''''''''''B''''@''YYYYYYYYYYY&&&&&~~~",
	"&&&&&&&&&&&&&&&&'''''''''''''''''''''''''YYYYYYYYYYYYYYYYYY&&&&&~~~~",
	"&&&&&&&&&&&&&&&&&&&&&&&&&:::::&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&~~~~",
	"~~~&&&&&&&&&&&&&&&&&&&''''''''''''''''''&&&&&&&&&&&&&&&&&&&&&&&~~~~~",
	"~~~~&&&&&&&&&&&&&&&&&&''''''''''''''''''&&&&&&&&&&&&&&&&&&&&&&&~~~~~",
	"~~~~~&&&&&&&&&&&&&&&&&~~~~~~~~~~~~~~~~~~&&&&&&&&&&&&&&&&&&&&&~~~~~~~",
	"~~~~~~&&&&&&&&&&&&&&&&~~~~~~~~~~~~~~~~~~&&&&&&&&&&&&&&&&&&&&~~~~~~~~",
	"~~~~~~~&&&&&&&&&&&&&&&``````````````````&&&&&&&&&&&&&&&&&&&&~~~~~&&&",
	"~~~~~~~~&&&&&&&&&&&&&&``````````````````&&&&&&&&&&&&&&&&&&&~~~~~&&&~",
	"~~~~~~~~~&&&&&&&&&&&&&``````````````````&&&&&&&&&&&&&&&&&&~~~~~~~~~~",
	"~~&&~~~~~~&&&&&&&&&&&&``````````````````&&&&&&&&&&&&&&&&&~~~~~~~~~~~",
	"~~&&~~~~~~~~&&&&&&&&&&``````````````````&&&&&&&&&&&&&&&~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~&&&&&&&&&``````````````````&&&&&&&&&&&&~~~~~~~~~~~~~~~~",
	"~~~~~~~~~~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|~~~~~~~~~~~",
	"~~~~~~&&&&~~~~~~~~~~~~~&&~~~~~~~~~~~~~~~~~~~~~________[:|::::]___~~~",
	"~~~~~~~~&&&&~~~~~~~~~~~~~~~~~~~~~~~~~~&&~~~~~~[::[  ]||[:|::::]:  |~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~&&~~~~~[::[  ]||[------]:  |~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[::[  ]::- - - - - ]~~",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
};

static vector <string> victory_road_1_map={
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&=====&&&",
	"&&*************************&&&**&&...&&&&&&&&...&&&&***********&&",
	"&&**************************&&**&&...&&&&&&&&...&&&&&**********&&",
	"&&**************************&&**&&..............&&&&&**********&&",
	"&&**************************&&**&&&&&&&&&&&&&&&&&&&&&**********&&",
	"&&****&&&&&&&&&&&&&&&&******&&**&&..............&&&&&**********&&",
	"&&****&..............&&&&&&&&&&&&&..............&&&&&*@********&&",
	"&&****&...........@..::::::::::::::..............&&&&**********&&",
	"&&****&..............::::::::::::::..............&&&&**********&&",
	"&&****&..............&&&&&&&&&&&&&&&&&&&::::&&&&&&&&&**********&&",
	"&&****&&&&:::&&&&&&&&&******&&**&&......::::.....&&&***********&&",
	"&&****&&&&:::&&&&&&&&&******&&**&&&&&&&&::::&&&&&**************&&",
	"&&**************************&&*********************************&&",
	"&&************&&&***********&&*********************************&&",
	"&&*************&&***********&&&&&&&**************************&&&&",
	"&&***********************X*******&&************************&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&=====&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"
};

static vector <string> victory_road_2_map={
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&......&&***************************************************&&&&",
	"&&......&&*******************************************@*******&&&&",
	"&&......&&***************************************************&&&&",
	"&&......&&*******@**********************************X********&&&&",
	"&&&&&&&&&&***************&&&&&&&&&&&&&&&&&&&&&&&&&&www&&&&&&&&&&&",
	"&&&.....&&***************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&.....&&***************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&.....&&***************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&.....&&***************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&.....&&***************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&.....&&***************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&.....&&***************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&.....&&***************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&.....&&***************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&=====&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"
};

static vector <string> ever_grande_league_map={
	"&&&&&&&&&&&&&&&&&&&&&&| |[][][][][][][][]| |&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&| |                | |&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&| |[][][][][][][][]| |&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&Y| |    [  000  ]   | |Y&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&YY| |     ||   ||    | |YY&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&YYY| |     ||   ||    | |YYY&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&YYY##Y##########WWW#########Y##YYY&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&YYYYY##''''''YY''''''''YY''''''##YYYYY&&&&&&&&&&&&&",
	"&&'''''''''''''''''##''''''YY''....''YY''''''##''''''''''''''''&&",
	"&&'''''''''''''''''##''''''YY''....''YY''''''##''''''''''''''''&&",
	"&&'''''''''''''''''##''''''YY''....''YY''''''##''''''''''''''''&&",
	"&&'''''''''''''''''##''''''YY''....''YY''''''##''''''''''''''''&&",
	"&&'''''''''''''''''##''''''YY''....''YY''''''##''''''''''''''''&&",
	"&&'''''''''''''''''##''''''YY''....''YY''''''##''''''''''''''''&&",
	"&&'''''''''''''''''#####'''''''.....''#######################''&&",
	"&&''''''''''''''''''''##'''''''.......'''''''''''''''''''''##''&&",
	"&&''''''''''''''''''''##'''''''.......................'''''##''&&",
	"&&''''''''''''''''''''##'''''''.......................'''''##''&&",
	"&&''''''''''''''''''''##''''''''''''''................'''''##''&&",
	"&&''''''''''''''''''''##''''''''''''''''''''''''''....'''''##''&&",
	"&&''''''''''''''''''''##''''''''''''''''''''''''''....'''''##''&&",
	"&&''''''''''''''''''''##''''''''''''''''''''''''''....'''''##''&&",
	"&&''''''''''''''''''''#########################'''....'''''##''&&",
	"&&'''''''''''''''''''''''''''''''''''''''''''##'''....'''''##''&&",
	"&&'''''''''''''''''''''''''''''''''''''''''''##'''....'''''##''&&",
	"&&'''''''''''''''''''''''''''''''''''''''''''##'''....'''''##''&&",
	"&&&&&&&&&&&&'''''''''''''''''''''''''''''''''##'''....'''''##''&&",
	"&&&&&&&&&&&&&&&''''''''''''''''''''''''''''''##'''....'''''##''&&",
	"&&&&&&&&&&&&&&&&&&'''''''''''''''''''''''''''''''''''''''''''''&&",
	"&&&&&&&&&&&&&&&&&&&&&&'''''''''''''''''''''''''''''''''''''''''&&",
	"&&&&&&&&&&&&&&&&&&&&&&'''''''''''''''''''''''''''''''''''''''''&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&''''''''''''''''''''''''''''''''''''''&&",
	"&&&&&&&&LLL&&&&&&&&&&&&&&''''''''''''''''''''''''''''''''''''''&&",
	"&&'''''''X'''''''''''''''''''''''''''''''''''''''''''''''''''''&&",
	"&&'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&",
	"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"
};

static vector <string> league_entrance_map={
	"#######################===########################",
	"#|||||||||||||||||||||#...#||||||||||||||||||||||#",
	"#######################...########################",
	"#__________| |...|....@...@.......| |____________#",
	"#_____@______|#cc#................|_______@______#",
	"#................................................#",
	"#................................................#",
	"#................................................#",
	"#................................................#",
	"#................................................#",
	"#................................................#",
	"#.......................X........................#",
	"#######################===########################"
};

static vector <string> league_rm1_map={
	"##################===#################",
	"#|  |  |  |  |  |#...#|  |  |  |  | |#",
	"##################...#################",
	"#....................................#",
	"#-----..........................-----#",
	"#-S--|.............@............|--S-#",
	"#-S--|..........................|--S-#",
	"#-----..........................-----#",
	"#....................................#",
	"##################...#################",
	"##################...#################",
	"##################...#################",
	"##################.X.#################",
	"######################################"
};

static vector <string> league_rm2_map={
	"##################===#################",
	"#|  |  |  |  |  |#...#|  |  |  |  | |#",
	"##################...#################",
	"#....................................#",
	"#-----..........................-----#",
	"#-P--|.............@............|--P-#",
	"#-P--|..........................|--P-#",
	"#-----..........................-----#",
	"#....................................#",
	"##################...#################",
	"##################...#################",
	"##################...#################",
	"##################.X.#################",
	"######################################"
};

static vector <string> league_rm3_map={
	"##################===#################",
	"#|  |  |  |  |  |#...#|  |  |  |  | |#",
	"##################...#################",
	"#....................................#",
	"#-----..........................-----#",
	"#-G--|.............@............|--G-#",
	"#-G--|..........................|--G-#",
	"#-----..........................-----#",
	"#....................................#",
	"##################...#################",
	"##################...#################",
	"##################...#################",
	"##################.X.#################",
	"######################################"
};

static vector <string> league_rm4_map={
	"##################===#################",
	"#|  |  |  |  |  |#...#|  |  |  |  | |#",
	"##################...#################",
	"#....................................#",
	"#-----..........................-----#",
	"#-D--|.............@............|--D-#",
	"#-D--|..........................|--D-#",
	"#-----..........................-----#",
	"#....................................#",
	"##################...#################",
	"##################...#################",
	"##################...#################",
	"##################.X.#################",
	"######################################"
};

static vector <string> league_rm5_map={
	"####| | | | | | | | | | | | | | | | |####",
	"| | | | | | | | | | | | | | | | | | | | |",
	"| | | | | | | | | | | | | | | | | | | | |",
	"| | | | | | | |...........| | | | | | | |",
	"| | | | |.......................| | | | |",
	"| | |...............................| | |",
	"| | |...............@...............| | |",
	"| | |...............................| | |",
	"| | |...............................| | |",
	"| | | | |.......................| | | | |",
	"| | | | | | | |...........| | | | | | | |",
	"| | | | | | | | ......... | | | | | | | |",
	"####| | | | | | | ..... | | | | | | |####",
	"##################..X..##################",
	"#########################################"
};
/*****************************************************************************
 * HELPER FUNCTIONS
 *****************************************************************************/
static inline bool isNPCTile(char tile) {
    return tile == TileChars::NPC_MARKER;
}

static inline bool isPassable(char tile) {
    auto it = tileMap.find(tile);
    return it != tileMap.end() && it->second.passable;
}

static inline bool hasWildEncounter(char tile) {
    auto it = tileMap.find(tile);
    return it != tileMap.end() && it->second.wildEncounter;
}

static inline bool isInBounds(const vector<string>& layout, int row, int col) {
    if (row < 0 || row >= (int)layout.size()) return false;
    if (col < 0 || col >= (int)layout[row].size()) return false;
    return true;
}

/*****************************************************************************
 * SPAWN UTILITIES
 *****************************************************************************/
static bool findSpawn(const vector<string>& layout, Position &pos, char marker = TileChars::SPAWN_MARKER) {
    // First pass: look for the specific marker
    for (int r = 0; r < (int)layout.size(); ++r) {
        for (int c = 0; c < (int)layout[r].size(); ++c) {
            if (layout[r][c] == marker) {
                pos.row = r;
                pos.col = c;
                return true;
            }
        }
    }
    
    // Fallback: find any walkable tile
    for (int r = 0; r < (int)layout.size(); ++r) {
        for (int c = 0; c < (int)layout[r].size(); ++c) {
            if (isPassable(layout[r][c])) {
                pos.row = r;
                pos.col = c;
                return true;
            }
        }
    }
    return false;
}

static bool findSpawnWithOffset(const vector<string>& layout, Position &pos, char marker, int rowOffset, int colOffset) {
    for (int r = 0; r < (int)layout.size(); ++r) {
        for (int c = 0; c < (int)layout[r].size(); ++c) {
            if (layout[r][c] != marker) continue;
            
            int newRow = r + rowOffset;
            int newCol = c + colOffset;
            
            if (isInBounds(layout, newRow, newCol) && isPassable(layout[newRow][newCol])) {
                pos.row = newRow;
                pos.col = newCol;
                return true;
            }
            
            // Try adjacent tiles if offset position is blocked
            const int directions[][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
            for (const auto& dir : directions) {
                int testRow = r + dir[0];
                int testCol = c + dir[1];
                
                if (isInBounds(layout, testRow, testCol) && isPassable(layout[testRow][testCol])) {
                    pos.row = testRow;
                    pos.col = testCol;
                    return true;
                }
            }
        }
    }
    return false;
}

static bool findEdgeSpawn(const vector<string>& layout, Position &pos, const string& fromEdge) {
    int height = layout.size();
    int width = layout[0].size();
    
    auto tryFindTile = [&](int r, int c) -> bool {
        if (!isInBounds(layout, r, c)) return false;
        char t = layout[r][c];
        if (t == TileChars::ROUTE_TRANSITION || (isPassable(t) && t != TileChars::SPAWN_MARKER)) {
            pos.row = r;
            pos.col = c;
            return true;
        }
        return false;
    };
    
    // First pass: look specifically for ROUTE_TRANSITION tiles ('=')
    auto findTransitionTile = [&]() -> bool {
        if (fromEdge == "top") {
            for (int r = height - 1; r >= 0; --r) {
                for (int c = 0; c < (int)layout[r].size(); ++c) {
                    if (layout[r][c] == TileChars::ROUTE_TRANSITION) {
                        pos.row = r;
                        pos.col = c;
                        return true;
                    }
                }
            }
        }
        else if (fromEdge == "bottom") {
            for (int r = 0; r < height; ++r) {
                for (int c = 0; c < (int)layout[r].size(); ++c) {
                    if (layout[r][c] == TileChars::ROUTE_TRANSITION) {
                        pos.row = r;
                        pos.col = c;
                        return true;
                    }
                }
            }
        }
        else if (fromEdge == "left") {
            for (int c = width - 1; c >= 0; --c) {
                for (int r = 0; r < height; ++r) {
                    if (layout[r][c] == TileChars::ROUTE_TRANSITION) {
                        pos.row = r;
                        pos.col = c;
                        return true;
                    }
                }
            }
        }
        else if (fromEdge == "right") {
            for (int c = 0; c < width; ++c) {
                for (int r = 0; r < height; ++r) {
                    if (layout[r][c] == TileChars::ROUTE_TRANSITION) {
                        pos.row = r;
                        pos.col = c;
                        return true;
                    }
                }
            }
        }
        return false;
    };
    
    // Try to find '=' tile first
    if (findTransitionTile()) {
        return true;
    }
    
    // Fallback to any passable tile if no '=' found
    if (fromEdge == "top") {
        for (int r = height - 1; r >= 0; --r) {
            for (int c = 0; c < (int)layout[r].size(); ++c) {
                if (tryFindTile(r, c)) return true;
            }
        }
    }
    else if (fromEdge == "bottom") {
        for (int r = 0; r < height; ++r) {
            for (int c = 0; c < (int)layout[r].size(); ++c) {
                if (tryFindTile(r, c)) return true;
            }
        }
    }
    else if (fromEdge == "left") {
        for (int c = width - 1; c >= 0; --c) {
            for (int r = 0; r < height; ++r) {
                if (tryFindTile(r, c)) return true;
            }
        }
    }
    else if (fromEdge == "right") {
        for (int c = 0; c < width; ++c) {
            for (int r = 0; r < height; ++r) {
                if (tryFindTile(r, c)) return true;
            }
        }
    }
    
    return false;
}

/*****************************************************************************
 * RENDERING
 *****************************************************************************/
static void renderMapWithPlayer(const vector<string>& layout, const Position &pos, const Player &player) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    int height = layout.size();
    int width = layout[0].size();
    
    vector<CHAR_INFO> buffer(height * width);
    
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < (int)layout[r].size(); ++c) {
            char tile = layout[r][c];
            bool isPlayer = (r == pos.row && c == pos.col);
            
            // Hide spawn marker
            if (tile == TileChars::SPAWN_MARKER|| tile== TileChars::BOAT_MARKER) {
                tile = TileChars::GROUND;
            }
            
            char toDraw = isPlayer ? TileChars::PLAYER_CHAR : tile;
            int color = COLOR_WHITE;
            
            if (isPlayer) {
                color = (player.gender == GIRL) ? COLOR_RED : COLOR_BLUE;
            }
            else if (tileMap.count(toDraw)) {
                color = tileMap[toDraw].color;
            }
            
            int idx = r * width + c;
            buffer[idx].Char.AsciiChar = toDraw;
            buffer[idx].Attributes = color;
        }
    }
    
    COORD bufferSize = {(SHORT)width, (SHORT)height};
    COORD bufferCoord = {0, 0};
    SMALL_RECT writeRegion = {0, 0, (SHORT)(width - 1), (SHORT)(height - 1)};
    
    WriteConsoleOutputA(hConsole, buffer.data(), bufferSize, bufferCoord, &writeRegion);
}

/*****************************************************************************
 * TRAINER SYSTEM
 *****************************************************************************/
static void scanTrainersInMap(MapID mapId, const vector<string>& layout);

static void registerRoute102Trainers() {
    RouteTrainerData& data = allTrainers[MAP_ROUTE102];
    if (data.initialized) return;
    
    data.trainers.push_back(createYoungsterCalvin(db));
    data.trainers.push_back(createBugCatcherRick(db));
    data.prizeMoney = {80, 64};
    data.initialized = true;
}

static void registerPetalburgWoods1Trainers() {
    RouteTrainerData& data = allTrainers[MAP_PETALBURGWOODS1];
    if (data.initialized) return;
    
    data.trainers.push_back(createBugCatcherLyle(db));
    data.trainers.push_back(createBugCatcherJames(db));
    data.prizeMoney = {48, 96}; 
    data.initialized = true;
}

static void registerPetalburgWoods2Trainers() {
    RouteTrainerData& data = allTrainers[MAP_PETALBURGWOODS2];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamAquaGrunt(db));
    data.prizeMoney = {180};
    data.initialized = true;
}
static void registerRustboro1Trainers() {
    RouteTrainerData& data = allTrainers[MAP_RUSTBORO1];
    if (data.initialized) return;
    
    // Create a placeholder that will be replaced with actual rival based on player
    data.trainers.push_back(Trainer());  // Placeholder
    data.prizeMoney = {800};
    data.initialized = true;
}

static void registerRustboroGymTrainers() {
    RouteTrainerData& data = allTrainers[MAP_RUSTBOROGYM];
    if (data.initialized) return;
    
    data.trainers.push_back(createGymLeaderRoxanne(db));
    data.prizeMoney = {1800};
    data.initialized = true;
}

static void registerDewfordGymTrainers() {
	 RouteTrainerData& data = allTrainers[MAP_DEWFORDGYM];
    if (data.initialized) return;
    
    data.trainers.push_back(createGymLeaderBrawley(db));
    data.prizeMoney = {1900};
    data.initialized = true;
}

static void registerOceanicMuseumTrainers() {
	 RouteTrainerData& data = allTrainers[MAP_OCEANIC_MUSEUM];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamAquaGruntOM1(db));
    data.prizeMoney = {300};
    
    data.trainers.push_back(createTeamAquaGruntOM2(db));
    data.prizeMoney = {280};
    data.initialized = true;
}

static void registerRoute110Trainers(){
    RouteTrainerData& data = allTrainers[MAP_ROUTE110_2];
    if (data.initialized) return;
    
    data.trainers.push_back(createCollectorEdwin(db));
    data.trainers.push_back(createPokefanIsabel(db));
    data.trainers.push_back(Trainer());  // Rival placeholder
    
    data.prizeMoney = {840, 1120, 1200};
    
    data.initialized = true;
}

static void registerMauvilleGymTrainers() {
	 RouteTrainerData& data = allTrainers[MAP_MAUVILLEGYM];
    if (data.initialized) return;
    
    data.trainers.push_back(createGymLeaderWattson(db));
    data.prizeMoney = {2400};
    data.initialized = true;
}

static void registerRoute111ATrainers(){
    RouteTrainerData& data = allTrainers[MAP_ROUTE111_1];
    if (data.initialized) return;
    
    data.trainers.push_back(createKindlerHayden(db));
    data.trainers.push_back(createCamperTravis(db));
    data.trainers.push_back(createPicnickerBianca(db));
    data.trainers.push_back(createAromaLadyCelina(db));
    
    data.prizeMoney = {576, 288, 288,720};
    
    data.initialized = true;
}

static void registerRoute111BTrainers(){
    RouteTrainerData& data = allTrainers[MAP_ROUTE111_2];
    if (data.initialized) return;
    
    data.trainers.push_back(createCoolTrainerBrooke(db));
    data.trainers.push_back(createBlackBeltDaisuke(db));
    data.trainers.push_back(createInterviewerGabbyAndTy(db));
    
    data.prizeMoney = {864,608,1824};
    
    data.initialized = true;
}

static void registerRoute111DesertTrainers(){
    RouteTrainerData& data = allTrainers[MAP_ROUTE111_DESERT];
    if (data.initialized) return;
    
    data.trainers.push_back(createCamperCliff(db));
    data.trainers.push_back(createPicnickerHeidi(db));
    data.trainers.push_back(createCamperDrew(db));
    data.trainers.push_back(createPicnickerBecky(db));
    
    data.prizeMoney = {352,368,384,384};
    
    data.initialized = true;
}

static void registerRoute112ATrainers(){
    RouteTrainerData& data = allTrainers[MAP_ROUTE112_1];
    if (data.initialized) return;
    
    data.trainers.push_back(createHikerBrice(db));
    data.trainers.push_back(createHikerTrent(db));
    data.trainers.push_back(createCamperLarry(db));
   
    data.prizeMoney = {680,640,288};
    
    data.initialized = true;
}

static void registerMtChimneySummitTrainers(){
    RouteTrainerData& data = allTrainers[MAP_MTCHIMNEY_SUMMIT];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamMagmaAdmin(db));
    data.trainers.push_back(createTeamMagmaGruntSummit1(db));
    data.trainers.push_back(createTeamMagmaGruntSummit2(db));
   
    data.prizeMoney = {880,400,400};
    
    data.initialized = true;
}

static void registerJaggedPassTrainers(){
	 RouteTrainerData& data = allTrainers[MAP_JAGGEDPASS];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamMagmaGruntJagged(db));
    data.trainers.push_back(createHikerEric(db));
    data.trainers.push_back(createTriathleteJulio(db));
    data.trainers.push_back(createPicnickerAutumn(db));
   
    data.prizeMoney = {440,800,840,336};
    
    data.initialized = true;
}

static void registerLavaridgeGymTrainers() {
	 RouteTrainerData& data = allTrainers[MAP_LAVARIDGEGYM];
    if (data.initialized) return;
    
    data.trainers.push_back(createGymLeaderFlannery(db));
    data.prizeMoney = {2900};
    data.initialized = true;
}

static void registerPetalburgGymTrainers() {
	 RouteTrainerData& data = allTrainers[MAP_PETALBURGGYM];
    if (data.initialized) return;
    
    data.trainers.push_back(createGymLeaderNorman(db));
    data.prizeMoney = {3100};
    data.initialized = true;
}

static void registerRoute118Trainers() {
	 RouteTrainerData& data = allTrainers[MAP_ROUTE118];
    if (data.initialized) return;
    
    data.trainers.push_back(createBirdKeeperChester(db));
    data.trainers.push_back(createInterviewerGabbyAndTyRoute118(db));
    data.trainers.push_back(createGuitaristDalton(db));
    data.prizeMoney = {800,2592,480};
    data.initialized = true;
}

static void registerRoute119Trainers() {
    RouteTrainerData& data = allTrainers[MAP_ROUTE119];
    if (data.initialized) return;
    
    data.trainers.push_back(Trainer());
    data.trainers.push_back(createBugManiacTaylor(db));
    data.trainers.push_back(createNinjaBoyTakashi(db));
    
    data.prizeMoney = {1860, 1500, 300};
    
    data.initialized = true;
}

static void registerFortreeGymTrainers() {
	 RouteTrainerData& data = allTrainers[MAP_FORTREEGYM];
    if (data.initialized) return;
    
    data.trainers.push_back(createGymLeaderWinona(db));
    data.prizeMoney = {3300};
    data.initialized = true;
}

static void registerMtPyreInteriorTrainers() {
	 RouteTrainerData& data = allTrainers[MAP_MTPYRE1];
    if (data.initialized) return;
    
    data.trainers.push_back(createHexManiacTasha(db));
    data.trainers.push_back(createHexManiacValerie(db));
    data.prizeMoney = {768, 768};
    data.initialized = true;
}

static void registerMtPyreExteriorTrainers() {
	 RouteTrainerData& data = allTrainers[MAP_MTPYRE2];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamAquaGruntMP1(db));
    data.trainers.push_back(createTeamAquaGruntMP2(db));
    data.prizeMoney = {640, 640};
    data.initialized = true;
}

static void registerMagmaHideoutTrainers(){
	RouteTrainerData& data = allTrainers[MAP_MAGMA_HIDEOUT_2];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamMagmaGruntMH1(db));
    data.trainers.push_back(createTeamMagmaGruntMH2(db));
    data.trainers.push_back(createTeamMagmaGruntMH3(db));
    data.trainers.push_back(createTeamMagmaGruntMH4(db));
    data.prizeMoney = {580, 580, 580, 580};
    data.initialized = true;
}

static void registerMagmaHideoutBossTrainers(){
	RouteTrainerData& data = allTrainers[MAP_MAGMA_HIDEOUT_4];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamMagmaLeaderMaxie(db));
    data.prizeMoney = {3120};
    data.initialized = true;
}

static void registerAquaHideout1Trainers(){
	RouteTrainerData& data = allTrainers[MAP_AQUA_HIDEOUT_1];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamAquaGruntAH1(db));
    data.prizeMoney = {640};
    data.initialized = true;
}

static void registerAquaHideout2Trainers(){
	RouteTrainerData& data = allTrainers[MAP_AQUA_HIDEOUT_2];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamAquaGruntAH2(db));
    data.trainers.push_back(createTeamAquaGruntAH3(db));
    data.prizeMoney = {640, 640};
    data.initialized = true;
}

static void registerAquaHideout3Trainers(){
	RouteTrainerData& data = allTrainers[MAP_AQUA_HIDEOUT_3];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamAquaGruntAH4(db));
    data.prizeMoney = {640};
    data.initialized = true;
}

static void registerAquaHideout4Trainers(){
	RouteTrainerData& data = allTrainers[MAP_AQUA_HIDEOUT_4];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamAquaGruntAH5(db));
    data.trainers.push_back(createTeamAquaGruntAH6(db));
    data.prizeMoney = {640, 640};
    data.initialized = true;
}

static void registerAquaHideout5Trainers(){
	RouteTrainerData& data = allTrainers[MAP_AQUA_HIDEOUT_5];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamAquaAdminMatt(db));
    data.prizeMoney = {1360};
    data.initialized = true;
}

static void registerSeafloorCavernTrainers(){
	RouteTrainerData& data = allTrainers[MAP_SEAFLOOR_CAVERN_2];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamAquaLeaderArchie(db));
    data.prizeMoney = {6880};
    data.initialized = true;
}

static void registerMossdeepGymTrainers(){
	RouteTrainerData& data = allTrainers[MAP_MOSSDEEPGYM];
    if (data.initialized) return;
    
    data.trainers.push_back(createGymLeaderTateLiza(db));
    data.prizeMoney = {8400};
    data.initialized = true;
}

static void registerMossDeepSpaceCenterTrainers(){
	RouteTrainerData& data = allTrainers[MAP_MOSSDEEP3];
    if (data.initialized) return;
    
    data.trainers.push_back(createTeamMagmaLeaderMaxieMD(db));
    data.prizeMoney = {5120};
    data.initialized = true;
}

static void registerSootopolisGymTrainers(){
	RouteTrainerData& data = allTrainers[MAP_SOOTOPOLISGYM];
    if (data.initialized) return;
    
    data.trainers.push_back(createGymLeaderJuan(db));
    data.prizeMoney = {4600};
    data.initialized = true;
}

static void registerVictoryRoad1Trainers(){
	RouteTrainerData& data = allTrainers[MAP_VICTORY_ROAD1];
    if (data.initialized) return;
    
    data.trainers.push_back(createCoolTrainerHalle(db));
    data.trainers.push_back(createCoolTrainerEdgar(db));
    data.prizeMoney = {2064,2064};
    data.initialized = true;
}

static void registerVictoryRoad2Trainers(){
	RouteTrainerData& data = allTrainers[MAP_VICTORY_ROAD2];
    if (data.initialized) return;
    
    data.trainers.push_back(createTrainerWally(db));
    data.trainers.push_back(createProgrammerSelwyn(db));
    
    data.prizeMoney = {2700,3500};
    data.initialized = true;
}

static void registerLeagueRM1Trainers(){
	RouteTrainerData& data = allTrainers[MAP_LEAGUE_RM1];
    if (data.initialized) return;
    
    data.trainers.push_back(createElite4Sidney(db));
    
    data.prizeMoney = {4900};
    data.initialized = true;
}

static void registerLeagueRM2Trainers(){
	RouteTrainerData& data = allTrainers[MAP_LEAGUE_RM2];
    if (data.initialized) return;
    
    data.trainers.push_back(createElite4Phoebe(db));
    
    data.prizeMoney = {5100};
    data.initialized = true;
}

static void registerLeagueRM3Trainers(){
	RouteTrainerData& data = allTrainers[MAP_LEAGUE_RM3];
    if (data.initialized) return;
    
    data.trainers.push_back(createElite4Glacia(db));
    
    data.prizeMoney = {5300};
    data.initialized = true;
}

static void registerLeagueRM4Trainers(){
	RouteTrainerData& data = allTrainers[MAP_LEAGUE_RM4];
    if (data.initialized) return;
    
    data.trainers.push_back(createElite4Drake(db));
    
    data.prizeMoney = {5500};
    data.initialized = true;
}

static void registerLeagueRM5Trainers(){
	RouteTrainerData& data = allTrainers[MAP_LEAGUE_RM5];
    if (data.initialized) return;
    
    data.trainers.push_back(createChampionWallace(db));
    
    data.prizeMoney = {11600};
    data.initialized = true;
}

static void initTrainersForMap(MapID mapId) {
    switch (mapId) {
        case MAP_ROUTE102: registerRoute102Trainers(); break;
        case MAP_PETALBURGWOODS1: registerPetalburgWoods1Trainers(); break;
        case MAP_PETALBURGWOODS2: registerPetalburgWoods2Trainers(); break;
        case MAP_RUSTBOROGYM: registerRustboroGymTrainers(); break;
        case MAP_RUSTBORO1: registerRustboro1Trainers(); break;
        case MAP_DEWFORDGYM: registerDewfordGymTrainers(); break;
        case MAP_OCEANIC_MUSEUM: registerOceanicMuseumTrainers(); break;
        case MAP_ROUTE110_2: registerRoute110Trainers(); break;
        case MAP_MAUVILLEGYM: registerMauvilleGymTrainers(); break;
        case MAP_ROUTE111_1: registerRoute111ATrainers(); break;
        case MAP_ROUTE111_2: registerRoute111BTrainers(); break;
        case MAP_ROUTE111_DESERT: registerRoute111DesertTrainers(); break;
        case MAP_ROUTE112_1: registerRoute112ATrainers(); break;
        case MAP_MTCHIMNEY_SUMMIT: registerMtChimneySummitTrainers(); break;
        case MAP_JAGGEDPASS: registerJaggedPassTrainers(); break;
        case MAP_LAVARIDGEGYM: registerLavaridgeGymTrainers(); break;
        case MAP_PETALBURGGYM: registerPetalburgGymTrainers(); break;
        case MAP_ROUTE118: registerRoute118Trainers(); break;
        case MAP_ROUTE119: registerRoute119Trainers(); break;
        case MAP_FORTREEGYM: registerFortreeGymTrainers(); break;
        case MAP_MTPYRE1: registerMtPyreInteriorTrainers();break;
        case MAP_MTPYRE2: registerMtPyreExteriorTrainers();break;
        case MAP_MAGMA_HIDEOUT_2: registerMagmaHideoutTrainers(); break;
        case MAP_MAGMA_HIDEOUT_4: registerMagmaHideoutBossTrainers(); break;
        case MAP_AQUA_HIDEOUT_1: registerAquaHideout1Trainers(); break;
        case MAP_AQUA_HIDEOUT_2: registerAquaHideout2Trainers(); break;
        case MAP_AQUA_HIDEOUT_3: registerAquaHideout3Trainers(); break;
        case MAP_AQUA_HIDEOUT_4: registerAquaHideout4Trainers(); break;
        case MAP_AQUA_HIDEOUT_5: registerAquaHideout5Trainers(); break;
        case MAP_SEAFLOOR_CAVERN_2: registerSeafloorCavernTrainers(); break;
        case MAP_MOSSDEEPGYM: registerMossdeepGymTrainers(); break;
        case MAP_MOSSDEEP3: registerMossDeepSpaceCenterTrainers(); break;
        case MAP_SOOTOPOLISGYM: registerSootopolisGymTrainers(); break;
        case MAP_VICTORY_ROAD1: registerVictoryRoad1Trainers(); break;
        case MAP_VICTORY_ROAD2: registerVictoryRoad2Trainers(); break;
        case MAP_LEAGUE_RM1: registerLeagueRM1Trainers(); break;
        case MAP_LEAGUE_RM2: registerLeagueRM2Trainers(); break;
        case MAP_LEAGUE_RM3: registerLeagueRM3Trainers(); break;
        case MAP_LEAGUE_RM4: registerLeagueRM4Trainers(); break;
        case MAP_LEAGUE_RM5: registerLeagueRM5Trainers(); break;
        default: break;
    }
}

static bool isTrainerDefeated(MapID mapId, int trainerId) {
    for (const auto& dt : defeatedTrainers) {
        if (dt.first == mapId && dt.second == trainerId) return true;
    }
    return false;
}

static void markTrainerDefeated(MapID mapId, int trainerId) {
    if (!isTrainerDefeated(mapId, trainerId)) {
        defeatedTrainers.push_back({mapId, trainerId});
    }
}

static void scanTrainersInMap(MapID mapId, const vector<string>& layout) {
    currentTrainers.clear();
    initTrainersForMap(mapId);
    
    auto it = allTrainers.find(mapId);
    if (it == allTrainers.end() || !it->second.initialized) return;
    
    int trainerIndex = 0;
    for (int r = 0; r < (int)layout.size(); ++r) {
        for (int c = 0; c < (int)layout[r].size(); ++c) {
            if (layout[r][c] != TileChars::NPC_MARKER) continue;
            if (trainerIndex >= (int)it->second.trainers.size()) break;
            
            bool wasDefeated = isTrainerDefeated(mapId, trainerIndex);
            currentTrainers.push_back({r, c, trainerIndex, wasDefeated});
            trainerIndex++;
        }
    }
}

static void triggerTrainerBattle(TrainerNPC& trainerNpc, Player& player) {
    auto it = allTrainers.find(currentMapID);
    if (it == allTrainers.end()) return;
    if (isTrainerDefeated(currentMapID, trainerNpc.trainerId)) return;
    
    RouteTrainerData& routeData = it->second;
    if (trainerNpc.trainerId >= (int)routeData.trainers.size()) return;
    
    Trainer& trainer = routeData.trainers[trainerNpc.trainerId];
    
    //RIVAL SPECIAL CASES
    //Rustboro
    if (currentMapID == MAP_RUSTBORO1 && trainerNpc.trainerId == 0) {
        if (player.getBadgeCount() < 1) {
            showDialogue(rivalName + ": You must be " + player.username + 
                        "! My mom always talks about you. Become stronger first and let's battle after you get your first badge!");
            pressAnyKey();
            return;
        }
        
        // Create the actual rival trainer based on player
        trainer = createRustboroRival(rivalName, player, db);
    }
    
    //Route110
    if(currentMapID == MAP_ROUTE110_2 && trainerNpc.trainerId == 2){
    	trainer = createRoute110Rival(rivalName, player, db);
	}
	
	// Route 119 Rival
	if (currentMapID == MAP_ROUTE119 && trainerNpc.trainerId == 0) {
	    trainer = createRoute119Rival(rivalName, player, db);
	}
    
    clearScreen();
    showDialogue(trainer.trainerName + ": " + trainer.preBattleLine);
    pressAnyKey();
    
    // Find player's first alive Pokemon
    Pokemon* playerActive = nullptr;
    for (auto& p : player.currentPokemon) {
        if (p.currentHP > 0) {
            playerActive = &p;
            break;
        }
    }
    
    if (!playerActive) {
        showDialogue("You have no usable POKEMON!");
        pressAnyKey();
        return;
    }
    
    // Reset trainer Pokemon for battle
    for (auto& p : trainer.currentPokemon) {
        p.currentHP = p.baseHP;
        p.initializePP(db.moveList);
    }
    
    int trainerFirst = trainer.firstAliveIndex();
    if (trainerFirst == -1) return;
    
    Pokemon& enemyPokemon = trainer.currentPokemon[trainerFirst];
    startBattleLoop(player, *playerActive, enemyPokemon, db, &trainer);
    
    // Post-battle handling
    if (!trainer.hasAlivePokemon()) {
        markTrainerDefeated(currentMapID, trainerNpc.trainerId);
        trainerNpc.defeated = true;
        
        int prizeMoney = 0;
        if (trainerNpc.trainerId < (int)routeData.prizeMoney.size()) {
            prizeMoney = routeData.prizeMoney[trainerNpc.trainerId];
        }
        player.money += prizeMoney;
        
        clearScreen();
        showDialogue(trainer.trainerName + ": " + trainer.postBattleLine);
        pressAnyKey();
        
        if (prizeMoney > 0) {
            showDialogue("You received " + to_string(prizeMoney) + " P!");
            pressAnyKey();
        }
        
        
        // BADGE AWARD EXPANSION HERE
        if (trainer.role == GYM_LEADER) {
            int badgeToAward = -1;
            string badgeName;
            
            if (currentMapID == MAP_RUSTBOROGYM) {
                badgeToAward = BadgeIndex::STONE_BADGE;
                badgeName = "STONE BADGE";
            }
            
            else if (currentMapID == MAP_DEWFORDGYM) {
		        badgeToAward = BadgeIndex::KNUCKLE_BADGE;
		        badgeName = "KNUCKLE BADGE";
		    }
		    else if (currentMapID == MAP_MAUVILLEGYM) {
		        badgeToAward = BadgeIndex::DYNAMO_BADGE;
		        badgeName = "DYNAMO BADGE";
		    }
		    else if (currentMapID == MAP_LAVARIDGEGYM) {
		        badgeToAward = BadgeIndex::HEAT_BADGE;
		        badgeName = "HEAT BADGE";
		    }
		    else if (currentMapID == MAP_PETALBURGGYM) {
		        badgeToAward = BadgeIndex::BALANCE_BADGE;
		        badgeName = "BALANCE BADGE";
		    }
		    else if (currentMapID == MAP_FORTREEGYM) {
		        badgeToAward = BadgeIndex::FEATHER_BADGE;
		        badgeName = "FEATHER BADGE";
		    }
		    else if (currentMapID == MAP_MOSSDEEPGYM){
		    	badgeToAward = BadgeIndex::MIND_BADGE;
		        badgeName = "MIND BADGE";
			}
			else if (currentMapID == MAP_SOOTOPOLISGYM){
		    	badgeToAward = BadgeIndex::RAIN_BADGE;
		        badgeName = "RAIN BADGE";
			}
            
            if (badgeToAward >= 0 && !player.hasBadge(badgeToAward)) {
                player.earnBadge(badgeToAward);
                clearScreen();
                showDialogue(player.username + " obtained the " + badgeName + "!");
                pressAnyKey();
            }
        }
        
        //OCEANIC MUSEUM SPECIAL CASE
         if (currentMapID == MAP_OCEANIC_MUSEUM && !flagRescueStern) {
            bool allGruntsDefeated = true;
            for (int i = 0; i < 2; i++) {  // Check first 2 trainers
                if (!isTrainerDefeated(MAP_OCEANIC_MUSEUM, i)) {
                    allGruntsDefeated = false;
                    break;
                }
            }
            
            if (allGruntsDefeated) {
                clearScreen();
                showDialogue("CAPTAIN STERN: Thank you for saving me! Those Team Aqua grunts were after my research!");
                pressAnyKey();
                showDialogue("CAPTAIN STERN: They mentioned something about their operations on ROUTE 110 being called off. The path should be clear now!");
                pressAnyKey();
                flagRescueStern = true;
                flagUnblockRoute110 = true;  // Unblock Route 110 after rescue
            }
        }
        
        //MT CHIMNEY SUMMIT SPECIAL CASE
        if (currentMapID == MAP_MTCHIMNEY_SUMMIT && !flagMtSummitCleared) {
            bool allMagmaDefeated = true;
            for (int i = 0; i < 3; i++) {
                if (!isTrainerDefeated(MAP_MTCHIMNEY_SUMMIT, i)) {
                    allMagmaDefeated = false;
                    break;
                }
            }
            
            if (allMagmaDefeated) {
                showDialogue("The TEAM MAGMA members have been defeated!");
                showDialogue("The path to their strange machine is now clear...");
                flagMtSummitCleared = true;
            }
        }
        
        //MAXIE MAGMA HIDEOUT SPECIAL CASE
        if(currentMapID== MAP_MAGMA_HIDEOUT_4 && !flagMaxieDefeated){
        	flagMaxieDefeated=true;
        	maxiePostDefeat();
		}
		
		//MATT AQUA HIDEOUT SPECIAL CASE
		if(currentMapID== MAP_AQUA_HIDEOUT_5 && !flagUnlockSeafloorCavern){
			flagUnlockSeafloorCavern=true;
			mattPostDefeat();
		}
		
		//ARCHIE AQUA CAVERN SPECIAL CASE
		if(currentMapID==MAP_SEAFLOOR_CAVERN_2 && !flagArchieDefeated){
			flagArchieDefeated=true;
			archiePostDefeat();
			
			if(!flagAwakenKyogre){
				playAwakenKyogre();
				flagAwakenKyogre= true;
				archiePostKyogre();
				maxieToArchie();
			}
		} 
		
		//MOSSDEEP SPACE CENTER MAXIE SPECIAL CASE
		if (currentMapID == MAP_MOSSDEEP3 && !flagRocketEvent){
			maxiePostDefeatRocket(player);
			stevenPostRocket(player);
			flagRocketEvent=true;
		}
		
		if (currentMapID== MAP_LEAGUE_RM5 && !flagWallaceDefeated){
			flagWallaceDefeated=true;
			wallacePostDefeat(player);
			hallOfFameSequence(player);
			playEndCredits();
			//TODO: reset loop and go back to mom house
			enterMap(MAP_MOMHOUSE, 'X', 0, 0);
		}
		
    }
}

static bool tryTriggerTrainer(int row, int col, Player& player) {
    for (auto& trainerNpc : currentTrainers) {
        if (trainerNpc.row != row || trainerNpc.col != col) continue;
        
        if (!isTrainerDefeated(currentMapID, trainerNpc.trainerId)) {
            triggerTrainerBattle(trainerNpc, player);
            return true;
        }
        
        // Already defeated - show post-battle line
        auto it = allTrainers.find(currentMapID);
        if (it != allTrainers.end() && trainerNpc.trainerId < (int)it->second.trainers.size()) {
            Trainer& trainer = it->second.trainers[trainerNpc.trainerId];
            showDialogue(trainer.trainerName + ": " + trainer.postBattleLine);
            pressAnyKey();
        }
        return true;
    }
    return false;
}

//***SPECIAL CHARACTER CASES

//Mt Chimney Machine
static bool tryInteractWithMachine(int row, int col, Player& player) {
    // Check if this is the machine tile '[+]'
    if (!currentMap) return false;
    if (!isInBounds(currentMap->layout, row, col)) return false;
    
    char tile = currentMap->layout[row][col];
    
    // Check if this is the machine (special character '+')
    if (tile != '+') return false;
    
    // Only on Mt. Chimney Summit
    if (currentMapID != MAP_MTCHIMNEY_SUMMIT) return false;
    
    // Check if player has beaten all Team Magma members
    if (!flagMtSummitCleared) {
        showDialogue("TEAM MAGMA ADMIN TABITHA: You're not going anywhere near that, kid. Beat it!");
        pressAnyKey();
        return true;
    }
    
    // Player has cleared the summit - allow interaction
    if (!flagEncounterMachine) {
        flagEncounterMachine = true; 
		showDialogue("A strange machine is whirring loudly.");
        showDialogue("Something inside it pulses with heat. It feels unstable.");
        showDialogue("The machine suddenly powers down with a loud hiss...");
        showDialogue("TEAM MAGMA must have been using this for something sinister!"); 
        showDialogue("With the machine disabled, the path down JAGGED PASS should be accessible now.");
    } else {
        showDialogue("The machine sits silent and inactive.");
        pressAnyKey();
    }
    
    return true;
}
/*****************************************************************************
 * NPC SYSTEM
 *****************************************************************************/
static void scanNPCsInMap(const vector<string>& layout) {
    currentNPCs.clear();
    scanTrainersInMap(currentMapID, layout);
    
    int counter = 0;
    for (int r = 0; r < (int)layout.size(); ++r) {
        for (int c = 0; c < (int)layout[r].size(); ++c) {
            if (!isNPCTile(layout[r][c])) continue;
            
            // Skip positions occupied by trainers
            bool isTrainerPos = false;
            for (const auto& trainer : currentTrainers) {
                if (trainer.row == r && trainer.col == c) {
                    isTrainerPos = true;
                    break;
                }
            }
            
            if (!isTrainerPos) {
                currentNPCs.push_back({r, c, counter});
                ++counter;
            }
        }
    }
}

static string getDialogueForNPC(MapID mapId, int npcId, const Player& player) {
    switch (mapId) {
        case MAP_LITTLEROOT:
            switch (npcId) {
                case 0: return "MOM: You must be tired after moving. Go see Prof. Birch!";
                case 1: 
                    if (flagHasStarter) 
					return "I heard Rustboro's Gym Leader really likes Rock-type POKEMON.";
                    return "It's a nice day to explore, but make sure you visit Professor Birch first.";
                case 2: return "Neighbors are friendly here. Say hi if you see them.";
                default: return "This town sure is peaceful.";
            }
        
        case MAP_MOMHOUSE:
            switch (npcId) {
                case 0: return "MOM: I'll prepare your things. Go to the lab later.";
                default: return "MOM: Take care out there!";
            }
        
        case MAP_RIVALHOUSE:
            switch (npcId) {
                case 0: return "My kid loves challenges, I think you two will get along!";
                case 1: return "Oh, you're the new neighbor? How do you like it here so far?";
                default: return "My kid's been really excited to meet you, head along to RUSTBORO";
            }
        
        case MAP_BIRCHLAB:
            return "Prof. Birch is often outside collecting samples.";
        
        case MAP_ROUTE101:
            switch (npcId) {
                case 0: return "If your POKEMON gets tired, visit a POKEMON CENTER.";
                default: return "Wild POKEMON jump at you in tall grass!";
            }
        
        case MAP_POKECENTER:
            switch (npcId) {
                case 0: return "NURSE: Welcome to the POKEMON CENTER! Would you like to heal your POKEMON?";
                default: return "NURSE: Have a nice day!";
            }
        
        case MAP_POKEMART:
            switch (npcId) {
                case 0: return "CLERK: Welcome to the POKEMART! What can I get for you today?";
                case 1: return "We have all the items a trainer needs!";
                default: return "Thanks for shopping with us!";
            }
        
        case MAP_OLDALE:
            switch (npcId) {
                case 0: return "Labada sakong ulo oy, mamalit kog Tylenol sa POKEMART.";
                case 1: return "Akong POKEMON gasakit kay gipa program ani sa C++, adto tag POKEMON CENTER.";
                default: return "Welcome to Old Dale!";
            }
            
        case MAP_PETALBURG1:
        	switch (npcId){
        		case 0:return "The strongest trainers are GYM LEADERS, and TRAINERS with EXTRACURRICULAR BADGES!";
        		case 1:return "The more TRAINERS you defeat, the stronger.. and richer you become!";
        		default: return"Welcome to Petalburg!";
			}
			
		case MAP_PETALBURG2:
			switch (npcId){
				case 0: return "Okay kid, I tried to challenge your father, but that SLAKING... THAT SLAKING WITHOUT TRUANT...";
				case 1: return "You must be the kid NORMAN is talking about. HE'S AN ABSOLUTE MENACE WITHOUT TRUANT!";
				default: return "Welcome to Petalburg!";
			}	
			
		case MAP_PETALBURGGYM:
		    switch (npcId) {
		        case 0: {
		            // Norman - Badge check
		            if (player.getBadgeCount() < 4) {
		                return "NORMAN: My child, you must prove yourself by earning FOUR BADGES before I can battle you.";
		            } else {
		                return "";
		            }
		        }
		        default: return "Train hard to become strong!";
		    }
		    
		case MAP_ROUTE104:
		    switch (npcId) {
		        case 0: {
		            // Mr. Briney - Badge check
		            if (player.getBadgeCount() < 1) {
		                return "MR. BRINEY: Ahoy there! You look like a beginning trainer. Come back after you've earned at least ONE GYM BADGE, and I'll give you a ride on my boat!";
		            }
		            return "MR. BRINEY: Ahoy there! Ready to sail?";
		        }
		        default: return "The sea breeze feels wonderful!";
		    }
		    
		case MAP_RUSTBORO1:
		    switch (npcId) {
		        case 0: return "I challenged the GYM LEADER, but my FIRE TYPE POKEMON could not beat her ROCK TYPE POKEMON!";
		        case 1: return "Welcome to Rustboro City! Home of the famous DEVON CORPORATION!";
		        case 2: return "ROXANNE scored the highest in the exams, and outperformed her peers on activities and still got ranked lower, life be critting unfairly sometimes";
		        case 3: {
		            if (isTrainerDefeated(MAP_RUSTBORO1, 0)) {
		                return rivalName + ": You've gotten really strong! I need to train more!";
		            }
		            //fallback case
		            return rivalName + ": " + player.username + "! I've been waiting for you!";
		        }
		        case 4: return "I've seen people outperform entire teams and STILL get overshadowed. Happens in Devon. happens in school...";
		        default: return "Hello there!";
		    }
	    
	    case MAP_RUSTBORO2:
	    	switch(npcId){
	    		case 0: return "I've seen Magikarp get more fair treatment.";
	    		case 1: return "You can outscore e'm all, but you can't outrank e'm all I guess...";
	    		case 2: return "Effort and Skill aren't super effective against a TRAINER with EXTRACURRICULAR buffs";
			}
    
    	case MAP_DEWFORD1:
		    switch (npcId) {
		        case 0: return "Funny how the TOP SPOT ain't always EARNED.";
		        case 1: {
		        	 // Mr. Briney
		            return "MR. BRINEY: Ahoy there! Ready to sail back?";
		        }
		        case 2: return "If life had Gym rules, you'd be Champion already.";
		        default: return "Welcome to Dewford!";
		    }
		
		case MAP_DEWFORD2:
			switch (npcId){
				case 0: return "I wish I could be a TRAINER, but the DEV can't seem to find my input on the BULBAPEDIA page.";
				default: return "Welcome to Dewford!";
			}
			
		case MAP_GRANITECAVE:
		    switch (npcId) {
		        case 0: {
		            if (!flagMetSteven) {
		                return "STEVEN: Oh! I didn't expect to see anyone else down here. I was researching rare stones when I got a bit turned around. Thanks for finding me! I should head back to Rustboro now.";
		            }
		            return "STEVEN: The rare stones in Hoenn are truly fascinating. Good luck on your journey!";
		        }
		        default: return "The cave is dark and mysterious...";
		    }
		    
		case MAP_SLATEPORT1:
			switch (npcId){
				case 0: return "Team Aqua's been acting suspiciously. I hope someone stops them soon...";
				case 1: return "I heard ruckus in the OCEANIC MUSEUM.";
				case 2: return "TEAM AQUA GRUNT: Erghh.. Why am I here again?.. Durgh..";
			}
			
		case MAP_SLATEPORT2:
			switch (npcId){
				case 0: return "What's up with that line over there....";
				case 1: return "TEAM AQUA GRUNT: Man, this line's more fair than your midterm class ranking...";
				case 2: return "TEAM AQUA GRUNT: Erghh.. Why am I here again?.. Durgh..";
				case 3: return "TEAM AQUA GRUNT: Bruh, I outwork these grunts and im still not in the front, Bias everywhere...";
				case 4: return "TEAM AQUA GRUNT: Bro I stole 5 DEVON GOODS. FIVE! and I'm still not GRUNT OF THE MONTH, smh...";
				case 5: return "TEAM AQUA GRUTN: Why do weak grunts get promotion? Oh right, connections...";
			}
		
		case MAP_ROUTE110_1:
	    if (!flagCheckBlockRoute110) {
	        // First time - any grunt triggers the flag
	        switch (npcId) {
	            case 0: 
	            case 1: 
	            case 2: 
	            case 3:
	                return "TEAM AQUA GRUNT: Beat it Punk! Team Aqua is busy doing definitely NOT sus activities at the OCEANIC MUSEUM in SLATEPORT...";
	            default: return "Hello.";
	        }
	    } 
		else if (!flagUnblockRoute110) {
	        // After checking but before rescuing Stern
	        switch (npcId) {
	            case 0: return "TEAM AQUA GRUNT: What are you looking at? Get lost!";
	            case 1: return "TEAM AQUA GRUNT: We're not moving until our mission is complete!";
	            case 2: return "TEAM AQUA GRUNT: You should probably go check on that museum situation...";
	            case 3: return "TEAM AQUA GRUNT: Nothing to see here, move along!";
	            default: return "Hello.";
	        }
	    }
		 else {
	       
	        return "TEAM AQUA GRUNT: Team Aqua will have its way!... fairly...";
	    }
				
		
		case MAP_OCEANIC_MUSEUM:
		    switch (npcId) {
		        case 0:  // Captain Stern
		            if (!flagRescueStern) {
		                return "CAPTAIN STERN: Help! These Team Aqua grunts are after my research!";
		            }
		            return "CAPTAIN STERN: Thank you so much for rescuing me! Team Aqua won't bother us anymore.";
		        default: 
		            return "What a commotion...";
		    }
		    
		 case MAP_MAUVILLE:
			switch (npcId){
				case 0: return "I wish there were bikes here, but the programmer doesn't know how to implement it...";
				case 1: return "Give the dev a break, bike physics on a terminal is over the top don't you think...";
				case 2: return "Electric Wiiizaaarr- Oh wait wrong game...";
				case 3: return "Sorry for the dialogues, the programmer making this is frankly, short-circuiting...";
				case 4: return "Sometimes I hear static in the air.. Oh wait, that's just the dev screaming at the compiler...";
			}  
		    
		case MAP_CABLECAR:
	    switch (npcId) {
	        case 0: {
	            if (player.getBadgeCount() < 3) {
	                return "CABLE CAR LADY: I'm sorry, but the CABLE CAR to MT. CHIMNEY is UNAVAILABLE at the moment...";
	            } else {
	                return "CABLE CAR LADY: The CABLE CAR is ready to go. Would you like to be on it?";
	            }
	        }
	    }
	    
	    case MAP_LAVARIDGE:
	    	switch (npcId){
				case 0: return "Being buried in hot sand is sigh.. so warm and Heavenly..Eh? Gyah! A POKEMON nipped my backside!";
				case 1: return "Oh you like hot springs do you? That's surprising for someone as young as you.";
				case 2: return "I bathe in the bath sprrings everyday. I want to become a beautiful and strong GYM LEADER like FLANNERY...";
				case 3: return "Welcome to LAVARIDGE! We have the best hot springs in HOENN!...";
			}
		case MAP_HARBOR:
			switch (npcId){
				case 0:
				//Mr Briney Badge Check - min 4
				if (player.getBadgeCount() < 4){
					return "MR BRINEY: Ahoy there! Quite the sight, ain't she? The grand vessel they're buildin, the S.S. Tidal. I'm hopin to ern me place as her first captain once she's finished buildin'!...";
				}
				else{
					return " CAPT'N BRINEY: Ho-ho! Look who's back! I've been chosen as the Tidal's captain! The sea is wide open, where to, matey?";
				}
			}
			
		case MAP_FORTREE:
		    switch (npcId) {
		        case 0: return "Living in the treetops is amazing! The view is incredible, and you feel so close to Flying-type POKEMON!";
		        case 1: return "FORTREE GYM LEADER WINONA is incredibly skilled with Flying-type POKEMON. Good luck challenging her!";
		        case 2: return "I heard there's an invisible POKEMON blocking the path to the next route. How strange...";
		        case 3: return "The houses here are built high in the trees. It took some getting used to, but now I can't imagine living anywhere else!";
		    }  
	    
	    case MAP_LILYCOVE1:
		    switch (npcId) {
		        case 0: return "Have you heard of the LEGENDARY POKEMON that trembles the LAND? What was that POKEMON's name again...";
		        case 1: return "I heard there's a mysterious rumbling cave entrance NORTH EAST of this city...";
		        case 2: return "I also heard that cave has something to do with TEAM AQUA...But what do I know...";
		    }
	    
		case MAP_LILYCOVE2:
		    switch (npcId) {
		        case 0: return "The city's Department Store has everything! Clothes, TMs, dolls... if only my wallet can keep up with my taste...";
		        case 1: return "Have you heard of the LEGENDARY POKEMON that trembles the OCEAN? What was that POKEMON's name again...";
		        case 2: return "I heard someone saw a suspicious TEAM AQUA GRUNT sneaking into a small cave entrance up ahead...Sus indeed....";
		        case 3: return "Did you know someone punted TEAM AQUA out of the way for us! Got the WAILMER out of the cove, so we can get our boats again!";
		    }
		  
	    case MAP_LILYCOVE3:
		    switch (npcId) {
		        case 0: return "Have you heard of the LEGENDARY POKEMON that trembles the SKY? What was that POKEMON's name again...";
		        case 1: return "That cave by the cove... I didn't notice it till now.. but it looks suspicious now that I see it...";
		    }
	    
	    case MAP_AQUA_ENTRANCE:
		    switch (npcId) {
		        case 0: {
		            if (!flagOrbStolen) {
		                flagCheckAquaEntrance = true;
		                return "TEAM AQUA GRUNT: TEAM AQUA is on a special mission at MT PYRE. About the mission? Ha! Do you really think I'm dumb enough to tell you that?";
		            }
		            if (!flagAwakenGroudon) {
		                return "TEAM AQUA GRUNT: You again?! Sorry, hideout's closed. Boss is out dealing with MAGMA...";
		            }
		            
		            return "TEAM AQUA GRUNT: *Snoring...* mmhmm.. hotdogs...";
		        }
		        case 1: {
		            if (!flagOrbStolen) {
		                flagCheckAquaEntrance = true;
		                return "TEAM AQUA GRUNT: TEAM AQUA is on a mission to steal the RED ORB. Where to? Ha! Do you really think I'm dumb enough to tell you that?";
		            }
		            if (!flagAwakenGroudon) {
		                return "TEAM AQUA GRUNT: You again?! Sorry, hideout's closed. We're currently dealing with VOLCANIC activities...";
		            }
		            return "TEAM AQUA GRUNT: *Snoring...* mmmhmm... burgers....";
		        }
		    }
	    
	    case MAP_MTPYRE_ENTRANCE:
		    switch (npcId) {
		        case 0: return "CAPT'N BRINEY: Quite the place we sailed to ain't it kid? The sea is wide open, where to, matey?";
		    }
		    
		case MAP_MTPYRE_SUMMIT:
		    switch (npcId) {
		        case 0: return "We must stop those sinister men from whatever they are about to do with those orbs!...";
		        case 1: 
					if(flagAwakenGroudon){
						return "I fear something bad may happen if those two orbs are separated, TEAM AQUA left a clue about ACTIVITIES inside a COVE...";
					}
					return "I fear something bad may happen if those two orbs are separated, TEAM MAGMA left a clue about ACTIVITIES inside a VOLCANO...";
		    }
		case MAP_MOSSDEEP1:
		    switch (npcId){
		        case 0: {
		            if (flagRocketEvent && flagTrioEvent) {
		                return "CAPT'N BRINEY: The seas are far wide for the S.S. Tidal, she can sail wherever you want, Where to matey?";
		            }
		            if (flagRocketEvent && !flagTrioEvent) {
		                return "CAPT'N BRINEY: Matey, we must sail forth to SOOTOPOLIS now that her sails can DIVE thanks to the twins.";
		            }
		            return "CAPT'N BRINEY: Matey, we can go back, but the tides are getting stronger. I must urge you to talk to TATE and LIZA at the GYM, We must stop the flood, she must sail freely without fear.";
		        }
		    }
			
		case MAP_MOSSDEEP2:
			switch(npcId){
				case 0: return "The TWINS are known for their incredible psychic prowess. MOSSDEEP is in good hands thanks to our GYM LEADERS.";
				case 1: return "The SPACE CENTER will open for visitors soon. Once that happens, MOSSDEEP will be booming with its tourism. Just you wait...";
			}
		case MAP_MOSSDEEP3:
		    switch(npcId){
		        case 0: {
		            if (!flagRocketEvent) {
		                return "STEVEN: MAXIE will not have his way! Let's stop him once and for all!";
		            }
		            if (flagRocketEvent && !flagTrioEvent) {
		                return "STEVEN: That will teach MAXIE a lesson. But we must hurry to SOOTOPOLIS. We have no time to spare.";
		            }
		            return "STEVEN: Thanks to you, the weather is in balance once more. MOSSDEEP owes you a lot.";
		        }
		    }
		case MAP_SOOTOPOLIS1:
			switch(npcId){
				case 0:
					if(!flagMetWallace && !flagTrioEvent){
						return "STEVEN: The two ancient POKEMON, GROUDON and KYOGRE, have awakened from their long sleep. Now in conflict with uncontrollable energy.";
					}
					if (flagMetWallace && !flagTrioEvent){
						return "STEVEN: We have no time to spare. Only the ANCIENT POKEMON of the skies, RAYQUAZA, can stop this madness.";
					}
					return "STEVEN: Thank you. There are forces in this world that humans must never attempt to control.";
					
				case 1:
					if(!flagTrioEvent){
						return "TEAM MAGMA LEADER MAXIE: So this is the true power of GROUDON. My vision of expanding the continents... was flawed...";
					}
					return "TEAM MAGMA LEADER MAXIE: Human ideals are insignificant before nature. I was mistaken...";
				
				case 2:
					if(!flagTrioEvent){
						return "TEAM AQUA LEADER ARCHIE: So this is the end result... KYOGRE's power won't stop. The sea just keeps rising!";
					}
					return "TEAM AQUA LEADER ARCHIE: We never stood a chance of controlling that power. I let my ideals blind me to the truth...";
				case 3:
					if(!flagMetWallace && !flagTrioEvent){
						return "CAPT'N BRINEY: Matey, we can go back, but the tides are getting stronger. I must urge you to talk to STEVEN. We must stop the weather crisis. She must sail freely without fear.";
					}
					if (!flagTrioEvent){
						return "CAPT'N BRINEY: WALLACE has informed me that I must take ya to the SKY PILLAR. There is no time to spare. We must sail forth matey!";
					}
					 return "CAPT'N BRINEY: The seas are far wide for the S.S. Tidal, she can sail wherever you want, Where to matey?";
			}
			
		case MAP_CAVE_OF_ORIGIN:
			switch (npcId){
				case 0:
					if(!flagTrioEvent){
						return "WALLACE: We must hurry and awaken RAYQUAZA in the SKY PILLAR to put an end to this weather crisis.";
					}
					return "WALLACE: HOENN is once again restored to balance. I, WALLACE, Am forever in your debt.";
			}
		case MAP_SKY_PILLAR_ENTRANCE:
			switch (npcId){
				case 0:
					if(!flagAwakenRayquaza && !flagTrioEvent){
						return "CAPT'N BRINEY: Matey, we can go back, but the tides are getting stronger, the great ANCIENT DRAGON is just above that tower, I wish you the the best of luck.";
					}
					if (flagAwakenRayquaza && !flagTrioEvent){
						return "CAPT'N BRINEY: Oooh wow that's my first time seeing a dragon fly so fast if I blink me eye I would have missed it! Hurry now! To SOOTOPOLIS we go!";
					}
					return "CAPT'N BRINEY: Quite the place we sailed to ain't it? This is where the great ANCIENT DRAGON rests. The sea is vast, where to next matey?";
			}
			case MAP_LEAGUE_ENTRANCE:
				switch (npcId){
					case 0:
						if (player.getBadgeCount()==8){
							return  "GUARD: You may enter the league. The ELITE 4 awaits.";
						}
						return "GUARD: Stop right there. You must have 8 GYM BADGES to enter the league...";
					case 1:
						if (player.getBadgeCount()==8){
							return "GUARD: You may enter the league. The ELITE 4 awaits.";
						}
						return "GUARD: Stop right there. You must have 8 GYM BADGES to enter the league...";
					case 2:
						return "NURSE: Welcome to the POKEMON LEAGUE! Would you like to heal your POKEMON?";
					case 3:
						return "CLERK: Welcome to the POKEMON LEAGUE! What can I get for you today?";
				}
		  
        default:
            return "Hello.";
    }
}

static void triggerNPCAt(const NPCPos &npc, Player &player) {
    string dlg = getDialogueForNPC(currentMapID, npc.id, player);
    
    if (!dlg.empty()) {
        showDialogue(dlg);
    }
    
    //Mr Briney- Route 104 Boat trigger
    if (currentMapID == MAP_ROUTE104 && npc.id == 0) {
        if (player.getBadgeCount() < 1) {
            return;
        }
        
        vector<string> destinations = {"DEWFORD TOWN", "CANCEL"};
        int choice = boatTravelMenu(destinations);
        
        if (choice == 0) {
            clearScreen();
            playBoatAnimation("DEWFORD TOWN");
            enterMap(MAP_DEWFORD1, 'B', 0, 0);
            return;
        }
        else {
            showDialogue("MR. BRINEY: Alright, let me know when you're ready!");
            return;
        }
    }
        
    //Steven Granite Cave flag trigger
    if (currentMapID == MAP_GRANITECAVE && npc.id == 0 && !flagMetSteven) {
        flagMetSteven = true;
        return;
    }
    
    // Mr Briney - Dewford1 Boat trigger
    if (currentMapID == MAP_DEWFORD1 && npc.id == 1) {
        vector<string> destinations = {"ROUTE 104"};
        
        if (flagMetSteven && player.getBadgeCount() >= 2) {
            destinations.push_back("SLATEPORT CITY");
        }
        
        destinations.push_back("CANCEL");
        
        int choice = boatTravelMenu(destinations);
        
        if (choice == 0) {
            clearScreen();
            playBoatAnimation("ROUTE 104");
            enterMap(MAP_ROUTE104, 'B', 0, 0);
            return;
        }
        else if (choice == 1 && (int)destinations.size() > 2 && destinations[1] == "SLATEPORT CITY") {
            clearScreen();
            playBoatAnimation("SLATEPORT CITY");
            enterMap(MAP_SLATEPORT1, 'B', 0, 0);
            return;
        }
        else {
            showDialogue("MR. BRINEY: Alright, let me know when you're ready!");
            return;
        }
    }
    
    // Mr Briney - Harbor S.S. Tidal Trigger (Main Travel Menu for Endgame) (Slateport & Lilycove)
    if (currentMapID == MAP_HARBOR && npc.id == 0) {
        if (player.getBadgeCount() < 4) {
            return;
        }
        
        vector<string> destinations;
         
        destinations.push_back("ROUTE 104");
        destinations.push_back("DEWFORD TOWN");
    
        if(lastBuildingOriginMap != MAP_SLATEPORT2){
            destinations.push_back("SLATEPORT CITY");
        }    
        if (player.getBadgeCount() >= 6 && lastBuildingOriginMap != MAP_LILYCOVE1) {
            destinations.push_back("LILYCOVE CITY");
        }
        if(player.getBadgeCount() >= 6 && flagCheckAquaEntrance){
            destinations.push_back("MT PYRE");
        }
        if (flagMaxieDefeated && flagArchieDefeated){
            destinations.push_back("MOSSDEEP CITY");
        }
        if(player.getBadgeCount() >= 7 && flagRocketEvent){
            destinations.push_back("SOOTOPOLIS CITY");
        }
        if (flagMetWallace) {
            destinations.push_back("SKY PILLAR");
        }
        if(player.getBadgeCount() == 8){
            destinations.push_back("EVER GRANDE CITY CENTER");
        }
        
        destinations.push_back("CANCEL");
        
        int choice = boatTravelMenu(destinations);
        
        // Cancel selected
        if (choice == (int)destinations.size() - 1) {
            showDialogue("CAPT'N BRINEY: Alright matey, let me know when you're ready to sail!");
            return;
        }
        
        // Handle the selected destination
        string destination = destinations[choice];
        
        if (destination == "ROUTE 104") {
            clearScreen();
            playBoatAnimation("ROUTE 104");
            enterMap(MAP_ROUTE104, 'B', 0, 0);
            return;
        }
        else if (destination == "DEWFORD TOWN") {
            clearScreen();
            playBoatAnimation("DEWFORD TOWN");
            enterMap(MAP_DEWFORD1, 'B', 0, 0);
            return;
        }
        else if (destination == "SLATEPORT CITY") {
            clearScreen();
            playBoatAnimation("SLATEPORT CITY");
            enterMap(MAP_SLATEPORT1, 'B', 0, 0);
            return;
        }
        else if (destination == "LILYCOVE CITY") {
            clearScreen();
            playBoatAnimation("LILYCOVE CITY");
            enterMap(MAP_LILYCOVE1, 'B', 0, 0);
            return;
        }
        else if (destination == "MT PYRE") {
            clearScreen();
            playBoatAnimation("MT PYRE");
            enterMap(MAP_MTPYRE_ENTRANCE, 'B', 0, 0);
            return;
        }
        else if (destination == "MOSSDEEP CITY") {
            clearScreen();
            playBoatAnimation("MOSSDEEP CITY");
            enterMap(MAP_MOSSDEEP1, 'B', 0, 0);
            return;
        }
        else if (destination == "SOOTOPOLIS CITY") {
            clearScreen();
            playBoatAnimation("SOOTOPOLIS CITY");
            enterMap(MAP_SOOTOPOLIS1, 'B', 0, 0);
            return;
        }
        else if (destination == "SKY PILLAR") {
            clearScreen();
            playBoatAnimation("SKY PILLAR");
            enterMap(MAP_SKY_PILLAR_ENTRANCE, 'B', 0, 0);
            return;
        }
        else if (destination == "EVER GRANDE CITY CENTER") {
            clearScreen();
            playBoatAnimation("EVER GRANDE CITY CENTER");
            enterMap(MAP_EVER_GRANDE_CENTER, 'B', 0, 0);
            return;
        }
    }
    
    // Mr Briney - Mt Pyre Entrance
    if(currentMapID == MAP_MTPYRE_ENTRANCE && npc.id == 0){
        
        vector<string> destinations = {"LILYCOVE CITY", "CANCEL"};
        
        int choice = boatTravelMenu(destinations);
        
        if (choice == 0) {
            clearScreen();
            playBoatAnimation("LILYCOVE CITY");
            enterMap(MAP_LILYCOVE1, 'B',0, 0);
            return;
        }
        else {
            showDialogue("CAPT'N BRINEY: Alright matey, let me know when you're ready to sail!");
            return;
        }
    }
    
    //Mr Briney - Mossdeep City SS Tidal trigger
    if(currentMapID == MAP_MOSSDEEP1 && npc.id == 0){
        
        vector<string> destinations = {"LILYCOVE CITY"};
        
        if (flagRocketEvent && lastBuildingOriginMap != MAP_SOOTOPOLIS1) {
            destinations.push_back("SOOTOPOLIS CITY");
        }
        
        destinations.push_back("CANCEL");
        
        int choice = boatTravelMenu(destinations);
        
        // Cancel selected
        if (choice == (int)destinations.size() - 1) {
            showDialogue("CAPT'N BRINEY: Alright matey, let me know when you're ready to sail!");
            return;
        }
        
        // Handle the selected destination
        string destination = destinations[choice];
        
        if (destination == "LILYCOVE CITY") {
            clearScreen();
            playBoatAnimation("LILYCOVE CITY");
            enterMap(MAP_LILYCOVE1, 'B',0, 0);
            return;
        }
        else if (destination == "SOOTOPOLIS CITY") {
            clearScreen();
            playBoatAnimation("SOOTOPOLIS CITY");
            
            if(!flagAwakenRayquaza && !flagTrioEvent && !flagGroudonKyogreClash){
                playGroudonVsKyogreAnimation();
                flagGroudonKyogreClash=true;
            }
            
            enterMap(MAP_SOOTOPOLIS1, 'B',0, 0);
            return;
        }
    }
    
    // Mr Briney - Sootopolis City SS Tidal trigger
    if (currentMapID == MAP_SOOTOPOLIS1 && npc.id == 3){
        vector<string> destinations = {"LILYCOVE CITY", "MOSSDEEP CITY"};
    	
        if (flagMetWallace) {
            destinations.push_back("SKY PILLAR");
        }
        
        if (player.getBadgeCount() == 8){
         	destinations.push_back("EVER GRANDE CITY CENTER");
		 }
		 
    	destinations.push_back("CANCEL");
    	
        int choice = boatTravelMenu(destinations);
        
        // Cancel selected
        if (choice == (int)destinations.size() - 1) {
            showDialogue("CAPT'N BRINEY: Alright matey, let me know when you're ready to sail!");
            return;
        }
        
        // Handle the selected destination
        string destination = destinations[choice];
        
        if (destination == "LILYCOVE CITY") {
            clearScreen();
            playBoatAnimation("LILYCOVE CITY");
            enterMap(MAP_LILYCOVE1, 'B', 0, 0);
            return;
        }
        else if (destination == "MOSSDEEP CITY") {
            clearScreen();
            playBoatAnimation("MOSSDEEP CITY");
            enterMap(MAP_MOSSDEEP1, 'B', 0, 0);
            return;
        }
        else if (destination == "SKY PILLAR") {
            clearScreen();
            playBoatAnimation("SKY PILLAR");
            enterMap(MAP_SKY_PILLAR_ENTRANCE, 'B', 0, 0);
            return;
        }
        else if (destination == "EVER GRANDE CITY CENTER") {
            clearScreen();
            playBoatAnimation("EVER GRANDE CITY CENTER");
            enterMap(MAP_EVER_GRANDE_CENTER, 'B', 0, 0);
            return;
        }
        
    }
    
    //Mr Briney - Sky Pillar trigger
    if (currentMapID == MAP_SKY_PILLAR_ENTRANCE && npc.id == 0){
    	vector<string> destinations = {"SOOTOPOLIS CITY", "CANCEL"};
        
        int choice = boatTravelMenu(destinations);
        
        if (choice == 0) {
            clearScreen();
            playBoatAnimation("SOOTOPOLIS CITY");
            
            if(flagAwakenRayquaza && !flagTrioEvent){
            	playTrioEventAnimation();
            	flagTrioEvent=true;
			}
            enterMap(MAP_SOOTOPOLIS1, 'B',0, 0);
            return;
        }
        else {
            showDialogue("CAPT'N BRINEY: Alright matey, let me know when you're ready to sail!");
            return;
        }
        
	}
    
    // Route 110 block check - triggers flag when talking to any grunt
    if (currentMapID == MAP_ROUTE110_1 && !flagCheckBlockRoute110) {
        if (npc.id >= 0 && npc.id <= 3) {
            flagCheckBlockRoute110 = true;
            return;
        }
    }
        
    // Cable Car trigger
    if (currentMapID == MAP_CABLECAR && npc.id == 0) {
        if (player.getBadgeCount() < 3) {
            return;
        }
        
        bool goingUp = (lastBuildingOriginMap == MAP_ROUTE112_2);
        string destination = goingUp ? "MT. CHIMNEY" : "ROUTE 112";
        MapID destinationMap = goingUp ? MAP_MTCHIMNEY : MAP_ROUTE112_2;
        
        if (yesNoMenu("Would you like to ride the CABLE CAR to " + destination + "?", false)) {
            clearScreen();
            playCableCarAnimation(goingUp);
            enterMap(destinationMap, 'K', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
            return;
        }
        else {
            showDialogue("CABLE CAR LADY: Please come back when you're ready!");
            return;
        }
    }
        
    // Pokemart clerk trigger
    if (currentMapID == MAP_POKEMART && npc.id == 0) {
        pressAnyKey();
        clearScreen();
        openPokemart(player);
        return;
    }
    // Pokemart clerk (league) trigger
    if (currentMapID == MAP_LEAGUE_ENTRANCE && npc.id == 3){
    	pressAnyKey();
        clearScreen();
        openPokemart(player);
        return;
	}
    
    // Pokecenter nurse trigger
    if (currentMapID == MAP_POKECENTER && npc.id == 0) {
        pressAnyKey();
        clearScreen();
        
        if (yesNoMenu("Would you like to heal your POKEMON?", false)) {
            healAtPokecenter(player, db.moveList);
            lastPokecenterMap = lastBuildingOriginMap;
            hasVisitedPokecenter = true;
        } else {
            showDialogue("We hope to see you again!");
        }
    }
    
    // Pokecenter nurse trigger (league)
    if (currentMapID == MAP_LEAGUE_ENTRANCE && npc.id == 2) {
        pressAnyKey();
        clearScreen();
        
        if (yesNoMenu("Would you like to heal your POKEMON?", false)) {
            healAtPokecenter(player, db.moveList);
            lastPokecenterMap = lastBuildingOriginMap;
            hasVisitedPokecenter = true;
        } else {
            showDialogue("We hope to see you again!");
        }
    }
    
    //Sootopolis Steven Introduces Wallace trigger
    if (currentMapID== MAP_SOOTOPOLIS1 && npc.id==0 && !flagMetWallace){
    	stevenIntroduceWallace(player);
    	flagSootopolisTalkToSteven=true;
    	showDialogue("Entering CAVE OF ORIGIN...");
    	wallaceIntro(player);
    	flagMetWallace=true;
    	enterMap(MAP_CAVE_OF_ORIGIN, 'B',0, 0);
    	return;
	}
}

/*****************************************************************************
 * MOVEMENT & INPUT
 *****************************************************************************/
static char getInput() {
    return _getch();
}

static void attemptMovePlayer(vector<string>& layout, Position &pos, char input, Player &player) {
    int newRow = pos.row;
    int newCol = pos.col;

    switch (input) {
        case 'w': case 'W': newRow--; break;
        case 's': case 'S': newRow++; break;
        case 'a': case 'A': newCol--; break;
        case 'd': case 'D': newCol++; break;
        default: return;
    }

    if (!isInBounds(layout, newRow, newCol)) return;

    char target = layout[newRow][newCol];

    // Check for NPC/trainer tiles
    if (isNPCTile(target)) {
        if (tryTriggerTrainer(newRow, newCol, player)) return;
        
        for (const auto &npc : currentNPCs) {
            if (npc.row == newRow && npc.col == newCol) {
                triggerNPCAt(npc, player);
                return;
            }
        }
        
        showDialogue("Hello there!");
        return;
    }
    
    //Magma machine
    if (tryInteractWithMachine(newRow, newCol, player)) {
        return;
    }
    
    // Check for PC tile
    if (target == TileChars::PC_TILE) {
        openPC(player);
        return;
    }

    // Check if tile is passable
    if (!isPassable(target)) return;

    pos.row = newRow;
    pos.col = newCol;
}

/*****************************************************************************
 * MAP TRANSITIONS
 *****************************************************************************/
static map<char, MapID> doorTransitions = {
    {'1', MAP_MOMHOUSE},
    {'2', MAP_RIVALHOUSE},
    {'3', MAP_BIRCHLAB},
    {'4', MAP_POKECENTER},
    {'5', MAP_POKEMART},
    {'6', MAP_PETALBURGGYM},
    {'7', MAP_RUSTBOROGYM},
    {'8', MAP_DEWFORDGYM},
    {'9', MAP_OCEANIC_MUSEUM},
    {'v', MAP_MAUVILLEGYM},
    {'K', MAP_CABLECAR},
	{'E', MAP_LAVARIDGEGYM},
	{'Q', MAP_HARBOR},
	{'F', MAP_FIERYPATH},
	{'t', MAP_FORTREEGYM},
	{'a', MAP_AQUA_ENTRANCE},
	{'p', MAP_MTPYRE1},
	{'J', MAP_MAGMA_HIDEOUT_1},
	{'h', MAP_SEAFLOOR_CAVERN_1},
	{'l', MAP_MOSSDEEPGYM},
	{'r', MAP_SKY_PILLAR},
	{'s', MAP_SOOTOPOLISGYM},
	{'g', MAP_VICTORY_ROAD1},
	{'w', MAP_EVER_GRANDE_LEAGUE},
	{'L', MAP_VICTORY_ROAD2},
	{'W', MAP_LEAGUE_ENTRANCE}
};


static void enterMap(MapID id, char entryMarker, int rowOffset, int colOffset) {
    currentMapID = id;
    currentMap = &worldMaps[id];

    Position spawnPos = {0, 0};
    bool found = false;
    
    if (rowOffset != 0 || colOffset != 0) {
        found = findSpawnWithOffset(currentMap->layout, spawnPos, entryMarker, rowOffset, colOffset);
    }
    
    if (!found) {
        found = findSpawn(currentMap->layout, spawnPos, entryMarker);
    }
    
    if (!found) {
        findSpawn(currentMap->layout, spawnPos);
    }
    
    playerPos = spawnPos;
    scanNPCsInMap(currentMap->layout);
}

static void enterMap(MapID id, const string& fromEdge) {
    currentMapID = id;
    currentMap = &worldMaps[id];

    Position spawnPos = {0, 0};
    if (!findEdgeSpawn(currentMap->layout, spawnPos, fromEdge)) {
        findSpawn(currentMap->layout, spawnPos);
    }
    
    playerPos = spawnPos;
    scanNPCsInMap(currentMap->layout);
}

static void enterMap(MapID id) {
    enterMap(id, TileChars::SPAWN_MARKER, 0, 0);
}

/*****************************************************************************
 * WHITEOUT RESPAWN
 *****************************************************************************/
static void respawnAfterWhiteout(Player &player) {
    // Heal all Pokemon
    for (auto& pokemon : player.currentPokemon) {
        pokemon.currentHP = pokemon.baseHP;
        pokemon.initializePP(db.moveList);
    }
    
    // Lose half money
    player.money /= MapConfig::WHITEOUT_MONEY_DIVISOR;
    
    // Spawn outside Pokemon Center
    if (hasVisitedPokecenter) {
        enterMap(lastPokecenterMap, '4', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    } else {
        // First time whiteout - spawn at Mom's house default location
        enterMap(MAP_MOMHOUSE);
    }
    
    showDialogue("You rush to the nearest Pokemon Center...");
    pressAnyKey();
    showDialogue("Your POKEMON were restored to full health!");
    pressAnyKey();
}

/*****************************************************************************
 * TRANSITION HANDLERS
 *****************************************************************************/
static void handleTruckTransition(Player &player) {
    if (storyState == INTRO_TRUCK) {
        showDialogue("You step out of the truck...");
        storyState = INTRO_MOM_DIALOGUE;
        enterMap(MAP_MOMHOUSE);
        momStarter(player);
        flagVisitedMom = true;
        storyState = INTRO_GO_TO_LAB;
    } else {
        enterMap(MAP_LITTLEROOT);
    }
}

static void handleMomHouseTransition(Player &player) {
    if (storyState == INTRO_MOM_DIALOGUE) {
        storyState = INTRO_GO_TO_LAB;
    }
    showDialogue("Leaving Mom's House...");
    enterMap(MAP_LITTLEROOT, '1', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
}

static void handleRivalHouseTransition(Player &player) {
    if (!currentMap) return;

    if (currentMap->layout[playerPos.row][playerPos.col] == TileChars::ROUTE_TRANSITION) {
        showDialogue("Leaving Neighbor's house...");
        enterMap(MAP_LITTLEROOT, '2', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
        return;
    }

    if (storyState < STORY_FREE_ROAM && currentMapID != MAP_RIVALHOUSE) {
        showDialogue("Entering Neighbor's house...");
        enterMap(MAP_RIVALHOUSE);
    }
}

static void handleBirchLabTransition(Player &player) {
    if (!currentMap) return;

    // First-time starter sequence
    if (!flagHasStarter) {
        drBirchStarter(player);
        chooseStarter(player, db);
        firstBattle(player, db, "ZIGZAGOON", MapConfig::WILD_LEVEL_MIN, MapConfig::WILD_LEVEL_MAX);
        drBirchStarter2(player);
        flagHasStarter = true;
        storyState = INTRO_PICKED_STARTER;
    }

    // Exit back to town
    int mapHeight = (int)currentMap->layout.size();
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD_WIDE) {
        showDialogue("Leaving Birch's Lab...");
        enterMap(MAP_LITTLEROOT, '3', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    }
}

static void handleLittlerootTransition(Player &player) {
    if (!flagHasStarter) {
        showDialogue("You should probably visit Professor Birch first.");
        return;
    }

    static bool flagUnlockedRoute101 = false;

    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        if (!flagUnlockedRoute101) {
            showDialogue("Entering ROUTE 101...");
            flagUnlockedRoute101 = true;
        }
        storyState = STORY_FREE_ROAM;
        enterMap(MAP_ROUTE101, "top");
    }
}

static void handleRoute101Transition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD_WIDE) {
        showDialogue("Returning to LITTLEROOT...");
        enterMap(MAP_LITTLEROOT, "top");
        return;
    }
    
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        showDialogue("Entering OLDALE TOWN...");
        enterMap(MAP_OLDALE, "top");
    }
}

static void handleOldaleTransition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD_WIDE) {
        showDialogue("Returning to ROUTE 101...");
        enterMap(MAP_ROUTE101, "bottom");
        return;
    }
    
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        showDialogue("Entering ROUTE 103...");
        // enterMap(MAP_ROUTE103, "bottom");
        return;
    }
    
    if (playerPos.col <= MapConfig::EDGE_THRESHOLD_WIDE) {
        showDialogue("Entering ROUTE 102...");
        enterMap(MAP_ROUTE102, "left");
    }
}

static void handlePokeCenterTransition(Player &player) {
    if (!currentMap) return;
    showDialogue("Leaving POKEMON CENTER...");
    
    if (lastBuildingOriginMap == MAP_LEAGUE_ENTRANCE) {
        enterMap(lastBuildingOriginMap, 'W', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    } else {
        enterMap(lastBuildingOriginMap, '4', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    }
}

static void handlePokeMartTransition(Player &player) {
    if (!currentMap) return;
    showDialogue("Leaving POKEMART...");
    
    if (lastBuildingOriginMap == MAP_LEAGUE_ENTRANCE) {
        enterMap(lastBuildingOriginMap, 'W', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    } else {
        enterMap(lastBuildingOriginMap, '5', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    }
}


static void handleRoute102Transition(Player &player) {
    if (!currentMap) return;
    
    int mapWidth = (int)currentMap->layout[0].size();
    
    if (playerPos.col >= mapWidth - MapConfig::EDGE_THRESHOLD_WIDE) {
        showDialogue("Returning to OLDALE TOWN...");
        enterMap(MAP_OLDALE, "right");
        return;
    }
    
    if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
        showDialogue("Entering PETALBURG CITY...");
        enterMap(MAP_PETALBURG1, "left");
    }
}

static void handlePetalburg1Transition(Player &player) {
    if (!currentMap) return;
    
    int mapWidth = (int)currentMap->layout[0].size();
    
    if (playerPos.col >= mapWidth - MapConfig::EDGE_THRESHOLD_WIDE) {
        showDialogue("Returning to ROUTE102...");
        enterMap(MAP_ROUTE102, "right");
        return;
    }
    
    if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
        enterMap(MAP_PETALBURG2, "top");
    }
}

static void handlePetalburg2Transition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    int mapWidth = (int)currentMap->layout[0].size();
    
    // Left edge - placeholder for Route 104 (rows 12-13 have '=' on left)
    if (playerPos.col <= MapConfig::EDGE_THRESHOLD && playerPos.row >= 12) {
        showDialogue("Entering ROUTE 104...");
        enterMap(MAP_ROUTE104, "left");
        return; 
    }
    
    // Bottom edge - return to Petalburg1 (spawn at left side)
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD_WIDE) {
        enterMap(MAP_PETALBURG1, "right");
        return;
    }
}

static void handlePetalburgGymTransition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    // Exit gym - spawn below door '6' in Petalburg2
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD) {
        showDialogue("Leaving PETALBURG GYM...");
        enterMap(MAP_PETALBURG2, '6', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    }
}

static void handleRoute104Transition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    int mapWidth = (int)currentMap->layout[0].size();
    
    // Right edge - return to Petalburg2
    if (playerPos.col >= mapWidth - MapConfig::EDGE_THRESHOLD_WIDE) {
        showDialogue("Returning to PETALBURG CITY...");
        enterMap(MAP_PETALBURG2, "right");
        return;
    }
    
    // Top edge - Petalburg Woods
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
    	showDialogue("Entering PETALBURG WOODS...");
        enterMap(MAP_PETALBURGWOODS1, "top");
        return;
    }
}
static void handlePetalburgWoods1Transition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    // Bottom edge - return to Route 104
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD_WIDE) {
        showDialogue("Returning to ROUTE 104...");
        enterMap(MAP_ROUTE104, "bottom");
        return;
    }
    
    // Top edge - enter Petalburg Woods 2
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        showDialogue("Deeper into PETALBURG WOODS...");
        enterMap(MAP_PETALBURGWOODS2, "top");
        return;
    }
}

static void handlePetalburgWoods2Transition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    // Bottom edge - return to Petalburg Woods 1
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD_WIDE) {
        enterMap(MAP_PETALBURGWOODS1, "bottom");
        return;
    }
    
    // Top edge - Rustboro City
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
    	showDialogue("Entering RUSTBORO CITY...");
        enterMap(MAP_RUSTBORO1, "top");
        return;
    }
}

static void handleRustboro1Transition(Player &player){
	if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    // Bottom edge - return to Petalburg Woods 2
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD_WIDE) {
    	showDialogue("Returning to PETALBURG WOODS...");
        enterMap(MAP_PETALBURGWOODS2, "bottom");
        return;
    }
    
    // Top edge - Rustboro City 2
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        enterMap(MAP_RUSTBORO2, "top");
        return;
    }
}

static void handleRustboro2Transition(Player &player){
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    int mapWidth = (int)currentMap->layout[0].size();
    
    // Bottom edge - return to Rustboro 1
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD_WIDE) {
        enterMap(MAP_RUSTBORO1, "bottom");
        return;
    }
    
    // Top edge - Route 115
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        showDialogue("ROUTE 115 UNDER CONSTRUCTION");
        return;
    }
    
    // Right edge - Route 116
    if (playerPos.col >= mapWidth - MapConfig::EDGE_THRESHOLD_WIDE) {
        showDialogue("ROUTE 116 UNDER CONSTRUCTION");
        return;
    }
}

static void handleRustboroGymTransition(Player &player){
	if (!currentMap) return;
	    
	    int mapHeight = (int)currentMap->layout.size();
	    
	    // Exit gym - spawn below door '7' in Rustboro2
	    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD) {
	        showDialogue("Leaving RUSTBORO GYM...");
	        enterMap(MAP_RUSTBORO2, '7', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
	    }
}

static void handleDewford1Transition(Player &player) {
    if (!currentMap) return;
    
    // Top edge - go to Dewford2
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        enterMap(MAP_DEWFORD2, "top");
        return;
    }
}

static void handleDewford2Transition(Player &player) {
    if (!currentMap) return;
    
    // Bottom edge - return to Dewford1
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
        enterMap(MAP_DEWFORD1, "bottom");
        return;
    }
    
    // Left edge - Granite Cave
    if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
    	showDialogue("Entering GRANITE CAVE...");
        enterMap(MAP_GRANITECAVE, "left");
        return;
    }
}

static void handleGraniteCaveTransition(Player &player) {
    if (!currentMap) return;
    
    // Bottom edge - return to Dewford2
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
        enterMap(MAP_DEWFORD2, "bottom");
        return;
    }
}

static void handleDewfordGymTransition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    // Exit gym - spawn below door '8' in Dewford1
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD) {
        showDialogue("Leaving DEWFORD GYM...");
        enterMap(MAP_DEWFORD1, '8', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    }
}

static void handleSlateport1Transition(Player &player){
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    int mapWidth = (int)currentMap->layout[0].size();
    
    
    // Top edge - Top part of Slateport
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
       	enterMap(MAP_SLATEPORT2,"top");
        return;
    }

}

static void handleSlateport2Transition(Player &player){
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    int mapWidth = (int)currentMap->layout[0].size();
    
    //Top Edge- Route110
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
    	showDialogue("Entering ROUTE 110...");
        enterMap(MAP_ROUTE110_1, "top");
        return;
    }
	
	// Bottom edge - Slateport1
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
        enterMap(MAP_SLATEPORT1, "bottom");
        return;
    }
}

static void handleRoute110ATransition(Player &player) {
    if (!currentMap) return;
    
    // Bottom edge - return to Slateport
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
        showDialogue("Returning to SLATEPORT CITY...");
        enterMap(MAP_SLATEPORT2, "bottom");
        return;
    }
    
    //Top edge to Route 110 B + Aqua flag check
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        if (!flagUnblockRoute110) {
            showDialogue("TEAM AQUA GRUNT: I have to stop you right there. Go away!");
            return;
        }
        enterMap(MAP_ROUTE110_2, "top");
        return;
    }
	 
    
    // Left edge - Route 103 (after unblocking)
    if (playerPos.col <= MapConfig::EDGE_THRESHOLD && flagUnblockRoute110) {
        showDialogue("ROUTE 103 UNDER CONSTRUCTION");
        return;
    }
    
}

static void handleOceanicMuseumTransition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    // Exit Museum- Spawn at Slateport 9 door
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD) {
        showDialogue("Leaving OCEANIC MUSEUM...");
        enterMap(MAP_SLATEPORT2, '9', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    }
}

static void handleRoute110BTransition(Player &player) {
    if (!currentMap) return;
    
     //Top Edge- Route110
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
    	showDialogue("Entering MAUVILLE CITY...");
    	enterMap(MAP_MAUVILLE, "top");
        return;
    }
    
    // Bottom edge - return to Route 110 A
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
        enterMap(MAP_ROUTE110_1, "bottom");
        return;
    }   
}

static void handleMauvilleTransition(Player &player){
	if (!currentMap) return;
	
	int mapHeight = (int)currentMap->layout.size();
	
	// Top edge - Route 111
	if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
		showDialogue("Entering Route 111...");
		enterMap(MAP_ROUTE111_1, "top");
		return;
	}
	
	// Bottom edge - Route 110
	if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD_WIDE) {
		showDialogue("Returning to ROUTE 110...");
		enterMap(MAP_ROUTE110_2, "bottom");
		return;
	}
	
	// Right edge - Route 118
	if (playerPos.col >= (int)currentMap->layout[0].size() - MapConfig::EDGE_THRESHOLD_WIDE) {
		
		 //Badge check - block route till Balance Badge
	    if(player.getBadgeCount() < 5){
	    	showDialogue("The path seems to be blocked by fast currents. You should probably ask your Dad for BALANCE advice...");
	    	return;
		}
		showDialogue("Entering ROUTE 118...");
		enterMap(MAP_ROUTE118, "right");
		return;
	}
	
	// Left edge - Route 117
	if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
		showDialogue("ROUTE 117 UNDER CONSTRUCTION");
		return;
	}
}

static void handleMauvilleGymTransition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD) {
        showDialogue("Leaving MAUVILLE GYM...");
        enterMap(MAP_MAUVILLE, 'v', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    }
}

static void handleRoute111ATransition(Player &player) {
    if (!currentMap) return;
    
     //Top Edge- Route111 B
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
    	enterMap(MAP_ROUTE111_2, "top");
		return;
    }
    
    // Bottom edge - return to Mauville
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	showDialogue("Returning to MAUVILLE CITY...");
        enterMap(MAP_MAUVILLE, "bottom");
        return;
    }   
}

static void handleRoute111BTransition(Player &player) {
    if (!currentMap) return;
    
     //Top Edge- Route111 C
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
    	enterMap(MAP_ROUTE111_3, "top");
		return;
    }
    
    // Bottom edge - return to Route 111 A
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
        enterMap(MAP_ROUTE111_1, "bottom");
        return;
    }   
}

static void handleRoute111CTransition(Player &player) {
    if (!currentMap) return;
    
     //Top Edge- Route111 Desert A
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
    	showDialogue("Entering DESERT...");
    	enterMap(MAP_ROUTE111_DESERT, "top");
		return;
    }
    
    //Left Edge - Route 112 A
	if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
		showDialogue("Entering ROUTE 112...");
		 enterMap(MAP_ROUTE112_1, "left");
		return;
	}
	
    // Bottom edge - return to Route 111 B
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
        enterMap(MAP_ROUTE111_2, "bottom");
        return;
    }   
}

static void handleRoute111DesertTransition(Player &player) {
    if (!currentMap) return;
   
    // Bottom edge - return to Route 111 C
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	showDialogue("Returning to ROUTE 111...");
        enterMap(MAP_ROUTE111_3, "bottom");
        return;
    }   
}

static void handleRoute112ATransition(Player &player) {
    if (!currentMap) return;
    
     //Top Edge- Route112 B
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
    	enterMap(MAP_ROUTE112_2, "top");
    	return;
    }
    
    //Right Edge- Return to Route 111 C
	if (playerPos.col >= (int)currentMap->layout[0].size() - MapConfig::EDGE_THRESHOLD_WIDE) {
		showDialogue("Returning to ROUTE 111...");
		enterMap(MAP_ROUTE111_3, "right");
		return;
	}
}

static void handleRoute112BTransition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
        
     // Bottom edge - return to Route 112 A
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
        enterMap(MAP_ROUTE112_1, "bottom");
        return;
    }   
}

static void handleFieryPathTransition(Player &player){
	 if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD) {
        showDialogue("Leaving FIERY PATH...");
        enterMap(MAP_ROUTE112_1, 'F', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    }
}

static void handleCableCarTransition(Player &player) {
    if (!currentMap) return;
    
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD) {
        showDialogue("Leaving CABLE CAR STATION...");
        enterMap(lastBuildingOriginMap, 'K', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    }
}

static void handleMtChimneyTransition(Player &player) {
    if (!currentMap) return;
    
    // Top - Mt Chimney Summit
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        showDialogue("Entering MT. CHIMNEY SUMMIT...");
        enterMap(MAP_MTCHIMNEY_SUMMIT, "top");
        return;
    }
    
    // Bottom - Jagged Pass (only accessible after machine encounter)
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
        if (!flagEncounterMachine) {
            showDialogue("The path seems blocked by intense heat. You should check out the summit first...");
            return;
        }
        showDialogue("Entering JAGGED PASS...");
        
	    //Magma hideout opened hint
		if (flagOrbStolen && !flagShowMagmaEntranceOnce){
			flagUnlockMagmaHideout=true;
			flagShowMagmaEntranceOnce=true;
			
			showDialogue("The ground rumbles ominously...");
			showDialogue("A previously blocked cave entrance has opened up!");
		} 	
        enterMap(MAP_JAGGEDPASS, "bottom");
        return;
    }
}

static void handleMtChimneySummitTransition(Player &player) {
    if (!currentMap) return;

     // Bottom edge - return to Mt Chimney 1
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
        enterMap(MAP_MTCHIMNEY, "bottom");
        return;
    }   
}

static void handleJaggedPassTransition(Player &player){
	 if (!currentMap) return;
	 	
	 	
    // Top - Mt Chimney
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        showDialogue("Returning to MT. CHIMNEY...");
        enterMap(MAP_MTCHIMNEY, "top");
        return;
    }
	 
		 
	// Bottom edge - Lavaridge
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
       	showDialogue("Entering LAVARIDGE TOWN...");
       	enterMap(MAP_LAVARIDGE, "bottom");
        return;
    }   
}

static void handleLavaridgeTransition(Player &player){
	 if (!currentMap) return;
	   
    // Top - Jaggedpass
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        showDialogue("Returning to JAGGED PASS...");
        enterMap(MAP_JAGGEDPASS, "top");
        return;
    }
}

static void handleLavaridgeGymTransition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD) {
        showDialogue("Leaving LAVARIDGE GYM...");
        enterMap(MAP_LAVARIDGE, 'E', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    }
}

static void handleHarborTransition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD) {
        showDialogue("Leaving HARBOR...");
        enterMap(lastBuildingOriginMap, 'Q', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    }
}

static void handleRoute118Transition(Player &player) {
    if (!currentMap) return;
    
    // Top - Route 119
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        showDialogue("Entering ROUTE 119...");
        enterMap(MAP_ROUTE119, "top");
        return;
    }
    
    //Left - Return to Mauville
	if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
		showDialogue("Returning to MAUVILLE CITY...");
		 enterMap(MAP_MAUVILLE, "left");
		return;
	}
}

static void handleRoute119Transition(Player &player) {
    if (!currentMap) return;
    
    // Top - Fortree City
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        showDialogue("Entering FORTREE CITY...");
        enterMap(MAP_FORTREE, "top");
        return;
    }
    
  	// Bottom edge - Route 118
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
       	showDialogue("Returning to ROUTE 118...");
       	enterMap(MAP_ROUTE118, "bottom");
        return;
    }   
}

static void handleFortreeTransition(Player &player) {
    if (!currentMap) return;
    
    //Left - Return to Route 119
	if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
		showDialogue("Returning to ROUTE 119...");
		 enterMap(MAP_ROUTE119, "left");
		return;
	}
}

static void handleFortreeGymTransition(Player &player) {
    if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD) {
        showDialogue("Leaving FORTREE GYM...");
        enterMap(MAP_FORTREE, 't', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    }
}

static void handleLilycove1Transition(Player &player){
	if (!currentMap) return;
	
	 // Top - Lilycove NW (Mart, Center, Supermarket)
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        enterMap(MAP_LILYCOVE2,"top");
        return;
    }
    
    //Right - Lilycove SE (Empty Bay)
	if (playerPos.col >= (int)currentMap->layout[0].size() - MapConfig::EDGE_THRESHOLD_WIDE) {
		enterMap(MAP_LILYCOVE4, "right");
		return;
	}
}

static void handleLilycove2Transition(Player &player){
	if (!currentMap) return;
    
    //Right - Lilycove NE (Aqua Hideout)
	if (playerPos.col >= (int)currentMap->layout[0].size() - MapConfig::EDGE_THRESHOLD_WIDE) {
		enterMap(MAP_LILYCOVE3, "right");
		return;
	}
	
	 //Left - Return to Route 119
	if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
		showDialogue("ROUTE 121 UNDER CONSTRUCTION...");
		return;
	}
	
	// Bottom edge -Lilycove SW (Harbor)
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
       	enterMap(MAP_LILYCOVE1, "bottom");
        return;
    }  
	
}

static void handleLilycove3Transition(Player &player){
	if (!currentMap) return;
    
	 //Left - Return to Lilycove NW (Mart)
	if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
		enterMap(MAP_LILYCOVE2,"left");
		return;
	}
	
	// Bottom edge - Lilycove SE (Empty Bay)
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	enterMap(MAP_LILYCOVE4, "bottom");
        return;
    }  
}

static void handleLilycove4Transition(Player &player){
	if (!currentMap) return;
    
	 //Left - Return to Lilycove SW (Harbor)
	if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
		enterMap(MAP_LILYCOVE1,"left");
		return;
	}
	
	// Top - Return to Lilycove NE (Aqua Hideout)
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        enterMap(MAP_LILYCOVE3,"top");
        return;
    }
	
}

static void handleAquaEntranceTransition(Player &player){
	if (!currentMap) return;
    
    int mapHeight = (int)currentMap->layout.size();
    
    // Bottom edge - Return to Lilycove3
    if (playerPos.row >= mapHeight - MapConfig::EDGE_THRESHOLD) {
        enterMap(MAP_LILYCOVE3, 'a', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
        return;
    }
    
    // Top edge - Aqua Hideout
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        // Pre-orb theft - block entrance
        if (!flagOrbStolen) {
            showDialogue("TEAM AQUA GRUNT: Ah ah ah no can do, beat it kid!");
            flagCheckAquaEntrance = true;
            return;
        }
        
        // Post-orb theft, pre-Groudon awakening - still blocked
        if (flagOrbStolen && !flagAwakenGroudon) {
            showDialogue("TEAM AQUA GRUNT: Ah ah ah no can do, don't make me tell you to scram twice!");
            return;
        }
        
        //Post-Groudon awakening, pre maxie defeat
        if(flagOrbStolen && flagAwakenGroudon && !flagMaxieDefeated){
        	showDialogue("TEAM AQUA GRUNT: Ah ah ah no can do, I've told you three times now!");
        	return;
		}
        
        // Post Groudon awakening - enter hideout
        showDialogue("Entering AQUA HIDEOUT...");
        enterMap(MAP_AQUA_HIDEOUT_1, "top");
        return;
    }
}

static void handleMtPyre1Transition(Player &player){
	if (!currentMap) return;
    
	 //Left - Mt Pyre Summit 1
	if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
		enterMap(MAP_MTPYRE2, "left");
		return;
	}
	
	//Bottom- Return to Mt Pyre Entrance
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	enterMap(MAP_MTPYRE_ENTRANCE, 'p', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
        return;
    }  
}

static void handleMtPyre2Transition(Player &player){
	if (!currentMap) return;
    
	//Top- Mt Pyre Summit
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
    	//orb event
    	showDialogue("Entering MT PYRE SUMMIT...");
    	if(!flagOrbStolen){
    		aquaArchieOrb();
    		oldGrandmaOrb();
    		flagOrbStolen=true;
		}
        enterMap(MAP_MTPYRE_SUMMIT, "top");
        return;
    }
	
	//Right - Mt Pyre1
	if (playerPos.col >= (int)currentMap->layout[0].size() - MapConfig::EDGE_THRESHOLD_WIDE) {
		enterMap(MAP_MTPYRE1, "right");
		return;
	}
	
}

static void handleMtPyreSummitTransition(Player &player){
	if (!currentMap) return;
	
	//Bottom- Return to Mt Pyre Entrance
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	enterMap(MAP_MTPYRE2, "bottom");
        return;
    }  
}

static void handleMagmaHideout1Transition(Player &player){
	if (!currentMap) return;
    
	 //Left - Magma Hideout 2
	if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
		showDialogue("Deeper into MAGMA HIDEOUT...");
		enterMap(MAP_MAGMA_HIDEOUT_2, "left");
		return;
	}
	
	// Top - Return to Jagged Pass
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
    	showDialogue("Leaving MAGMA HIDEOUT...");
       	enterMap(MAP_JAGGEDPASS, 'J', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
       	return;
    }
}

static void handleMagmaHideout2Transition(Player &player){
	if (!currentMap) return;
    
	 //Left - Magma Hideout 3
	if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
		enterMap(MAP_MAGMA_HIDEOUT_3, "left");
		return;
	}
	
	//Right - Return to Magma Hideout 1
	if (playerPos.col >= (int)currentMap->layout[0].size() - MapConfig::EDGE_THRESHOLD_WIDE) {
		enterMap(MAP_MAGMA_HIDEOUT_1, "right");
		return;
	}
}

static void handleMagmaHideout3Transition(Player &player){
    if (!currentMap) return;
    
    //Bottom- Magma Hideout 4
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	
    	if (!flagAwakenGroudon) {
            maxiePreGroudon();
            playAwakenGroudon();
            flagAwakenGroudon = true;
            maxiePostGroudon();
        }
    	enterMap(MAP_MAGMA_HIDEOUT_4, "bottom");
        return;
    }  
    
   //Right - Return to Magma Hideout 2
	if (playerPos.col >= (int)currentMap->layout[0].size() - MapConfig::EDGE_THRESHOLD_WIDE) {
		enterMap(MAP_MAGMA_HIDEOUT_2, "right");
		return;
	}
	
}

static void handleMagmaHideout4Transition(Player &player){
	if (!currentMap) return;
        
	// Top - Return to Magma Hideout 3
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
       	enterMap(MAP_MAGMA_HIDEOUT_3, "top");
       	return;
    }
}

static void handleAquaHideout1Transition(Player &player){
	if (!currentMap) return;
	
	// Top - Aqua Hideout 2
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
       	enterMap(MAP_AQUA_HIDEOUT_2, "top");
       	return;
    }
    
    //Bottom - Return to Aqua Hideout Entrnce
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	showDialogue("Returning to LILYCOVE...");
    	enterMap(MAP_AQUA_ENTRANCE, "bottom");
        return;
    }  
}

static void handleAquaHideout2Transition(Player &player){
	if (!currentMap) return;
	
	// Top - Aqua Hideout 3
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
       	enterMap(MAP_AQUA_HIDEOUT_3, "top");
       	return;
    }
    
    //Bottom - Return to Aqua Hideout 1
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	enterMap(MAP_AQUA_HIDEOUT_1, "bottom");
        return;
    }  
}

static void handleAquaHideout3Transition(Player &player){
	if (!currentMap) return;
	
	//Right - Aqua Hideout 4
	if (playerPos.col >= (int)currentMap->layout[0].size() - MapConfig::EDGE_THRESHOLD_WIDE) {
		enterMap(MAP_AQUA_HIDEOUT_4, "right");
		return;
	}
    
    //Bottom - Return to Aqua Hideout 2
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	enterMap(MAP_AQUA_HIDEOUT_2, "bottom");
        return;
    }  
}

static void handleAquaHideout4Transition(Player &player){
	if (!currentMap) return;
	
	//Bottom - Aqua Hideout 5
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	enterMap(MAP_AQUA_HIDEOUT_5, "bottom");
        return;
    }
	
	//Left - Return to Aqua Hideout 3
	if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
		enterMap(MAP_AQUA_HIDEOUT_3, "left");
		return;
	}
}

static void handleAquaHideout5Transition(Player &player){
	if (!currentMap) return;
	
	//Top - Return to Aqua Hideout 4
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
       	enterMap(MAP_AQUA_HIDEOUT_4, "top");
       	return;
    }
}
	
static void handleSeafloorCavern1Transition(Player &player){
	if (!currentMap) return;
	
	//Left- Return to Aqua Hideout 5
	if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
		showDialogue("Leaving SEAFLOOR CAVERN...");
		enterMap(MAP_AQUA_HIDEOUT_5, 'h', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
		return;
	}
	
	//Bottom - Seafloor Cavern 2
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	if(!flagAwakenKyogre){
    		archiePreKyogre();
		}
    	enterMap(MAP_SEAFLOOR_CAVERN_2, "bottom");
        return;
    }
	
}

static void handleSeafloorCavern2Transition(Player &player){
	if (!currentMap) return;
	
	//Top - Return to Seafloor Cavern 1
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
       	enterMap(MAP_SEAFLOOR_CAVERN_1, "top");
       	return;
    }
	
}

static void handleMossdeep1Transition(Player &player){
	if (!currentMap) return;
	
	//Top - Mossdeep 2
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
       	enterMap(MAP_MOSSDEEP2, "top");
       	return;
    }
    
    //Right - Mossdeep 3
    if (playerPos.col >= (int)currentMap->layout[0].size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	if(player.getBadgeCount() < 7){
    		showDialogue("The path seems to be blocked by an invisible psychic barrier, you should probably talk to GYM LEADERS TATE and LIZA first...");
    		return;
			}
		if (!flagRocketEvent){
			stevenToMaxie(player);
		}
		enterMap(MAP_MOSSDEEP3, "right");
		return;
	}
    
}

static void handleMossdeep2Transition(Player &player){
	if (!currentMap) return;
	
	//Bottom - Mossdeep 1
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	enterMap(MAP_MOSSDEEP1, "bottom");
        return;
    }
}

static void handleMossdeepGymTransition(Player &player){
	if (!currentMap) return;
	
	//Bottom - return to Mossdeep 2
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	showDialogue("Leaving MOSSDEEP GYM...");
    	enterMap(MAP_MOSSDEEP2, 'l', MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
        return;
    }
	
}

static void handleMossdeep3Transition(Player &player){
	if (!currentMap) return;
	
	//Left - return to Mossdeep 1
	if (playerPos.col <= MapConfig::EDGE_THRESHOLD) {
		enterMap(MAP_MOSSDEEP1, "left");
		return;
	}
}

static void handleSootopolis1Transition(Player &player){
	if (!currentMap) return;
	
	//Top - Sootopolis 2
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
    	if(!flagSootopolisTalkToSteven){
    		showDialogue("You should probably talk to STEVEN first...");
    		return;
		}
       	enterMap(MAP_SOOTOPOLIS2, "top");
       	return;
    }
}

static void handleSootopolis2Transition(Player &player){
	if (!currentMap) return;
	
	//Top - Cave of Origins
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
       	showDialogue("Entering CAVE OF ORIGIN...");
       	enterMap(MAP_CAVE_OF_ORIGIN, "top");
       	return;
    }
    
    //Bottom - return to Sootopolis1
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	enterMap(MAP_SOOTOPOLIS1,"bottom");
        return;
    }
}

static void handleCaveOfOriginTransition(Player &player){
	if (!currentMap) return;

	 //Bottom - return to Sootopolis 2
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	enterMap(MAP_SOOTOPOLIS2,"bottom");
        return;
    }
}

static void handleSkyPillarTransition(Player &player){
	if (!currentMap) return;

	 //Bottom - return to Sky Pillar Entrance
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	enterMap(MAP_SKY_PILLAR_ENTRANCE,'r',MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
        return;
    }
}

static void handleSootopolisGymTransition(Player &player){
	if (!currentMap) return;

	 //Bottom - return to Sootopolis 1
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	enterMap(MAP_SOOTOPOLIS1,'s',MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
        return;
    }
}

static void handleVictoryRoad1Transition (Player &player){
	if (!currentMap) return;
	
	 //Bottom - return to Ever Grande Center
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	showDialogue("Returning to EVER GRANDE CENTER...");
    	enterMap(MAP_EVER_GRANDE_CENTER,'g',MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
        return;
    }
    
    //Top - Victory Road 2
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
       	enterMap(MAP_VICTORY_ROAD2, "top");
       	return;
    }
}

static void handleVictoryRoad2Transition (Player &player){
	if (!currentMap) return;
	
	 //Bottom - return to Victory Road 1
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	enterMap(MAP_VICTORY_ROAD1, "bottom");
        return;
    }
}

static void handleLeagueEntranceTransition(Player &player){
	if (!currentMap) return;
	
	//Bottom - return to Victory Road 1
    if (playerPos.row >= (int)currentMap->layout.size() - MapConfig::EDGE_THRESHOLD_WIDE) {
    	enterMap(MAP_EVER_GRANDE_LEAGUE,'W',MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
        return;
    }
    
    //Top - League Room 1
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
       	if(player.getBadgeCount()==8){
       		showDialogue("Entering THE ELITE 4 POKEMON LEAGUE...");
       		 enterMap(MAP_LEAGUE_RM1, 'X', 0, 0);
       		return;
		   }
       	showDialogue("You must have all the GYM BADGES to enter the LEAGUE...");
       	return;
    }
}

static void handleLeagueRm1Transition(Player &player){
    if (!currentMap) return;
    
    //Top - League Room 2
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        if(!isTrainerDefeated(MAP_LEAGUE_RM1, 0)){
            showDialogue("You must beat ELITE 4 SIDNEY before proceeding...");
            return;
        }
        enterMap(MAP_LEAGUE_RM2, 'X', 0, 0);
        return;
    }
}

static void handleLeagueRm2Transition(Player &player){
    if (!currentMap) return;
    
    //Top - League Room 3    
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        if(!isTrainerDefeated(MAP_LEAGUE_RM2, 0)){
            showDialogue("You must beat ELITE 4 PHOEBE before proceeding...");
            return;
        }
        enterMap(MAP_LEAGUE_RM3, 'X', 0, 0);
        return;
    }
}

static void handleLeagueRm3Transition(Player &player){
    if (!currentMap) return;
    
    //Top - League Room 4    
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        if(!isTrainerDefeated(MAP_LEAGUE_RM3, 0)){
            showDialogue("You must beat ELITE 4 GLACIA before proceeding...");
            return;
        }
        enterMap(MAP_LEAGUE_RM4, 'X', 0, 0);
        return;
    }
}

static void handleLeagueRm4Transition(Player &player){
    if (!currentMap) return;
    
    //Top - League Room 5    
    if (playerPos.row <= MapConfig::EDGE_THRESHOLD) {
        if(!isTrainerDefeated(MAP_LEAGUE_RM4, 0)){
            showDialogue("You must beat ELITE 4 DRAKE before proceeding...");
            return;
        }
        enterMap(MAP_LEAGUE_RM5, 'X', 0, 0);
        return;
    }
}

static void checkTransitions(Player &player) {
    if (!currentMap) return;
    if (!isInBounds(currentMap->layout, playerPos.row, playerPos.col)) return;

    char tile = currentMap->layout[playerPos.row][playerPos.col];

    // Route transitions ('=' tiles)
    if (tile == TileChars::ROUTE_TRANSITION) {
        switch (currentMapID) {
            case MAP_TRUCK:      handleTruckTransition(player);      break;
            case MAP_MOMHOUSE:   handleMomHouseTransition(player);   break;
            case MAP_RIVALHOUSE: handleRivalHouseTransition(player); break;
            case MAP_BIRCHLAB:   handleBirchLabTransition(player);   break;
            case MAP_LITTLEROOT: handleLittlerootTransition(player); break;
            case MAP_ROUTE101:   handleRoute101Transition(player);   break;
            case MAP_OLDALE:     handleOldaleTransition(player);     break;
            case MAP_POKECENTER: handlePokeCenterTransition(player); break;
            case MAP_POKEMART:   handlePokeMartTransition(player);   break;
            case MAP_ROUTE102:   handleRoute102Transition(player);   break;
            case MAP_PETALBURG1: handlePetalburg1Transition(player); break;
            case MAP_PETALBURG2: handlePetalburg2Transition(player); break;
            case MAP_PETALBURGGYM: handlePetalburgGymTransition(player); break;
            case MAP_ROUTE104: handleRoute104Transition(player); break;
            case MAP_PETALBURGWOODS1: handlePetalburgWoods1Transition(player); break;
            case MAP_PETALBURGWOODS2: handlePetalburgWoods2Transition(player); break;
            case MAP_RUSTBORO1: handleRustboro1Transition(player); break;
            case MAP_RUSTBORO2: handleRustboro2Transition(player); break;
            case MAP_RUSTBOROGYM: handleRustboroGymTransition (player); break;
            case MAP_DEWFORD1: handleDewford1Transition(player); break;
            case MAP_DEWFORD2: handleDewford2Transition(player); break;
            case MAP_DEWFORDGYM: handleDewfordGymTransition(player); break;
            case MAP_GRANITECAVE: handleGraniteCaveTransition(player); break;
            case MAP_SLATEPORT1: handleSlateport1Transition(player); break;
            case MAP_SLATEPORT2: handleSlateport2Transition(player); break;
            case MAP_ROUTE110_1: handleRoute110ATransition(player); break;
            case MAP_OCEANIC_MUSEUM: handleOceanicMuseumTransition(player); break;
            case MAP_ROUTE110_2: handleRoute110BTransition(player); break;
            case MAP_MAUVILLE: handleMauvilleTransition(player); break;
            case MAP_MAUVILLEGYM: handleMauvilleGymTransition(player); break;
            case MAP_ROUTE111_1: handleRoute111ATransition(player); break;
            case MAP_ROUTE111_2: handleRoute111BTransition(player); break;
            case MAP_ROUTE111_3: handleRoute111CTransition(player); break;
            case MAP_ROUTE111_DESERT: handleRoute111DesertTransition(player); break;
            case MAP_ROUTE112_1: handleRoute112ATransition(player); break;
            case MAP_FIERYPATH: handleFieryPathTransition(player); break;
            case MAP_ROUTE112_2: handleRoute112BTransition(player); break;
            case MAP_CABLECAR: handleCableCarTransition(player); break;
            case MAP_MTCHIMNEY: handleMtChimneyTransition(player); break;
            case MAP_MTCHIMNEY_SUMMIT: handleMtChimneySummitTransition(player); break;
            case MAP_JAGGEDPASS: handleJaggedPassTransition(player); break;
            case MAP_LAVARIDGE:  handleLavaridgeTransition(player); break;
            case MAP_LAVARIDGEGYM: handleLavaridgeGymTransition(player); break;
            case MAP_HARBOR: handleHarborTransition(player); break;
            case MAP_ROUTE118: handleRoute118Transition(player); break;
            case MAP_ROUTE119: handleRoute119Transition(player); break;
            case MAP_FORTREE: handleFortreeTransition(player); break;
            case MAP_FORTREEGYM: handleFortreeGymTransition(player); break;
            case MAP_LILYCOVE1: handleLilycove1Transition(player); break;
            case MAP_LILYCOVE2: handleLilycove2Transition(player); break;
            case MAP_LILYCOVE3: handleLilycove3Transition(player); break;
            case MAP_LILYCOVE4: handleLilycove4Transition(player); break;
            case MAP_AQUA_ENTRANCE: handleAquaEntranceTransition(player); break;
            case MAP_MTPYRE1: handleMtPyre1Transition(player); break;
            case MAP_MTPYRE2: handleMtPyre2Transition(player); break;
            case MAP_MTPYRE_SUMMIT: handleMtPyreSummitTransition(player); break;
            case MAP_MAGMA_HIDEOUT_1: handleMagmaHideout1Transition(player); break;
            case MAP_MAGMA_HIDEOUT_2: handleMagmaHideout2Transition(player); break;
            case MAP_MAGMA_HIDEOUT_3: handleMagmaHideout3Transition(player); break;
            case MAP_MAGMA_HIDEOUT_4: handleMagmaHideout4Transition(player); break;
            case MAP_AQUA_HIDEOUT_1: handleAquaHideout1Transition(player); break;
            case MAP_AQUA_HIDEOUT_2: handleAquaHideout2Transition(player); break;
            case MAP_AQUA_HIDEOUT_3: handleAquaHideout3Transition(player); break;
            case MAP_AQUA_HIDEOUT_4: handleAquaHideout4Transition(player); break;
            case MAP_AQUA_HIDEOUT_5: handleAquaHideout5Transition(player); break;
            case MAP_SEAFLOOR_CAVERN_1: handleSeafloorCavern1Transition(player); break;
            case MAP_SEAFLOOR_CAVERN_2: handleSeafloorCavern2Transition(player); break;
            case MAP_MOSSDEEP1: handleMossdeep1Transition(player); break;
            case MAP_MOSSDEEP2: handleMossdeep2Transition(player); break;
            case MAP_MOSSDEEPGYM: handleMossdeepGymTransition(player); break;
            case MAP_MOSSDEEP3: handleMossdeep3Transition(player); break;
            case MAP_SOOTOPOLIS1:  handleSootopolis1Transition(player); break;
            case MAP_SOOTOPOLIS2:  handleSootopolis2Transition(player); break;
            case MAP_SOOTOPOLISGYM: handleSootopolisGymTransition(player); break;
            case MAP_CAVE_OF_ORIGIN: handleCaveOfOriginTransition(player); break;
            case MAP_SKY_PILLAR: handleSkyPillarTransition(player); break;
            case MAP_VICTORY_ROAD1: handleVictoryRoad1Transition (player); break;
            case MAP_VICTORY_ROAD2: handleVictoryRoad2Transition (player); break;
            case MAP_LEAGUE_ENTRANCE: handleLeagueEntranceTransition(player); break;
            case MAP_LEAGUE_RM1: handleLeagueRm1Transition(player); break;
            case MAP_LEAGUE_RM2: handleLeagueRm2Transition(player); break;
            case MAP_LEAGUE_RM3: handleLeagueRm3Transition(player); break;
            case MAP_LEAGUE_RM4: handleLeagueRm4Transition(player); break;
        }
        return;
    }

    if (doorTransitions.find(tile) == doorTransitions.end()) return;
    
    MapID dest = doorTransitions[tile];
    
    lastBuildingOriginMap = currentMapID;
    
    // Display entrance dialogue based on destination
    switch (dest) {
        case MAP_MOMHOUSE:           showDialogue("Entering MOM'S HOUSE...");        break;
        case MAP_RIVALHOUSE:         showDialogue("Entering RIVAL'S HOUSE...");      break;
        case MAP_BIRCHLAB:           showDialogue("Entering BIRCH'S LAB...");        break;
        case MAP_POKECENTER:         showDialogue("Entering POKEMON CENTER...");     break;
        case MAP_POKEMART:           showDialogue("Entering POKEMART...");           break;
        case MAP_PETALBURGGYM:       showDialogue("Entering PETALBURG GYM...");      break;
        case MAP_RUSTBOROGYM:        showDialogue("Entering RUSTBORO GYM...");       break;
        case MAP_DEWFORDGYM:         showDialogue("Entering DEWFORD GYM...");        break;
        case MAP_OCEANIC_MUSEUM:     showDialogue("Entering OCEANIC MUSEUM...");     break;
        case MAP_MAUVILLEGYM:        showDialogue("Entering MAUVILLE GYM...");       break;
        case MAP_CABLECAR:           showDialogue("Entering CABLE CAR STATION..."); break;
        case MAP_LAVARIDGEGYM:       showDialogue("Entering LAVARIDGE GYM...");      break;
        case MAP_HARBOR:             showDialogue("Entering HARBOR...");             break;
        case MAP_FIERYPATH:          showDialogue("Entering FIERY PATH...");         break;
        case MAP_FORTREEGYM:         showDialogue("Entering FORTREE GYM...");        break;
        case MAP_AQUA_ENTRANCE:      showDialogue("Entering AQUA HIDEOUT...");       break;
        case MAP_MTPYRE1:            showDialogue("Entering MT PYRE...");            break;
        case MAP_MAGMA_HIDEOUT_1:    showDialogue("Entering MAGMA HIDEOUT...");      break;
        case MAP_SEAFLOOR_CAVERN_1:  showDialogue("Entering SEAFLOOR CAVERN...");    break;
        case MAP_MOSSDEEPGYM:        showDialogue("Entering MOSSDEEP GYM...");       break;
        case MAP_SKY_PILLAR:         showDialogue("Entering SKY PILLAR...");         break;
        case MAP_SOOTOPOLISGYM:		 showDialogue("Entering SOOTOPOLIS GYM...");	 break;
        case MAP_VICTORY_ROAD1: 	 showDialogue("Entering VICTORY ROAD...");		 break;
        case MAP_VICTORY_ROAD2:		 showDialogue("Returning to VICTORY ROAD..."); 	 break;
        default: break;
    }
    
    // Block checks
    if (dest == MAP_OCEANIC_MUSEUM && !flagCheckBlockRoute110) {
        showDialogue("The museum seems busy right now. Maybe you should explore other areas first...");
        return;
    }
    
    if (dest == MAP_MAGMA_HIDEOUT_1 && !flagUnlockMagmaHideout) {
        showDialogue("The entrance inside the VOLCANO seems to be blocked by heavy machinery....");
        return;
    }
    
    if (dest == MAP_SOOTOPOLISGYM && !flagTrioEvent){
    	showDialogue("SOOTOPOLIS GYM is closed at the moment...");
    	return;
	}
    
    if (dest == MAP_SEAFLOOR_CAVERN_1 && !flagUnlockSeafloorCavern) {
        showDialogue("TEAM AQUA ADMIN MATT: You have to go through me first you little brat!");
        return;
    }
    
    // Sky Pillar Rayquaza Awakening Sequence
    if (dest == MAP_SKY_PILLAR && !flagAwakenRayquaza) {
        AwakenRayquazaTxt();
        flagAwakenRayquaza = true;
    }
    
    // Register entering maps with char based entrance tiles
    if (currentMapID == MAP_LITTLEROOT || currentMapID == MAP_OLDALE
        || currentMapID == MAP_PETALBURG1 || currentMapID == MAP_PETALBURG2
        || currentMapID == MAP_RUSTBORO1 || currentMapID == MAP_RUSTBORO2
        || currentMapID == MAP_DEWFORD1 || currentMapID == MAP_DEWFORD2
        || currentMapID == MAP_SLATEPORT2 || currentMapID == MAP_MAUVILLE
        || currentMapID == MAP_ROUTE112_1 || currentMapID == MAP_ROUTE112_2
        || currentMapID == MAP_MTCHIMNEY || currentMapID == MAP_JAGGEDPASS
        || currentMapID == MAP_LAVARIDGE || currentMapID == MAP_FORTREE
        || currentMapID == MAP_LILYCOVE1 || currentMapID == MAP_LILYCOVE2 
        || currentMapID == MAP_LILYCOVE3 || currentMapID == MAP_MTPYRE_ENTRANCE
        || currentMapID == MAP_MAGMA_HIDEOUT_3 || currentMapID == MAP_AQUA_HIDEOUT_5
        || currentMapID == MAP_MOSSDEEP2 || currentMapID == MAP_SOOTOPOLIS1
        || currentMapID == MAP_SKY_PILLAR_ENTRANCE || currentMapID == MAP_EVER_GRANDE_CENTER
        || currentMapID == MAP_VICTORY_ROAD2 || currentMapID == MAP_EVER_GRANDE_LEAGUE
    ) {
        enterMap(dest);
        
        // Birch Starter Pokemon Sequence
        if (dest == MAP_BIRCHLAB && !flagHasStarter) {
            drBirchStarter(player);
            chooseStarter(player, db);
            firstBattle(player, db, "ZIGZAGOON", MapConfig::WILD_LEVEL_MIN, MapConfig::WILD_LEVEL_MAX);
            drBirchStarter2(player);
            flagHasStarter = true;
            storyState = INTRO_PICKED_STARTER;
        }
    }
    else {
        enterMap(dest, tile, MapConfig::DOOR_EXIT_ROW_OFFSET, 0);
    }
}

/*****************************************************************************
 * MAP REGISTRATION
 *****************************************************************************/
static void registerMaps() {
    worldMaps.clear();

    auto addMap = [](MapID id, const vector<string>& layout, const string& name, 
                     Pokemon (*wildFunc)(const PokemonDatabase&, int, int) = nullptr,
					 int minLevel = 2, int maxLevel = 3) {
        MapData data;
        data.id = id;
        data.layout = layout;
        data.name = name;
        data.spawn = {0, 0};
        data.wildEncounterFunc = wildFunc;
        data.wildMinLevel = minLevel;
        data.wildMaxLevel = maxLevel;
        worldMaps[id] = data;
    };

    addMap(MAP_TRUCK,      truck_map,      "TRUCK");
    addMap(MAP_LITTLEROOT, littleroot_map, "LITTLEROOT");
    addMap(MAP_MOMHOUSE,   mom_house_map,  "MOMHOUSE");
    addMap(MAP_RIVALHOUSE, rival_house_map,"RIVAL HOUSE");
    addMap(MAP_BIRCHLAB,   birch_lab_map,  "BIRCHLAB");
    addMap(MAP_ROUTE101,   route101_map,   "ROUTE 101",getWildPokemonRoute101,2,3);
    addMap(MAP_OLDALE,     oldale_map,     "OLDALE TOWN");
    addMap(MAP_POKECENTER, pokecenter_map, "POKEMON CENTER");
    addMap(MAP_POKEMART,   pokemart_map,   "POKEMART");
    addMap(MAP_ROUTE102,   route102_map,   "ROUTE 102",getWildPokemonRoute102,2,4);
    addMap(MAP_PETALBURG1, petalburg1_map, "PETALBURG TOWN");
    addMap(MAP_PETALBURG2, petalburg2_map, "PETALBURG TOWN ");
    addMap(MAP_PETALBURGGYM, petalburggym_map, "PETALBURG GYM");
    addMap(MAP_ROUTE104, route104_map, "ROUTE 104",getWildPokemonRoute104,3,5);
    addMap(MAP_PETALBURGWOODS1, petalburgwoods1_map, "PETALBURG WOODS",getWildPokemonPetalburgWoods,5,6);
    addMap(MAP_PETALBURGWOODS2, petalburgwoods2_map, "PETALBURG WOODS",getWildPokemonPetalburgWoods,5,6);
    addMap(MAP_RUSTBORO1, rustboro1_map, "RUSTBORO CITY");
    addMap(MAP_RUSTBORO2, rustboro2_map, "RUSTBORO CITY ");
    addMap(MAP_RUSTBOROGYM,rustborogym_map, "RUSTBORO GYM");
    addMap(MAP_DEWFORD1, dewford1_map, "DEWFORD TOWN");
	addMap(MAP_DEWFORD2, dewford2_map, "DEWFORD ISLAND");
	addMap(MAP_DEWFORDGYM, dewfordgym_map, "DEWFORD GYM");
	addMap(MAP_GRANITECAVE, granitecave_map, "GRANITE CAVE", getWildPokemonGraniteCave,10, 12);
	addMap(MAP_SLATEPORT1, slateport1_map, "SLATEPORT");
	addMap(MAP_SLATEPORT2, slateport2_map, "SLATEPORT");
	addMap(MAP_ROUTE110_1, route110_1_map, "ROUTE 110", getWildPokemonRoute110,12,13);
	addMap(MAP_OCEANIC_MUSEUM, oceanic_museum_map, "OCEANIC MUSEUM");
	addMap(MAP_ROUTE110_2, route110_2_map, "ROUTE 110",getWildPokemonRoute110,12,13); 
	addMap(MAP_MAUVILLE, mauville_map, "MAUVILLE");
	addMap(MAP_MAUVILLEGYM, mauvillegym_map, "MAUVILLE GYM");
	addMap(MAP_ROUTE111_1, route111_1_map, "ROUTE 111");
	addMap(MAP_ROUTE111_2, route111_2_map, "ROUTE 111");
	addMap(MAP_ROUTE111_3, route111_3_map, "ROUTE 111");
	addMap(MAP_ROUTE111_DESERT, route111_desert_map, "DESERT", getWildPokemonRoute111Desert,19,22);
	addMap(MAP_ROUTE112_1, route112_1_map, "ROUTE 112",getWildPokemonRoute112,14,16);
	addMap(MAP_ROUTE112_2, route112_2_map, "ROUTE 112");
	addMap(MAP_FIERYPATH, fierypath_map, "FIERY PATH",getWildPokemonFieryPath,14,16);
	addMap(MAP_CABLECAR, cablecar_map, "CABLE CAR BLDG.");
	addMap(MAP_MTCHIMNEY, mtchimney_1_map, "MT CHIMNEY");
	addMap(MAP_MTCHIMNEY_SUMMIT, mtchimney_summit_map, "MT CHIMNEY SUMMIT");
	addMap(MAP_JAGGEDPASS, jagged_pass_map, "JAGGED PASS",getWildPokemonJaggedPass,20,22);
	addMap(MAP_LAVARIDGE, lavaridge_map, "LAVARIDGE TOWN");
	addMap(MAP_LAVARIDGEGYM, lavaridgegym_map, "LAVARIDGE GYM");
	addMap(MAP_HARBOR, harbor_map, "HARBOR");
	addMap(MAP_ROUTE118, route118_map, "ROUTE 118",getWildPokemonRoute118,24,26);
	addMap(MAP_ROUTE119, route119_map, "ROUTE 119",getWildPokemonRoute119,24,27);
	addMap(MAP_FORTREE, fortree_map, "FORTREE CITY");
	addMap(MAP_FORTREEGYM, fortreegym_map, "FORTREE GYM");
	addMap(MAP_LILYCOVE1, lilycove1_map, "LILYCOVE CITY");
	addMap(MAP_LILYCOVE2, lilycove2_map, "LILYCOVE CITY");
	addMap(MAP_LILYCOVE3, lilycove3_map, "LILYCOVE CITY");
	addMap(MAP_LILYCOVE4, lilycove4_map, "LILYCOVE CITY");
	addMap(MAP_AQUA_ENTRANCE, aqua_entrance_map, "AQUA HIDEOUT ENTRANCE");
	addMap(MAP_MTPYRE_ENTRANCE, mtpyre_entrance_map, "MT PYRE");
	addMap(MAP_MTPYRE1, mtpyre1_map, "MT PYRE",getWildPokemonMtPyreInterior,22,29);
	addMap(MAP_MTPYRE2, mtpyre2_map, "MT PYRE",getWildPokemonMtPyreExterior,25,29);
	addMap(MAP_MTPYRE_SUMMIT, mtpyre_summit_map, "MT PYRE SUMMIT", getWildPokemonMtPyreSummit,26,30);
	addMap(MAP_MAGMA_HIDEOUT_1, magma_hideout_1_map, "MAGMA HIDEOUT",getWildPokemonMagmaHideout, 27,30);
	addMap(MAP_MAGMA_HIDEOUT_2, magma_hideout_2_map, "MAGMA HIDEOUT",getWildPokemonMagmaHideout, 27,30);
	addMap(MAP_MAGMA_HIDEOUT_3, magma_hideout_3_map, "MAGMA HIDEOUT",getWildPokemonMagmaHideout, 27,30);
	addMap(MAP_MAGMA_HIDEOUT_4, magma_hideout_4_map, "MAGMA HIDEOUT");
	addMap(MAP_AQUA_HIDEOUT_1, aqua_hideout_1_map, "AQUA HIDEOUT");
	addMap(MAP_AQUA_HIDEOUT_2, aqua_hideout_2_map, "AQUA HIDEOUT"); 
	addMap(MAP_AQUA_HIDEOUT_3, aqua_hideout_3_map, "AQUA HIDEOUT"); 
	addMap(MAP_AQUA_HIDEOUT_4, aqua_hideout_4_map, "AQUA HIDEOUT"); 
	addMap(MAP_AQUA_HIDEOUT_5, aqua_hideout_5_map, "AQUA HIDEOUT");
	addMap(MAP_SEAFLOOR_CAVERN_1,seafloor_cavern_1_map, "SEAFLOOR CAVERN",getWildPokemonSeafloorCavern, 33, 36);
	addMap(MAP_SEAFLOOR_CAVERN_2,seafloor_cavern_2_map, "SEAFLOOR CAVERN");
	addMap(MAP_MOSSDEEP1, mossdeep_1_map, "MOSSDEEP CITY");
	addMap(MAP_MOSSDEEP2, mossdeep_2_map, "MOSSDEEP CITY");
	addMap(MAP_MOSSDEEPGYM, mossdeepgym_map, "MOSSDEEP GYM");
	addMap(MAP_MOSSDEEP3, mossdeep_3_map, "MOSSDEEP CITY");
	addMap(MAP_SOOTOPOLIS1, sootopolis_1_map, "SOOTOPOLIS CIY");
	addMap(MAP_SOOTOPOLIS2, sootopolis_2_map, "SOOTOPOLIS CIY");
	addMap(MAP_CAVE_OF_ORIGIN, cave_of_origin_map, "CAVE OF ORIGIN");
	addMap(MAP_SKY_PILLAR_ENTRANCE, sky_pillar_entrance_map, "SKY PILLAR");
	addMap(MAP_SKY_PILLAR, sky_pillar, "SKY PILLAR");
	addMap(MAP_SOOTOPOLISGYM, sootopolisgym_map, "SOOTOPOLIS GYM");
	addMap(MAP_EVER_GRANDE_CENTER, ever_grande_center_map, "EVER GRANDE CITY");
	addMap(MAP_VICTORY_ROAD1, victory_road_1_map, "VICTORY ROAD",getWildPokemonVictoryRoad, 38,40);
	addMap(MAP_VICTORY_ROAD2, victory_road_2_map, "VICTORY ROAD",getWildPokemonVictoryRoad, 38,40);
	addMap(MAP_EVER_GRANDE_LEAGUE, ever_grande_league_map, "EVER GRANDE LEAGUE");
	addMap(MAP_LEAGUE_ENTRANCE, league_entrance_map, "POKEMON LEAGUE ENTRANCE");
	addMap(MAP_LEAGUE_RM1, league_rm1_map, "LEAGUE SIDNEY");
	addMap(MAP_LEAGUE_RM2, league_rm2_map, "LEAGUE PHOEBE");
	addMap(MAP_LEAGUE_RM3, league_rm3_map, "LEAGUE GLACIA");
	addMap(MAP_LEAGUE_RM4, league_rm4_map, "LEAGUE DRAKE");
	addMap(MAP_LEAGUE_RM5, league_rm5_map, "LEAGUE WALLACE");
}

static void initMaps() {
    registerMaps();
    
    // Hide cursor
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    if(!DEBUG_MODE){
    	enterMap(MAP_TRUCK);
	}
}

/*****************************************************************************
 * DEBUG DEV MODE : for pure testing
 *****************************************************************************/
static void initDebugMode(Player &player) {
    if (!DEBUG_MODE) return;
    
    
    storyState = STORY_FREE_ROAM;
    flagVisitedMom = true;
    flagHasStarter = true;
    
    flagMetSteven=true;
    flagRescueStern=true;
    flagCheckBlockRoute110=true;
    flagUnblockRoute110= true;
    
	flagCheckMagmaMachine= true;
	flagEncounterMachine = true;
	flagMtSummitCleared= true;
	
	flagCheckAquaEntrance= true;

	flagOrbStolen= true;


	flagShowMagmaEntranceOnce= true;
	flagUnlockMagmaHideout= true;
	flagMaxieDefeated= true;
	flagAwakenGroudon= true; 

	flagUnlockSeafloorCavern= true;
    flagArchieDefeated= true;
	flagAwakenKyogre= true;

	flagRocketEvent= true;

    flagSootopolisTalkToSteven=true;
    flagMetWallace=true;
    flagAwakenRayquaza= true; 
	flagTrioEvent= true;
	
    flagWallaceDefeated=true;
    
    if (player.currentPokemon.empty()) {
        Pokemon starter = db.pokedex[118];
        starter.level = 59;
        starter.scaleStatsToLevel();
        starter.currentHP = starter.baseHP;
        starter.initializePP(db.moveList);
        starter.sanitizeMoves(db.moveList);
        player.currentPokemon.push_back(starter);
        
        Pokemon p2 = db.pokedex[126];  // First non-starter
		p2.level = 58;
		p2.scaleStatsToLevel();
		p2.currentHP = p2.baseHP;
		p2.initializePP(db.moveList);
		p2.sanitizeMoves(db.moveList);
		player.currentPokemon.push_back(p2);
		
		// p3
		Pokemon p3 = db.pokedex[82];
		p3.level = 55;
		p3.scaleStatsToLevel();
		p3.currentHP = p3.baseHP;
		p3.initializePP(db.moveList);
		p3.sanitizeMoves(db.moveList);
		player.currentPokemon.push_back(p3);
		
		// p4
		Pokemon p4 = db.pokedex[132];
		p4.level = 58;
		p4.scaleStatsToLevel();
		p4.currentHP = p4.baseHP;
		p4.initializePP(db.moveList);
		p4.sanitizeMoves(db.moveList);
		player.currentPokemon.push_back(p4);
		
		// p5
		Pokemon p5 = db.pokedex[57];
		p5.level = 58;
		p5.scaleStatsToLevel();
		p5.currentHP = p5.baseHP;
		p5.initializePP(db.moveList);
		p5.sanitizeMoves(db.moveList);
		player.currentPokemon.push_back(p5);
		
		//p6
		Pokemon p6 = db.pokedex[131];
		p6.level = 58;
		p6.scaleStatsToLevel();
		p6.currentHP = p6.baseHP;
		p6.initializePP(db.moveList);
		p6.sanitizeMoves(db.moveList);
		player.currentPokemon.push_back(p6);
		
    }
    
    player.money = 5000;
    
    player.earnBadge(BadgeIndex::STONE_BADGE);
    player.earnBadge(BadgeIndex::KNUCKLE_BADGE);
    player.earnBadge(BadgeIndex::DYNAMO_BADGE);
    player.earnBadge(BadgeIndex::HEAT_BADGE);
    player.earnBadge(BadgeIndex::BALANCE_BADGE);
    player.earnBadge(BadgeIndex::FEATHER_BADGE);
    player.earnBadge(BadgeIndex::MIND_BADGE);
	player.earnBadge(BadgeIndex::RAIN_BADGE);
    
    enterMap(DEBUG_SPAWN_MAP);
    
}

/*****************************************************************************
 * GAME LOOP
 *****************************************************************************/
static void runMapLoop(Player &player) {
    if (worldMaps.empty()) initMaps();
    
    // Set rival name once based on player gender
    if (rivalName.empty()) {
        rivalName = (player.gender == BOY) ? "MAY" : "BRENDAN";
    }

	/***DEBUG TEST MODE REMOVE UPON GAME DEV COMPLETION***/
	 if (DEBUG_MODE) {
        initDebugMode(player);
    }
	
    bool gameRunning = true;
    
    while (gameRunning) {
        clearScreen();
        renderMapWithPlayer(currentMap->layout, playerPos, player);

        char input = getInput();

        // Main menu
        if (input == 'm' || input == 'M') {
            if (openMainMenu(player, db)) {
                gameRunning = false;
                continue;
            }
            continue;
        }

        attemptMovePlayer(currentMap->layout, playerPos, input, player);
        checkTransitions(player);

        char currentTile = currentMap->layout[playerPos.row][playerPos.col];

        // Wild encounter check
        if (!hasWildEncounter(currentTile)) continue;
        if (!currentMap->wildEncounterFunc) continue;
        if (rand() % 100 >= MapConfig::WILD_ENCOUNTER_CHANCE) continue;
        
        Pokemon wild = currentMap->wildEncounterFunc(db, currentMap->wildMinLevel, currentMap->wildMaxLevel);

        Pokemon* playerActive = nullptr;
        for (auto &p : player.currentPokemon) {
            if (p.currentHP > 0) {
                playerActive = &p;
                break;
            }
        }

        if (playerActive) {
            startBattleLoop(player, *playerActive, wild, db, nullptr);
        }
    }
    
    clearScreen();
    showDialogue("Exiting...");
}

#endif // POKEMON_MAPS_H