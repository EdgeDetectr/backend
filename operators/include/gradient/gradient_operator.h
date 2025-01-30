#ifndef GRADIENT_OPERATOR_H
#define GRADIENT_OPERATOR_H

#include <opencv2/core.hpp>
#include <string>

/**
 * @file Operator.cpp
 * @brief This file contains the declaration of the Operator class.
 */
class GradientOperator {
public:

    /**
     * @brief Detects edges in the input image.
     */
    virtual ~GradientOperator() = default;

    /**
     * @brief Detects edges in the input image.
     * @param input The input image.
     * @throws std::runtime_error if the input image is empty.
     * @return The image with the edges detected.
     */
    virtual cv::Mat getEdges(const std::string& inputPath, const std::string& outputName) = 0;

    /**
     * @brief Get the name of the operator.
     *
     * @return The name of the operator.
     */
    [[nodiscard]] virtual std::string getOperatorName() const = 0;
};

#endif // GRADIENT_OPERATOR_H
