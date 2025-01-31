#ifndef BACKEND_OCV_PREWITT_H
#define BACKEND_OCV_PREWITT_H

#include "gradient_operator.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

/**
 * @file ocv_prewitt.h
 * @brief This file contains the declaration of the OpenCV Prewitt class that uses the OpenCV libary to detect edges.
 */
class OcvPrewitt : public GradientOperator {
public:
    /**
     * @brief Constructs a OCVPrewitt object.
     */
    explicit OcvPrewitt();

    Mat getEdges(const string &inputPath, const string &outputName) override;

    string getOperatorName() const override;

private:
    /**
     * @brief Converts the input image to RGB.
     * @param image The input image.
     * @return The image in RGB format.
     */
    Mat convertToRGB(const cv::Mat &image);

    /**
     * @brief Converts the input image to grayscale.
     * @param image The input image.
     * @return The image in grayscale format.
     */
    Mat convertToGrayscale(const cv::Mat &image);

    /**
     * @brief Computes the gradient in the x-direction.
     * @param grayImage The input image.
     * @return The gradient in the x-direction.
     */
    Mat computeGradientX(const cv::Mat& grayImage);

    /**
     * @brief Computes the gradient in the y-direction.
     * @param grayImage The input image.
     * @return The gradient in the y-direction.
     */
    Mat computeGradientY(const cv::Mat& grayImage);

    /**
     * @brief Combines the gradients in the x and y directions.
     * @param gradX The gradient in the x-direction.
     * @param gradY The gradient in the y-direction.
     * @return The combined gradients.
     */
    Mat combineGradients(const cv::Mat& gradX, const cv::Mat& gradY);

};

#endif //BACKEND_OCV_PREWITT_H
