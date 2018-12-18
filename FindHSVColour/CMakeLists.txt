# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Using C++14 because we need a few components present in it
# Please Update Compiler On Target System to Latest VERSION
# Update Other Applications as well
set (CMAKE_CXX_STANDARD 14)

project ("FindHSVColour")

find_package(OPENCV REQUIRED)

INCLUDE_DIRECTORIES(PRIVATE ${OPENCV_INCLUDE_DIR})

# Add source to this project's executable.
add_executable (FindHSVColour "main.cxx")

target_link_libraries( FindHSVColour PRIVATE ${OpenCV_LIBS} )
