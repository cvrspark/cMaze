#include "main.hh"

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::vector;
using std::string;
using std::cout;
using std::endl;

int mapWidth = 40;
int mapHeight = 25;
int botSpeed = 2;


void setupConsoleWindow() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { (SHORT)(mapWidth * 2), (SHORT)(mapHeight + 5) };
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    SMALL_RECT windowSize = { 0, 0, (SHORT)(mapWidth * 2 - 1), (SHORT)(mapHeight + 2) };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
#endif
}

void showStartMenu() {
    int sizeSelection = 1;
    int difficultySelection = 1;
    int activeRow = 0;

    while (true) {
        #ifdef _WIN32
        cout << "\x1B[H";
        #endif
        cout << "========================================================\n";
        cout << "               🏁 cRace SETUP 🏁             \n";
        cout << "========================================================\n\n";
        cout << " Use W/S to move rows, A/D to change choices, Enter to select.\n\n";

        if (activeRow == 0) cout << " > "; else cout << "   ";
        cout << "MAP SIZE:     ";
        if (sizeSelection == 0) cout << "[ SMALL (25x10) ]  Medium            Large          \n";
        else if (sizeSelection == 1) cout << "  Small          [ MEDIUM (40/25) ]  Large          \n";
        else cout << "  Small            Medium          [ LARGE (60x45) ]\n";

        cout << "\n";

        if (activeRow == 1) cout << " > "; else cout << "   ";
        cout << "DIFFICULTY:   ";
        if (difficultySelection == 0) cout << "[ EASY ]      Normal        Hard  \n";
        else if (difficultySelection == 1) cout << "  Easy       [ NORMAL ]     Hard  \n";
        else cout << "  Easy        Normal       [ HARD ]\n";

        cout << "\n\n";

        if (activeRow == 2) {
            cout << "              >> [ START GAME ] << \n";
        } else {
            cout << "                 [ START GAME ]    \n";
        }
        cout << "\n========================================================\n";

        char key = _getch();
        if (key == 'w' || key == 'W') {
            activeRow = (activeRow - 1 + 3) % 3;
        }
        if (key == 's' || key == 'S') {
            activeRow = (activeRow + 1) % 3;
        }
        if (key == 'a' || key == 'A') {
            if (activeRow == 0) sizeSelection = (sizeSelection - 1 + 3) % 3;
            if (activeRow == 1) difficultySelection = (difficultySelection - 1 + 3) % 3;
        }
        if (key == 'd' || key == 'D') {
            if (activeRow == 0) sizeSelection = (sizeSelection + 1) % 3;
            if (activeRow == 1) difficultySelection = (difficultySelection + 1) % 3;
        }
        if (key == 13) {
            if (activeRow == 2) {
                if (sizeSelection == 0) { mapWidth = 25; mapHeight = 10; }
                else if (sizeSelection == 1) { mapWidth = 40; mapHeight = 25; }
                else { mapWidth = 60; mapHeight = 45; }

                if (difficultySelection == 0) botSpeed = 4;
                else if (difficultySelection == 1) botSpeed = 2;
                else botSpeed = 1;

                break;
            }
        }
    }
}

bool askRestartMenu(string winner) {
    int selection = 0; 
    while (true) {
        #ifdef _WIN32
        cout << "\x1B[H";
        #endif
        cout << "========================================================\n";
        if (!winner.empty()) {
            cout << " 🏁 RACE FINISHED! Winner: " << winner << " \n";
        } else {
            cout << " ⏸️  GAME PAUSED \n";
        }
        cout << "========================================================\n\n";
        cout << "What would you like to do next?\n\n";

        if (selection == 0) {
            cout << "  > [ RESTART WITH NEW MAP ] <      [ QUIT GAME ]  \n";
        } else {
            cout << "    [ RESTART WITH NEW MAP ]    > [ QUIT GAME ] <  \n";
        }
        cout << "\n(Use A/D to navigate selection, press Enter to select)\n";

        char key = _getch();
        if (key == 'a' || key == 'A') selection = 0; 
        if (key == 'd' || key == 'D') selection = 1; 
        if (key == 13) { 
            return (selection == 0); 
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    #ifdef _WIN32
    system("chcp 65001 > nul");
    system("cls");
    #endif

    showStartMenu();
    setupConsoleWindow();

    Map gameMap(mapWidth, mapHeight);
    Screen screen(Dimension{mapWidth, mapHeight});

    bool keepPlaying = true;

    while (keepPlaying) {
        Entity player(Position{1.0, 1.0}, 1.0, "Player", "🔴", false);
        vector<Entity> bots = {
            Entity(Position{1.0, 3.0}, 1.0, "Bot Alpha", "🟦", true),
            Entity(Position{3.0, 1.0}, 1.0, "Bot Beta",  "🟨", true),
            Entity(Position{3.0, 1.0}, 1.0, "Bot Delta", "🟩", true)
        };

        bool raceRunning = true;
        string winnerName = "";
        int botTickCounter = 0;

        #ifdef _WIN32
        system("cls");
        #endif

        while (raceRunning) {
            screen.clear();
            screen.drawMap(gameMap);

            char currentDir = ' ';
            
            #ifdef _WIN32
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { 
                raceRunning = false; winnerName = ""; break; 
            }
            if (GetAsyncKeyState('W') & 0x8000) currentDir = 'w';
            else if (GetAsyncKeyState('S') & 0x8000) currentDir = 's';
            else if (GetAsyncKeyState('A') & 0x8000) currentDir = 'a';
            else if (GetAsyncKeyState('D') & 0x8000) currentDir = 'd';
            #else
            if (_kbhit()) {
                char ch = _getch();
                if (ch == 27) { raceRunning = false; winnerName = ""; break; }
                if (ch == 'w' || ch == 'W' || ch == 's' || ch == 'S' || 
                    ch == 'a' || ch == 'A' || ch == 'd' || ch == 'D') {
                    currentDir = ch;
                }
            }
            #endif

            if (currentDir != ' ') {
                player.moveControlled(currentDir, gameMap);
            }

            if (gameMap.getTile(static_cast<int>(player.getPos().x), static_cast<int>(player.getPos().y)) == 2) {
                winnerName = player.getModel() + " (YOU)";
                raceRunning = false;
            }

            botTickCounter++;
            for (auto& bot : bots) {
                if (botTickCounter % botSpeed == 0) { 
                    bot.updateBotAI(gameMap);
                }
                if (gameMap.getTile(static_cast<int>(bot.getPos().x), static_cast<int>(bot.getPos().y)) == 2) {
                    winnerName = bot.getModel();
                    raceRunning = false;
                }
            }

            screen.setPixel(static_cast<int>(player.getPos().x), static_cast<int>(player.getPos().y), player.getAvatar());
            for (const auto& bot : bots) {
                screen.setPixel(static_cast<int>(bot.getPos().x), static_cast<int>(bot.getPos().y), bot.getAvatar());
            }

            screen.render();
            sleep_for(milliseconds(45)); 
        }

        #ifdef _WIN32
        system("cls");
        #endif
        
        keepPlaying = askRestartMenu(winnerName);
        
        if (keepPlaying) {
            #ifdef _WIN32
            system("cls");
            #endif
            showStartMenu();
            setupConsoleWindow();
            gameMap = Map(mapWidth, mapHeight);
            screen = Screen(Dimension{mapWidth, mapHeight});
        }
    }

    cout << "\nThanks for playing!\n";
    return 0;
}