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

//Check for a win condition; Return 1 or -1 depending on who won
int checkWinCondition(square board)
{
    int checkSum = 1;
    for (int i = 0; i < (allPairs.size()); i++)
    {
        if (board.square[allPairs[i][0]] + board.square[allPairs[i][1]] + board.square[allPairs[i][2]] == checkSum*3);
        {
            return checkSum;
        }
        
    }
    checkSum = -1;
    for (int i = 0; i < (allPairs.size()); i++)
    {
        if (board.square[allPairs[i][0]] + board.square[allPairs[i][1]] + board.square[allPairs[i][2]] == checkSum*3);
        {
            return checkSum;
        }
        
    }
    return 0;
};

//Evaluates the whole board 
float evaluateBoard(raw_boardstate boardstate) {
    square subBoard;
    piece player = boardstate.turn;
    piece opponent = (player == CROSS) ? DOT : CROSS;
    int currentBoard = boardstate.current;
    subBoard.square = boardstate.board[currentBoard];
    float eval = 0;

    //Bias each field
    for (int i = 0; i < subBoard.square.size(); i++)
    {
        eval += subBoard.square[i] * fieldBias[i] * player;
    }

    //Check opponent advantage
    for (int i = 0; i < horizontalVerticalPairs.size(); i++) {
        if (subBoard.square[horizontalVerticalPairs[i][0]] + subBoard.square[horizontalVerticalPairs[i][1]] + subBoard.square[horizontalVerticalPairs[i][2]] == opponent*2)
        {
            eval -= 6;
            //std::cout << "Pair found: " << horizontalVerticalPairs[i][0] << ", " << horizontalVerticalPairs[i][1] << ", " << horizontalVerticalPairs[i][2] << std::endl;
        }
    }
    for (int i = 0; i < crossPairs.size(); i++) {
        if (subBoard.square[crossPairs[i][0]] + subBoard.square[crossPairs[i][1]] + subBoard.square[crossPairs[i][2]] == opponent*2)
        {
            eval -= 7;
            //std::cout << "Pair found: " << crossPairs[i][0] << ", " << crossPairs[i][1] << ", " << crossPairs[i][2] << std::endl;
        }
    }
    for (int i = 0; i < (allPairs.size()); i++)
    {
        if (subBoard.square[allPairs[i][0]] + subBoard.square[allPairs[i][1]] + subBoard.square[allPairs[i][2]] == -opponent && subBoard.square[allPairs[i][0]] != EMPTY && subBoard.square[allPairs[i][1]] != EMPTY && subBoard.square[allPairs[i][2]] != EMPTY)
        {
            eval -= 9;
            //std::cout << "Block found: " << allPairs[i][0] << ", " << allPairs[i][1] << ", " << allPairs[i][2] << std::endl;
        }
        
    }

    //Check for AI advantage
    for (int i = 0; i < horizontalVerticalPairs.size(); i++) {
        if (subBoard.square[horizontalVerticalPairs[i][0]] + subBoard.square[horizontalVerticalPairs[i][1]] + subBoard.square[horizontalVerticalPairs[i][2]] == player*2)
        {
            eval -= 6;
            //std::cout << "Pair found: " << horizontalVerticalPairs[i][0] << ", " << horizontalVerticalPairs[i][1] << ", " << horizontalVerticalPairs[i][2] << std::endl;
        }
    }
    for (int i = 0; i < crossPairs.size(); i++) {
        if (subBoard.square[crossPairs[i][0]] + subBoard.square[crossPairs[i][1]] + subBoard.square[crossPairs[i][2]] == player*2)
        {
            eval -= 7;
            //std::cout << "Pair found: " << crossPairs[i][0] << ", " << crossPairs[i][1] << ", " << crossPairs[i][2] << std::endl;
        }
    }
    for (int i = 0; i < (allPairs.size()); i++)
    {
        if (subBoard.square[allPairs[i][0]] + subBoard.square[allPairs[i][1]] + subBoard.square[allPairs[i][2]] == -player && subBoard.square[allPairs[i][0]] != EMPTY && subBoard.square[allPairs[i][1]] != EMPTY && subBoard.square[allPairs[i][2]] != EMPTY)
        {
            eval -= 9;
            //std::cout << "Block found: " << allPairs[i][0] << ", " << allPairs[i][1] << ", " << allPairs[i][2] << std::endl;
        }
        
    }
    eval += checkWinCondition(subBoard) * player * 12;
    return eval;
}

float evaluateGame(raw_boardstate boardstate)
{
    float eval = 0;
    raw_boardstate mainBoard;
    square mainSquare;
    mainBoard.turn = boardstate.turn;
    mainBoard.current = 0;
    for (int i = 0; i < 9; i++)
    {
        square evalSquare;
        evalSquare.square = boardstate.board[i];
        int winCondition = checkWinCondition(evalSquare);
        eval += evaluateBoard(boardstate) * 1.5 * squareBias[i];
        if (i == boardstate.current)
        {
            eval += evaluateBoard(boardstate) * squareBias[i];
        }
        eval += winCondition * squareBias[i] * boardstate.turn;
        mainSquare.square[i] = static_cast<piece>(winCondition);
    }
    eval += checkWinCondition(mainSquare) * 5000 * boardstate.turn;
    mainBoard.board[0] = mainSquare.square;
    eval += evaluateBoard(mainBoard) * 150;
    return eval;
}

int main() {
    srand ( time(NULL) );
    struct board generatedBoard = generateRandomBoard();
    struct raw_boardstate boardstate;
    boardstate.board = generatedBoard.board;
    boardstate.turn = CROSS;
    boardstate.current = 0;
    visualizeBoard(generatedBoard);
    for (int i = 0; i < 9; i++)
    {
        //std::cout << "Evaluating subboard " << i << std::endl;
        evaluateBoard(boardstate);
        boardstate.current++;
    }
    return 0;
};

move El_damache(raw_boardstate boardstate){
//ai stuff

return move{0,0};
};


/*
//This function actually evaluates a board fairly, is talked about in video.
function realEvaluateSquare(pos){ 

    POS = SUBBOARD
    X = 1 Player
    O = -1 AI
    a = Smart way of figuring out pairs

    var evaluation = 0;
    

    for(var bw in pos){
        evaluation -= pos[bw]*points[bw];
    }

    Checks for player advantage
    var a = 2;
    if(pos[0] + pos[1] + pos[2] === a || pos[3] + pos[4] + pos[5] === a || pos[6] + pos[7] + pos[8] === a) { Horizontal paris
        evaluation -= 6;
    }
    if(pos[0] + pos[3] + pos[6] === a || pos[1] + pos[4] + pos[7] === a || pos[2] + pos[5] + pos[8] === a) { Vertical pairs
        evaluation -= 6;
    }
    if(pos[0] + pos[4] + pos[8] === a || pos[2] + pos[4] + pos[6] === a) { Cross pairs
        evaluation -= 7;
    }

    a = -1;
    if((pos[0] + pos[1] === 2*a && pos[2] === -a) || (pos[1] + pos[2] === 2*a && pos[0] === -a) || (pos[0] + pos[2] === 2*a && pos[1] === -a) Any blocks
        || (pos[3] + pos[4] === 2*a && pos[5] === -a) || (pos[3] + pos[5] === 2*a && pos[4] === -a) || (pos[5] + pos[4] === 2*a && pos[3] === -a)
        || (pos[6] + pos[7] === 2*a && pos[8] === -a) || (pos[6] + pos[8] === 2*a && pos[7] === -a) || (pos[7] + pos[8] === 2*a && pos[6] === -a)
        || (pos[0] + pos[3] === 2*a && pos[6] === -a) || (pos[0] + pos[6] === 2*a && pos[3] === -a) || (pos[3] + pos[6] === 2*a && pos[0] === -a)
        || (pos[1] + pos[4] === 2*a && pos[7] === -a) || (pos[1] + pos[7] === 2*a && pos[4] === -a) || (pos[4] + pos[7] === 2*a && pos[1] === -a)
        || (pos[2] + pos[5] === 2*a && pos[8] === -a) || (pos[2] + pos[8] === 2*a && pos[5] === -a) || (pos[5] + pos[8] === 2*a && pos[2] === -a)
        || (pos[0] + pos[4] === 2*a && pos[8] === -a) || (pos[0] + pos[8] === 2*a && pos[4] === -a) || (pos[4] + pos[8] === 2*a && pos[0] === -a)
        || (pos[2] + pos[4] === 2*a && pos[6] === -a) || (pos[2] + pos[6] === 2*a && pos[4] === -a) || (pos[4] + pos[6] === 2*a && pos[2] === -a)){
        evaluation-=9;
    }

    Checks for AI advantage
    a = -2;
    if(pos[0] + pos[1] + pos[2] === a || pos[3] + pos[4] + pos[5] === a || pos[6] + pos[7] + pos[8] === a) {
        evaluation += 6;
    }
    if(pos[0] + pos[3] + pos[6] === a || pos[1] + pos[4] + pos[7] === a || pos[2] + pos[5] + pos[8] === a) {
        evaluation += 6;
    }
    if(pos[0] + pos[4] + pos[8] === a || pos[2] + pos[4] + pos[6] === a) {
        evaluation += 7;
    }

    a = 1;
    if((pos[0] + pos[1] === 2*a && pos[2] === -a) || (pos[1] + pos[2] === 2*a && pos[0] === -a) || (pos[0] + pos[2] === 2*a && pos[1] === -a)
        || (pos[3] + pos[4] === 2*a && pos[5] === -a) || (pos[3] + pos[5] === 2*a && pos[4] === -a) || (pos[5] + pos[4] === 2*a && pos[3] === -a)
        || (pos[6] + pos[7] === 2*a && pos[8] === -a) || (pos[6] + pos[8] === 2*a && pos[7] === -a) || (pos[7] + pos[8] === 2*a && pos[6] === -a)
        || (pos[0] + pos[3] === 2*a && pos[6] === -a) || (pos[0] + pos[6] === 2*a && pos[3] === -a) || (pos[3] + pos[6] === 2*a && pos[0] === -a)
        || (pos[1] + pos[4] === 2*a && pos[7] === -a) || (pos[1] + pos[7] === 2*a && pos[4] === -a) || (pos[4] + pos[7] === 2*a && pos[1] === -a)
        || (pos[2] + pos[5] === 2*a && pos[8] === -a) || (pos[2] + pos[8] === 2*a && pos[5] === -a) || (pos[5] + pos[8] === 2*a && pos[2] === -a)
        || (pos[0] + pos[4] === 2*a && pos[8] === -a) || (pos[0] + pos[8] === 2*a && pos[4] === -a) || (pos[4] + pos[8] === 2*a && pos[0] === -a)
        || (pos[2] + pos[4] === 2*a && pos[6] === -a) || (pos[2] + pos[6] === 2*a && pos[4] === -a) || (pos[4] + pos[6] === 2*a && pos[2] === -a)){
        evaluation+=9;
    }

    evaluation -= checkWinCondition(pos)*12;

    return evaluation;
}


//The most important function, returns a numerical evaluation of the whole game in it's current state
function evaluateGame(position, currentBoard) {
    var evale = 0;
    var mainBd = [];
    var evaluatorMul = [1.4, 1, 1.4, 1, 1.75, 1, 1.4, 1, 1.4];
    for (var eh = 0; eh < 9; eh++){
        evale += realEvaluateSquare(position[eh])*1.5*evaluatorMul[eh];
        if(eh === currentBoard){
            evale += realEvaluateSquare(position[eh])*evaluatorMul[eh];
        }
        var tmpEv = checkWinCondition(position[eh]);
        evale -= tmpEv*evaluatorMul[eh];
        mainBd.push(tmpEv);
    }
    evale -= checkWinCondition(mainBd)*5000;
    evale += realEvaluateSquare(mainBd)*150;
    return evale;
}
    */

