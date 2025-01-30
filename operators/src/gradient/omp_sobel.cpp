#include "gradient/omp_sobel.h"
#include "../include/utils/image_utils.h"
#include "../include/utils/sobel_util.h"
using namespace std;
using namespace cv;

OmpSobel::OmpSobel(int kernelSize) : ksize(kernelSize), scale(1), delta(0) {
    height = 0;
    width = 0;
}

string OmpSobel::getOperatorName() const {
    return "OpenMP Sobel";
}

Mat OmpSobel::getEdges(const string& inputPath, const string& outputName) {
    clock_t t = clock();

    Mat image = ImageUtils::getImage(inputPath);
    height = image.rows;
    width = image.cols;

    Mat rgbImage = convertToRGB(image);
    Mat grayImage = convertToGrayscale(rgbImage);
    Mat gradX = computeGradientX(grayImage);
    Mat gradY = computeGradientY(grayImage);

    Mat edges = combineGradients(gradX, gradY);

    ImageUtils::writeImage(edges, outputName);

    printf("Time taken: %.4fs\n", (float)(clock() - t)/CLOCKS_PER_SEC);

    return edges;
}

Mat OmpSobel::convertToRGB(const Mat& input) const {
    Mat rgbImage(height, width, CV_8UC3);

#pragma omp parallel for default(none) shared(input, rgbImage) schedule(dynamic)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            const auto& pixel = input.at<Vec3b>(i, j);
            rgbImage.at<Vec3b>(i, j)[0] = pixel[2];
            rgbImage.at<Vec3b>(i, j)[1] = pixel[1];
            rgbImage.at<Vec3b>(i, j)[2] = pixel[0];
        }
    }

    return rgbImage;
}

Mat OmpSobel::convertToGrayscale(const Mat& rgbImage) const {
    Mat grayMatrix(height, width, CV_8UC1);

#pragma omp parallel for default(none) shared(rgbImage, grayMatrix) schedule(dynamic)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            const auto& pixel = rgbImage.at<Vec3b>(i, j);
            grayMatrix.at<uint8_t>(i, j) = static_cast<uint8_t>(
                    0.299 * pixel[2] +
                    0.587 * pixel[1] +
                    0.114 * pixel[0]
            );
        }
    }

    return grayMatrix;
}

Mat OmpSobel::computeGradientX(const Mat& grayImage) const {
    int kernelSize = static_cast<int>(SobelUtil::kernelX.size());
    int offset = kernelSize / 2;

    Mat gradX(height, width, CV_32SC1, Scalar(0));

#pragma omp parallel for default(none) shared(gradX, grayImage, kernelSize, offset) shared(SobelUtil::kernelX) schedule(dynamic)
    for (int i = offset; i < height - offset; ++i) {
        for (int j = offset; j < width - offset; ++j) {
            int gradient = 0;
            for (int ki = 0; ki < kernelSize; ++ki) {
                for (int kj = 0; kj < kernelSize; ++kj) {
                    gradient += SobelUtil::kernelX[ki][kj] * grayImage.at<uint8_t>(i + ki - offset, j + kj - offset);
                }
            }
            gradX.at<int>(i, j) = static_cast<int>(scale * gradient + delta);
        }
    }

    return gradX;
}

Mat OmpSobel::computeGradientY(const Mat& grayImage) const {
    int kernelSize = static_cast<int>(SobelUtil::kernelY.size());
    int offset = kernelSize / 2;

    Mat gradY(height, width, CV_32SC1, Scalar(0));

#pragma omp parallel for default(none) shared(gradY, grayImage, kernelSize, offset) shared(SobelUtil::kernelY) schedule(dynamic)
    for (int i = offset; i < height - offset; ++i) {
        for (int j = offset; j < width - offset; ++j) {
            int gradient = 0;
            for (int ki = 0; ki < kernelSize; ++ki) {
                for (int kj = 0; kj < kernelSize; ++kj) {
                    gradient += SobelUtil::kernelY[ki][kj] * grayImage.at<uint8_t>(i + ki - offset, j + kj - offset);
                }
            }
            gradY.at<int>(i, j) = static_cast<int>(scale * gradient + delta);
        }
    }

    return gradY;
}

Mat OmpSobel::combineGradients(const Mat& gradX, const Mat& gradY) const {
    Mat combined(height, width, CV_8UC1);

#pragma omp parallel for default(none) shared(combined, gradX, gradY) schedule(dynamic)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int magnitude = static_cast<int>(sqrt(gradX.at<int>(i, j) * gradX.at<int>(i, j) + gradY.at<int>(i, j) * gradY.at<int>(i, j)));

            combined.at<uint8_t>(i, j) = static_cast<uint8_t>(min(255, magnitude));
        }
    }

    return combined;
}