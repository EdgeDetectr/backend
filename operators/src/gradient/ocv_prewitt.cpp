#include "../../include/gradient/ocv_prewitt.h"
#include "../include/utils/image_utils.h"
#include "../include/utils/kernels_util.h"

using namespace KernelUtil;

OcvPrewitt::OcvPrewitt() {}

Mat OcvPrewitt::getEdges(const string& inputPath, const string& outputName) {
    clock_t t = clock();

    Mat image = ImageUtils::getImage(inputPath);
    Mat rgbImage = convertToRGB(image);
    Mat grayImage = convertToGrayscale(rgbImage);
    Mat gradX = computeGradientX(grayImage);
    Mat gradY = computeGradientY(grayImage);
    Mat edges = combineGradients(gradX, gradY);
    ImageUtils::writeImage(edges, outputName);

    printf("Time taken: %.4fs\n", (float)(clock() - t)/CLOCKS_PER_SEC);

    return edges;
}

string OcvPrewitt::getOperatorName() const {
    return "OcvPrewitt";
}

Mat OcvPrewitt::convertToRGB(const Mat& image) {
    Mat rgbImage;
    cvtColor(image, rgbImage, COLOR_BGR2RGB);
    return rgbImage;
}

Mat OcvPrewitt::convertToGrayscale(const Mat& image) {
    Mat grayImage;
    cvtColor(image, grayImage, COLOR_RGB2GRAY);
    return grayImage;
}

Mat OcvPrewitt::computeGradientX(const Mat& grayImage) {
    Mat gradX;
    filter2D(grayImage, gradX, CV_64F, KernelUtil::prewittX);
    return gradX;
}

Mat OcvPrewitt::computeGradientY(const Mat& grayImage) {
    Mat gradY;
    filter2D(grayImage, gradY, CV_64F, KernelUtil::prewittY);
    return gradY;
}

Mat OcvPrewitt::combineGradients(const Mat& gradX, const Mat& gradY) {
    Mat edges;
    magnitude(gradX, gradY, edges);
    normalize(edges, edges, 0, 255, NORM_MINMAX, CV_8U);
    return edges;
}
