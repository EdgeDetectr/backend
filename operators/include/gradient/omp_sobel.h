#ifndef OPERATORS_OMP_SOBEL_H
#define OPERATORS_OMP_SOBEL_H

#include <omp.h>
#include "gradient_operator.h"
using namespace std;
using namespace cv;

/**
 * @file ocv_sobel.h
 * @brief This file contains the declaration of the OcvSobel operator based on 
 * the Alt OcvSobel operator with OpenMP parallelization.
 */
class OmpSobel : public GradientOperator {
private:
    int ksize;  // kernel size for the OcvSobel operator
    double scale; // scaling factor for the gradient values
    double delta; // offset added to the gradient values
    int height; // height of the image
    int width; // width of the image

public:
    /**
     * Constructs an OmpSobel object.
     * @param kernelSize The size of the kernel for the OcvSobel operator. Default is 3.
     */
    explicit OmpSobel(int kernelSize = 3);

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
    Mat convertToRGB(const Mat& input) const;

    /**
     * @brief Converts the input image to grayscale.
     * The RGB values are converted to grayscale using the National Television System Committee formula:
     * 0.299 ∙ Red + 0.587 ∙ Green + 0.114 ∙ Blue.
     * @param rgbMatrix The input image in RGB format.
     * @return The image in grayscale format represented in a 2D vector.
     */
    Mat convertToGrayscale(const Mat& rgbImage) const;

    /**
     * @brief Computes the gradient in the x-direction.
     * @param input The input image.
     * @return The gradient in the x-direction.
     */
    [[nodiscard]] Mat computeGradientX(const Mat& grayImage) const;

        /**
         * @brief Computes the gradient in the y-direction.
         * @param input The input image.
         * @return The gradient in the y-direction.
         */
    [[nodiscard]] Mat computeGradientY(const Mat& grayImage) const;

    /**
     * @brief Combines the gradients in the x and y directions.
     * @param gradX The gradient in the x-direction.
     * @param gradY The gradient in the y-direction.
     * @return The combined gradients.
     */
    Mat combineGradients(const Mat& gradX, const Mat& gradY) const;
    };


#endif //OPERATORS_OMP_SOBEL_H
