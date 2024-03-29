cmake_minimum_required(VERSION 3.8)
##
## PROJECT
## name and version
##
project(csari_flexInvokable VERSION 1.0.1 LANGUAGES CXX)
##
## OPTIONS
##
## No options yet

##
## TARGET
## create target and add include path
##
add_library(${PROJECT_NAME} INTERFACE)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE ./include)

## Another project to display csari_flexInvokable on project list
add_custom_target(${PROJECT_NAME}_ SOURCES ./include/csari/flexInvokable.hpp)
set_target_properties(${PROJECT_NAME}_ PROPERTIES FOLDER flexInvokable PROJECT_LABEL ${PROJECT_NAME})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
file(WRITE ${PROJECT_BINARY_DIR}/flexInvokable.cmake "
cmake_minimum_required(VERSION 3.8)
##
## PROJECT
project(csari_flexInvokable VERSION 1.0.1 LANGUAGES CXX)

##
## TARGET
add_library(${PROJECT_NAME} INTERFACE)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE ./include)
")

install(FILES "${PROJECT_BINARY_DIR}/flexInvokable.cmake" DESTINATION .)
install(FILES "${PROJECT_SOURCE_DIR}/../LICENSE" DESTINATION .)
install(FILES "${PROJECT_SOURCE_DIR}/../readme.md" DESTINATION .)