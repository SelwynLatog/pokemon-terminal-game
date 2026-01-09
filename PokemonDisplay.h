#ifndef POKEMON_DISPLAY_H
#define POKEMON_DISPLAY_H

#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cmath>

#include "Player.h"

using namespace std;

/*****************************************************************************
 * FORWARD DECLARATIONS
 *****************************************************************************/
void clearScreen();
void pressAnyKey();

/*****************************************************************************
 * DISPLAY CONFIGURATION CONSTANTS
 *****************************************************************************/
namespace DisplayConfig {
    // Dialogue box dimensions
    constexpr int DEFAULT_DIALOGUE_WIDTH = 40;
    constexpr int BATTLE_DIALOGUE_WIDTH = 70;
    constexpr int BATTLE_DIALOGUE_HEIGHT = 4;
    
    // Battle frame dimensions
    constexpr int BATTLE_FRAME_WIDTH = 70;
    constexpr int BATTLE_FRAME_HEIGHT = 18;
    
    // HP bar configuration
    constexpr int DEFAULT_HP_BAR_WIDTH = 25;
    constexpr int ENEMY_HP_BAR_WIDTH = 28;
    constexpr int PLAYER_HP_BAR_WIDTH = 28;
    constexpr int MIN_HP_BAR_WIDTH = 10;
    
    // Item menu width
    constexpr int ITEM_MENU_WIDTH = 50;
    
    // Text animation speeds (milliseconds)
    constexpr int TYPEWRITER_SPEED_FAST = 5;
    constexpr int TYPEWRITER_SPEED_MEDIUM = 20;
    constexpr int TYPEWRITER_SPEED_DEFAULT = 25;
    
    // Visual effect timings
    constexpr int ARROW_BLINK_DELAY = 300;
    constexpr int ARROW_BLINK_COUNT = 3;
    constexpr int BATTLE_ARROW_BLINK_COUNT = 2;
    constexpr int FLASH_TRANSITION_DELAY = 120;
    constexpr int FLASH_TRANSITION_COUNT = 3;
    constexpr int PROMPT_BLINK_DELAY = 500;
    constexpr int INPUT_DELAY = 50;
    constexpr int MENU_UPDATE_DELAY = 100;
    
    // Battle frame positioning
    constexpr int ENEMY_INFO_ROW_1 = 0;
    constexpr int ENEMY_INFO_ROW_2 = 1;
    constexpr int ENEMY_INFO_INDENT = 1;
    constexpr int PLAYER_INFO_PADDING = 1;
}

namespace KeyCodesDisplay {
    constexpr int ARROW_PREFIX_1 = 224;
    constexpr int ARROW_PREFIX_2 = 0;
    constexpr int ARROW_UP = 72;
    constexpr int ARROW_DOWN = 80;
    constexpr int KEY_ENTER = 13;
    constexpr int KEY_ESC = 27;
}

namespace ConsoleColors {
    // Background colors
    constexpr int BG_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
    constexpr int BG_BLACK = 0;
    constexpr int BG_BLUE = BACKGROUND_BLUE;
    constexpr int BG_BLUE_BRIGHT = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
    constexpr int BG_RED = BACKGROUND_RED;
    constexpr int BG_RED_BRIGHT = BACKGROUND_RED | BACKGROUND_INTENSITY;
    constexpr int BG_GREEN = BACKGROUND_GREEN;
    constexpr int BG_GREEN_BRIGHT = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
    
    // Foreground colors
    constexpr int FG_BRIGHT = FOREGROUND_INTENSITY;
    constexpr int FG_DEFAULT = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    constexpr int FG_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    constexpr int FG_RED = FOREGROUND_RED | FOREGROUND_INTENSITY;
    constexpr int FG_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    constexpr int FG_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    constexpr int FG_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    constexpr int FG_CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    
    // Combined styles
    constexpr int HIGHLIGHT_STYLE = BG_WHITE | FG_BRIGHT;
    constexpr int DEFAULT_STYLE = FG_DEFAULT;
}

/*****************************************************************************
 * CURSOR MOVEMENT UTILITIES
 *****************************************************************************/
void moveCursorUp(int lines) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(h, &csbi);
    
    COORD coord;
    coord.X = csbi.dwCursorPosition.X;
    coord.Y = csbi.dwCursorPosition.Y - lines;
    SetConsoleCursorPosition(h, coord);
}

void moveCursorDown(int lines) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(h, &csbi);
    
    COORD coord;
    coord.X = csbi.dwCursorPosition.X;
    coord.Y = csbi.dwCursorPosition.Y + lines;
    SetConsoleCursorPosition(h, coord);
}

/*****************************************************************************
 * TEXT FORMATTING UTILITIES
 *****************************************************************************/
void typeText(const string &text, int speed = DisplayConfig::TYPEWRITER_SPEED_DEFAULT) {
    bool skip = false;

    for (char c : text) {
        // Allow player to skip animation with Enter
        if (_kbhit()) {
            char key = _getch();
            if (key == KeyCodesDisplay::KEY_ENTER) {
                skip = true;
            }
        }

        cout << c << flush;

        if (!skip) {
            Sleep(speed);
        }
    }
}

vector<string> wrapText(const string &text, int width) {
    vector<string> lines;
    istringstream words(text);
    string word, line;

    while (words >> word) {
        if (line.size() + word.size() + 1 > (size_t)width) {
            lines.push_back(line);
            line = word;
        } else {
            if (!line.empty()) line += " ";
            line += word;
        }
    }
    
    if (!line.empty()) {
        lines.push_back(line);
    }

    return lines;
}

/*****************************************************************************
 * HIGHLIGHT SELECTION UTILITY
 *****************************************************************************/
void drawHighlight(HANDLE h, const string& text, bool highlighted) {
    if (highlighted) {
        SetConsoleTextAttribute(h, ConsoleColors::HIGHLIGHT_STYLE);
        cout << "> " << text;
        SetConsoleTextAttribute(h, ConsoleColors::DEFAULT_STYLE);
    } else {
        cout << "  " << text;
    }
}

/*****************************************************************************
 * SCREEN TRANSITION EFFECTS
 *****************************************************************************/
void flashTransition(int flashes = DisplayConfig::FLASH_TRANSITION_COUNT, 
                     int speed = DisplayConfig::FLASH_TRANSITION_DELAY) {
    for (int i = 0; i < flashes; ++i) {
        system("color F0"); // White background, black text
        Sleep(speed);
        system("color 07"); // Black background, gray text
        Sleep(speed);
    }
    system("cls");
}

/*****************************************************************************
 * DIALOGUE BOX SYSTEM
 *****************************************************************************/
void showDialogue(const string &text, 
                  int width = DisplayConfig::DEFAULT_DIALOGUE_WIDTH, 
                  int speed = DisplayConfig::TYPEWRITER_SPEED_DEFAULT, 
                  bool blinkArrow = true) {
    clearScreen();
    vector<string> lines = wrapText(text, width);
    
    // Top border
    cout << "+" << string(width + 2, '-') << "+\n";

    // Print all lines with typewriter effect
    for (auto &line : lines) {
        cout << "| ";
        typeText(line, speed);

        int pad = max(0, width - (int)line.size());
        cout << string(pad, ' ') << " |\n";
    }

    // Bottom border
    cout << "+" << string(width + 2, '-') << "+";

    // Optional blinking arrow indicator
    if (blinkArrow && !lines.empty()) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

        // Move cursor up to last line
        moveCursorUp(1);

        // Print last line again with space for blinking arrow
        int lastPad = max(0, width - (int)lines.back().size() - 1);
        cout << "\r| " << lines.back() << string(lastPad, ' ') << ">" << flush;

        // Blink the arrow
        for (int i = 0; i < DisplayConfig::ARROW_BLINK_COUNT; ++i) {
            cout << "\b" << flush;

            // Inverted blink (white background)
            SetConsoleTextAttribute(h, ConsoleColors::HIGHLIGHT_STYLE);
            cout << ">" << flush;
            Sleep(DisplayConfig::ARROW_BLINK_DELAY);

            // Reset to normal
            cout << "\b" << flush;
            SetConsoleTextAttribute(h, ConsoleColors::DEFAULT_STYLE);
            cout << ">" << flush;
            Sleep(DisplayConfig::ARROW_BLINK_DELAY);
        }

        // Reset console
        SetConsoleTextAttribute(h, ConsoleColors::DEFAULT_STYLE);
        cout << endl << endl;
    }
}

void showBattleDialogue(const string &text, 
                        int width = DisplayConfig::BATTLE_DIALOGUE_WIDTH, 
                        int speed = DisplayConfig::TYPEWRITER_SPEED_MEDIUM, 
                        bool blinkArrow = true) {
    vector<string> lines = wrapText(text, width);

    // Top border
    cout << "+" << string(width + 2, '-') << "+\n";

    // Fill remaining empty space (consistent dialogue height)
    for (int i = (int)lines.size(); i < DisplayConfig::BATTLE_DIALOGUE_HEIGHT - 1; ++i) {
        cout << "| " << string(width, ' ') << " |\n";
    }

    // Bottom border
    cout << "+" << string(width + 2, '-') << "+";

    // Optional blinking arrow prompt
    if (blinkArrow && !lines.empty()) {
        moveCursorUp(1);
        cout << "\r| " << lines.back();
        cout << string(width - (int)lines.back().size() - 1, ' ') << ">" << flush;

        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        
        for (int i = 0; i < DisplayConfig::BATTLE_ARROW_BLINK_COUNT; ++i) {
            cout << "\b" << flush;
            SetConsoleTextAttribute(h, ConsoleColors::BG_WHITE);
            cout << ">" << flush;
            Sleep(DisplayConfig::ARROW_BLINK_DELAY);
            
            cout << "\b" << flush;
            SetConsoleTextAttribute(h, ConsoleColors::DEFAULT_STYLE);
            cout << ">" << flush;
            Sleep(DisplayConfig::ARROW_BLINK_DELAY);
        }
        
        SetConsoleTextAttribute(h, ConsoleColors::DEFAULT_STYLE);
        cout << endl << endl;
    }
}

/*****************************************************************************
 * MENU SYSTEMS
 *****************************************************************************/
void pressAnyKeyPrompt() {
    string message = "Press Any Key To Continue...";
    bool visible = true;

    // Keep flashing until a key is pressed
    while (!_kbhit()) {
        if (visible) {
            cout << message << flush;
        } else {
            cout << string(message.size(), ' ') << flush;
        }

        Sleep(DisplayConfig::PROMPT_BLINK_DELAY);
        cout << "\r";
        visible = !visible;
    }

    _getch();
    cout << "\r" << string(message.size(), ' ') << "\r";
}

int titleScreenMenu() {
    const vector<string> options = {"NEW GAME","CONTINUE", "QUIT"};
    int currentChoice = 0;

    // Find longest option length for padding
    size_t maxLength = 0;
    for (const auto& opt : options) {
        if (opt.length() > maxLength) {
            maxLength = opt.length();
        }
    }

    while (true) {
        system("cls");

        for (int i = 0; i < (int)options.size(); ++i) {
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

            string padded = options[i] + string(maxLength - options[i].length(), ' ');

            if (i == currentChoice) {
                SetConsoleTextAttribute(h, ConsoleColors::HIGHLIGHT_STYLE);
                cout << "> " << padded;
                SetConsoleTextAttribute(h, ConsoleColors::DEFAULT_STYLE);
            } else {
                cout << "  " << padded;
            }
            cout << endl;
        }

        int key = _getch();

        if (key == KeyCodesDisplay::ARROW_PREFIX_1 || key == KeyCodesDisplay::ARROW_PREFIX_2) {
            key = _getch();
            
            if (key == KeyCodesDisplay::ARROW_UP) {
                currentChoice--;
                if (currentChoice < 0) currentChoice = (int)options.size() - 1;
            }
            else if (key == KeyCodesDisplay::ARROW_DOWN) {
                currentChoice++;
                if (currentChoice >= (int)options.size()) currentChoice = 0;
            }
        }
        else if (key == KeyCodesDisplay::KEY_ENTER) {
            return currentChoice; // 0 = NEW GAME, 1 = QUIT
        }

        Sleep(DisplayConfig::MENU_UPDATE_DELAY);
    }
}

int selectItemMenu(Player &player, int width = DisplayConfig::ITEM_MENU_WIDTH) {
    // If inventory is empty, immediately return -1
    if (player.items.empty()) return -1;

    int cursor = 0;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    while (true) {
        system("cls");

        // Draw box header
        cout << "+" << string(width + 2, '-') << "+\n";
        string prompt = "ITEMS";
        cout << "| " << prompt << string(width - prompt.length(), ' ') << " |\n";
        cout << "+" << string(width + 2, '-') << "+\n\n";

        // Display items with highlight for selection
        for (int i = 0; i < (int)player.items.size(); ++i) {
            Item &it = player.items[i];
            string line = it.itemName + " (" + to_string(it.quantity) + ")";
            int pad = max(1, width - (int)line.size());

            if (i == cursor) {
                SetConsoleTextAttribute(h, ConsoleColors::HIGHLIGHT_STYLE);
                cout << "> " << line << string(pad, ' ') << "\n";
                SetConsoleTextAttribute(h, ConsoleColors::DEFAULT_STYLE);
            } else {
                cout << "  " << line << string(pad, ' ') << "\n";
            }
        }
        
        cout << endl;
        cout << "ARROW KEYS  -  MOVE" << endl;
        cout << "ENTER       -  SELECT" << endl;
        cout << "ESC         -  BACK" << endl;

        // Handle input
        int key = _getch();
        
        if (key == KeyCodesDisplay::ARROW_PREFIX_1 || key == KeyCodesDisplay::ARROW_PREFIX_2) {
            key = _getch();
            
            if (key == KeyCodesDisplay::ARROW_UP) {
                cursor--;
                if (cursor < 0) cursor = (int)player.items.size() - 1;
            } 
            else if (key == KeyCodesDisplay::ARROW_DOWN) {
                cursor++;
                if (cursor >= (int)player.items.size()) cursor = 0;
            }
        } 
        else if (key == KeyCodesDisplay::KEY_ENTER) {
            return cursor;
        } 
        else if (key == KeyCodesDisplay::KEY_ESC) {
            return -1;
        }

        Sleep(DisplayConfig::INPUT_DELAY);
    }
}

/*****************************************************************************
 * BATTLE UI: HP BAR
 *****************************************************************************/
string buildHPBarString(int currentHP, int maxHP, int barWidth = DisplayConfig::DEFAULT_HP_BAR_WIDTH) {
    if (maxHP <= 0) maxHP = 1;
    
    int filled = (int)((double)currentHP / maxHP * barWidth);
    filled = max(0, min(filled, barWidth));

    string bar = "HP: [";
    for (int i = 0; i < barWidth; ++i) {
        bar += (i < filled ? '#' : ' ');
    }
    bar += "] " + to_string(currentHP) + " / " + to_string(maxHP);
    
    return bar;
}

void printColoredHPBar(int currentHP, int maxHP, int barWidth = DisplayConfig::DEFAULT_HP_BAR_WIDTH) {
    if (maxHP <= 0) maxHP = 1;
    
    int filled = (int)((double)currentHP / maxHP * barWidth);
    filled = max(0, min(filled, barWidth));
    
    // Determine HP color based on percentage
    double hpPercent = (double)currentHP / maxHP;
    int hpColor;
    if (hpPercent > 0.5) {
        hpColor = ConsoleColors::FG_GREEN;
    } else if (hpPercent > 0.2) {
        hpColor = ConsoleColors::FG_YELLOW;
    } else {
        hpColor = ConsoleColors::FG_RED;
    }
    
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Print
    cout << "HP: [";
    
    // Color the filled portion
    SetConsoleTextAttribute(h, hpColor);
    for (int i = 0; i < filled; ++i) {
        cout << '#';
    }
    SetConsoleTextAttribute(h, ConsoleColors::FG_DEFAULT);
    
    // Empty portion
    for (int i = filled; i < barWidth; ++i) {
        cout << ' ';
    }
    
    cout << "] " << currentHP << " / " << maxHP;
}

/*****************************************************************************
 * BATTLE UI: BATTLE FRAME
 *****************************************************************************/
void drawBattleFrame(const Pokemon& player, const Pokemon& enemy) {
    // Top border
    cout << "+" << string(DisplayConfig::BATTLE_FRAME_WIDTH, '-') << "+\n";

    // Prepare display strings
    string enemyLabel = enemy.pokemonName + " (Lv" + to_string(enemy.level) + ")";
    string playerLabel = player.pokemonName + " (Lv" + to_string(player.level) + ")";
    
    int playerBarWidth = DisplayConfig::PLAYER_HP_BAR_WIDTH;
    if ((int)playerLabel.length() + playerBarWidth + 10 > DisplayConfig::BATTLE_FRAME_WIDTH) {
        playerBarWidth = max(DisplayConfig::MIN_HP_BAR_WIDTH, 
                            DisplayConfig::BATTLE_FRAME_WIDTH - (int)playerLabel.length() - 10);
    }

    // Calculate positioning
    int playerLineLabel = DisplayConfig::BATTLE_FRAME_HEIGHT - 3;
    int playerLineHP = DisplayConfig::BATTLE_FRAME_HEIGHT - 2;

    // Draw frame content
    for (int row = 0; row < DisplayConfig::BATTLE_FRAME_HEIGHT; ++row) {
        cout << "|";

        if (row == DisplayConfig::ENEMY_INFO_ROW_1) {
            // Enemy label at left
            cout << enemyLabel;
            cout << string(DisplayConfig::BATTLE_FRAME_WIDTH - enemyLabel.length(), ' ');
        }
        else if (row == DisplayConfig::ENEMY_INFO_ROW_2) {
            // Enemy HP with small indent and color
            cout << string(DisplayConfig::ENEMY_INFO_INDENT, ' ');
            printColoredHPBar(enemy.currentHP, enemy.baseHP, DisplayConfig::ENEMY_HP_BAR_WIDTH);
            
            // Calculate remaining space after HP bar
            string hpStr = buildHPBarString(enemy.currentHP, enemy.baseHP, DisplayConfig::ENEMY_HP_BAR_WIDTH);
            int remaining = DisplayConfig::BATTLE_FRAME_WIDTH - DisplayConfig::ENEMY_INFO_INDENT - (int)hpStr.length();
            if (remaining > 0) {
                cout << string(remaining, ' ');
            }
        }
        else if (row == playerLineLabel) {
            // Player label aligned to right
            int spacing = DisplayConfig::BATTLE_FRAME_WIDTH - (int)playerLabel.length() - DisplayConfig::PLAYER_INFO_PADDING;
            spacing = max(0, spacing);
            cout << string(spacing, ' ') << playerLabel;
            cout << string(DisplayConfig::PLAYER_INFO_PADDING, ' ');
        }
        else if (row == playerLineHP) {
            // Player HP aligned to right with color
            string hpStr = buildHPBarString(player.currentHP, player.baseHP, playerBarWidth);
            int spacing = DisplayConfig::BATTLE_FRAME_WIDTH - (int)hpStr.length() - DisplayConfig::PLAYER_INFO_PADDING;
            spacing = max(0, spacing);
            
            cout << string(spacing, ' ');
            printColoredHPBar(player.currentHP, player.baseHP, playerBarWidth);
            cout << string(DisplayConfig::PLAYER_INFO_PADDING, ' ');
        }
        else {
            // Empty row
            cout << string(DisplayConfig::BATTLE_FRAME_WIDTH, ' ');
        }
        
        cout << "|\n";
    }

    // Bottom border
    cout << "+" << string(DisplayConfig::BATTLE_FRAME_WIDTH, '-') << "+\n";
}

/*****************************************************************************
 * BATTLE UI: BATTLE DIALOGUE BOX
 *****************************************************************************/
void drawBattleDialogue(const string& message, int textDelay = DisplayConfig::TYPEWRITER_SPEED_FAST) {
    cout << "+" << string(DisplayConfig::BATTLE_DIALOGUE_WIDTH, '-') << "+\n";

    // Print message with typewriter effect
    cout << "| ";
    int charCount = 0;
    
    for (char c : message) {
        cout << c << flush;
        Sleep(textDelay);
        charCount++;
        
        // Wrap if line too long
        if (charCount >= DisplayConfig::BATTLE_DIALOGUE_WIDTH - 4) {
            cout << " |\n| ";
            charCount = 0;
        }
    }

    // Fill remaining space
    while (charCount++ < DisplayConfig::BATTLE_DIALOGUE_WIDTH - 3) {
        cout << ' ';
    }
    cout << "|\n";

    // Empty lines to complete box height
    for (int i = 0; i < DisplayConfig::BATTLE_DIALOGUE_HEIGHT - 2; ++i) {
        cout << "|" << string(DisplayConfig::BATTLE_DIALOGUE_WIDTH, ' ') << "|\n";
    }

    cout << "+" << string(DisplayConfig::BATTLE_DIALOGUE_WIDTH, '-') << "+\n";
}

/*****************************************************************************
 * ASCII ART
 *****************************************************************************/
void pokemonLogo() {
    cout << R"(
    
                                                                             -++.                                                              
                                                                           -#++++.                                                             
                                                                         .++++--++-                                                            
                                                                        -+++-..--++-.                                                          
                                                       .. ..          .++++-...-++++-                                                          
                   .....                         ..-+++++++++.      .++++-.-++++-.             .++++++++++                                     
           .-+++++++++++++++-.              .-+++++++++++++++++.   -##+++++++-..     .++++++++++++++++++++         .--.                        
       .++++++++----------+++++-        .+#+++++------+++++--++++.  +++++++++++++++. #++++------+++...--++-       -##++++++++-..               
    -+++++---.............----+++-      .#++++.-....--+++-...--+++++++++---------+++++#++-.....-+++....-+++       -++++---++++++++.++-..       
 -+++++-.....................--+++-     .+++++++....--++.......--++++-...........---+++++-.....--+-....--++       -+++++-...----++++++++++++--.
+##+++-........................--++.    .+++++++-...--+........-+++-....++++++-...-++++++.......-+.....--++.     .-++++++......-+++++------+++-
 +##+++-.............++++++-...--++-    .. +##++-...--.......-+++++...-++ .++-..-++++#+++.......-+......-+++.++++++++++++......--++++.....-+++ 
  ++++++-...........-+++-+++-...-++.-+++++++++++-...........+++-++-...-+--++...+++++++++-...............-+++++--------+++-.....--+++......-++. 
   -+++++++++........-++..++-..-++++++--------+++-........++++..++-...-+++-..++++--+++++................-+++..++-.....--++-.....-+++.....-++-  
    -+++++++++.......--+++++...++++-..++--...---++-.....----+++++++....--..----....--++++-...-.........-++-.-+++-...-+.--++-....--+-....-+++   
     .++++++++-.......-+++-...++++..-+++-...-+.--++.........----++++................-+++-...--+....-..-++-..-++++++++-..-++-....--+.....-++.   
         -+++++-.......--...+++++...-+++++++++.--++...--........--++++-..........-+++++.....-+++..-+---++...---+++++-...-++--+...--....-+++.   
          +##+++-.........++++++-...--++++++-...-++..--++++-.....---+++++++++++++++++++.....-+++-.+++--++.....----.....-++-.-+........-+++.    
           ###+++......--+++++++-.....-----....-+++..--++++++++-...-----+++++++#+-##++-.....-++++++++--+++............-+++..-+........-++-     
           .###+++......--+++++++..............+++...--++###++++++-......-++-.   +#++++++++++++++++++---+++-........-+++-..-++.......-+++.     
            .###+++.....--++++#+++...........-++++...--++..-+###++++++-..-++.    +######++++++##++#++-.---+++++-+++++++....-++.......-++.      
             -##+++-.....-++++##++++-.....-+++++++...--++.    .-####++++++++.           ...- .#####+++------+++++++#+++....-++......-++-       
              +##+++-....--+++-###+++++++++++++++++++++++.        .+####++++.                 .+.##++++++++++++. .##++++++++++.....-+++.       
               ###+++-....-+++. .+#######+-  ++++++###+.              .+####.                    .--+########++. .#####+++++++.....-++-        
                ###+++....--+++              ..          ........................       ..                 ...        ...+##++-...-+++         
                .###+++++++++++             .. .-+++###- +#--.......-##.........-##.   .+#+++###+   -++---...            .##+++++++++.         
                 .###+++++#+-.    ..  ++###++##-#.    .#++#.        .##           .#. .+#.     .#- .+# ..-+#+  .###+--.   .-+####+++-          
                  -###+.   ..--+##+#- +#     .###      +###.    #######     ##.    ##.+#.       +#..++    .#-  -#  ..-+##+.    ..+-.           
                       .+##+-.     -#.+#      .#+       ###.        .##     --     ####.   ..    ##.#-    -#. .+#        .+#+.                 
                        ##.      .-+####       .-  .    .##-        .##         .+####.    ##    -###     ##  -#.    .      +#                 
                        .#+    .+-. ####    #      -+    ###    .++++##     +    .###-            ###     ##+--#     ###     ##                
                        .##-        -###    ##     ##     ##         +#     #+     -+    ....     .#-     .-++#-    -###.    ##                
                         .##.   .+###+##    ##+    ##+    -#..---++++##+++++########+++++#####     #.        -#.    -#+.     ##                
                          +#-         +#    +##+++#####################################################++-.. ++             ##.                
                          .##.     .-+############+##-.... ##       .###.     +#.  .##+.     .####.--###########+-..    .-###.                 
                           +#+-+####+--##..  ##+   ##   -++##.  .#.  .#.  .-++##.  .#    -+.   -#-   .##.  -#- .++########-.                   
                           .##+-.      .##.  .#.  .##      -#.     .-###+-..   #.  .#   -###   .#.  . .#   ##.                                 
                                         +#.  .   ###.   +++#-   +   +#+      +#.  .#-        .#+   #. .  .#+                                  
                                          .#+     ###-      ++---##++++##+++####++++###+.. .-###-  .##.   .#.                                  
                                            ##.--+#+##########+++++++++++++++.+++++++.+################+-.##                                   
                                             +##+--..                                           ...-++#####+                                   

    )" << endl;
    
    
     cout << R"(
        

                                                                                  #.                                                           
                                                                  .        ###       ##  ##                                                    
                                                                  #            #+###       #                                                   
                                                          -###   # ###          #      ###.                                                    
                                                      ##        #           -+-#    ###   ###                                                  
                                                   #           ##                 ##           ###                                             
                                                #              #-                ##                +#-                                         
                                             .#       #      #  #                #                     #                                       
                                            #          #     #   #               #  #    #                #                                    
                                          +   #  #  .#    ###-     ###.           #     # #                 #                                  
                                                   -#            . #        .###   #     .    ###      ####  -#-     .                         
                                       # ##    #                 #-   #-         # #######  -  #  +  ##       #  ####                          
                                          .##                         ##+#       #       #.##    . ##           #     # #                      
                                     #     .#                                                 #.   #              #    #  #                    
                                   .#    ###                                                    ## #               #     # #                   
                                   #    #                                                         ##                 #    # #                  
                                   #   #                                                           + #                #    # #                 
                                + .   #                                                           #  #                 #    .                  
 ####                             #       #                                                         #                    #  # #                
      #                       #  #   #  #                                                         -     #                 ## #-                
 ##    ##                        #      #                                                          #-#   #                    ###              
  #      #                          -+ #                                                            . #   #    ####            #               
   ##     ##                       #    #                                                            + #   ####  #   ########## # #            
    ##      #                     -                                                                   # # #     #  ##          ### #           
     ##      ##                                              ##                                         ##     #  #-             #  #          
      ##       ##                                                ##                                    #      #  #                .            
       ++       ##                                           #      #                                #       ### #   -             #           
        ##        ##                                          #  # #   #                           ##       #  +#- -  #            #           
         ##        ##                                          #  -   #  #  -######               #        #    -##   #             #          
          ##         ##                                            ##############   ##+         ##        #        #  #              #         
           ##         ##                                       ####            ####    ##      #         #          # #              #         
            -#          #-                                  ###                  ###     #+ ###         #           #                ##        
             ##          ##                               ###                     ###   ## +#          #             -   -            #        
               #           #                             ##                  ##.   ###    #           # #            # # #            #        
               +#           #                          ##                 #     ######  ##           ## #            #  #                      
                 #           #                        ##               ##   ##      ## #            # #  -            # -   ######    #+       
                  #          #                      +#  #             #   #         ###            #   # #            # ###       #    #       
                   #         #                     ##    #    ####-     ##          #             ##   #  #           ##         +#+ + #       
                    #        #                    ##  ##  ##         -##          ##             #  ##  # +          #               # #       
                     #       #                   ##    #      #####              #              #     #    #    #  ###           -   # ##      
                      #      +#                 ##    #.   #                   ##              #       # # #   + #  #            #   # ##      
                       #      #                +#    #    #                   #              ##        #  # # # #   #            #   # +# #    
                        #      #               #    ##   #                  ##           ###           #  # + # #   #            #   # ###  #  
                         #      #             #    ##   #                 ##         ####         #    #   .  # -   +            #   # #  # #  
                          #      #           ##           #              ##       ###  ###      ##  ###   - ##     #             +   # #  # #  
                         ###      #       ##  ###     ##    #          ##       ##    ######    .    - #  # ## #   #            #    # #  # #  
                       #    #      ##   ##  #           #    #        #.     ###### ###     #####     ## # # # #   #            #   #  #  # #  
                     ##      #       ###  #              #    #     ##     ####   ####           #     ## - -  #  -#            #  #  ##  # #  
                     #   -    #       #  #                #   #    #      ##   #    ##           #     ###  # #   #             ##  ## #  # #  
                   #      #    #     ##  #                    #  ##     +#      #    ##           #     #   ##  # #     ####   #   #   -       
                         #      # ## #   #               #   #  #      ##        #    ##           # ##   #+   #  . ###      ####   ###  # #   
                 ##   #   #      ##  #   #              #  ####       ##          #   ###          ##         #- ###      ##          #- # #   
               ## # #     #     ##   ##   #           ##  #          #             #   ###           #            ##   .##             ## #    
               #          #    ##    ##     #      ##    ##          #              #   ###          ##             ##   #              #      
              #  #    #  #    #      # #                 #          #                 #  ###      #### ##        ##-      #              #     
               # +       ##-###      #  #              ##+          + #                 ########### #    ########          #             #     
                #######-      .#     #    ###+    ####  #       #  #  #                             # # #       ##          ##   -    ## #     
                               #     -                 #       ##     ##                  #         # ##           #          #  -      +#     
                                #     #                #      ##      # #                  #        # #             #           ##     .#      
                                #     #               #      -#       #  #                  ##        #              #            #    #  #    
                               # #                    #      ##       #   #                   ##      #           #  #            #   #     #  
                               # ##                  #     .###      #     #                    ##     ##       #       ##        #. #       # 
                              #  ##                 ##     ###     ##       #-                     ##+   #                    -  + ##   ##     
                              #  # #                #    ## ##    # ##        #                        ######   ##         ####               #
                               #  #-               #     .  #   +#    ##       #                               #                #         +  # 
                              # #   #              #   #####   ##       ##      #                                                 ###    # ##  
                             #   #   #            #   ##      ##          #      #                                                    ####-    
                            #    ##   +          #   #       #             ##     ##                                                           
                           #   ##  ##            #         ## #              ##     #                                                          
                         #    #      #                 ####  #                 #     #                                                         
                        #   #         #             ##    #  +                   #    #                                                        
                       #  #+          # #         # #   ### #                      #   ##                                                      
                         #             # #     ##   #    # #                        ##   -                                                     
                     # #                 # ####   #        -                          #  #                                                     
                    ##                  #          #   ## #                             ##                                                     
                                         # #         ##+ #                                                                                     
                                          #  #    +#   ##                                                                                      
                                             ########                                                                                          

    )" << endl;
    
}


void pokemonSampleArt() {
    cout << R"(                                      
                                                                           #################                                      
                                                                ######+++----++++++++++++++#                                      
                                                        #####++++++++++++++++++++++++++++++##                                     
                                                  ####+++++++++++++++++++++++++++++++++++++##                                     
                                              ###+-++++++++++++++++++++++++++++++++++++++++##                                     
                                           ##---+++++++++++++++++++++++++++++++++++++++++++#                                      
                                       ##+-+++++++++++++++++++++++++++++++##########++++++##                                      
                                   ###-+++++++++++++++++++++++++++####+++++++++++++++++++##                                       
                                ###--++++++++++++++++++++++####+++#+++++++++++++++++++++#                            ##           
                              ##--++++++++++++++++++++####++++++++#+++++++++++++++++++##                             ###          
                           ##---++++++++++++++++++###+++++++++++++#++++++++++++++++++#                               ####         
                         #+--++++++++++++++++++###++++++++++++++++#++++++++++++++++##                               #+####        
                       #---+++++++++++++++++##+++++++++++++++++++#++++++++++++++++#                              ###+++#-#        
                     #--+++++++++++++++++##+++###++++++++++++++++#+++++++++++++++#                          ####+++++++#-#        
                  ##+-++++++++++++++++##+++#+++++++####++++++++++#+++++++++++++##                      ###+++++++++++++#-+#       
                 #+-++++++++++++++++##+++++#++++++++++++++#####++#++++++++++++#                  ####-+++++++++++++++++#-+#       
               #--+++++++++++++++##++++++++#+++++++++++++++++++++++++++++++++#########     ####++++++++++++++++++++++++#-++#      
             ##-#++++++++++++++##++++++++++#++++++++++++++++++++++++++++++++#+++++++--###+++++++++++++++++++++++++++++#-+++#      
            ##-#+++++++++++++##++++++++++++#+++++++++++++++++++++++++++++++#+++++###+++++++++++++++++++++++++++++++++#--+++#      
           #-++++++++++++++##++++++++++++++#++++++++++++++++++++++++++++++####++++++++++++++++++++++++++++++++++++++#+-++++#      
          #-#++++++++++++##+++++++++++++++++#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#+-+++++#      
         #-#++++++++++++#+++++++++++++++++++#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#--++++++#      
        #+++++++++++++#+++###########+++++++#+++++++++++++++++++++++++++++++++++++++++++++#############++++++++++#--++++++##      
       ##-#++++++++++#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#++++++++++++++++++++##--+++++++##      
       #-#+++++++++##+++++++++++++++++++++++++++++++++++++#+++++++++++++++++++++++++++++++#+++++++++++++++++++#--+++++++++#       
      ##-#+++++++++#+++++++++++++++++++++++++++++++++++#++++++++++++++++++++++++++++++++++#+++++++++++++++++#---+++++++++#        
      #+++++++++++#+++++++++++++++++++++++++++++++++##++++++++++++++++++++++++++++++++++++#+++++++++++++++#+--++++++++++#         
      #-+++++++++##++++++++++++++++++++++++++++++##+++++++++++++++++++++++++++++++++++++++##++++++++++++#+-++++++++++++#          
      #--#+++++++#++++++++++++++++++++++++++++##+++++++++++++++++++++++++++++++++++++++++++#+++++++++##+++++++++++++++#           
      #--#+++++++#++++++++++++++++++++++++++#+++++++++++++++#++++####+-+++++++++++++++++++-#+++++++##+++++++++++++++##            
      #+--#+++++##+++++++++++++++++++++++##+++++++++++++++++#++++++++++++####++++++++++++++#++++##+++++++++++++++++#              
       #+--#++++#++++++++++++++++++++++#++++++++++++++++++++#++++++++++++++++++++####++++++###+++++++++++++++++++##               
       #+++--##++#+++++++++++++++++++#+++++++++++++++++++++#++++++++++++++++++++++++++++##+++++++++++++++++++++##                 
        #++++--####++++++++++++++++#+++++++++++++++++++++++#+++++++++++++++++++++++###++++++++++++++++++++++###                   
         #+++++----###++++++++++##+++++++++++++++++++++++++#+++++++++++++++++###+++++++++++++++++++++++++-###                     
          #++++++++-----+###+##++++++++++++++++++++++++++++#++++++++++####++++++++++++++++++++++++++++-##+++##                    
           ##+++++++++++-------++########+++++++++++++++++########++++++++++++++++++++++++++++++++++##++++++++##                  
             ##++++++++++++++++-----+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++##++++++++++++##                 
              ###++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++##+++++++++++++++-#                  
                 ##++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++##++++++++++++++######                   
                    ###+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++####++++++++++++++++#                         
                        #####++++++++++++++++++++++++++++++++++++++++++++++++++###+++++++++++++++++++++#                          
                               #####+++++++++++++++++++++++++++++-+++++#####++++++++++++++++++++++++++##                          
                                 +--#---+####################+-###-..#+++++++++++++++++++++++++++++++#                            
                                #----#+---++++++++++++++++++-.......+++++++++++++++++++++++++++++++++##                           
                                #-------#++++++++++++++++++++###.+##++++++++++++++++++++++++++++++++++#                           
                                #-----------##+++++++++++++++++++++####+++++++++++++++++++++++++++++++#                           
                                #..----------------+#####+-------------#++++++++++++++++++++++#+++++++#                           
                                #-.----------------------------------+#+++++++++++++++++++-##    #####                            
                                 #---------------------------------##+++++++++++++++++-###                                        
                                   #----------------------------##-+++++++++++++++++##                                            
                                     ##+------------------+### ##########+--++++++++#                                             
                                            ###########                   #--++++++##                                             
                                                                           #--++++##                                              
                                                                            #####                                                 
                                                                                         
    )" << endl;
}

void samplePokemon() {
    cout << R"(
        [INSERT SAMPLE POKEMON ASCII ART HERE]
    )" << endl;
}

vector <string> boat={
	"         |        ",
	"         |___     ",
	"    ____[:::]:_--|",
	"    [[::[   ]:   |",
	"    [[::[____----]",
	"    [[___######>>>"
};

vector<string> waves = {
   "~~~~~~~~~~~~~~~~~~~~~~~~~~~",
   "~~~~~~~~~~~~~~~~~~~~~~~~~~~"
};

void playBoatAnimation(const string& destination) {
    const int POSITIONS = 3;  
    const int DELAY = 1500;         // 1.5 seconds per position
    const int START_OFFSET = 5;     // Starting position from left edge
    const int MOVE_DISTANCE = 15;   // How far to shift each frame
    
    for (int frame = 0; frame < POSITIONS; frame++) {
        clearScreen();
        
        // Title message with ASCII box characters
		cout << "\n";
		cout << "  +============================================================+\n";
		cout << "  |           Traveling to " << destination << "...";
		
		// Pad to match box width
		int padding = 44 - destination.length() - 19;  // Changed from 18 to 19
		cout << string(padding, ' ') << "        |\n";
		cout << "  +============================================================+\n";
		
        // Calculate boat position
        int offset = START_OFFSET + (frame * MOVE_DISTANCE);
        
        // Draw alternating waves
        cout << string(offset, ' ') << waves[frame % 2] << "\n";
        
        // Draw boat (each line shifted by offset)
        for (const string& line : boat) {
            cout << string(offset, ' ') << line << "\n";
        }
        
        // More waves below
        cout << string(offset, ' ') << waves[(frame + 1) % 2] << "\n";
        
        Sleep(DELAY);
    }
    
    // Final arrival message
    showDialogue("Arrived at " + destination + "!");
}

vector<string> cableCar = {
    "  _____  ",
    "    |    ",
    "    |    ",
    " ####### ",
    "| |   | |",
    "| | @ | |",
    "---------"
};

void playCableCarAnimation(bool goingUp) {
    unsigned short positions = 7;
    const int DELAY = 800;
    if(!goingUp){
    	positions= 6;
	}
    
    for (int frame = 0; frame < positions; frame++) {
        clearScreen();
        
        cout << "\n";
        cout << "  +==============================================================+\n";
        if (goingUp) {
            cout << "  |            Ascending to MT. CHIMNEY...                       |\n";
        } else {
            cout << "  |            Descending to ROUTE 112...                        |\n";
        }
        cout << "  +==============================================================+\n\n";
        
        // Calculate cable car position
        int carCol, carRow;
        if (goingUp) {
            // Start bottom-right, move to top-left
            carCol = 55 - (frame * 8);
            carRow = 5 - (frame * 1); 
        } else {
            // Start top-left, move to bottom-right
            carCol = 15 + (frame * 8);
            carRow = 0 + (frame * 1);
        }
        
        vector<pair<int, int>> ashPositions;
        for (int i = 0; i < 40; i++) {  // ash particles
            ashPositions.push_back({rand() % 20, rand() % 68});  // Spread across rows
        }
        
        // Define pole positions - evenly scattered across the frame
        vector<int> poleColumns = {8, 18, 28, 38, 48, 58};
        
        // Draw the scene
        for (int row = 0; row < 20; row++) {  // More vertical space
            string line(70, ' ');
            
            // Draw cable line (diagonal from top-left to mid-right)
            if (row >= 1 && row <= 12) {
                int cableCol = 10 + ((row - 1) * 50 / 11);  // Adjusted slope
                if (cableCol < 70) {
                    line[cableCol] = '*';
                }
            }
            
            // Draw simple vertical poles from top to bottom
            for (int poleCol : poleColumns) {
                if (poleCol < 70 && line[poleCol] == ' ') {
                    line[poleCol] = '|';
                    if (poleCol + 1 < 70 && line[poleCol + 1] == ' ') {
                        line[poleCol + 1] = '|';
                    }
                }
            }
            
            // Add volcanic ash particles falling
            for (const auto& ash : ashPositions) {
                if (ash.first == row && ash.second < 70) {
                    if (line[ash.second] == ' ') {
                        line[ash.second] = '.';
                    }
                }
            }
            
            // Draw cable car at its current position (LAST so it's on top)
            if (row >= carRow && row < carRow + (int)cableCar.size()) {
                int carLineIndex = row - carRow;
                for (int i = 0; i < (int)cableCar[carLineIndex].length(); i++) {
                    if (carCol + i < 70 && carCol + i >= 0) {
                        char carChar = cableCar[carLineIndex][i];
                        if (carChar != ' ') {
                            line[carCol + i] = carChar;
                        }
                    }
                }
            }
            
            cout << "  " << line << "\n";
        }
        
        // mountain terrain
        cout << "     &    ||    &   ||      & ||      & ||        &|      & ||    "      << endl;
        cout << "    &&&   || & &&&  ||   & &&&||     &&&||      &&&&     &&&||    "     << endl;
        cout << "   #####& |#&&&#####|| #&&&#####   ######|    ########  #####|    "    << endl;
        cout << "  ####### ########### ########## ########|  ##################   #"   << endl;
        cout << " ############################################################# ###"   << endl;
        cout << "##################################################################"  << endl;
        
        Sleep(DELAY);
    }
    
    // Final arrival message
    clearScreen();
    if (goingUp) {
        showDialogue("Arrived at MT. CHIMNEY!");
    } else {
        showDialogue("Arrived at ROUTE 112!");
    }
}

void awakenGroudonFrame1(){
	cout << R"(
..........X.Xx;X$.X&........
.........&x$:x$Xx$x$........
.......:&$xx$xXx.xx$$:......
.....XxX&$$xxxxxxxx.&&x$....
.....X&$$&.+$xxxx..$$$&&....
....&xxx$x$$$xXX$$$$$xxxX:..
...&$XX$xX$.&$$$&$:$X$XX$&..
..xxx+;x$::X$$$$$XX$xx+xx$..
..&:+++.X;X$X$$X;:x$xxXxx:..
...$XXx:X.X;$$$$$;:X$x:;x&..
...;$+xX;X$$$$$$X$.X:X:+&...
...;$+$$$$$$$$&&$$XX.x++$:..
....&x:.$$&&&X..&&$$.$$.$:..
....+X+;&&..:.....&.$.$+;...
...................XX;xx....

)" << endl;
}

void awakenGroudonFrame2(){
	cout << R"(
..............&&..Xxx.;x&..X&............
.............;$x&xXxXx$xxX&x$+...........
.......:.....+$xX$+xX$Xxx$$x$X....:......
............&$$xx$;xx$xx.$xx$$&..........
..........$&&X$xxxX.xXx.Xxxx$+&&$........
........&xX$&&.$xxxxxxxxxxxX.$&&Xx&:.....
........&$$XX$$..$xxxxxxx$..$$XX$$&......
...:...&$XX$&$x$+++xxxxxx;+XxX&&XX$&.....
......&Xxxxx$$$$&$$xX$xx$$&&$X$xxxxX&....
.....&$$$xxX$$$$$&$$$$$$$&$x$X$Xxx$$$&...
...&xxxx.+xX$$$$$$$&&$&&$$$$$x.$$$$$$&...
..Xxxxxx;;xx$+::X$$$$$$$$XX$$Xxx;xxxx$+..
..X.xxxx$Xxx$XX;;;$$$$$$X::+$xx..xxxxx$..
...&;x:x.Xx;X::;$$$$$$$;XX$$$+..$$Xx..$..
....$X+XXx;;X;+++X$$$$$$$+::Xxxx.x:x+&...
.....&x:;X::;X$$$$$$$$$$x;;XXxxxXX.x$...:
.....&$$Xx..XX$$.$$$$$$$$$.X;::X;:x&.....
.....&$.x$$$x;$$XXX$&&&$XX$xxx.xx:x$&....
......&X.xx;.x$$$$$$&&&$$$$$$;$$$$:$&....
......;.$$.$$;$&...........&$+.$$.X$x....
......;;&+;X&x.........::..&.$$.$$:&....:
..........X................X;;+:+X:&.....
...:.........................X&;&.;......



)" << endl;
}

void awakenGroudonFrame3(){
	cout << R"(
......................;;....&&x:..:;&&....;;:......:...........
......................&x&..&Xxx$X;&xxx&..&x&;..................
...:.............:...&$xx$&$xxxX$&Xxxx$&$xxX&+.............:...
.....................&$xx$++xxxX$$Xxxx++$xxX&+.................
................:....&$xx$$.xxxX$$Xxxx.$$xxX&+.................
..................:&$$$xxx$$:+xxXXxxx.$$xxxX$$&x..:............
:................;&$$$$xxxxXX;;xXXxx.XXxxxxX$$$$X..............
.............;+X$$$$$.$Xxxxxx:;xxxxx.xxxxxX$:x$$$$$++..........
............&XxxX$&&&$.$xxxxxxxxxxxxxxxxxx$.x&&&$Xxxx&.........
.............$&XxxXXX&$..$xxxxxxxxxxxxxx$..X&XXXxxx&&..........
............&&&&$$$$$&$..;;$xxxxxxxxxx$;;..X&$$$$$$&&&.........
....:......:&&&&&&$$&Xx$.+..+xxxxxxxxx..+.$xX&&$&&&&&&:........
..........&xxxxxxXX.$xxx$$$.XxXXxXx$x$.$$$xxx$+;Xxxxxxx&.......
.........&$$xxxxxxx$$x$$&&$$$$$XxXx$$$$$&&$$X$$Xxxxxxx$$&....:.
........x&$$$XxxxX$$$x$$.&&$$$$$$$$$$$$&&.$$X$$$Xxxxx$$$&+.....
.......&$$$$$XxX$$$$$x$$..&&&$$$$$$$$$&&..$$X$$$$$Xxx$$$$&.....
.....&xxxxxx..+xxX$$$$$$$$$$$&&&$$$&&$$$$$$$$;.+$$$$$$$$$&.....
....&xxxxxxxx:+xxxX$X;;;X$$$$$$$$$$$$$$$$$$$$XXXx:.++xxxX&.....
....&xxxxxxx:::+xxx$X::::x&$$$$$$$$$$$$xxxX$XXxx+:xxxxxxxx&:...
....&.xxxxxxX$Xxxxx$$XX;;;;X$$$$$$$$$X::::XXXxx;...xxxxxxx&:...
.....&x.$.$x.+xx+..X:::X$$XX$$$$$$$X;;;;XX$$XxxxX$Xxxxxxx.&:...
.....&X;x.xx.;xXXX+X:::;+X$$$$$$$$$X+XXXXXX+;::;;+$$$XX.;xx:...
...:..&xx..$$:+XXXXXXX;;;X$$$$$$$$$$$$$X:::x:+xxx+.x$.x.x&.....
.......&$X::;xX+:::X$$$$XXX$$$$$$$$$$X;;:::xX$$$x..xx.xx$&.....
........:&x;;xXX+;;;X$$$$$$$$$$$$$$$X;;xx$$X+::++:XXxxXX&......
........&$$X+;+xx++++X$$;x$$$$$$$$$$XXX$x;$x:::;Xx;::X$X.......
........&$$$:+XX$$X.$$$$$..$$$$$$$$$$$$..$X;;;+XXx;;X$$&.......
........&$$.X$$$$$$$.$$$$$$$$$&&&&&&$$$$$XXXX;.;x;x..$$$&......
........+&$x.:x$$X;.+$$$$&&&&&&$$$$$&&&&$$$$:;XXXX$$+.$$&......
.........&$...........$&$$$$$$&&&&&&$$$$&$$$:xX$$$$$X.$$&..::..
..........&.X$$+;$$$.$$&....:............&$$$;.;$$X..$$&.......
.........:&;;x+;;;x;;;&&....:............&$.X$$+.:$$$.$&.......
..........xxxxx++xxxxx...................x&;X$$x;+$$X;&........
...............xX..:.:....................&;:+X+::XX:;&.....:..
...........................................&+$x;::;&;&.........
............................................;::x$X;.;..........

)" << endl;
}

void awakenGroudonFrame4(){
	cout << R"(
..................:...................&&&.......$&&:.................................
.............................+&$x+..:+$Xx$X+.;+$xx$+;..;+$&&.........................
............:................+&xxx&;:&xxxx$&.X&xxxx&X.&$xx$&.........................
............................&&$xxx$&&$xxxxX$&$$xxxxX$&$XxxX$&:.......................
............................&&$xxx$;:XxxxxX$$$$xxxxX+:$XxxX$&:............:..........
...............:............&&$xxx$$$.xxxxX$$$$xxxx.;$$XxxX$&:.......................
..........................;&$$$xxxxX$$.:xxxx$Xxxx+.$$$xxxxX$$&$......................
.........................&&$$$$xxxxX$$.:xxxx$Xxxx+.$$$xxxxX$$$$&.....................
:......................:x$$$$$$xxxxxx$+;;xxx$Xxx+;;XXxxxxxX$$$$&+;...................
.....:...............&&&&&&$.+$xxxxxxxx+.xxxxxxx::xxxxxxxx$$.$&&&&&&+.:.:.......:....
..................&&&xxxxxX$.+$$Xxxxxxx+.xxxxxxx::xxxxxxx$$$.$$xxxxx$&&;.............
................+&xxxXX$$$&&$+.XXXXxxxxxxxxxxxxxxxxxxxXXXX;.$&&$$$$XXxx&&:...........
............:...:;&&&XXXxxXX&$x.:;$Xxxxxxxxxxxxxxxxxxx$x;.+x&XXxxxXX$&&+;............
................+&$$$$$$XXXX&$$....X$xxxxxxxxxxxxxxxX$....x$&XXXXX$$$$$&&............
................+&&&&$$$$$$$&$$...+::$xxxxxxxxxxxxx$x.+:..x$&$$$$$$$&&&&&............
......:........xX&$$$$$$&&&&XXx$x++;:.xxxxxxxxxxxxx..:+++$XxX&&&&$$$$$$$$x:..........
.....:.......;&xxxxxxxxx$:;$xxxxX$$$X.XXx$Xx$xx$xx$.;$$$$xxxx$X.$Xxxxxxxxx&$.........
............$&$xxxxxxxxxxX$$xX$$$$$$$$$Xx$Xx$xx$xx$$$$$$$$$Xx$$$xxxxxxxxxxX$&........
............$&$$XXxxxxxxXX$$xX$$$X&&$$$$$$Xx$Xx$$$$$$$&$X$$Xx$$$XXxxxxxXX$$&&.......:
..........:&$$$$$$xxxxxx$$$$xX$$+.&&&$$$$$$$$$$$$$$$&&&+.$$Xx$$$$Xxxxxx$$$$$$&x......
.........X&$$$$$$$xxx$$$$$$$xX$$+..X&&$$$$$$$$$$$$$&&&...$$Xx$$$$$$$Xxx$$$$$$&x......
........&Xxxx+....$$$$$$$:..$$$$$$..:$&&$$$$$$$$$&&$x..+$$$$$:..$$$$$$$$$$$$$&x......
.....XX$xxxxxxxxx+.;xxxx$$$$XXXX$$$$$$$&&&&$$$$&&&$$$$$$$$$$$xxx$$$;;;;xXXXX$&x......
.....&$xxxxxxxxxxx.;xxxxxXXX:::;+X$$$$$$$$$$$$$$$$$$$$$$$$$$$xxxx+..;xxxxxxxx&x......
.....&$xxxxxxxxx+....xxxxXXX::::::X$&$$$$$$$$$$$$$$$XXXXX$$$$xxxx+.xxxxxxxxxxx$&.....
.....&X+xxxxxxxxX$$XxxxxxXXX;;;:::;+x$&&&&&&&&&&&&$+;::::XXXxxxx+;.+xxxxxxxxxx$&.....
.....&&&...xXXXX$$$+....xXXXXXXxxxx;;X$$$$$$$$$$Xx;::::::XXXxxxxxxXXXxxxxxxxxx$&.....
......x&x:.$::$x+.xxxx:..xx::::X$$$XXX$$$$$$$$$X;;;;;;XXX$$XxxxxxX$$Xxxxxxxxx.+&.....
......x&x;:x::xx+.XXxxXX:xx::::;+x$$$$$$$$$$$$$X++XXXXXXXXXXx......$$$$Xxx...:+&.....
......:+&Xxx:.:XXx.+XXXXXXx++;:;;;X$$$$$$$$$$$$$$$$$$X;;;;++.++xxxx;+X$+;X.:x&x......
........&$$$XXXXXX.;X:::;x+..;XX+;;xX$$$$$$$$$$$$$$X+;::::+X$$XxxX$.;xx:.xxxx&x......
.........X&$Xx:::;XXX::::xX$$$$$XXXXX$$$$$$$$$$$$$X;;;::::+X$$$$$+..;xx:.xxX$&x......
..........:x$X;::;XXX;;::;;X$$$$$$$$$$$$$$$$$$$$$X+;;++xXXXx++++X+.XXX;;;XX$&........
.::.......:&$$$X+;;+X$XXX;;;XX$$+.$$$$$$$$$$$$$$XXXXX$$+.$Xx::::XXX;:::xX$&$.........
..........:&$$$$XX;+X....xXXXX$$$$.X$$$$$$$$$$$$$$$$$$.+$X+;::::XXX;:::xX&:..........
..........:&$$$x+.XXX$$$$::x$$$$$$...XX$$$$$$$$$$$XX+.:x$x;;;;+xXXx;;;XX$$&$;........
..........:&$$$.:x$$$$$$$x+.$$$$$$xxxxxX$&&&&&&&$$xxxx$XXxxxX;:.X+;x;::x$$$$&........
..........:&$$$...$$$$$$$:..$$$$$$&&&&&&&$$$$$$$&&&&&&$$$$$X..:$XXX$$$..:$$$&........
...:........$&$$x..+$$$X..:$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$X.X$$$$$$$$$::$$$&........
............$&$.:++:....++;.+x$&&&&&&&&&&xxxxxxx$&&&&&&&$$$X.;+$$$$$$$+::$$$&........
.............;&.;$$$$.x$$$X.$$$&x.....:................x&$$$$:..$$$$;..X$$&$.........
.............;&XXX$XXXXX$XXXXXX&+......................x&$;.............:$&$.........
.........:...;&;;+X+;;;;X++;+x$$+......................x&$;.$$$$...$$$$::&$x.........
:.........:..::&X::x&;X$::+&::............:............::&XxX$$$xxx$$$Xxx&:..........
.....................&+..................................&x;:xXX:::XXX::;&:..........
.......................:.:.......................:.......&x;:xx;:::;+X::;&:..........
..........................................................;+X+xX;;:Xx+Xx+............
...............................:................................&&&.........:........

)" << endl;
}

static void blueOrbActivation() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Phase 1: Groudon starts glowing blue
    for (int i = 0; i < 3; i++) {
        clearScreen();
        SetConsoleTextAttribute(hConsole, ConsoleColors::FG_BLUE);
        awakenGroudonFrame4();
        Sleep(150);
        
        clearScreen();
        SetConsoleTextAttribute(hConsole, ConsoleColors::FG_RED);
        awakenGroudonFrame4();
        Sleep(100);
    }
    
    // Phase 2: Blue energy starts spreading
    clearScreen();
    SetConsoleTextAttribute(hConsole, ConsoleColors::BG_BLUE | ConsoleColors::FG_BLUE);
    awakenGroudonFrame4();
    Sleep(300);
    
    // Phase 3: Blue energy intensifies
    clearScreen();
    SetConsoleTextAttribute(hConsole, ConsoleColors::BG_BLUE_BRIGHT | ConsoleColors::FG_CYAN);
    awakenGroudonFrame4();
    Sleep(300);
    
    // Phase 4: Full blue takeover - Groudon glowing bright white/cyan
    clearScreen();
    SetConsoleTextAttribute(hConsole, ConsoleColors::BG_BLUE_BRIGHT | ConsoleColors::FG_WHITE);
    awakenGroudonFrame4();
    Sleep(400);
    
    // Reset to default colors
    SetConsoleTextAttribute(hConsole, ConsoleColors::FG_DEFAULT);
}

void awakenKyogreFrame1(){
	cout << R"(                      
              R0ah1zwu    
            S3333W1Shh    
      554W  13XXX1XXXwb   
   iiW51whh33XXXXXXXXhhh  
  hywwww1hh333X1U1TWSA    
    wwwwwh333WXhX     4A  
   a3311XAXXXXhXXXS       
  w33UZXC   RXW3XGHWX     
   61Vl     3 7ESWaRa     
            0a 0    E     
                          
)" << endl;
}

void awakenKyogreFrame2(){
	cout << R"(                                       
                           KR1ccccn    xZ       
                        Yhc1111111K     oZ      
                       R1111111XSSYS0oooF       
                      A1111111SSWWWWWWWA        
         c7q  hhhhhLK AX11SSSWSSYWWWWWn   nZ    
      KhlheOSWggZ1ZZc3ZEXSWWWWRZSUUYYYSoq   Y   
     YhhS55SY    xooY111XWYWWWWWWSWWWWWFdoooA   
    A5         oZZRoY11111YQSYLLLLYYYVTL4n      
                 to5Z11111YWTRAAAAADLLYCp  m    
      x          xoX111112WW7dF       ADZRooR   
        AARooooooYX111XSWWWWo7WA         RoR    
      1ch1111WWFFWYWWWWWWWooTWWFLK         A    
    Ve01VWY1ZWRVSA AAE00ogRWWWWWRWWWNK          
   Z S111S2YYWWWA      KWWLR1XWAQXJKWXA         
   AAt RWYWWVWF       QWJKXXARXAY7KW47A         
     Y3AnxKloA        U4AKY7NKI XcV YcT         
           ZA         RcJ KccA  Wc7AK7cA        
                      KT4A YET                                                                                           
)" << endl;
}

void awakenKyogreFrame3(){
	cout << R"(                                  
                                                  XXXXXXXX            
                                        LN11ccccc0        Z           
                                    RTcggghhhh111Wl        8Y         
                                  Lch1111111111111SAo     ooZ         
                                 J11111111111XXSSS21LoooooRA          
                                 P1111111111SSSWWWWWWWWALA            
                                 D11111XXXWWUSYWWWWWWWWQUJZZZ         
              c11   ehhhhhhhLL   B1111SSSSWWSSSRWWWWWWWX      ZY      
          LghhhhZSSZhhhhhhffcccZUKW1TSQWWWQQYWSWSSSZZZZXAor     lY    
         7hhhh7ATdAASSYYooooYY1111LWTYWWWWWWWWWYYXTWWWWWAooooooooA    
       LcYSYw            soooYY11111XSSWWWWWWWWWTWLLLLLLLLL2LAAA      
      JSY            o    AooYY111111SYSSZWWPLLLLMYLLLLLLLLZZ         
      o                oooAoYY11111111YWWVZAZYYYTTLTZLLVYX    Z       
       o                  ooY111111111YWWVZLAAAAAAAALLLZYKoo   b      
         z               uooZ11111111YWWWXoZE           AAYLIooooA    
          Zo           o ooY11111111UWWWWXoZWA            AAIoAAA     
            YooAARoooooYYX111SSYYZWWWWWWooZWWLA             AoooA     
          11cAA111WWAAAWWWWYWWWWWWWWWWWoolWWWWA               AoA     
       1ch1111VY111WWYUWWAAAOWWWWWWooooLWWWWWWLSWWWLK                 
      Jeh1VVSSZ11ZWWSZWWA      AAAAiXWWWWWWWWWWWWWWWWWM               
     Z AY1111SZYYYYWWWWA          LWWWLL111WWAAU1WA QWWB              
    Lo iPMQXZWSWWXWWWWL          NWWRLWWXAV1WA X1W  U15B              
       Z voAAYWWWWYWA           LXW1AL111A LXA Xc2L KcccA             
        Y5A   lCXsoA            L46A LT86V LH  XccD KcccA             
            YAA ZxA             LccLK Lcc5     KcccAWWcccA            
                                VVccE  5c4QA    O8cA LcJcA            
                                 TAWO  LcAcA                          
                                    A                                                                                                               
)" << endl;
}

void awakenKyogreFrame4(){
	cout << R"(                                                                   
                                                                  7777777777                
                                                     WX99chhhhhhh7          r6              
                                                WWhhhhiqqqqqa9999A            6             
                                              Wghpp000099999999999A          sn6            
                                             Wndd99999999999999555WArrwwwwwwrrjJ            
                                            W9999999999999988WW22289R1rrrrrrrZA             
                                           W999999999999994WW668833333GGeejXA               
                                           A999999999999981WW33333333333333A                
                                           A999999993333334W6333333333333W777776            
                  hh99     hqqqqqqqq5WW    A399999WWWWW134WWW7333333333337       l76        
              VWqvqqqqqqqqqqqqqqqqqqjhhhh3AAW993WW73333473WWW333333333333Arr       t7       
            VjqvvqqqWFFFXW7hhhhX777777hh9999WA93W733333333337WW77777777777Arrrrw   trA      
          Vaqqqqhh7WXvAvXW7     rrrrrr77999999W33W3333333333337733333333333ArrrrrrrrrA      
         V4h0W67                 xfrrr779999999933W73333333333337WWWWWWWWWWWWW7VAAAA        
         XZm                r     j8rr77999999999W73WW7333YWWWWWW77WWWWWWWWWWW76            
         v                   7rrrr58rr7999999999997333UAK77ZWWWWW7X77XX777XWA   s6          
         w                     9KKYnr04999999999997333372MFTYYYWWWWWY5555WZZAv    f6        
         ry                       rri699999999999773334f5VGAAAAAAAAAJJJJW123MZt    u6       
          qx                     tvra999999999999163334pb3IA            AANO3ZMdrx   U      
            6k                  w rpc99999999999X733334rk5YFA               AUXCG6kkkA      
              67rr           rrrrri48999999999W7333333rrk53GA                 KrrrNA        
                7rrrAAArrrrrrrr7659999WWW777333333333rrn533XC                  AXrrrA       
             99hhAA99999333AAA333336433333333333333rrrq63333D                    AerA       
          8choq99999WWW7777XX331AAM6433333333333rrrrr7333333D333WWW                         
         ahlf99999W7999993333733WA   AAAA3rrrrrrrrW3333333333W3333333WW                     
         3k993W77979999333X73331A             AA333333333333333333333333W                   
       7 wK899999W7977777333333A             W3333WWW999733LFW3933F W3339F                  
      7r  aAV997733733X7333333A            W33333W3333GY973LDW999F  W9999F                  
       DDIl  sH33373333Y7333B             W2333H W9393FV195LDWg9gV  24g0gF                  
         5ewtZEVaKI333IIK3IA              W7571R W9999F V25LDWehehK  WhhhhF                 
          5c7K 7  r1Q0zrrA                Wc9cF  V3ccc7FUZOE 36hhhO  WhhhhF                 
               6yeeA6y rA                 WahaJV  WahafF      WhhhhS XdhhhPE                
                      7A                  W9hh3F   WhhhbF     3hh9hF  Wh9hhF                
                                           Whhh9F  99h4fZF     WhFhF  WhWFhF                
                                           WhF3hF   WhG17F                                  
                                               FF                                                                                                                                        
)" << endl;
}

static void redOrbActivation() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Phase 1: Red orb flashing - red overlay alternating with blue Kyogre
    for (int i = 0; i < 3; i++) {
        // Red overlay flash
        SetConsoleTextAttribute(hConsole, ConsoleColors::BG_RED | ConsoleColors::FG_RED);
        system("cls");
        awakenKyogreFrame4();
        Sleep(150);
        
        // Blue Kyogre (normal) 
        SetConsoleTextAttribute(hConsole, ConsoleColors::FG_BLUE);
        system("cls");
        awakenKyogreFrame4();
        Sleep(100);
    }
    
    // Phase 2: Red energy starts spreading
    SetConsoleTextAttribute(hConsole, ConsoleColors::BG_RED | ConsoleColors::FG_RED);
    system("cls");
    awakenKyogreFrame4();
    Sleep(300);
    
    // Phase 3: Red energy intensifies
    SetConsoleTextAttribute(hConsole, ConsoleColors::BG_RED_BRIGHT | ConsoleColors::FG_YELLOW);
    system("cls");
    awakenKyogreFrame4();
    Sleep(300);
    
    // Phase 4: Full red takeover - Kyogre glowing bright white/yellow
    SetConsoleTextAttribute(hConsole, ConsoleColors::BG_RED_BRIGHT | ConsoleColors::FG_WHITE);
    system("cls");
    awakenKyogreFrame4();
    Sleep(400);
    
    // Reset to default colors
    SetConsoleTextAttribute(hConsole, ConsoleColors::FG_DEFAULT);
    system("cls");
}

static void playAwakenGroudon() {
   
    clearScreen();
    awakenGroudonFrame1();
    Sleep(1000);
    
    clearScreen();
    awakenGroudonFrame2();
    Sleep(1000);
    
    clearScreen();
    awakenGroudonFrame3();
    Sleep(1000);
    

    clearScreen();
    awakenGroudonFrame4();
    Sleep(1000);
    
    blueOrbActivation();
    
    flashTransition(2, 50);
    
    clearScreen();
    Sleep(300);
}

static void playAwakenKyogre() {
   
    clearScreen();
    awakenKyogreFrame1();
    Sleep(1000);
    
    clearScreen();
    awakenKyogreFrame2();
    Sleep(1000);
    
    clearScreen();
    awakenKyogreFrame3();
    Sleep(1000);
    

    clearScreen();
    awakenKyogreFrame4();
    Sleep(1000);
    
    redOrbActivation();
    
    flashTransition(2, 50);
    
    clearScreen();
    Sleep(300);
}

void groudonKyogreFrame1(){
	cout << R"(                                                                                                                                 
                            +-+++-                                                   
                           ++++++-.-                                                 
                         +#+-++++#+.-                                                
                         ....--+#++++-+                                              
                       -++---+++++...-++                                             
                     -.....++++++++.-.++                                             
                   +.....-++++...-+-                                                 
                  #+---+##+++#--  +-                                                 
               #-.....-+++--...-                                                     
             .+--....-#+++++.-                                                       
             -+-....-+++-.-+++-                                                      
             #-.....#+++-++#++++.+.+             --.----+-                           
            +......-+#+-++++++++++-++#+++     --++----+-----                         
           -++++---++#-++++-+#+++-#++++++.   ......+--++......    +----+             
           -+++--++###-+++   -+++-#+++-+#++  ........----+....-++++-+++              
          +++++-++-+++++...-   ++++#+-++...  ............--+-..+++++++               
          ++++-+++-++-+#        #++++-+++-+---.............--++++++++                
         +#++++++++####...          -+++++--++..............++++--+++                
         ++++-++++++++++           +-+++++++++..+-..........++++++++---                                                                                                                         
)" << endl;
}

void groudonKyogreFrame2(){
	cout << R"(                                                                                                                                                                               
                           -+++--                                                    
                          -+-+++.-                                                   
                        ++++++-++-.-                                                 
                       -....-+#+++#+-+                                               
                      ---.-+++-++...-+                                               
                    .....-+++++#+.--+--                                              
                 +-.....-++-....+-+                                                  
                 ###+###++++-.  --+                                                  
             -.......++++-...                                                        
           -++#.....#+++++-..                                                        
           --+.....#+++.-+++-                             -                          
           ++.....+++++-++++++.+.--                +-.-----                          
           -.....-+++++-+++++++++#++##          --+---+-------                       
          --++++-+-+++++++++#+-+#++++++.       ......-----......-   -+--+-           
         +--+++++++#++++   ++++-#++++##        ........+-++--...-++--+++-            
         +++++++---+++-..--- ++++++++--..      -...........-+--..-+-++++-            
        -+++++++--+++#  ++-+  +++++++++       --.............--++-+++++-             
        #+++++++-####-....+++     ++++---+----++..............+++++++++              
       ++++++++++++++#+  ++++        --++++++++-..--..........+++++++-----                                                                                                                         
)" << endl;
}

void groudonKyogreFrame3(){
	cout << R"(                                                                                                                                                                               
.....................................................................................
...........................#-+#-#....................................................
...........................#+#####...................................................
..........................##########.................................................
.........................#############...............................................
.......................######-#########..............................................
.....................###################.............................................
.................+##########+##########..............................................
..................###################................................................
...............+###########-###+.-##.................................................
.............##.######-########+.....................................................
...........###############.###-......................................................
...........###+######-###+####...........................+###-.......................
...........##+################-######+##...........##+######+#+......................
..........+##-######.##################-.........####-######+####-.......++..........
..........###################.######+#####......##################++###-####.........
.........##########-########+###########-.......####+#############-########..........
........####################-+############-......##+#############+########...........
........##########-########+#+..########.....-###################-#######............
........#####################+........++#############-####################...........
.......######################+.......-##############-########################........                                                                                                                      
)" << endl;
}

void groudonKyogreFrame4(){
	cout << R"(                                                                                                                                                                                                                  
                          -+++++-                                                    
                          ++++++-.                                                   
                        +-++++++#+-.                                                 
                       -....--++-++#-+                                               
                     +-##+-++++#+...-++                                              
                   -.....-++--+#+--+.-+-                                             
                 --....-+++-....--+                                                  
                -+....++#++#     --                                                  
             -+-.....+++++....                                                       
            -+#.....++-+-++.                                                         
           --#.....++++..++-+                                                        
           #+.....+-++++++++++-.-.-+                -----+-                          
           -.....--+++++++++++++++++##+         -------+--+--                        
          +-++++-+-+++++++++++++###++++..     .....-------.....     ++--             
         +-++++-+++#+++     ++++-#++++#+      ........+#+--.....----+++++            
         +++++++-+--++-..-   +++++#++-+..-     ........-..-+--...+++++++             
        -+++++++-++-++          +++++#.       -............----+-++++++              
        ##+++++++#+##-...           -++-+-----+-.............-+--++++++              
       ++++++++++++++#               --+-++++++..............-+-+++++----                                                                                                                                     
)" << endl;
}

void drawRainOverlay(int density = 30) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    
    // Rain characters
    const char rainChars[] = {'\'', '`', '.', ',', '|'};
    int numChars = sizeof(rainChars) / sizeof(rainChars[0]);
    
    // Draw random rain drops across the screen
    for (int i = 0; i < (width * height * density / 100); i++) {
        COORD pos;
        pos.X = rand() % width;
        pos.Y = rand() % height;
        
        SetConsoleCursorPosition(hConsole, pos);
        cout << rainChars[rand() % numChars];
    }
    
    // Reset cursor to bottom
    COORD bottomPos;
    bottomPos.X = 0;
    bottomPos.Y = height - 1;
    SetConsoleCursorPosition(hConsole, bottomPos);
}

void playGroudonVsKyogreAnimation() {
    // FRAME 1 - Initial clash with light rain
    system("cls");
    groudonKyogreFrame1();
    drawRainOverlay(15);
    Sleep(800);
    
     // Thunder flash effect
    for (int i = 0; i < 2; i++) {
        system("color F0"); // White flash
        Sleep(60);
        system("color 07"); // Back to normal
        Sleep(60);
    }
        
    // FRAME 2 - Intensifying with heavier rain
    system("cls");
    groudonKyogreFrame2();
    drawRainOverlay(25);
    Sleep(800);
    
    // FRAME 3 
    system("cls");
    groudonKyogreFrame3();
    drawRainOverlay(35);
    Sleep(300);
    
    // Thunder flash effect
    for (int i = 0; i < 2; i++) {
        system("color F0");
        Sleep(60);
        system("color 07"); // Back to normal
        Sleep(60);
    }
    
    // FRAME 4 
    system("cls");
    groudonKyogreFrame4();
    drawRainOverlay(50);
    Sleep(300);
    
    // Power clash - fast alternating flashes
    for (int i = 0; i < 4; i++) {
        system("color 4E"); // Red background
        Sleep(80);
        system("color 1B"); // Blue background
        Sleep(80);
    }
    
   
    
    // Back to normal, show final frame with rain
    system("color 07");
    system("cls");
    groudonKyogreFrame4();
    drawRainOverlay(50);
    Sleep(80);
    
    for (int i = 0; i < 8; i++) {
        system("color 4E"); // Red background
        Sleep(80);
        system("color 1B"); // Blue background
        Sleep(80);
    }
    
    flashTransition(2, 100);
    system("cls");
}


void rayquazaDescendsFrame1(){
	cout << R"(                                                                                                                                                                                                                                                    
                                  --                  +                                       
                                   --               --+                                       
                                    ---           ---+                                        
                                     +---       ----+                                         
                                      ++---------++                                           
                                         +-....+                                              
                                         +-...--+                                             
                                       -++-..--++-                                            
                                     --+ +---+--  +-                                          
                                    -    +-++++-+   +                                         
                                         +-++++-+                                             
                                         -+++++-+        --                                   
                          -+--  -++-     -+-+++-+ +      ++                                   
                     --.....-+-.-----+- ---#-------    +---+-                                 
                  --........---+------++ ++++------+    --++                                  
                 -........-----#-------- -++++-------+  --+                                   
                ---.----------##++-------++++++----+++-+#+                                    
                -.--.----++++#+++++++--+   +++++++++   +                                      
               +.----.--+##++++++++++++--  +##+++##+                                          
               +-------++#+++++++++++++++----..---+# ++++      ----                           
               +-++++--++ ++++++++++++++++-------+++-----+--+-------.-                                                                                                                                                     
)" << endl;
}

void rayquazaDescendsFrame2(){
	cout << R"(                      
                                         .        .                                           
                                          .      -.                                           
                                           .-..--.                                            
                                            ---#.                                             
                                          .-.-+- .                                            
                                            .--+                                              
                                    ....#-+ .###+. .-.                                        
                                 -.--.-+---#.++--. .+.                                        
                                -----+-###.--##++#.+.                                         
                                --+--#++++##..#--#                                            
                                -+-++.##+++##+++#+----..--.                                   
                                .+-###. .  -....#---.###----                                  
                                 --#-+++-#      ---##++##+--.                                 
                                 -+++++--++   .-###++##+ +-+.                                 
                                 #+++++###.-.-----#+-+.  -+-                                  
                                  .----#---------.      .+-.                                  
                                       .--++--.       .+--                                    
                                                   .--+++                                     
                                                .-++++-+                                      
                                                  +-++.                                                                                                                                                                                              
)" << endl;
}

void rayquazaDescendsFrame3(){
	cout << R"(                                                      
                                         .        .                                           
                                          .      -.                                           
                                           .-..--.                                            
                                            ---#.                                             
                                          .-.-+- .                                            
                                            .--+                                              
                                    ....#-+ .###+. .-.                                        
                                 -.--.-+---#.++--. .+.                                        
                                -----+-###.--##++#.+.                                         
                                --+--#++++##..#--#                                            
                                -+-++.##+++##+++#+----..--.                                   
                                .+-###. .  -....#---.###----                                  
                                 --#-+++-#      ---##++##+--.                                 
                                 -+++++--++   .-###++##+ +-+.                                 
                                 #+++++###.-.-----#+-+.  -+-                                  
                                  .----#---------.      .+-.                                  
                                       .--++--.       .+--                                    
                                                   .--+++                                     
                                                .-++++-+                                      
                                                  +-++.                                                                                                                                                                                                                     
)" << endl;
}

void rayquazaDescendsFrame4(){
	cout << R"(
   ...-... .-..  .....  ......--+++-. ..-+..++-.-+++##########+++-.-++..+-.. .-+++--......  .....  .
  .... . .....  ....   ..-...--..--  ..-++-..+++##################+++..-++-..  --..--...-..   ....  
.....    ...- ..........-. ....+.-..---...--. -+-++++++####++++++-+- .--...---..--+.... .-..........
... ..   ........ ......   ...----..+++.   .--.  ....----.---....  .-..   .++-..----..   ....... ...
.  ..... ...-....  ...--..   .  ..++...--++++--....-++-.  .-++-....--++++--...++.. ..   ..--...  ...
   .....  .. .... ..   ....----++.  -  ...    .---+++--....-++++---.    -.. .-  .++----....   .. ...
   .. ...  .......-..     ... .....-...-----++..  .-.. ---- ..-   ..+------...-..... ...     .--....
   . ...     ..   .-..-...  . . .   ...  ......-.++---.----.---++.-......  ..    .   .  ...-..-.  ..
        .....      .....-.............-..     ..-.--++--..--++--.-..     ..-.............-.....     
          ......  .     . .......----...-.....-.... ..      .. ....-....--...----....... .     .  ..
          ... ...   ......  .    ...... . .......-..--.    .--.......... . .....        ......   ...
              ..............        ...... .. ...................... .. ......       ...............
              ...    .. .... ...................  ..  ..  ..  ..  ................... ...  ..    ...
                               ....  ....  ...........................   ....   ...                 
                                                  ... .....  ...                                    
                                                                                                    
                                                                                                    
.... . ..  . ... .....  ... . ....  .. . .....  ..  . ....  ..    ...  ....  ....  .. .  . . . .... 
....................................................................................................
....................................................................................................
....................................................................................................
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------                                                                                                                                                                                                                     
)" << endl;
}

void rayquazaDescendsFrame5(){
	cout << R"(
  ....-.....-.. ..-..   ......------..--+-.++++-+#################-++++-.+--. .--.--.......   .-.   
  .... . .....  ....  ...-....--.--  .--++-..+######################+...+++-.. .+..+-........ ..-.. 
.....    ...- .... ....... ....+--...-....-++-+####################+..+-....--..-.+-...  ....... ...
......   ........  .....    ..----..----. .-+##++++++########++++++-+-.  ..+--...---...   .....  ...
.  ....  ........  ....--.   .. ..-+....-#########+#####++#####+###++++-++....+-.. ...   .--...   ..
   .....  .. .... ..   ....-----+.  .. .-#+++++#########++#########+.   .... -. .++----....   ......
   .. ...  .......--.      .. .....-..-########++++###+####+###+++++++-----...--.... ...     ..--...
   . ...     ..   .-..-...  . . ..  .+-++++++#######################+....   ..     . .  ...-..-.   .
        .....      .....-..........--###++++++++#+##############+##+-.   ..--..................     
         .......  .     . .......-+#######+++###+++++#++++++##++++##+....-...----....... .     .  .-
           .. ...   .....  ..   .-+++++++++++##+######++++++######+++... . ......        ......  ...
             ...............  ..--++++++++++++++++++++++++++++++++++--   .....        ..............
              ...    .. .... .................... ..           .  ................... ....  .    ...
                .              ....  .....  ........................... .....   ...    .            
                                                  .... .. .. ...                                    
                                                                                                    
                                                                                                    
                                                                                                    
....................................................................................................
....................................................................................................
....................................................................................................
-.---.---.--.--.------.--.--.--..----.--.--.--.---.--.---.-----.--.--.----.-----.----.----.---.----.
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------                                                                                                                                                                                                      
)" << endl;
}

void rayquazaDescendsFrame6(){
	cout << R"(
   ...-... ..-.  ...... .......--++-. ..-+..++--.+###############+..-++..+-.  .-++--....... ......  
   .... . ..... ..-.    ......--..-.  ..+++..-#####################++...+++..  .-..--......    .-.. 
......    .... ... .......  ...-+.-.----...++-+#####################. -+...---..+.+-...  .-.........
.......   ...............   ...---..-+++. .-+##++++#+#########+#+++--+..  .+++-..---...   ..........
.   ....  ...-...   ...--.   ..  .-+- ..-+########+#####+++#####+#+++-++++-.. .+-.  ..   .--...   ..
     ....  . ..... ..  .....----+-. .. .-++++++##########+##########-.  .-.. -.  -+----.....   . ...
    . ....  .. ...--..     ........-...-#######++++###++###++###++++++-----....-........     ..--...
   .. ...    ...  ..-.....  ..       -+++++++##############+########+-...   ..       .. ......-..  .
          ...  .    .....-..........-+##++++++++##+################+-.   ................-.....    .
         . ......  .     . .......+########+++###++++#+++++++#+++++#+-. .--..----....... .     .  ..
            ......  ......      .-+++++++++#++########+++++++######++-... .......        ......  ...
              ...............  .--++++++++++++++++++++++++++++++++++--.  .....        ..............
               ...    .  .....-+++###+++++++###++++++++++++++++++++++-............... ....  .    ...
                           ..--++++++++++++++++####++#########++###++-.  ....   ...                 
                          ..---+++++++++++++++++++++++++++++++++++++--.                             
                         .---+-+-+-++-+-+-+-++-+-+--+-+-++--+--+-++---.                             
                       .---+++++++++++++++++++++++++++++++++++++++++--.                             
                      .--++++++++++++++++++++++++++++++++++++++++++++-..                            
.....................-++++++++++++++++++++++++++++++++++++++++++++++++-.............................
...................-+++#############################################++-.............................
.................-++#################################################++-............................
----------------++####################################################+-----------------------------
---------------+#######################################################+----------------------------
-------------+#########################################################+----------------------------
-----------+###########################################################++---------------------------                                                                                                                                                                     
)" << endl;
}

void rayquazaDescendsFrame7(){
	cout << R"(
+++.-+++++++++++++++++++-------------.---++-........--++-.....-#####+++++#+++++++++++##+......
++++++++++++++++++++++-+------------+-..++++-.......-++#..+++++++++-+####+####-.....-##++.....
++++++++++++++++++++--+-----------+.--...+++++.....++++++++++-----+++##++++#.........-++-.....
+++++++--...-+++++--------------+.-+.....-+++....-++++-------++++++++#+++-+#-.--+++++#+-....--
++++++-....-++++--------------+--+........++#.+--#++#+++++++++++++++...-+++-..+-#++++++....---
+++++.....-++++-------------+--+-..........#++++++++++++++++#-....++--++++.....++++++-#..-----
++++-..--++++-------------.-++-............-++-++++++++#+......-+++#+++#-......+-+.++-.-------
+++++++++++---------------..............-++++-+#++++++#-.......--+##++#-.............-.-------
+++++++++---------------................++++-+#++++++-+++++.....+.##--+............-.---------
+++++++-+--+----------..................++++++#++#+++..--++++......--............-------------
+++++-+-----+-+----.-...................+++++++####++..........................-------------+-
++++-------+.+---.-....................-++++++++##+++-........................-------------+-+
++-------+.-+--.-......................+-++-...###+++-......................-------------++-++
--------+++--.-........................-+++-...-+++++.....................-------------++--+++
-------------...................................---.....................--------------+--+++++
-----------...........................................................-.--------------++++++++
---------...........................................................---------------+++++++++++
--------.........--...-............................................------------+-+++++++++++++
----.-.........--....-...........................................---------------++++++++++++++
--.-..........-.....-..........................................-.------------+++++++++++++++++
--...........--..--..........................................-.-------------++++++++++++++++++
..............--...........................................-.------+.-----++++++++++++++++++++
.........................................................--------+.-+---++++++++++++++++++++++
.......................................................-----------+---++++++++++-.++++++++++++                                                                                                                    
)" << endl;
}

void rayquazaDescendsFrame8(){
	cout << R"(
++++++++++++++++++++++++---------------...............+++++++++++++#+++#+--+++++-.............
++++++++++++++++++++++--+------------..............-++++++++++++++++++++#++++++...............
+++++++++++--++++++++-+-----------.-........-+-..++++++++++++++++++++++#+#+++.................
++++++++-...-++++++-------+-+---.-.........+++..++++++++++++++++###+##+++#...................-
++++++-.....-++++-------+.-++++-..........--++.....-++++++++++#+-++##++++..................-+.
+++++-....--++++------+--+----++-.........-++-......+####+++++#+++++---++#++-............-+--+
+++++...--+++--+----+-.+----.--+++.......-+++..--+++++++-+#+#####+#+...--#++++.........-.-++--
++++++++++++-+------.+----....+++++.....++++++++++++---+++##++++#-........--+-.......-.-------
++++++++++-------------.-......+++....-+-++-------++++++++##++-+++..+++++#+++......-----------
++++++++-------------.-........-++.---+-+#+++++++++++++##++-.+++-..+.#+#+++#......------------
++++++-------------.-...........+++++-++++++++++++#-...--..#++#.....++++++-+....--------------
+++++--------------.............-+++-++++++++#-......+++#+++#+......+-+.+-+...--------------++
++---------------............--++-+++++++++#........--+###+#-...............-.-------------+++
--+------------..............--+++++#++++-+++++-.....+-##+-#-.............-.-------------+++++
-----++.+---.-...............--++++#++#++++.-++++#-.....++-.............--------------++++++++
----+.-+--.-.................+++-+++####++.............................--------+----++++++++++
--+--+--.-...........--......+++++##++##+++..........................-------+-.+---+++++++++++
------.-.........--...-.....+-++#...+##++++........................-.-----+-.++--+++++++++++++
------.........--....--.....-++#+...-++#+#.......................-.---------+---++++++++++.-++
----..........--....--........-......-#+-......................-.-------------++++++++++--++++
-.-...........-...--.........................................---------------++++++++++--++++++
-..........................................................---------------++++++++++--++++++++
..........................................................-------------+++++++++++++++++++++++
........................................................--------------++++++++++++++++++++++++                                                                                                                                        
)" << endl;
}

void rayquazaDescendsFrame9(){
	cout << R"(
-----------------------------.-............+++#-........-.........----------------------------
-----------------------------................-+#........-........-----------------------------
-----------------------------..-..............+++...............-.----------------------------
----------------------------...-..............-++-..............-.----------------------------
----------------------------...-..............-+++.-............-.----------------------------
----------------------------...-........-....-+-+++.............-.----------------------------
+++++++++++++++-------------...-.............++-++++............-..------------+++++++++++++++
+++++++++++++++-------------...-.............-###+#-............-..------------+++++++++++++++
+++++++++++++++-------------...-.....-....-+-+--++..-+-.........-.-------------+++++++++++++++
+++++++++++++++-------------...-.....-+-+-#+-++#---++-..........-.--------------++++++++++++++
++++++++++++++--------------...-.....-+-+++#+-++-++.......-.....-.-------------+++++++++++++++
++++++++++++++++-----------....-....-+++++++-.+++++-....--......-..-----------+-++++++++++++++
++++++++++++++-+-----------....-....+-+#-+-++++++#+++-.++--.....-..-------------++++++++++++++
++++++++++++++----------------------+-++---+#+##++#+##+#+----------------------+++++++++++++++
+++++++++++++++-------------+--------+++#+------++++---------------------------+-+++++++++++++
+++++++++++++++-----------++#+##-----++-+++#----++++----------------------------++++++++++++++
++++++++++++++--------+##++######++---+++++#+++++++-----------------------------++++++++++++++
++++++++++++++------####+-##--######-----+++-+++--------------------------------++++++++++++++
++++++++++++++----#####-++++#+#+#+----------------------------------------------++++++++++++++
+++++++++++++++-+##+####++---##+--------------------------------+++------------+++++++++++++++
++++++++++++++++##++####+###------------------------------+++-+----++++##--+#####+++++++++++++
+++++++++++++++#####-+++####-+-----------------------------++#+---+++++++##++###+#++++++++++++
#########+++++#####++++++#+-++#---------------------------+-----++++++++#++#####+++++#########
#########+++++######++++#-++--------------------------------+--++++++#+#++-++###+++++###+#####                                                                                                                
)" << endl;
}


void rayquazaDescendsFrame10(){
	cout << R"(
+++++++++++++++++++++++++++#######+######+#########-+#######++######++++++++++++++++++++++++++
++++++++++++++++++++++++++++####+########+++#####-+-##########++###+++++++++++++++++++++++++++
++++++++++++++++++++++++++++###############++-+-++##############+##+++++++++++++++++++++++++++
+++++++++++++++++++++++++++##+##############+++--################+##++++++++++++++++++++++++++
+++++++++++++++++++++++++++#++#############+--+---################+#++++++++++++++++++++++++++
+++++++++++++++++++++++++++#+############-+##-----+-################++++++++++++++++++++++++++
++++++++++++++++++++++++++#+##########+--+--##++---###+-+##########+#+++++++++++++++++++++++++
++++++++++++++++++++++++++#+########++--++--------------###########+#+++++++++++++++++++++++++
++++++++++++++++++++++++++#+#######+++------##-+-+---##############+#+++++++++++++++++++++++++
++++++++++++++++++++++++++#+######-++-#--++####+------+############+#+++++++++++++++++++++++++
++++++++++++++++++++++++++#+#######+--+-++---++++-##+-+-------#####+#+++++++++++++++++++++++++
+++++++++++++++++++++++++###+########-+++----+-+########++----+#######++++++++++++++++++++++++
+++++++++++++++++++++++++###++########+---+#######################+###++++++++++++++++++++++++
+++++++++++++++++++++++++####+###################################+####++++++++++++++++++++++++
--------------++++++++++++####+#################################+#####++++++++++--------------
-------------+-++++++++++---+##++#############################++#####++++++++++++++-----------
-------------+++++++-----------+##+#########################++#######+++++++++++++------------
--------------+++-----#--++------++++++++++++++++++++++++++++++++++++++++++++++++-+-----------
-----------++++-----#++++---++-++++++++++++++++++++++++++++++++++++++++++++++++++++-----------
-------------+--------++++---+++++++++++++++++++++++++++++++++++++++++++++++++++++------------
--------------------------+++++++++++++++++++++++++++++++++++++-+++---------++++--------------
-----------+-----#++-----+-+++++++++++++++++++++++++++++++++++----+++++-----------------------
------------------++----#--++++++++++++++++++++++++++++++++++++++++---------------------------
------------------++++++++-+++++++++++++++++++++++++++++++++++++++++---------+------+---------                                                                                     
)" << endl;
}

void rayquazaDescendsFrame11(){
	cout << R"(
++++++++++++++++++++++++++###############+#########-+################+++++++++++++++++++++++++
++++++++++++++++++++++++++###############+++#####-+-################++++++++++++++++++++++++++
++++++++++++++++++++++++++++###############++-+-++##################++++++++++++++++++++++++++
+++++++++++++++++++++++++++#################+++--###################++++++++++++++++++++++++++
+++++++++++++++++++++++++++################+--+---##################++++++++++++++++++++++++++
+++++++++++++++++++++++++++##############-+##-----+-################++++++++++++++++++++++++++
++++++++++++++++++++++++++#############--+--##++---###+-+############+++++++++++++++++++++++++
++++++++++++++++++++++++++##########++--++--------------#############+++++++++++++++++++++++++
++++++++++++++++++++++++++#########+++------##-+-+---################+++++++++++++++++++++++++
++++++++++++++++++++++++++########-++-#--+#####+------################++++++++++++++++++++++++
++++++++++++++++++++++++++#########+--+-++---++++-##+-+-------#######+++++++++++++++++++++++++
+++++++++++++++++++++++++############-+++----+-+########++----+#######++++++++++++++++++++++++
+++++++++++++++++++++++++#############+---+###########################++++++++++++++++++++++++
++++++++++++++++++++++++###############################################+++++++++++++++++++++++
--------------++++++++++#+#############################################+++++++++--------------
-------------+-++++++++++---+##########################################++++++++++++-----------
-------------+++++++------..---+############################+##########+++++++++++------------
--------------+++-----#--+#------###+#####################+#+#########+++++++++++-+-----------
-----------++++-----#++++---++-+#########+##############+#############+++++++++++++-----------
-------------+--------++++---+++++++++++++++#######+++++++++++++++++++++++++++++++------------
--------------------------+++++++++++++++++++++++++++++++++++++-+++---------++++--------------
-----------+-----#++-----+-+++++++++++++++++++++++++++++++++++----+###+-----------------------
------------------++----#--++++++++++++++++++++++++++++++++++++#+#+---------------------------
------------------++++++++-+++++++++++++++++++++++++++++++++++++++++---------+------+---------                                                                        
)" << endl;
}

void rayquazaDescendsFrame12(){
	cout << R"(
+++++++++++++++++##+++###################+#########-+###################++++++++++++++++++++++
+++++++++++++++++++++++#+################+++#####-+-####################++++++++++++++++++++++
+++++++++++++++++++++++#+##################++-+-++####################++++++++++++++++++++++++
+++++++++++++++++++++++++###################+++--#####################++++++++++++++++++++++++
+++++++++++++++++++++++++##################+--+---####################++++++++++++++++++++++++
+++++++++++++++++++++++++################-+##-----+-##################++++++++++++++++++++++++
++++++++++++++++++++++++++#############--+--##++---###+-+#############++++++++++++++++++++++++
++++++++++++++++++++++++++##########++--++---.---..-----#############+++++++++++++++++++++++++
+++++++++++++++++++++++++##########+++-..---##-+-+---#################++++++++++++++++++++++++
+++++++++++++++++++++++++#########-++-#--+#####-----.-################++++++++++++++++++++++++
+++++++++++++++++++++++++##########+--+-++---++++-##+-+-------########++++++++++++++++++++++++
++++++++++++++++++++++++#############-+++----+-+########++.---+########+++++++++++++++++++++++
++++++++++++++++++++++++##############+---+############################+++++++++++++++++++++++
+++++++++++++++++++++++#################################################++++++++++++++++++++++
--------------+++++++++##+##############################################++++++++--------------
-------------+-++++++++##---############################################++++++++++------------
-------------+++++++-.----..---+#########################################+++++++++------------
--------------+++-----#--+#--.---#######################################+++++++++-+-----------
-----------++++-----##+++---++-##########################################++++++++++-----------
-------------+--------++##-.-############################################+++++++++------------
--------------------------+###################################+-+++---------++++--------------
-----------+--.--#++-----+-+#################################+----####+-----------------------
------------------++----#--++###++++++++++++++++++++++++++++######+------------.--------------
------------------++++++++-##++++++++++++++++++++++++++++++++##+###+---------+------+---------                                                          
)" << endl;
}

void rayquazaDescendsFrame13(){
	cout << R"(
###################################################-##########################################
##########################################++#####--+##########################################
############################################--# -#############################################
##############################################+-.#############################################
############################################--+-- ############################################
#########################################+###. ---# ##########################################
#######################################- +-+###+.-.####-######################################
######################################-.---.  --.  .- --######################################
###################################---   . -##-- - - #########################################
################################## #--#  ######-  -  -########################################
####################################------. -#+-#+###-#-++..- ################################
##################################### +#-----#+########### -  ################################
##############+#######################+   ####################################################
################################################################################+#############
++++++++++++++#++##############################################################+++#+++++++++++
++++++++++++---++########.  ##################################################++--++++++++++++
++++++++++++++-++###           ###############################################++---+++++++++++
----------------+     #  ##      ###########################################+#+---------------
---------------     ##+##   +#.#############################################++#---------------
...........---       -####   ###############################################+++-----..........
..........---            -#####################################-+#--------. ++-+   --.........
          --     ###     #+###################################   +#####----.-  .-.-..         
          -      .##    #+-######################################## +------- ..     -.        
         --      .####+###+#########################################. .----  #-.    --     -.                                                 
)" << endl;
}

static void rayquazaAura() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Phase 1: Quick green flashes
    for (int i = 0; i < 2; i++) {
        // Bright green flash
        SetConsoleTextAttribute(hConsole, ConsoleColors::BG_GREEN | ConsoleColors::FG_GREEN);
        system("cls");
        rayquazaDescendsFrame13();
        Sleep(80);
        
        // Normal Rayquaza
        SetConsoleTextAttribute(hConsole, ConsoleColors::FG_GREEN);
        system("cls");
        rayquazaDescendsFrame13();
        Sleep(60);
    }
    
    SetConsoleTextAttribute(hConsole, ConsoleColors::BG_GREEN | ConsoleColors::FG_YELLOW);
    system("cls");
    rayquazaDescendsFrame13();
    Sleep(300);
    
    SetConsoleTextAttribute(hConsole, ConsoleColors::BG_GREEN | ConsoleColors::FG_WHITE);
    system("cls");
    rayquazaDescendsFrame13();
    Sleep(400);
    
    // Reset to default colors
    SetConsoleTextAttribute(hConsole, ConsoleColors::FG_DEFAULT);
    system("cls");
}

void playTrioEventAnimation() {
    // Frames 1-9: Sequential animation
    clearScreen();
    rayquazaDescendsFrame1();
    Sleep(1000);
    
    clearScreen();
    rayquazaDescendsFrame2();
    Sleep(1000);
    
    clearScreen();
    rayquazaDescendsFrame3();
    Sleep(1000);
    
    clearScreen();
    rayquazaDescendsFrame4();
    Sleep(1000);
    
    clearScreen();
    rayquazaDescendsFrame5();
    Sleep(1000);
    
    clearScreen();
    rayquazaDescendsFrame6();
    Sleep(1000);
    
    clearScreen();
    rayquazaDescendsFrame7();
    Sleep(1000);
    
    clearScreen();
    rayquazaDescendsFrame8();
    Sleep(1000);
    
    clearScreen();
    rayquazaDescendsFrame9();
    Sleep(1000);
    
    // Frames 10-13: With quick light green flashes
    for (int frame = 10; frame <= 13; frame++) {
        clearScreen();
        
        // Display the frame
        if (frame == 10) rayquazaDescendsFrame10();
        else if (frame == 11) rayquazaDescendsFrame11();
        else if (frame == 12) rayquazaDescendsFrame12();
        else if (frame == 13) rayquazaDescendsFrame13();
        
        Sleep(800);
        
        // Quick double flash for each frame
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        for (int flash = 0; flash < 2; flash++) {
            system("color A0"); // Light green background
            Sleep(80);
            system("color 07"); // Back to normal
            Sleep(60);
        }
    }
    
    rayquazaAura();
    
    // Flash transition
    flashTransition(2, 50);
    
    clearScreen();
    Sleep(300);
}

static void playEndCredits() {
    clearScreen();
    
    vector<string> credits = {
        "POKEMON HOENN ADVENTURE",
        "",
        "A TERMINAL-BASED RPG",
        "",
        "",
        "",
        "",
        "GAME DESIGN",
        "SELWYN ANDRAE LATOG",
        "LATOG SELWYN",
        "SELWYN",
        "TOG",
        "",
        "",
        "",
        "PROGRAMMING",
        "SELWYN MAKABUGTO",
        "PRRT PRRT SKRRT SKRRT"
        "",
        "",
        "",
        "MAP DESIGN",
        "SELWYN JAPON"
        "",
        "",
        "",
        "BATTLE SYSTEM",
        "SI LATOG"
        "",
        "",
        "",
        "DIALOGUE SYSTEM",
        "ANDRAE LATOG",
        "",
        "",
        "",
        "UI/UX DESIGN",
        "ROWENA ASENCIA",
        "ASENCIA ROWENA",
        "DORA",
        "",
        "",
        "",
        "DEBUGGING",
        "SELWYN + AI",
        "",
        "",
        "",
        "COFFEE CONSUMER",
        "SELWYN LATOG",
        "",
        "",
        "",
        "PROFESSIONAL PROMPT ENGINEER",
        "SELWYYYNNN ANDRAEEE LATOOOG",
        "",
        "",
        "",
        "PPT MAKER",
        "ROWENA A. ASENCIA",
        "ASENCIA A. ROWENA",
        "ASENCIA ROWENA A.",
        "ROWENA",
        "A.",
        "ASENCIA",
        "",
        "",
        "",
        "",
        "SPECIAL THANKS TO",
        "SIR PAU",
        "MYSELF",
        "FOR NOT GIVING UP",
        "",
        "",
        "",
        "BASED ON",
        "Pokemon Emerald",
        "by Game Freak",
        "",
        "",
        "",
        "ALSO THANKS TO",
        "Bulbapedia",
        "Claude",
        "My Keyboard",
        "My Mouse",
        "ChatGPT",
        "Nescafe Premium Stick",
        "Kiss niya",
        "",
        "",
        "",
        "",
        "CONGRATULATIONS!",
        "",
        "YOU COMPLETED THE GAME!",
        "",
        "",
        "",
        "",
        "",
        "THANK YOU FOR PLAYING!",
        "",
        "",
        "",
        "THE END",
        "",
        "",
        "",
        "",
        ""
    };
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Scrolling credits animation
    const int visibleLines = 20;
    const int scrollSpeed = 150; // milliseconds per scroll
    
    for (size_t startLine = 0; startLine < credits.size() + visibleLines; startLine++) {
        clearScreen();
        
        // Display visible window of credits
        for (int i = 0; i < visibleLines; i++) {
            int creditsIndex = (int)startLine - (visibleLines - i);
            
            if (creditsIndex >= 0 && creditsIndex < (int)credits.size()) {
                int padding = (50 - credits[creditsIndex].length()) / 2;
                if (padding < 0) padding = 0;
                
                cout << string(padding, ' ') << credits[creditsIndex] << "\n";
            } else {
                cout << "\n";
            }
        }
        
        Sleep(scrollSpeed);
        
    }

}

#endif //POKEMON_DISPLAY_H