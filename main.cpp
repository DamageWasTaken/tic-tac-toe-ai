#include <iostream>
#include <array>
#include "globals.hpp"
#include <list>
#include <cmath>
#include <iterator>
#include <algorithm>

piece playerPiece = EMPTY;

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
        if (board.square[allPairs[i][0]] + board.square[allPairs[i][1]] + board.square[allPairs[i][2]] == checkSum*3)
        {
            return checkSum;
        }
        
    }
    checkSum = -1;
    for (int i = 0; i < (allPairs.size()); i++)
    {
        if (board.square[allPairs[i][0]] + board.square[allPairs[i][1]] + board.square[allPairs[i][2]] == checkSum*3)
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

miniMaxReturn miniMax(raw_boardstate boardstate, int depth, float alpha, float beta) {
    int bestMove = -1;

    //If boardstate is won, return the evaluation
    float gameEvaluation = evaluateGame(boardstate);
    if (depth <= 0 || std::abs(gameEvaluation) > 5000) {
        return miniMaxReturn{gameEvaluation, static_cast<float>(bestMove)};
    }
    std::cout << "Check 3" << " : " << boardstate.turn << std::endl;

    if (boardstate.turn == playerPiece)
    {
        //Player eval
        float maxEval = -std::numeric_limits<float>::infinity();
        std::cout << "Check 4" << std::endl;
        for (int i = 0; i < 9; i++)
        {
            miniMaxReturn eval = {-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()};
            if (boardstate.current == -1)
            {
                //Full board eval
                for (int j = 0; j < 9; j++)
                {
                    square subBoard;
                    subBoard.square = boardstate.board[j];
                    if (checkWinCondition(subBoard) != 0)
                    {
                        continue;
                    }
                    if (subBoard.square[j] == piece::EMPTY)
                    {
                        boardstate.board[i][j] = boardstate.turn;
                        boardstate.current = (CROSS == boardstate.turn) ? DOT : CROSS;
                        eval.evaluation = miniMax(boardstate, depth - 1, alpha, beta).evaluation;
                        boardstate.board[i][j] = EMPTY;
                    }
                    if (eval.evaluation > maxEval) {
                        maxEval = eval.evaluation;
                        bestMove = i;
                    }
                    alpha = std::max(alpha, eval.evaluation);
                }
                if (beta <= alpha) {
                    break;
                }
            } else 
            {
                //Square eval
                if (boardstate.board[boardstate.current][i] == piece::EMPTY)
                {
                    boardstate.board[boardstate.current][i] = boardstate.turn;
                    boardstate.current = (CROSS == boardstate.turn) ? DOT : CROSS;
                    eval = miniMax(boardstate, depth - 1, alpha, beta);
                    boardstate.board[boardstate.current][i] = EMPTY;
                }
                float blob = eval.evaluation;
                if (blob > maxEval) {
                    maxEval = blob;
                    bestMove = eval.bestMove;
                }
                alpha = std::max(alpha, blob);
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return miniMaxReturn{maxEval, static_cast<float>(bestMove)};
    } else {
        //Opponent eval
        float minEval = -std::numeric_limits<float>::infinity();
        for (int i = 0; i < 9; i++)
        {
            miniMaxReturn eval = {-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()};
            if (boardstate.current == -1)
            {
                //Full board eval
                for (int j = 0; j < 9; j++)
                {
                    square subBoard;
                    subBoard.square = boardstate.board[j];
                    if (checkWinCondition(subBoard) != 0)
                    {
                        continue;
                    }
                    if (subBoard.square[j] == piece::EMPTY)
                    {
                        boardstate.board[i][j] = boardstate.turn;
                        boardstate.current = (CROSS == boardstate.turn) ? DOT : CROSS;
                        eval.evaluation = miniMax(boardstate, depth - 1, alpha, beta).evaluation;
                        boardstate.board[i][j] = EMPTY;
                    }
                    if (eval.evaluation > minEval) {
                        minEval = eval.evaluation;
                        bestMove = i;
                    }
                    beta = std::max(beta, eval.evaluation);
                }
                if (beta <= alpha) {
                    break;
                }
            } else 
            {
                //Square eval
                if (boardstate.board[boardstate.current][i] == piece::EMPTY)
                {
                    boardstate.board[boardstate.current][i] = boardstate.turn;
                    boardstate.current = (CROSS == boardstate.turn) ? DOT : CROSS;
                    eval = miniMax(boardstate, depth - 1, alpha, beta);
                    boardstate.board[boardstate.current][i] = EMPTY;
                }
                float blob = eval.evaluation;
                if (blob > minEval) {
                    minEval = blob;
                    bestMove = eval.bestMove;
                }
                beta = std::max(beta, blob);
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return miniMaxReturn{minEval, static_cast<float>(bestMove)};
    }
};

float evaluateMove(raw_boardstate boardstate, int evaluatedMove)
{
    float eval = 0;
    square subBoard;
    subBoard.square = boardstate.board[evaluatedMove];
    subBoard.square[evaluatedMove] = (boardstate.turn == CROSS) ? DOT : CROSS;

    //Bias the move
    eval += fieldBias[evaluatedMove];

    //Blocks
    for (int i = 0; i < (allPairs.size()); i++)
    {
        if (subBoard.square[allPairs[i][0]] + subBoard.square[allPairs[i][1]] + subBoard.square[allPairs[i][2]] == -boardstate.turn*3)
        {
            eval += 2;
            //std::cout << "Block found: " << allPairs[i][0] << ", " << allPairs[i][1] << ", " << allPairs[i][2] << std::endl;
        }
        
    }

    subBoard.square[evaluatedMove] = boardstate.turn;

    //Pairs
    for (int i = 0; i < allPairs.size(); i++) {
        if (subBoard.square[allPairs[i][0]] + subBoard.square[allPairs[i][1]] + subBoard.square[allPairs[i][2]] == boardstate.turn*2)
        {
            eval += 1;
            //std::cout << "Pair found: " << allPairs[i][0] << ", " << allPairs[i][1] << ", " << allPairs[i][2] << std::endl;
        }
    }
    //Wins
    for (int i = 0; i < allPairs.size(); i++) {
        if (subBoard.square[allPairs[i][0]] + subBoard.square[allPairs[i][1]] + subBoard.square[allPairs[i][2]] == boardstate.turn*3)
        {
            eval += 5;
            //std::cout << "Pair found: " << allPairs[i][0] << ", " << allPairs[i][1] << ", " << allPairs[i][2] << std::endl;
        }
    }

    
    eval += checkWinCondition(subBoard) * boardstate.turn * 15;
    subBoard.square[evaluatedMove] = piece::EMPTY;
    return eval;
}

move El_damache(raw_boardstate boardstate){
    //Variable declerations
    playerPiece = boardstate.turn;
    int bestMove = -1;
    float bestScore[9] = {-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()};
    int availableMoves = 0;

    //Check amount of remaining moves
    for (int i = 0; i < 9; i++)
    {
        square subBoard;
        subBoard.square = boardstate.board[i];
        if (checkWinCondition(subBoard) != 0)
        {
            continue;
        }
        for (int j = 0; j < 9; j++)
        {
            if (boardstate.board[i][j] == piece::EMPTY)
            {
                availableMoves++;
            }
        }
    }

    std::cout << "Check 1" << " : " << availableMoves << std::endl;

    //If there's no current board, figure out which board to play on
    if (boardstate.current == -1)
    {
        std::cout << "Check 2" << " : " << boardstate.turn << std::endl;
        miniMaxReturn bestBoard;

        if (availableMoves < 10)
        {
            std::cout << "Check 2.1" << std::endl;

            bestBoard = miniMax(boardstate, std::min(4, availableMoves), -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
        } else if (availableMoves < 18)
        {
            std::cout << "Check 2.2" << std::endl;
            bestBoard = miniMax(boardstate, std::min(5, availableMoves), -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());

        } else 
        {
            std::cout << "Check 2.3" << std::endl;
            bestBoard = miniMax(boardstate, std::min(6, availableMoves), -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
        }
        boardstate.current = static_cast<int>(bestBoard.bestMove);
    }

    std::cout << "Check success" << std::endl;

    //Create a default move for if all else fails
    for (int i = 0; i < 9; i++)
    {
        if (boardstate.board[boardstate.current][i] == piece::EMPTY)
        {
            bestMove = i;
            break;
        }        
    }

    for (int i = 0; i < 9; i++)
    {
        if (boardstate.board[boardstate.current][i] == piece::EMPTY)
        {
            bestScore[i] = evaluateMove(boardstate, i) * 45;
        }
    }
    
    for (int i = 0; i < 9; i++)
    {
        square subBoard;
        subBoard.square = boardstate.board[boardstate.current];
        if (checkWinCondition(subBoard) == 0)
        {
            if (subBoard.square[i] == piece::EMPTY)
            {
                boardstate.board[boardstate.current][i] = boardstate.turn;
                miniMaxReturn bestMove;
                if (availableMoves < 20)
                {
                    bestMove = miniMax(boardstate, std::min(5, availableMoves), -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
                } else if (availableMoves < 32)
                {
                    bestMove = miniMax(boardstate, std::min(6, availableMoves), -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
                } else 
                {
                    bestMove = miniMax(boardstate, std::min(7, availableMoves), -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
                }
                boardstate.board[boardstate.current][i] = piece::EMPTY;
                bestScore[i] = bestMove.evaluation;
            }     
        }
    }

    bestMove = std::distance(bestScore, std::max_element(bestScore, bestScore + 9));

    return move{boardstate.current, bestMove};
};

int main() {
    srand ( time(NULL) );
    struct board generatedBoard = generateRandomBoard();
    struct raw_boardstate boardstate;
    boardstate.board = generatedBoard.board;
    boardstate.turn = CROSS;
    //boardstate.current = rand() % 9 - 1;
    boardstate.current = -1;
    visualizeBoard(generatedBoard);
    move returnedMove = El_damache(boardstate);
    std::cout << "AI Move: " << returnedMove.sub << ", " << returnedMove.spot << std::endl;
    return 0;
};