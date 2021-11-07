cmake_minimum_required(VERSION 3.8)
project(flexInvokable)
##
## TESTS
## create and configure the unit test target
##
enable_testing()
add_subdirectory(flexInvokable)
add_subdirectory(test)
