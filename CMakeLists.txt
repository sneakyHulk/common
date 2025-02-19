cmake_minimum_required(VERSION 3.26)
project(common)

set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}_test test/${PROJECT_NAME}_test.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_literals_test test/${PROJECT_NAME}_literals_test.cpp)
target_link_libraries(${PROJECT_NAME}_literals_test PRIVATE ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_output_test test/${PROJECT_NAME}_output_test.cpp)
target_link_libraries(${PROJECT_NAME}_output_test PRIVATE ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_math_test test/${PROJECT_NAME}_math_test.cpp)
target_link_libraries(${PROJECT_NAME}_math_test PRIVATE ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_exception_test test/${PROJECT_NAME}_exception_test.cpp)
target_link_libraries(${PROJECT_NAME}_exception_test PRIVATE ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_istream_test test/${PROJECT_NAME}_istream_test.cpp)
target_link_libraries(${PROJECT_NAME}_istream_test PRIVATE ${PROJECT_NAME})

enable_testing()

add_test(NAME ${PROJECT_NAME}_test COMMAND $<TARGET_FILE:${PROJECT_NAME}_test> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME ${PROJECT_NAME}_literals_test COMMAND $<TARGET_FILE:${PROJECT_NAME}_literals_test> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME ${PROJECT_NAME}_output_test COMMAND $<TARGET_FILE:${PROJECT_NAME}_output_test> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME ${PROJECT_NAME}_math_test COMMAND $<TARGET_FILE:${PROJECT_NAME}_math_test> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME ${PROJECT_NAME}_exception_test COMMAND $<TARGET_FILE:${PROJECT_NAME}_exception_test> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME ${PROJECT_NAME}_istream_test COMMAND $<TARGET_FILE:${PROJECT_NAME}_istream_test> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})