cmake_minimum_required(VERSION 3.12)
project( DisplayImage )
set(OpenCV_DIR "D:/Libs/openCV_test/opencv/x64/vc15/lib")
include_directories("D:/Libs/openCV_test/opencv/x64/vc15/bin")
include_directories("D:/Libs/openCV_test/opencv/include")
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( DisplayImage opencv.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )