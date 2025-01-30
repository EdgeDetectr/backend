cmake_minimum_required(VERSION 3.30)
project(operators)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)
include_directories(src)

add_executable(operators
        src/gradient/ocv_sobel.cpp
        include/gradient/ocv_sobel.h
        main.cpp
        include/gradient/gradient_operator.h
        src/gradient/alt_sobel.cpp
        include/gradient/alt_sobel.h
        src/utils/image_utils.cpp
        include/utils/image_utils.h
        include/utils/sobel_util.h
        src/utils/path_helper.cpp
        include/utils/path_helper.h
        src/gradient/omp_sobel.cpp
        include/gradient/omp_sobel.h
)

target_link_libraries(operators ${OpenCV_LIBS} OpenMP::OpenMP_CXX)