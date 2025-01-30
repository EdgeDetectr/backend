#ifndef OPERATORS_ALT_SOBEL_H
#define OPERATORS_ALT_SOBEL_H

#include "gradient/gradient_operator.h"
using namespace std;
using namespace cv;

/**
 * @file alt_sobel.h
 * @brief This file contains the declaration of the Alternative Sobel class that has all operators 
 * handwritten using pixels instead of using OpenCV functions.
 */
class AltSobel : public GradientOperator {
private:
    int ksize;  // kernel size for the OcvSobel operator
    double scale; // scaling factor for the gradient values
    double delta; // offset added to the gradient values
    int height; // height of the image
    int width; // width of the image

public:
    /**
     * Constructs an AltSobel object.
     * @param kernelSize The size of the kernel for the OcvSobel operator. Default is 3.
     */
     explicit AltSobel(int kernelSize = 3);

    /**
     * @brief Detects edges in the input image.
     * @param input The input image.
     * @throws runtime_error if the input image is empty.
     * @return The image with the edges detected.
     */
    Mat getEdges(const string& inputPath, const string& outputName) override;

    /**
     * @brief Get the name of the operator.
     *
     * @return The name of the operator.
     */
    [[nodiscard]] virtual string getOperatorName() const override;

private:

    /**
     * @brief Converts the input image to RGB.
     * @param input The input image.
     * @return The image in RGB format represented in a 3D vector.
     */
     [[nodiscard]] vector<vector<vector<uint8_t>>> convertToRGB(const Mat& input) const;

    /**
     * @brief Converts the input image to grayscale.
     * The RGB values are converted to grayscale using the NTSC formula:
     * 0.299 ∙ Red + 0.587 ∙ Green + 0.114 ∙ Blue.
     * @param rgbMatrix The input image in RGB format.
     * @return The image in grayscale format represented in a 2D vector.
     */
    [[nodiscard]] vector<vector<uint8_t>> convertToGrayscale(const vector<vector<vector<uint8_t>>>& rgbMatrix) const;

    /**
     * @brief Computes the gradient in the x-direction.
     * @param input The input image.
     * @return The gradient in the x-direction.
     */
    [[nodiscard]] vector<vector<int>> computeGradientX(const vector<vector<uint8_t>>& grayImage) const;

    /**
     * @brief Computes the gradient in the y-direction.
     * @param input The input image.
     * @return The gradient in the y-direction.
     */
    [[nodiscard]] vector<vector<int>> computeGradientY(const vector<vector<uint8_t>>& grayImage) const;

    /**
     * @brief Combines the gradients in the x and y directions.
     * @param gradX The gradient in the x-direction.
     * @param gradY The gradient in the y-direction.
     * @return The combined gradients.
     */
    Mat combineGradients(const vector<vector<int>>& gradX, const vector<vector<int>>& gradY) const;
};


#endif //OPERATORS_ALT_SOBEL_H
