project(flexInvokableTests)

# Add flexinvokable include
add_executable(${PROJECT_NAME} src/unit.cpp src/examples.cpp src/catch.hpp)
target_include_directories(${PROJECT_NAME} PRIVATE src)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17
                                                 CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME} csari_flexInvokable)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
