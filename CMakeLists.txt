cmake_minimum_required(VERSION 2.8.3)
project(SymphonyLakeDataset)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

find_package(PkgConfig REQUIRED)

# find_package(OpenCV 3 REQUIRED)
pkg_check_modules(OpenCV REQUIRED opencv)


###########
## Build ##
###########

set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_compile_options(-std=c++11 -g -ggdb -Wall)

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
)

## Declare a C++ library
add_library(${PROJECT_NAME}
  src/ParseSurvey.cpp
  src/Survey.cpp
  src/SurveyVector.cpp
  src/Function.cpp
)



add_executable(load_survey test/load_survey.cpp)
target_link_libraries(load_survey ${PROJECT_NAME} ${OpenCV_LIBRARIES})

