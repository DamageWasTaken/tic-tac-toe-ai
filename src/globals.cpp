#include "globals.hpp"
#include <vector>
#include <fstream>
#include <list>


void impose_line_n(std::ifstream *file, std::string *line, int n){
    for (int i = 0; i<n; i++){
        std::getline(*file, *line);
    };
};

Color BG_COLOR = {224, 211, 175, 255};

std::vector<std::vector<int>> winningSets = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},
    {0, 3, 6},
    {1, 4, 7},
    {2, 5, 8},
    {0, 4, 8},
    {2, 4, 6}
};

std::vector<std::vector<int>> twoPairs = {
    {0,1},
    {0,3},
    {0,4},
    {1,2},
    {1,4},
    {2,4},
    {2,5},
    {3,4},
    {3,6},
    {4,5},
    {4,6},
    {4,7},
    {4,8},
    {5,8},
    {6,7},
    {7,8},
};