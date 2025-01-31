#include <iostream>
#include "include/gradient/gradient_operator.h"
#include "include/gradient/ocv_sobel.h"
#include "include/gradient/alt_sobel.h"
#include "include/gradient/omp_sobel.h"
#include "include/gradient/ocv_prewitt.h"
#include "include/gradient/ocv_roberts_cross.h"
using namespace std;

// helper method that applies the operator and gets the edges and onwards.
void applyOperator(GradientOperator* operatorPtr, const string& inputPath, const string& outputPath) {
    cout << operatorPtr->getOperatorName() << " operator: " << endl;
    operatorPtr->getEdges(inputPath, outputPath);
    delete operatorPtr;
}

// main method that processes the input arguments from the backend and applies the operator.
int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: operators <operator> <input_path> <output_path>" << endl;
        return 1;
    }

    string operatorType = argv[1];
    string inputPath = argv[2];
    string outputPath = argv[3];

    try {
        if (operatorType == "opencv%20sobel") {
            OcvSobel sobelOperator;
            sobelOperator.getEdges(inputPath, outputPath);
        } else if (operatorType == "alternative%20sobel") {
            AltSobel altSobelOperator;
            altSobelOperator.getEdges(inputPath, outputPath);
        } else if (operatorType == "openmp%20sobel") {
            OmpSobel ompSobelOperator;
            ompSobelOperator.getEdges(inputPath, outputPath);
        } else if (operatorType == "prewitt") {
            OcvPrewitt prewittOperator;
            prewittOperator.getEdges(inputPath, outputPath);
        } else if (operatorType == "roberts%20cross") {
            OcvRobertsCross robertsCross;
            robertsCross.getEdges(inputPath, outputPath);
        } else {
            cerr << "Unknown operator: " << operatorType << endl;
            return 1;
        }
        cout << "Processing completed successfully!" << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

//    string inputPath = "/Users/kailinx/Desktop/EdgeUnity/backend/operators/test/gradient/datasets/image.jpg";
//    list<GradientOperator*> operators = {new OcvSobel(), new AltSobel(), new OmpSobel(), new OcvPrewitt(), new OcvRobertsCross()};
//    for (auto& operatorPtr : operators) {
//        string outputPath = "/Users/kailinx/Desktop/EdgeUnity/backend/operators/test/gradient/datasets/"+operatorPtr->getOperatorName()+".jpg";
//        applyOperator(operatorPtr, inputPath, outputPath);
//    }

    return 0;
}
