#include "globals.hpp"
#include <vector>
#include <fstream>
#

// Color, 4 components, R8G8B8A8 (32bit)
typedef struct Color {
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
} Color;


void impose_line_n(std::ifstream *file, std::string *line, int n){
    for (int i = 0; i<n; i++){
        std::getline(*file, *line);
    };
};

Color BG_COLOR = {224, 211, 175, 255};

std::vector<std::vector<int>> WINNING_SETS = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},
    {0, 3, 6},
    {1, 4, 7},
    {2, 5, 8},
    {0, 4, 8},
    {2, 4, 6}
};