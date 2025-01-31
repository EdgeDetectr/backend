#include "gradient/ocv_roberts_cross.h"
#include "utils/image_utils.h"
#include "utils/kernels_util.h"

OcvRobertsCross::OcvRobertsCross(int kernelSize) {}

Mat OcvRobertsCross::getEdges(const std::string& inputPath, const std::string& outputName) {
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

string OcvRobertsCross::getOperatorName() const {
    return "OcvRobertsCross";
}

Mat OcvRobertsCross::convertToRGB(const cv::Mat &image) {
    Mat rgbImage;
    cvtColor(image, rgbImage, COLOR_BGR2RGB);
    return rgbImage;
}

Mat OcvRobertsCross::convertToGrayscale(const Mat& image) {
    Mat grayImage;
    cvtColor(image, grayImage, COLOR_RGB2GRAY);
    return grayImage;
}

Mat OcvRobertsCross::computeGradientX(const Mat& grayImage) {
    Mat gradX;
    filter2D(grayImage, gradX, CV_64F, KernelUtil::robertCrossX);
    return gradX;
}

Mat OcvRobertsCross::computeGradientY(const Mat& grayImage) {
    Mat gradY;
    filter2D(grayImage, gradY, CV_64F, KernelUtil::robertCrossY);
    return gradY;
}

cv::Mat OcvRobertsCross::combineGradients(const cv::Mat& gradX, const cv::Mat& gradY) {
    cv::Mat edges;
    cv::magnitude(gradX, gradY, edges);
    cv::normalize(edges, edges, 0, 255, cv::NORM_MINMAX, CV_8U);
    return edges;
}