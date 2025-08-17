#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <windows.h>
#include <vector>
#include "Interface.h"

using namespace std;
HANDLE fcolor = GetStdHandle(STD_OUTPUT_HANDLE); // Handle for colors
int x, y; // Board dimensions
vector <Vampire> Vamp;
vector <Werewolf> Wolf;

Object **CreateGrid() { // Creates a x*y grid (and each tile's adjacency array)
    Object **g=new Object*[x];
    for (int i = 0; i < x; i++) // Creates the 2D array of type Object
        g[i] = new Object[y];

    for (int i = 0; i < x; i++) { // Traversal of 2d array, tested, works, maybe need to update obj.p(x,y) later
        for (int j = 0; j < y; j++) {
            g[i][j].T = 'E'; // Initializes the array cells as empty.
            g[i][j].p.X = i; // Initialize coord positions
            g[i][j].p.Y = j;
        }
    }
    return g;
}

void CalculateAdj(Object** Gr, Object Entity, char* Adj) { //an Array of objects [8]
    int ex = Entity.p.X, ey = Entity.p.Y;
    int c = 0; // Counter for array
    for (int j = -1; j < 2; j++) {
        for (int i = -1; i < 2; i++) {
            if (!((ex + i) < 0 || (ex + i) == x || (ey + j) < 0 || (ey + j) == y || (i == 0 && j == 0))) { // If out of bounds skip next steps
                Adj[c] = Gr[ex + i][ey + j].T;            }
            else {
                Adj[c] = 'N';
            }
            if (!(i == 0 && j == 0)) // Target is not current entity
                c++; // Increase 
        }
    }
}

void PrettyPrint(Object **G, char time) { // INSERT A VARIABLE FOR DAY VS NIGHT, COLOR 8 FOR NIGHT, COLOR 15 FOR DAY
    system("cls");
    int dn=time=='D'?15:8; // Day/Night
    SetConsoleTextAttribute(fcolor, dn); // Set colour day/night
    cout << "     ";
    for (int j = 0; j < x; j++) {
        if (j<9)
            cout << j+1 << "   ";
        else
            cout << '\b'<<j + 1 << "   ";
        }
    cout << endl;
    for (int i = 0; i < y; i++) {
        cout << "   +";
        for (int j = 0; j < x; j++) {
            cout << "---+";
        }
        cout << endl;
        if (i < 9)
            cout << " " << i + 1 << " | ";
        else
            cout << i + 1 << " | ";
        char type;
        int colour; //colour
        for (int j = 0; j < x; j++) {
            // Function to declare type to be printed, 'space for empty"...
            type = G[j][i].T;
            switch (type) {
                case 'A': 
                    colour = 14;
                    break;
                case 'V':
                    colour = 4;
                    break;
                case 'W':
                    colour = 5;
                    break;
                case 'T':
                    colour = 2;
                    break;
                case 'L':
                    colour = 11;
                    break;
                case 'P':
                    colour = 15;
                    break;
                default:
                    type = ' ';
                    colour = dn;
            }
            SetConsoleTextAttribute(fcolor, colour);
            cout << type;
            SetConsoleTextAttribute(fcolor, dn); 
            cout<<" | ";
        }
        cout << endl;
    }
    cout << "   +";
    for (int i = 0; i < x; i++)
        cout << "---+";
    cout << endl;
}

void DelayPrint(std::string s, int cc) { //cc for color code
    SetConsoleTextAttribute(fcolor, cc);
    for (int i = 0; i < size(s); i++) {
        cout << s[i];
        Sleep(33); // Ta xronia tou Ihsou
    }
}

void PrintMM() {
    SetConsoleTextAttribute(fcolor, 14);
    cout << R"(
o0<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>0o
%                               __  __   _   ___ _  _   __  __ ___ _  _ _   _                                  %
%                              |  \/  | /_\ |_ _| \| | |  \/  | __| \| | | | |                                 %
%                              | |\/| |/ _ \ | || .` | | |\/| | _|| .` | |_| |                                 %
%                              |_|  |_/_/ \_\___|_|\_| |_|  |_|___|_|\_|\___/                                  %
%-----------------------------------------------------+--------------------------------------------------------%
%                   o0<><><><><>0o                    |         __   ___   __  __ ___ ___ ___ ___ ___          %
%===================== CONTROLS ======================|         \ \ / /_\ |  \/  | _ \_ _| _ \ __/ __|         %
%                   o0<><><><><>0o                    |          \ V / _ \| |\/| |  _/| ||   / _|\__ \         %
%     [ ACTION: ]   <>    /\    <>    [ HOTKEY: ]     |           \_/_/ \_\_|  |_|_| |___|_|_\___|___/         %
%-----------------------------------------------------|                             __                         %
%                      CHARACTER:                     |                     /\   /\/ _\                        %
%-----------------------------------------------------|                     \ \ / /\ \                         %
%     MOVE UP:            ||          > W             |                      \ V / _\ \                        %
%     MOVE LEFT:          ||          > A             |                       \_/  \__/                        %
%     MOVE DOWN:          ||          > S             |  __      _____ ___ _____      _____  _ __   _____ ___  %
%     MOVE RIGHT:         ||          > D             |  \ \    / / __| _ \ __\ \    / / _ \| |\ \ / / __/ __| %
%     POTION:             ||          > E             |   \ \/\/ /| _||   / _| \ \/\/ / (_) | |_\ V /| _|\__ \ %
%-----------------------------------------------------|    \_/\_/ |___|_|_\___| \_/\_/ \___/|____\_/ |___|___/ %
%                   MISCELLANEOUS:                    |=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-%
%-----------------------------------------------------|        _____ _  _ ___    ___   _   __  __ ___          %
%     PAUSE GAME:         ||          > P             |       |_   _| || | __|  / __| /_\ |  \/  | __|         %
%     START GAME:         ||          > B             |         | | | __ | _|  | (_ |/ _ \| |\/| | _|          %
%     EXIT GAME:          ||          > X             |         |_| |_||_|___|  \___/_/ \_\_|  |_|___|         %
%-------------------------\/-----------------------------------------------------------------------------------%
o0<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>0o
)";
}

coord CalculatePosition(Object **G,Object O) {
    coord pos; // Variable for randomly generated coordinates
    pos.X = rand() % (x); // Generates random number between [0,x-1]
    pos.Y = rand() % (y); // Generates random number between [0,y-1]
    while (G[pos.X][pos.Y].T != 'E') { // If spawned entity overlaps with another object, recalculate position
        pos=CalculatePosition(G, O);
    }
    if (O.T == 'V' || O.T == 'W') {
        O.p.X = pos.X;
        O.p.Y = pos.Y;
        char Adj[8];
        CalculateAdj(G, O, Adj);
        for (int i = 0; i < 8; i++) {
            if (Adj[i] == 'A' || Adj[i] == 'P') {
                pos = CalculatePosition(G, O); // Recalculate
                i = 9;
            }
        }
    }
    char type;
    type = O.T; // Object Type
    
    int adjcount=0;
    if (type == 'T' || type == 'L') { // TO ELIMINATE GRID PARTITIONING DUE TO RANDOMIZED OBJECT SPAWNING, OBJECTS CANNOT SPAWN WITHIN A RADIUS OF 1 FROM ANOTHER OBJECT
       for(int tx = (pos.X > 0 ? -1 : 0); tx<=(pos.X < x-1 ? 1 : 0); tx++){  //CHECKS IF THE RANDOMLY GENERATED X COORDINATE IS !=0, IF SET BACK THE LOOP BY -1. LOOP UNTIL THE LAST VALID X-AXIS POSITION, SETBACK BY +1 IF RANDOM X IS WITHIN BOUNDS
            for(int ty = (pos.Y > 0 ? -1 : 0); ty <= (pos.Y < y-1 ? 1 : 0); ty++){ // SAME FOR Y AXIS // X AND Y -1 DUE TO ARRAY RANGE STARTING FROM 0
                if(tx!=0 || ty!=0){ // ONLY POSITIONS THAT ARE WITHIN-BOUNDS AND ADJECENT TO THE RANDOMIZED COORDINATE FULLFIL THIS CONDITION
                    if(G[pos.X + tx][pos.Y + ty].T=='L' || G[pos.X + tx][pos.Y + ty].T=='T' || G[pos.X + tx][pos.Y + ty].T == 'A') // CHECK IF CURRENT VALID ADJACENT POSITION HAS ANOTHER OBSTACLE OR AVATAR
                        adjcount++; // IF IT DOES, NULL THE GENERATED OBJECT PLACEMENT POSITION
                }
            }
       }
    }
    if(adjcount!=0)
    pos = CalculatePosition(G,O); // RE-CALCULATE POSITION IF THE CURRENT RANDOMIZED POSITION DOESNT MEET THE ADJACENCY CRITERIA

    return pos;
}

Object** SpawnAll(Object** G, int Ecnt, int Ocnt) {
    for (int i = 0; i < Ocnt; i++) {
        int r = rand() % 2;
        char rndm; // Character to define if obstacle is going to be tree or lake
        if (r == 0)
            rndm = 'T';
        else
            rndm = 'L';
        Obstacle O = Obstacle(rndm,rndm);
        coord pos=CalculatePosition(G,O);
        G[pos.X][pos.Y] = O;
    }
    Obstacle Potion = Obstacle('P','P');
    coord ppos = CalculatePosition(G,Potion);
    G[ppos.X][ppos.Y] = Potion;
     // ENTITY SPAWN
    for (int j = 0; j < Ecnt; j++) { // Entities
        Werewolf W = Werewolf(5, rand() % 3 + 1, rand() % 2 + 1, rand() % 3, 'W', 0, 0);
        Vampire V = Vampire(5, rand() % 3 + 1, rand() % 2 + 1, rand() % 3, 'V', 0, 0); // Construct Vampire subclass with randomized stat values and a constant HP variable
        coord pos1 = CalculatePosition(G,V);
        G[pos1.X][pos1.Y] = V;
        V.p.X = pos1.X;
        V.p.Y = pos1.Y;
        Vamp.push_back(V);
        coord pos2 = CalculatePosition(G,W); // Calculate both coord position placements accordingly
        G[pos2.X][pos2.Y] = W; // Place Objects on grid
        W.p.X = pos2.X;
        W.p.Y = pos2.Y;
        Wolf.push_back(W);
    } // Repeat
    return G;
}
void GameOver(char type, Object ** Grid) {
    Wolf.clear(); // Clear Vectors
    Vamp.clear(); // Clear Vectors
    system("cls");
    char play;
    if (type == 'V') { // Vampires win
        cout << R"(
o0<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>0o
%                                                                                                              %
%                      _____          __  __ ______    ______      ________ _____                              %
%                     / ____|   /\   |  \/  |  ____|  / __ \ \    / /  ____|  __ \                             %
%                    | |  __   /  \  | \  / | |__    | |  | \ \  / /| |__  | |__) |                            %
%                    | | |_ | / /\ \ | |\/| |  __|   | |  | |\ \/ / |  __| |  _  /                             %
%                    | |__| |/ ____ \| |  | | |____  | |__| | \  /  | |____| | \ \                             %
%                     \_____/_/    \_\_|  |_|______|  \____/   \/   |______|_|  \_\                            %
%                                                                                                              %
%                                                                                                              %
%                                                                                                              %
%                                                                                                              %
%                                                                                                              %
%                                                                                                              %
%             __      __     __  __ _____ _____ _____  ______  _____  __          _______ _   _                %
%             \ \    / /\   |  \/  |  __ \_   _|  __ \|  ____|/ ____| \ \        / /_   _| \ | |               %
%              \ \  / /  \  | \  / | |__) || | | |__) | |__  | (___    \ \  /\  / /  | | |  \| |               %
%               \ \/ / /\ \ | |\/| |  ___/ | | |  _  /|  __|  \___ \    \ \/  \/ /   | | | . ` |               %
%                \  / ____ \| |  | | |    _| |_| | \ \| |____ ____) |    \  /\  /   _| |_| |\  |               %
%                 \/_/    \_\_|  |_|_|   |_____|_|  \_\______|_____/      \/  \/   |_____|_| \_|               %
%                                                                                                              %
%                                                                                                              %
%                                                                                                              %
o0<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>0o 
)";
    }
    else { // Werewolves win
        cout << R"(
o0<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>0o
%                                                                                                              %
%                      _____          __  __ ______    ______      ________ _____                              %
%                     / ____|   /\   |  \/  |  ____|  / __ \ \    / /  ____|  __ \                             %
%                    | |  __   /  \  | \  / | |__    | |  | \ \  / /| |__  | |__) |                            %
%                    | | |_ | / /\ \ | |\/| |  __|   | |  | |\ \/ / |  __| |  _  /                             %
%                    | |__| |/ ____ \| |  | | |____  | |__| | \  /  | |____| | \ \                             %
%                     \_____/_/    \_\_|  |_|______|  \____/   \/   |______|_|  \_\                            %
%                                                                                                              %
%                                                                                                              %
%                                                                                                              %
%                                                                                                              %
%                                                                                                              %
%                                                                                                              %
%  __          ________ _____  ________          ______  _ __      ________  _____  __          _______ _   _  %
%  \ \        / /  ____|  __ \|  ____\ \        / / __ \| |\ \    / /  ____|/ ____| \ \        / /_   _| \ | | %
%   \ \  /\  / /| |__  | |__) | |__   \ \  /\  / / |  | | | \ \  / /| |__  | (___    \ \  /\  / /  | | |  \| | %
%    \ \/  \/ / |  __| |  _  /|  __|   \ \/  \/ /| |  | | |  \ \/ / |  __|  \___ \    \ \/  \/ /   | | | . ` | %
%     \  /\  /  | |____| | \ \| |____   \  /\  / | |__| | |___\  /  | |____ ____) |    \  /\  /   _| |_| |\  | %
%      \/  \/   |______|_|  \_\______|   \/  \/   \____/|______\/   |______|_____/      \/  \/   |_____|_| \_| %
%                                                                                                              %
%                                                                                                              %
%                                                                                                              %
o0<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>0o                                                                                                                                                                                                                 
)";
    }
    Sleep(5000);
        system("cls");
        cout << R"(
o0<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>0o
%                  _____  _           __     __           _____          _____ _   _   ___                     %
%                 |  __ \| |        /\\ \   / /     /\   / ____|   /\   |_   _| \ | | |__ \                    %
%                 | |__) | |       /  \\ \_/ /     /  \ | |  __   /  \    | | |  \| |    ) |                   %
%                 |  ___/| |      / /\ \\   /     / /\ \| | |_ | / /\ \   | | | . ` |   / /                    %
%                 | |    | |____ / ____ \| |     / ____ \ |__| |/ ____ \ _| |_| |\  |  |_|                     %
%                 |_|    |______/_/    \_\_|    /_/    \_\_____/_/    \_\_____|_| \_|  (_)                     %
%                                                                                                              %
%                                                   __   __                                                    %
%                                                .-'  "."  '-.                                                 %
%                                              .'   ___,___   '.                                               %
%             _.._                            ;__.-; | | | ;-.__;                           _.._               %
%           .' .-'`                           | \  | | | | |  / |                           `'-. '.            %
%          /  /                                \ \/`"`"`"`"`\/ /                                \  \           %
%          |  |                                 \_.-,-,-,-,-._/                                  |  |          %
%          \  '.___.;                            \`-:_|_|_:-'/                             .___.'  /           %
%           '._  _.'                              '.       .'                               '.___.'            %
%              ``                                   `'---'`                                                    %
%                                                                                                              %
%    o0<><><><><><><><><><><><><>0o                                          o0<><><><><><>0<><><><><><>0o     %
%    [] PRESS > B TO PLAY AGAIN! []                                          [] PRESS > X TO EXIT GAME! []     %
%    o0<><><><><><><><><><><><><>0o                                          o0<><><><><><>0<><><><><><>0o     %
%                                                                                                              %
o0<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>0o                                                                    
)";
        do {
            cout << "> ";
            cin >> play;
            play = tolower(play); // Convert input to lowercase
            if (play == 'b') {
                // Delete Grid and restart game
                for (int i = 0; i < x; i++)
                    delete[] Grid[i];
                delete[] Grid;
                StartGame();
                continue;
            }
            else if (play == 'x') {
                TerminateGame();
            }
        } while (play != 'b' && play != 'x'); // Wrong input
}

Object MoveEntity(Object** G, Object Entity, int Direction, int *Pot) { // upleft:0 up:1 upright:2 left:3 right:4 downleft:5 down:6 downright:7
    char Adj[8];
    int ex = Entity.p.X, ey = Entity.p.Y;
    Object Empty; // Create and initialize empty object to be replaced with moved entity on grid (swap positions) or return if unable
    Empty.T = 'E';
    CalculateAdj(G, Entity, Adj);
    if (Adj[Direction] == 'E' || (Adj[Direction] == 'P' && Entity.T == 'A')) {
        if (Adj[Direction] == 'P')
            *Pot = 1;
        if (Direction <= 3) { // Before current entity on adjacent table
            Entity.p.X = ex + (Direction % 3) - 1;
            Entity.p.Y = ey + (Direction / 3) - 1;
            G[Entity.p.X][Entity.p.Y] = Entity;
        }
        else { // Past current entity on adjacent table
            Entity.p.X = ex + ((Direction + 1) % 3) - 1;
            Entity.p.Y = ey + ((Direction + 1) / 3) - 1;
            G[Entity.p.X][Entity.p.Y] = Entity;
        }
        Empty.p.X = ex;
        Empty.p.Y = ey;
        G[ex][ey] = Empty;
        return Entity; // Return updated position entity
    }
    return Empty; // if not successful
}

int RandCondition() {
    return rand() % 2;
}

Object** MoveWerewolves(Object** G) {
    Object moved;
    moved.T = 'E';
    for (int i = 0; i < Wolf.size(); i++) {
        Wolf[i].SetHF(0);
        Wolf[i].SetCF(0);
        if (RandCondition()) {
            int movedir = (rand() % 4)+1; // direction for werewolf to move
            if (movedir == 2)
                movedir = 6;
            moved = MoveEntity(G, Wolf[i], movedir, 0);
            if (moved.T != 'E') {
                Wolf[i].p.X = moved.p.X;
                Wolf[i].p.Y = moved.p.Y;
            }
        }
    }
    return G;
}

Object** MoveVampires(Object** G) {
    Object moved;
    moved.T = 'E';
    for (int i = 0; i < Vamp.size(); i++) {
        Vamp[i].SetHF(0);
        Vamp[i].SetCF(0);
        if (RandCondition()) {
            moved = MoveEntity(G, Vamp[i], rand() % 8, 0);
            if (moved.T != 'E') {
                Vamp[i].p.X = moved.p.X;
                Vamp[i].p.Y = moved.p.Y;
            }
        }
    }
    return G;
}

void TerminateGame() {
    DelayPrint("- TERMINATING GAME . . .\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\                        ", 12); // Added visual effects
    system("cls");
    exit(666);
}

void PrintPM(int pc) { // Print Pause Menu
    system("cls");
    SetConsoleTextAttribute(fcolor, 14);
    cout << "o0<><><><><><><><><><><><><><><><><><><><><><><><><><><><>0o" << endl;
    cout << "%" << setw(24) << " " << "PAUSE MENU"  << setw(25) << "%" << endl; 
    cout << "%" << setw(24) << " " << "^^^^^^^^^^"  << setw(25) << "%" << endl;
    cout << "%" << setw(59) << "%" << endl;
    cout << "%" << setw(24) << " " << "POTIONS: " << pc <<setw(25) << " %" << endl;

    cout << "%";
    SetConsoleTextAttribute(fcolor, 4);
    cout  << setw(23) << " " << "VAMPIRES: "<<setw(2)<<Vamp.size();
    SetConsoleTextAttribute(fcolor, 14);
    cout  << setw(24) << "%" << endl;
    cout << "%";
    SetConsoleTextAttribute(fcolor, 4);
    cout  << setw(24) << " " << "vvvvvvvvvv";
    SetConsoleTextAttribute(fcolor, 14);
    cout  << setw(25) << "%" << endl;
    cout << "%"  << setw(59) << "%" << endl;
    cout << "% ";
    int i = 0;
    while(i<Vamp.size()){
        int hp = Vamp[i].GetHP();
        SetConsoleTextAttribute(fcolor, 4);
        cout << "  V";
        SetConsoleTextAttribute(fcolor, 14);
        cout <<"(" << setw(2)<<Vamp[i].p.X + 1 <<',' << setw(2)<<Vamp[i].p.Y + 1 << ") - HP: " << hp;
        i++;
        if (i % 3 == 0 && i!= Vamp.size())
            cout << "   %\n% ";
        if (i == Vamp.size()) {
            if (i % 3 == 1)
                cout << setw(41) << " %\n";
            else if (i % 3 == 2)
                cout << setw(23) << " %\n";
            else
                cout << "   %\n";
        }
    }
    cout << "%" << setw(59) << "%" << endl;
    cout << "%";
    SetConsoleTextAttribute(fcolor, 5);
    cout  << setw(22) << " " << "WEREWOLVES: "<<setw(2)<<Wolf.size();
    SetConsoleTextAttribute(fcolor, 14);
    cout  << setw(23) << "%" << endl;
    cout << "%";
    SetConsoleTextAttribute(fcolor, 5);
    cout  << setw(23) << " " << "wwwwwwwwwwww";
    SetConsoleTextAttribute(fcolor, 14);
    cout  << setw(24) << "%" << endl;
    cout << "%" << setw(59) << "%" << endl;
    i = 0;
    cout << "% ";
    while (i < Wolf.size()) {
        int hp = Wolf[i].GetHP();
        SetConsoleTextAttribute(fcolor, 5);
        cout << "  W";
        SetConsoleTextAttribute(fcolor, 14);
        cout << "(" << setw(2) << Wolf[i].p.X +1 << ',' << setw(2) << Wolf[i].p.Y +1 << ") - HP: " << hp;
        i++;
        if (i % 3 == 0 && i!= Wolf.size())
            cout << "   %\n% ";
        if (i == Wolf.size()) {
            if (i % 3 == 1)
                cout << setw(41) << " %\n";
            else if (i % 3 == 2)
                cout << setw(23) << " %\n";
            else
                cout << "   %\n";
        }
    }
    cout << "%" << setw(59) << "%" << endl;
    cout << "%" << setw(8) << " " << "UNPAUSE GAME > P" << setw(13) << " " << "EXIT GAME > X" << setw(10) << " % " << endl;
    cout << "%" << setw(59) << "%" << endl;
    cout << "o0<><><><><><><><><><><><><><><><><><><><><><><><><><><><>0o" << endl;
}

void PauseGame(int pc) { // Pause and show stats until user unpauses
    char userin = 'n'; // Initialize user input as 'n' -> null
    do {
        PrintPM(pc);
        cout << "> ";
        cin >> userin;
        userin = tolower(userin); // Convert to lowercase
        if (userin == 'x')
            TerminateGame();
    } while (userin!='p');
}

void HealAll(Object** G,Avatar* avatar, char dt) {
    if (avatar->GetTeam() == 'v') {
        for (int i = 0; i < Vamp.size(); i++) {
            Vamp[i].SetHP(5);
            G[Vamp[i].p.X][Vamp[i].p.Y] = Vamp[i]; 
        }
    }
    if (avatar->GetTeam() == 'w') {
        for (int i = 0; i < Wolf.size(); i++) {
            Wolf[i].SetHP(5);
            G[Wolf[i].p.X][Wolf[i].p.Y] = Wolf[i]; 
        }
    }
    avatar->SetPotionCount((avatar->GetPotionCount()) - 1);
    DelayPrint("- YOUR TEAM HAS BEEN FULLY HEALED! ! !\n", 13);
    DelayPrint("- REMAINING POTIONS: ", 13);
    cout << avatar->GetPotionCount()<<endl;
    SetConsoleTextAttribute(fcolor, 14);
}

void PrintTeamSelect() {
    DelayPrint("- TEAM", 14);
    DelayPrint(" WEREWOLVES", 5);
    DelayPrint(" OR TEAM", 14);
    DelayPrint(" VAMPIRES", 4);
    DelayPrint(" ? ? ? \n", 14);
    DelayPrint("> W", 5);
    DelayPrint(" FOR", 14);
    DelayPrint(" WEREWOLVES", 5);
    DelayPrint(" OR", 14);
    DelayPrint(" > V", 4);
    DelayPrint(" FOR", 14);
    DelayPrint(" VAMPIRES\n", 4);
    SetConsoleTextAttribute(fcolor, 14);
}

Object** EntityDeath(Object** Grid, Object entity, int entpos) {
    if (entity.T == 'W') {
        DelayPrint("\a- A WEREWOLF HAS BEEN SENT TO THE SHADOW REALM! ! !\n", 5);
        Sleep(500);
        SetConsoleTextAttribute(fcolor,14);
        if (Wolf.size() == 1) { // Last Werewolf to be killed,
            GameOver('V', Grid);
        }
        Grid[Wolf[entpos].p.X][Wolf[entpos].p.Y].T = 'E';
        Wolf.erase(Wolf.begin() + entpos);
    }
    else {
        DelayPrint("\a- A VAMPIRE HAS BEEN SENT TO THE SHADOW REALM! ! !\n", 4);
        Sleep(500);
        SetConsoleTextAttribute(fcolor, 14);
        if (Vamp.size() == 1) { // Last Vampire to be killed,
            GameOver('W', Grid);
        }
        Grid[Vamp[entpos].p.X][Vamp[entpos].p.Y].T = 'E';
        Vamp.erase(Vamp.begin() + entpos);
    }
    return Grid;
}

int SearchWW(int x, int y) { // Search Werewolf, Return position in Vector
    for (int i = 0; i < Wolf.size(); i++)
        if (Wolf[i].p.X == x && Wolf[i].p.Y == y)
            return i;
    return -1; // Default, Not found
}

int SearchV(int x, int y) { // Search Vampire, Return position in Vector
    for (int i = 0; i < Vamp.size(); i++)
        if (Vamp[i].p.X == x && Vamp[i].p.Y == y)
            return i;
    return -1; // Default, Not found
}
Object** HealAlly(Object** Grid, Object en1, int en1pos, Object en2, int en2pos) {
    if (RandCondition()) { // WILL HEAL ALLY
        if (en1.T == 'W') {
            // Access wolf vector
            if (Wolf[en1pos].GetMED() && Wolf[en2pos].GetHP()!= 5 && Wolf[en1pos].GetHF() == 0) { // Has Med kit, target is not full HP, current entity did not heal ally this round
                Wolf[en1pos].SetMED(Wolf[en1pos].GetMED() - 1); // Reduce number of medkits on Wolf
                Wolf[en2pos].SetHP(Wolf[en2pos].GetHP() + 1); // Heal for 1 HP
                Wolf[en1pos].SetHF(1); // Set heal flag to 1
            }
        }
        else {
            // Access vamp vector
            if (Vamp[en1pos].GetMED() && Vamp[en2pos].GetHP() != 5 && Vamp[en1pos].GetHF() == 0) { // Has Med kit, target is not full HP, current entity did not heal ally this round
                Vamp[en1pos].SetMED(Vamp[en1pos].GetMED() - 1); // Reduce number of medkits on Wolf
                Vamp[en2pos].SetHP(Vamp[en2pos].GetHP() + 1); // Heal for 1 HP
                Vamp[en1pos].SetHF(1); // Set heal flag to 1
                // CHECK IF IT WORKS WITHOUT GRID ASSIGNMENT
            }
        }
    }
    return Grid;
}

Object** Kombat(Object** Grid, Object en1, int en1pos, Object en2, int en2pos) {
    int newhp = 0, dmg = 0;
    if (en1.T == 'W') { // Entity is a werewolf
        if (Wolf[en1pos].GetATK() >= Vamp[en2pos].GetATK()) { // Wolf attacks vamp
            dmg = (Wolf[en1pos].GetATK() - Vamp[en2pos].GetDEF());
            if (dmg <= 0)
                dmg = 1;
            newhp = Vamp[en2pos].GetHP() - dmg;
            if (newhp <= 0)
                Grid = EntityDeath(Grid, en2, en2pos);
            else {
                Vamp[en2pos].SetHP(newhp);
                Grid[Vamp[en2pos].p.X][Vamp[en2pos].p.Y] = Vamp[en2pos];
                Wolf[en1pos].SetCF(1); // Set Combat flag to 1
            }
        }
        else { // vamp attacks wolf
            dmg = (Vamp[en2pos].GetATK() - Wolf[en1pos].GetDEF());
            if (dmg <= 0)
                dmg = 1;
            newhp = Wolf[en1pos].GetHP() - dmg;
            if (newhp <= 0)
                Grid = EntityDeath(Grid, en1, en1pos); // FLAG
            else {
                Wolf[en1pos].SetHP(newhp);
                Grid[Wolf[en1pos].p.X][Wolf[en1pos].p.Y] = Wolf[en1pos];
                Wolf[en1pos].SetCF(1); // Set Combat flag to 1
            }
        }
    }
    else { // Entity is a vampire
        if (Vamp[en1pos].GetATK() >= Wolf[en2pos].GetATK()) { // vamp attacks wolf
            dmg = (Wolf[en2pos].GetATK() - Vamp[en1pos].GetDEF());
            if (dmg <= 0)
                dmg = 1;
            newhp = Wolf[en2pos].GetHP() - dmg;
            if (newhp <= 0)
                Grid = EntityDeath(Grid, en2, en2pos); // FLAG
            else {
                Wolf[en2pos].SetHP(newhp);
                Grid[Wolf[en2pos].p.X][Wolf[en2pos].p.Y] = Wolf[en2pos];
                Vamp[en1pos].SetCF(1); // Set Combat flag to 1
            }
        }
        else { // wolf attacks vamp
            dmg = (Wolf[en2pos].GetATK() - Vamp[en1pos].GetDEF());
            if (dmg <= 0)
                dmg = 1;
            newhp = Vamp[en1pos].GetHP() - dmg;
            if (newhp <= 0)
                Grid = EntityDeath(Grid, en2, en2pos);
            else {
                Vamp[en1pos].SetHP(newhp);
                Grid[Vamp[en1pos].p.X][Vamp[en1pos].p.Y] = Vamp[en1pos];
                Vamp[en1pos].SetCF(1); // Set Combat flag to 1
            }
        }

    }
    return Grid;
}

Object** EntityAction(Object** Grid) {
    char A[8];
    int SearchPos = 0;
    // VAMPIRES
    for (int i = 0; i < Vamp.size(); i++) {
        CalculateAdj(Grid, Vamp[i], A); // Calculate Adjacency Table for current entity
        for (int j = 0; j < 8; j++) {
            if (A[i] == 'W') {
                if (i <= 3) {
                    SearchPos = SearchWW(Vamp[i].p.X - 1 + (j % 3)/*X*/, Vamp[i].p.Y - 1 + (j / 3)/*Y*/);
                }
                else {
                    SearchPos = SearchWW(Vamp[i].p.X - 1 + (j + 1) % 3/*X*/, Vamp[i].p.Y - 1 + ((j + 1) / 3)/*Y*/);
                }
                if (SearchPos != -1)
                    Grid = Kombat(Grid, Vamp[i], i, Wolf[SearchPos], SearchPos);
            }
            else if (A[i] == 'V') {
                if (i <= 3) {
                    SearchPos = SearchV(Vamp[i].p.X - 1 + (j % 3)/*X*/, Vamp[i].p.Y - 1 + (j / 3)/*Y*/);
                }
                else {
                    SearchPos = SearchV(Vamp[i].p.X - 1 + (j + 1) % 3/*X*/, Vamp[i].p.Y - 1 + ((j + 1) / 3)/*Y*/);
                }
                if (SearchPos != -1)
                    Grid = HealAlly(Grid, Vamp[i], i, Vamp[SearchPos], SearchPos);
            }
        }
    }
    // WEREWOLVES
    for (int i = 0; i < Wolf.size(); i++) {
        CalculateAdj(Grid, Wolf[i], A); // Calculate Adjacency Table for current entity
        for (int j = 0; j < 8; j++) { // Traverse Adjacent
            if (A[j] == 'W') {
                if (j <= 3) {
                    SearchPos = SearchWW(Wolf[i].p.X - 1 + (j % 3)/*X*/, Wolf[i].p.Y - 1 + (j / 3)/*Y*/);
                }
                else {
                    SearchPos = SearchWW(Wolf[i].p.X - 1 + (j + 1) % 3/*X*/, Wolf[i].p.Y - 1 + ((j + 1) / 3)/*Y*/);
                }
                if (SearchPos != -1)
                    Grid = HealAlly(Grid, Wolf[i], i, Wolf[SearchPos], SearchPos);
            }
            else if (A[j] == 'V') {
                if (j <= 3) {
                    SearchPos = SearchV(Wolf[i].p.X - 1 + (j % 3)/*X*/, Wolf[i].p.Y - 1 + (j / 3)/*Y*/);
                }
                else {
                    SearchPos = SearchV(Wolf[i].p.X - 1 + (j + 1) % 3/*X*/, Wolf[i].p.Y - 1 + ((j + 1) / 3)/*Y*/);
                }
                if (SearchPos != -1)
                    Grid = Kombat(Grid, Wolf[i], i, Vamp[SearchPos], SearchPos);
            }
        }
    }
    return Grid;
}

void StartGame() {
    DelayPrint("- STARTING GAME ! ! !", 14);
    Sleep(500);
    system("cls");
    DelayPrint("- ENTER MAP DIMENSIONS:\n",14);
    cout << "- X: "; // Set limitations on dimensions, NO 0 AND NO NEGATIVE NUMBERS
    cin >> x;
    cout << "- Y: ";
    cin >> y;
    PrintTeamSelect();
    char team;
    Avatar avatar = Avatar(1, ' ','A'); // 1 number of potions, type 'A'
    do {
        cout << "> ";
        cin >> team;
        team = tolower(team); // Convert team to lowercase
        if (team == 'v') {
            DelayPrint("- HM. . .  A BLOODTHIRSTY CHOICE. . .\n",4);
            DelayPrint("- VAMPIRES IT IS!\n",4);
        }
        else if (team == 'w') {
            DelayPrint("- A CHILD OF THE MOON. . . \n",5);
            DelayPrint("- WELCOME TO THE PACK!\n",5);
        }
        else {
            DelayPrint("- INVALID OPTION, TRY AGAIN. . .\n", 12);
            SetConsoleTextAttribute(fcolor, 14);
        }
    } while (team != 'v' && team != 'w'); // Wrong input

    avatar.SetTeam(team); // Avatar now supports user-given team
    Object** grid = CreateGrid();
    Sleep(1000); // Sleep for 1s to display message then start
    int Ecount =(rand() % ((x * y / 15) + 1)); // Randomly generated Entity (V/W) Count [0,X*Y/15] (At least one Entity)
    if (Ecount==0)
        Ecount++;
    int Ocount = ((x * y / 10) + 1); // Obstacle Count ( At least one obstacle)
    coord apos = CalculatePosition(grid,avatar); // SPAWN AVATAR WITHIN THE GRID
    grid[apos.X][apos.Y]=avatar;
    avatar.p.X = apos.X; // Set avatar's own position
    avatar.p.Y = apos.Y;
    grid = SpawnAll(grid, Ecount, Ocount);// Insert entities and obstacle on grid
    // Variables used for game loop
    char input='B', dn='N',cond=0; // Starts with day, cond=condition
    int roundcnt = 0, pflag = 0; // Print flag
    Object moved;
    // Game loop starts here
    do {
        if (roundcnt % 5 == 0) { // Every 5 rounds swap day with night
            if (dn == 'D') { // Is day
                dn = 'N'; // Set night.
                if (pflag < 2) {
                    DelayPrint("- THE FULL MOON RISES. . .\n", 14);
                    Sleep(500);
                    pflag++;
                }
            }
            else { // Is night
                dn = 'D'; // Set Day
                if (pflag < 2) {
                    DelayPrint("- THE BRIGHT SUN SHINES ONCE AGAIN. . .\n", 14);
                    Sleep(500);
                    pflag++;
                }
            }
        }
        PrettyPrint(grid, dn);
        do {
            SetConsoleTextAttribute(fcolor, 14); // Set colour to 14(yellow)
            if (cond == 2)
                PrettyPrint(grid, dn);
            cond = 0;
            int gotpot = 0; // Condition if avatar picked up a potion while moving 0=NO 1=YES
            moved.T = 'E';
            cout << "> ";
            cin >> input; // Get input
            input = (tolower(input)); // Convert input to lowercase

            // Character Inputs Switch
            switch (input) { // Converts input to lowercase
            case 'w': // up
                moved = MoveEntity(grid, avatar, 1, &gotpot);
                if (moved.T == 'E')
                    cond = 3;
                else
                    cond = 6;
                break;
            case 's': // down
                moved = MoveEntity(grid, avatar, 6, &gotpot);
                if (moved.T == 'E')
                    cond = 3;
                else
                    cond = 6;
                break;
            case 'a': // left
                moved = MoveEntity(grid, avatar, 3, &gotpot);
                if (moved.T == 'E')
                    cond = 3;
                else
                    cond = 6;
                break;
            case 'd': // right
                moved = MoveEntity(grid, avatar, 4, &gotpot);
                if (moved.T == 'E')
                    cond = 3;
                else
                    cond = 6;
                break;
            case 'e': // heal
                if (avatar.GetTeam() == 'v' && dn == 'N' || avatar.GetTeam() == 'w' && dn != 'N') { // day/night conditions to not heal
                    DelayPrint("- HEALING YOUR TEAM IS NOT AVAILABLE AT THIS TIME. . .\n", 12);
                    cond = 5; // Did not heal, do not waste turn
                }
                else if (avatar.GetPotionCount() == 0) { // Not enough potions
                    DelayPrint("- TRY BREWING SOME POTIONS! ! !\n", 12);
                    cond = 5; // Did not heal, do not waste turn
                }
                else {
                    HealAll(grid, &avatar, dn);
                    Sleep(500);
                }
                break;
            case 'p': // pause
                PauseGame(avatar.GetPotionCount());
                cond = 2; // Game was paused
                break;
            case 'x':
                TerminateGame();
                break;
            default: // ERROR MESSAGE
                cond = 1;
            }
            SetConsoleTextAttribute(fcolor, 14); // Set colour to 14(yellow)
            if (cond == 0) {
                continue; // Do not enter else with moved
            }
            if (cond == 1) {
                DelayPrint("- WRONG INPUT, TRY AGAIN.\n", 12);
            }
            else if (cond == 2) { // Game was paused
                continue;
            }
            else if (cond == 3) { // Avatar is blocked when trying to move
                DelayPrint("- SOMETHING'S IN THE WAY. . .\n", 12);
                SetConsoleTextAttribute(fcolor, 14);
                cond = 1; // Repeat
            }
            else if (cond == 5) { // Did not heal, do not waste turn
                continue;
            }
            else if (cond == 6) {
                avatar.p.X = moved.p.X;
                avatar.p.Y = moved.p.Y;
                avatar.SetPotionCount(avatar.GetPotionCount() + gotpot); // If avatar got potion increase amount of potions in invetory.
                if (gotpot == 1) {
                    DelayPrint("- PICKED UP POTION!\n", 14);
                    Sleep(500);
                }
                cond = 0;
            }
        } while (cond != 0);
        // Character Input Ends
        // Entity Movements
        grid = MoveWerewolves(grid); // Werewolves get priority on moving because their moving options are limited
        grid = MoveVampires(grid);
        // Calculate General actions
        grid = EntityAction(grid);
        roundcnt++; // Round ends
    } while (true); // Game Loop
}