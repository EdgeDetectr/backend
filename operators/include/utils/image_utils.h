#ifndef OPERATORS_IMAGE_UTILS_H
#define OPERATORS_IMAGE_UTILS_H

#include <opencv2/opencv.hpp>
using namespace std;

class ImageUtils {
public:
    /**
     * @brief Reads an image from the specified file.
     * @param inputPath The path to the input image.
     * @return The image.
     */
    static cv::Mat getImage(const std::string &inputPath);

    /**
     * @brief Writes an image to the specified file.
     * @param image The image to write.
     * @param filename The name of the output file.
     */
    static void writeImage(const cv::Mat& image, const std::string& outputName);
};


#endif //OPERATORS_IMAGE_UTILS_H
