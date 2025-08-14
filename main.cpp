#include <iostream>
#include <array>
#include "globals.hpp"
#include <list>
#include <cmath>

board generateRandomBoard() {
    struct board newBoard = {};
    for (int i = 0; i < 9; i++) {
        newBoard.board.push_back(std::vector<piece>(9, piece::EMPTY));
        for (int j = 0; j < 9; j++)
        {
            newBoard.board[i][j] = static_cast<piece>(rand() % 3 - 1);
        }
    }
    return newBoard;
};

void visualizeBoard(board unsortedBoard) {
    struct board tempBoard = {};
    std::cout << "Current Board State:" << std::endl; 
    int resettableI = 0;
    int number = 0;
    int fullLoops = 0;
    std::string symbols[3] = {"O", " ", "X"};
    for (int i = 0; i < 81; i++)
    {
        if (number >= 81) 
        {
            number -= 80;
            resettableI = 0;
            fullLoops++;
        }
        int mainIndex = floor(resettableI/3)*3+floor(fullLoops/3); // Row
        int subIndex = 3*(resettableI%3)+(fullLoops%3); // col
        tempBoard.board[mainIndex][subIndex] = unsortedBoard.board[resettableI][floor(i/9)];
        number += 9;
        resettableI += 1;
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            std::cout << symbols[static_cast<int>(tempBoard.board[i][j])+1] << " ";
        }
        std::cout << std::endl;
    }
    
    
};


int evaluateBoard(raw_boardstate boardstate) {
    square subBoard;
    piece player = boardstate.turn;
    int currentBoard = boardstate.current;
    subBoard.square = boardstate.board[currentBoard];
    int eval = 0;
    for (int i = 0; i < twoPairs.size(); i++) {
        if (subBoard.square[twoPairs[i][0]] == player && subBoard.square[twoPairs[i][1]] == player)
        {
            std::cout << "Pair found: " << twoPairs[i][0] << ", " << twoPairs[i][1] << std::endl;
        }
    }
    return eval;
}

int main() {
    struct board generatedBoard = generateRandomBoard();
    struct raw_boardstate boardstate;
    boardstate.board = generatedBoard.board;
    boardstate.turn = DOT;
    boardstate.current = 0;
    std::cout << boardstate.turn << std::endl;
    visualizeBoard(generatedBoard);
    evaluateBoard(boardstate);
    return 0;
};

move El_damache(raw_boardstate boardstate){
//ai stuff

return move{0,0};
};


