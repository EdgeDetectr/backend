#include "utils/path_helper.h"
using namespace std;

std::string PathHelper::getProjectPath() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    while (currentPath.has_parent_path()) {
        if (currentPath.filename() == "operators") {
            return currentPath.string();
        }
        currentPath = currentPath.parent_path();
    }
    throw std::runtime_error("Could not find the 'operators' folder.");
}
