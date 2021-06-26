cmake_minimum_required(VERSION 2.8.3)
project(fake_laser_pkg)

find_package(catkin REQUIRED COMPONENTS
  depthimage_to_laserscan
  nodelet
  roscpp
)

catkin_package()


include_directories(
  ${catkin_INCLUDE_DIRS}
)


