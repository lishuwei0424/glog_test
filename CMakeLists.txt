cmake_minimum_required(VERSION  2.8)
project(glog_test)

find_package(glog)

add_executable(main main.cpp)
target_link_libraries(main glog)