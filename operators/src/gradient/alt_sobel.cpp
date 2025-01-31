#include "../include/gradient/alt_sobel.h"
#include "../include/utils/image_utils.h"
#include "../include/utils/kernels_util.h"

AltSobel::AltSobel(int kernelSize) : ksize(kernelSize), scale(1), delta(0) {
    height = 0;
    width = 0;
}

cv::Mat AltSobel::getEdges(const string &inputPath, const string &outputName) {
    clock_t t = clock();

    cv::Mat image = ImageUtils::getImage(inputPath);
    height = image.rows;
    width = image.cols;

    vector<vector<vector<uint8_t>>> rgbImage = convertToRGB(image);
    vector<vector<uint8_t>> grayImage = convertToGrayscale(rgbImage);
    vector<vector<int>> gradX = computeGradientX(grayImage);
    vector<vector<int>> gradY = computeGradientY(grayImage);
    cv::Mat edges = combineGradients(gradX, gradY);

    ImageUtils::writeImage(edges, outputName);

    printf("Time taken: %.4fs\n", (float)(clock() - t)/CLOCKS_PER_SEC);

    return edges;
}

string AltSobel::getOperatorName() const {
    return "AltSobel";
}

vector<vector<vector<uint8_t>>> AltSobel::convertToRGB(const cv::Mat& input) const {
    vector<vector<vector<uint8_t>>> rgbMatrix(
            height, vector<vector<uint8_t>>(width, vector<uint8_t>(3)));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            const auto& pixel = input.at<cv::Vec3b>(i, j);
            rgbMatrix[i][j][0] = pixel[2];
            rgbMatrix[i][j][1] = pixel[1];
            rgbMatrix[i][j][2] = pixel[0];
        }
    }

    return rgbMatrix;
}

vector<vector<uint8_t>> AltSobel::convertToGrayscale(const vector<vector<vector<uint8_t>>>& rgbMatrix) const {
    vector<vector<uint8_t>> grayMatrix(height, vector<uint8_t>(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            const auto& pixel = rgbMatrix[i][j];
            grayMatrix[i][j] = static_cast<uint8_t>(
                    0.299 * pixel[0] +
                    0.587 * pixel[1] +
                    0.114 * pixel[2]
            );
        }
    }

    return grayMatrix;
}

vector<vector<int>> AltSobel::computeGradientX(const vector<vector<uint8_t>>& grayImage) const {
    int kernelSize = static_cast<int>(KernelUtil::sobelX.size());
    int offset = kernelSize / 2;

    vector<vector<int>> gradX(height, vector<int>(width, 0));

    for (int i = offset; i < height - offset; ++i) {
        for (int j = offset; j < width - offset; ++j) {
            int gradient = 0;
            for (int ki = 0; ki < kernelSize; ++ki) {
                for (int kj = 0; kj < kernelSize; ++kj) {
                    gradient += KernelUtil::sobelX[ki][kj] * grayImage[i + ki - offset][j + kj - offset];
                }
            }
            gradX[i][j] = static_cast<int>(scale * gradient + delta);
        }
    }

    return gradX;
}

vector<vector<int>> AltSobel::computeGradientY(const vector<vector<uint8_t>>& grayImage) const {
    int kernelSize = static_cast<int>(KernelUtil::sobelY.size());
    int offset = kernelSize / 2;

    vector<vector<int>> gradY(height, vector<int>(width, 0));

    for (int i = offset; i < height - offset; ++i) {
        for (int j = offset; j < width - offset; ++j) {
            int gradient = 0;
            for (int ki = 0; ki < kernelSize; ++ki) {
                for (int kj = 0; kj < kernelSize; ++kj) {
                    gradient += KernelUtil::sobelY[ki][kj] * grayImage[i + ki - offset][j + kj - offset];
                }
            }
            gradY[i][j] = static_cast<int>(scale * gradient + delta);
        }
    }

    return gradY;
}

cv::Mat AltSobel::combineGradients(const vector<vector<int>>& gradX, const vector<vector<int>>& gradY) const {
    cv::Mat combined(height, width, CV_8UC1);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int magnitude = static_cast<int>(sqrt(gradX[i][j] * gradX[i][j] + gradY[i][j] * gradY[i][j]));

            combined.at<uint8_t>(i, j) = static_cast<uint8_t>(min(255, magnitude));
        }
    }

    return combined;
}