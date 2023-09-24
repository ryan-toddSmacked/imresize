cmake_minimum_required(VERSION 3.15)

project(ImageResizer)

set(CMAKE_CXX_STANDARD 11)

# This is the path to the conda environment that contains the opencv libraries and boost libraries
# opencv is used for image reading and writing and resizing
# boost is used for command line argument parsing
set(ENV ${CMAKE_CURRENT_SOURCE_DIR}/env)


add_executable(imresize imresize.cpp)
target_include_directories(imresize PUBLIC ${ENV}/include ${ENV}/include/opencv4)
target_link_directories(imresize PUBLIC ${ENV}/lib)
target_link_libraries(imresize
    opencv_core
    opencv_imgcodecs
    opencv_imgproc
    boost_program_options
    stdc++)

set(CMAKE_INSTALL_RPATH ${ENV}/lib)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


