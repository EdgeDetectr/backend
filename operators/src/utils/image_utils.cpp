#include "../include/utils/image_utils.h"

cv::Mat ImageUtils::getImage(
        const std::string &inputPath
) {
    cv:: Mat image = cv::imread(inputPath, cv::IMREAD_COLOR);
    if (image.empty()) {
        throw std::runtime_error("Could not read the image: " + inputPath);
    }
    return image;
}

void ImageUtils::writeImage(const cv::Mat& image, const std::string& outputName) {
    cv::imwrite(outputName, image);
}