cmake_minimum_required(VERSION 2.8.3)
project(youbot_gazebo_publisher)

find_package(catkin REQUIRED COMPONENTS
  brics_actuator
  geometry_msgs
  trajectory_msgs
  roscpp
)

catkin_package()

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(youbot_gazebo_publisher src/youbot_gazebo_publisher.cpp)


target_link_libraries(youbot_gazebo_publisher
  ${catkin_LIBRARIES}
)

install(TARGETS youbot_gazebo_publisher youbot_gazebo_publisher
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

