cmake_minimum_required(VERSION 3.22)

project(utils)

set(CMAKE_CXX_STANDARD 23)

find_package(nlohmann_json REQUIRED)
find_package(Eigen3 REQUIRED)

project(eigen_utils)

add_library(${PROJECT_NAME} src/EigenJsonUtils.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PUBLIC Eigen3::Eigen)

project(test_eigen_utils)

add_executable(${PROJECT_NAME} test/test_eigen_utils.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC eigen_utils)
target_link_libraries(${PROJECT_NAME} PUBLIC common)
target_compile_definitions(${PROJECT_NAME} PRIVATE CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

enable_testing()

add_test(NAME test_eigen_utils COMMAND $<TARGET_FILE:test_eigen_utils> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
