#ifndef OPERATORS_OCV_ROBERTS_CROSS_H
#define OPERATORS_OCV_ROBERTS_CROSS_H

#include "gradient_operator.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

/**
 * @file ocv_roberts_cross.h
 * @brief This file contains the declaration of the OpenCV Roberts Cross class that uses the OpenCV library to detect edges.
 */
class OcvRobertsCross : public GradientOperator {
public:
    /**
     * @brief Constructs a OcvRobertsCross object.
     * @param kernelSize The size of the kernel.
     * @return The OcvRobertsCross object.
     */
    explicit OcvRobertsCross(int kernelSize = 2);

    /**
     * @brief Gets the edges of the input image.
     * @param inputPath The input path.
     * @param outputName The output path.
     * @return The edges of the input image.
     */
    Mat getEdges(const string &inputPath, const string &outputName) override;

    /**
     * @brief Gets the operator name.
     * @return The operator name.
     */
    string getOperatorName() const override;

private:

    /**
     * @brief Converts the input image to RGB.
     * @param image The input image.
     * @return The image in RGB format.
     */
    Mat convertToRGB(const cv::Mat &image);

    /**
     * @brief Converts the input image to RGB.
     * @param image The input image.
     * @return The image in RGB format.
     */
    Mat convertToGrayscale(const Mat& image);

    /**
     * @brief Converts the input image to grayscale.
     * @param grayImage The input image.
     * @return The gradient in the x-direction.
     */
    Mat computeGradientX(const Mat& grayImage);

    /**
     * @brief Computes the gradient in the y-direction.
     * @param grayImage The input image.
     * @return The gradient in the y-direction.
     */
    Mat computeGradientY(const Mat& grayImage);

    /**
     * @brief Combines the gradients in the x and y directions.
     * @param gradX The gradient in the x-direction.
     * @param gradY The gradient in the y-direction.
     * @return The combined gradients.
     */
    Mat combineGradients(const Mat& gradX, const Mat& gradY);

};


#endif //OPERATORS_OCV_ROBERTS_CROSS_H
