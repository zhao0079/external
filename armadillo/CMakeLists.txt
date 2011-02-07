cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

rosbuild_init()

#MAKE_DIRECTORY(${PROJECT_SOURCE_DIR}/build2/)

#execute_process(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
#         COMMAND cmake ../armadillo-0.6.12/
#         RESULT_VARIABLE _make_failed)

if(_make_failed)
  message(FATAL_ERROR "Build of armadillo failed")
endif(_make_failed)

#include_directories(build/include)
#link_directories(lib)

#add_subdirectory(armadillo-0.8.2)
