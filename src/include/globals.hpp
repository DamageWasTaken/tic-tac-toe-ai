#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <list>

// Color, 4 components, R8G8B8A8 (32bit)
typedef struct Color {
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
} Color;

// Vector2, 2 components
typedef struct Vector2 {
    float x;                // Vector x component
    float y;                // Vector y component
} Vector2;

extern Color BG_COLOR;

extern std::vector<std::vector<int>> winningSets;

extern std::vector<std::vector<int>> twoPairs;

void impose_line_n(std::ifstream *file, std::string *line, int n);

enum piece{
    CROSS = 1,
    EMPTY = 0,
    DOT = -1
};

struct move{
    int sub; 
    int spot;
};

struct raw_boardstate {
    std::vector<std::vector<piece>> board = std::vector<std::vector<piece>>(9, std::vector<piece>(9, piece::EMPTY));
    piece turn; 
    short current; 
};

struct board {
    std::vector<std::vector<piece>> board = std::vector<std::vector<piece>>(9, std::vector<piece>(9, piece::EMPTY));
};

struct square {
    std::vector<piece> square = std::vector<piece>(9, piece::EMPTY);
};