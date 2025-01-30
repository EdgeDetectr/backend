#ifndef OPERATORS_SOBEL_UTIL_H
#define OPERATORS_SOBEL_UTIL_H

#include <vector>
using namespace std;

namespace SobelUtil {
    static const vector<vector<int>> kernelX = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
    };
    static const vector<vector<int>> kernelY = {
            {-1, -2, -1},
            { 0,  0,  0},
            { 1,  2,  1}
    };
};


#endif //OPERATORS_SOBEL_UTIL_H
