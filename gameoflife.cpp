// Carl Nester
// Spring 2023 ISDS 371-01
// Brett Robblee
// Semester Programming Project: Level Up: C++

// This project was made to help identify what is happening in the game of life
// without requiring user input.  After removing the cin statement and clearing the screen,
// the program was running too fast to get an idea of what was happening.
// Included is a method to try to make a framerate somewhat constant.
// Start and end times of the loop are recorded and there is a pause at the end if the 
// loop takes less than 2 frames per second.
// I also expanded the board and added a new pattern, light-weight spaceship

// C++ Code Challenges, LinkedIn Learning

// Challenge #13: Conway's Game of Life
// Write an application that runs Conway's Game of Life, the best-known cellular automaton.
// Do this for a 10x10 grid.
// The grid wraps around in all directions.

// Rules:
// Any live cell with fewer than two live neighbors dies, as if by underpopulation.
// Any live cell with two or three live neighbors lives on to the next generation.
// Any live cell with more than three live neighbors dies, as if by overpopulation.
// Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <cstdlib> // added for system function (clear)
#include <chrono> // for std::chrono time functions
#include <thread> // for std::this_thread::sleep_for

// This defines the clear_command to clear the screen for Windows or Unix-based systems
#ifdef _WIN32
#define CLEAR_COMMAND "cls" // for Windows
#else
#define CLEAR_COMMAND "clear" // for Unix-based systems
#endif

#define N 20
#define M 20

// This defines the frame rate and frame time that we will try to adhere to
const int FRAME_RATE = 2; // in frames per second
const int FRAME_TIME_MS = 1000 / FRAME_RATE; // in milliseconds

// live_neighbors()
// Summary: This function returns the number of live neighbors to a given cell.
// Arguments:
//           game[N][M]: The state of the game.
//                    i: The cell's row.
//                    j: The cell's column.
// Returns: An integer with the live neighbor count.
int live_neighbors(char game[][M], int i, int j) {
    int neighbors = 0;
    i += N;
    j += M;
    if (game[(i - 1) % N][(j - 1) % M] == 'X')
        neighbors++;
    if (game[(i - 1) % N][j % M] == 'X')
        neighbors++;
    if (game[(i - 1) % N][(j + 1) % M] == 'X')
        neighbors++;
    if (game[i % N][(j - 1) % M] == 'X')
        neighbors++;
    if (game[i % N][(j + 1) % M] == 'X')
        neighbors++;
    if (game[(i + 1) % N][(j - 1) % M] == 'X')
        neighbors++;
    if (game[(i + 1) % N][j % M] == 'X')
        neighbors++;
    if (game[(i + 1) % N][(j + 1) % M] == 'X')
        neighbors++;
    return neighbors;
}

// Conway's Game of Life, main()
// Summary: This application is a simulation of Conway's game of life.
int main() {
    // The game, initialized with a glider and a blinker.
    char game[N][M];
    std::memset(game, '-', N * M);

    // A glider         // - - - - - - - - - -
    game[1][1] = 'X';   // - X - - - - - - - -
    game[2][2] = 'X';   // - - X - - - - - - -
    game[3][0] = 'X';   // X X X - - - - - - -
    game[3][1] = 'X';
    game[3][2] = 'X';

    // A blinker        // - - - - - - - - - -
    game[4][6] = 'X';   // - - - - - - - - - -
    game[4][7] = 'X';   // - - - - - - - - - -
    game[4][8] = 'X';   // - - - - - - X X X -

    
    // An R-pentomino   // - - - - -
    game[11][12]='X';   // - - X X -
    game[11][13]='X';   // - X X - -
    game[12][11]='X';   // - - X - -
    game[12][12]='X';
    game[13][12]='X';
    
    // A light weight spaceship // X - - X - - 
    game[14][1] = 'X';          // - - - - X - 
    game[14][4] = 'X';          // X - - - X - 
    game[15][5] = 'X';          // - X X X X - 
    game[16][1] = 'X'; 
    game[16][5] = 'X';
    game[17][2] = 'X'; 
    game[17][3] = 'X'; 
    game[17][4] = 'X';
    game[17][5] = 'X';

    char new_game[N][M];
    int generation = 0;
    std::string go_on;
    do {
        auto frame_start = std::chrono::steady_clock::now();  // marks the beginning of the frame, in nanoseconds
        std::cout << "Generation #" << generation++ << "\n";
        for (auto& str : game) {
            for (auto& ch : str)
                std::cout << " " << ch;
            std::cout << "\n";
        }
        std::cout << "\n";

        std::memcpy(new_game, game, N * M);
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++) {
                int n = live_neighbors(game, i, j);

                // Any live cell with fewer than two live neighbors dies, as if by underpopulation.
                if (game[i][j] == 'X' && n < 2)
                    new_game[i][j] = '-';
                // Any live cell with two or three live neighbors lives on to the next generation.
                //if(game[i][j]=='X' && (n==2 || n==3))
                //    new_game[i][j]='X'; 
                // Any live cell with more than three live neighbors dies, as if by overpopulation.
                if (game[i][j] == 'X' && n > 3)
                    new_game[i][j] = '-';
                // Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
                if (game[i][j] == '-' && n == 3)
                    new_game[i][j] = 'X';
            }
        std::memcpy(game, new_game, N * M);

        // The only way that I know how to get out of an infinite loop is to break.
        // Every user input method that I know of will pause the generations.
        std::cout << "Break program with CTL+C" << std::endl;

        // Sleep to maintain frame rate
        auto frame_end = std::chrono::steady_clock::now();   // marks the beginning of the frame, in nanoseconds
        
        auto frame_time = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start);  // Calculates the time between frame start and end, cast as milliseconds
        // Checks if the actual frame_time is less than the frame time we require
        if (frame_time < std::chrono::milliseconds(FRAME_TIME_MS))
        {
            // if yes, sleep for the difference
            std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_TIME_MS) - frame_time);
        }
        // Clear screen
        system(CLEAR_COMMAND);
    } while (go_on.compare("EXIT") != 0);

    return 0;
}
