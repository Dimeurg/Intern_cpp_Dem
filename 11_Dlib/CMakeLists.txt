cmake_minimum_required(VERSION 2.8.12)
project(FaceFind)

add_subdirectory(../dlib dlib_build)
set(OpenCV_DIR "D:/Libs/openCV_test/opencv/x64/vc15/lib")
include_directories("D:/Libs/openCV_test/opencv/x64/vc15/bin")

macro(add_example name)
   add_executable(${name} ${name}.cpp)
   target_link_libraries(${name} dlib::dlib )
endmacro()


if (DLIB_NO_GUI_SUPPORT)
   message("No GUI support, so we won't build the webcam_face_pose_ex example.")
else()
   find_package(OpenCV QUIET)
   if (OpenCV_FOUND)
      include_directories(${OpenCV_INCLUDE_DIRS})

      add_executable(dlib_ex dlib_ex.cpp)
      target_link_libraries(dlib_ex dlib::dlib ${OpenCV_LIBS} )
   else()
      message("OpenCV not found, so we won't build the webcam_face_pose_ex example.")
   endif()
endif()
