cmake_minimum_required(VERSION 3.4)
project(zed_open_driver)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)
list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs
)

find_package( OpenCV REQUIRED )

message("* Video module available")
add_definitions(-DVIDEO_MOD_AVAILABLE)

find_package(LIBUSB 1 REQUIRED)
message(STATUS "libusb: include dir at ${LibUSB_INCLUDE_DIRS}")
message(STATUS "libusb: libraries at ${LibUSB_LIBRARIES}")
include_directories(${LibUSB_INCLUDE_DIRS})
set(SRC_FULL ${SRC_FULL} ${SRC_VIDEO})
set(HDR_FULL ${HDR_FULL} ${HEADERS_VIDEO})
set(DEP_LIBS ${DEP_LIBS}
     ${LibUSB_LIBRARIES}
     )




include_directories(
    include
    ${OpenCV_INCLUDE_DIRS}
    ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node src/zed_open_driver_node.cpp src/zed_open_driver_node.cpp)

target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
        zed_open_capture
)

