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

std::vector<std::vector<int>> horizontalVerticalPairs = {
    {0,1,2},
    {3,4,5},
    {6,7,8},
    {0,3,6},
    {1,4,7},
    {2,5,8}
};

std::vector<std::vector<int>> crossPairs = {
    {0,4,8},
    {2,4,6}
};

std::vector<std::vector<int>> allPairs = {
    {0,1,2},
    {3,4,5},
    {6,7,8},
    {0,3,6},
    {1,4,7},
    {2,5,8},
    {0,4,8},
    {2,4,6}
};

std::vector<float> fieldBias = {0.2, 0.17, 0.2, 0.17, 0.22, 0.17, 0.2, 0.17, 0.2};

std::vector<float> squareBias = {1.4, 1, 1.4, 1, 1.75, 1, 1.4, 1, 1.4};