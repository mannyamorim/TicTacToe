/*
Simple single player console TicTacToe game using the Minimax algorithm

MIT License

Copyright (c) 2019 Emmanuel Mathi-Amorim

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <iostream>
#include <algorithm>

#include "fmt/format.h"
#include "fmt/color.h"

//If compiling on Windows, include windows library for color support
#ifdef _WIN32
    #include <windows.h>
    #undef min
    #undef max
#endif // _WIN32

enum Piece {
    X,
    O,
    Empty
};

enum WinState {
    XWin,
    OWin,
    Draw,
    NotFinished
};

Piece board[9] = { Empty };

#ifdef _WIN32
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

    inline void printX() {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        fmt::print("X");
        SetConsoleTextAttribute(hConsole, screenBufferInfo.wAttributes);
    }
    inline void printO() {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        fmt::print("O");
        SetConsoleTextAttribute(hConsole, screenBufferInfo.wAttributes);
    }
#else
    inline void printX() {
        fmt::print(fg(fmt::color::red), "X");
    }
    inline void printO() {
        fmt::print(fg(fmt::color::blue), "O");
    }
#endif // _WIN32

void printBoardState() {
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            switch (board[(x * 3) + y]) {
                case X:
                    printX();
                    break;
                case O:
                    printO();
                    break;
                case Empty:
                    fmt::print("*");
                    break;
            }
        }

        fmt::print(" {:d}{:d}{:d}\n", (x * 3), (x * 3) + 1, (x * 3) + 2);
    }
}

void resetGame() {
    for (int i = 0; i < 9; i++) {
        board[i] = Empty;
    }
}

WinState checkWin() {
    const int w1[] = { 0, 3, 6, 0, 1, 2, 0, 2 };
    const int w2[] = { 1, 4, 7, 3, 4, 5, 4, 4 };
    const int w3[] = { 2, 5, 8, 6, 7, 8, 8, 6 };

    for (int i = 0; i < 8; i++) {
        if (board[w1[i]] == X && board[w2[i]] == X && board[w3[i]] == X) {
            return XWin;
        }
    }

    for (int i = 0; i < 8; i++) {
        if (board[w1[i]] == O && board[w2[i]] == O && board[w3[i]] == O) {
            return OWin;
        }
    }

    int spaces = 0;
    for (int i = 0; i < 9; i++) {
        if (board[i] == Empty) {
            spaces++;
        }
    }

    if (spaces == 0) {
        return Draw;
    } else {
        return NotFinished;
    }
}

int getUserMove() {
    while (1) {
        fmt::print("Enter move: ");
        int move;
        std::cin >> move;
        if (move >= 0 && move <= 8 && board[move] == Empty) {
            return move;
        }
    }
}

int minmax(Piece piece, int maximize) {
    switch (checkWin()) {
        case XWin:
            return -1;
        case OWin:
            return 1;
        case Draw:
            return 0;
        case NotFinished: {
            int bestScore = 0;
            if (maximize)
                bestScore = -1;
            else
                bestScore = 1;
            for (int i = 0; i < 9; i++) {
                if (board[i] == Empty) {
                    board[i] = piece;

                    enum Piece opposite;
                    if (piece == O)
                        opposite = X;
                    else
                        opposite = O;

                    int score = minmax(opposite, !maximize);
                    if (maximize) {
                        bestScore = std::max(bestScore, score);
                    } else {
                        bestScore = std::min(bestScore, score);
                    }

                    board[i] = Empty;
                }
            }
            return bestScore;
        }
    }
}

int getComputerMove() {
    int moveScore = -1, move = 0;

    for (int i = 0; i < 9; i++) {
        if (board[i] == Empty) {
            board[i] = O;

            int score = minmax(X, 0);
            if (score > moveScore) {
                moveScore = score;
                move = i;
            }

            board[i] = Empty;
        }
    }

    return move;
}

int main() {
#ifdef _WIN32
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == NULL) {
        fmt::print("Error getting console handle\n");
        return 1;
    }
    if (!GetConsoleScreenBufferInfo(hConsole, &screenBufferInfo)) {
        fmt::print("Error getting console screen buffer info\n");
        return 1;
    }
#endif // _WIN32

    while (1) {
        resetGame();
        printBoardState();
        while (checkWin() == NotFinished) {
            fmt::print("Turn X\n");
            int move = getUserMove();
            board[move] = X;

            printBoardState();

            if (checkWin() != NotFinished)
                break;

            fmt::print("Turn O\n");
            move = getComputerMove();
            board[move] = O;

            printBoardState();
        }

        switch (checkWin()) {
            case OWin:
                fmt::print("O Wins!\n");
                break;
            case XWin:
                fmt::print("X Wins!\n");
                break;
            case Draw:
                fmt::print("Its a draw.\n");
                break;
            case NotFinished:
                break;
        }
    }

    return 0;
}
