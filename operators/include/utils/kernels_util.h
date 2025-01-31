#ifndef OPERATORS_KERNELS_UTIL_H
#define OPERATORS_KERNELS_UTIL_H

#include <vector>
using namespace std;

namespace KernelUtil {
    static const vector<vector<int>> sobelX = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
    };
    static const vector<vector<int>> sobelY = {
            {-1, -2, -1},
            { 0,  0,  0},
            { 1,  2,  1}
    };
    static const Mat prewittX = (Mat_<double>(3,3) <<
            -1, 0, 1,
            -1, 0, 1,
            -1, 0, 1);
    static const Mat prewittY = (Mat_<double>(3,3) <<
            -1, -1, -1,
            0,  0,  0,
            1,  1,  1);
    static const Mat robertCrossX = (cv::Mat_<double>(2, 2) <<
            1, 0,
            0, -1);
    static const Mat robertCrossY = (cv::Mat_<double>(2, 2) <<
            0, 1,
            -1, 0);
}


#endif //OPERATORS_KERNELS_UTIL_H
