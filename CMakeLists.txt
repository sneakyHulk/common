cmake_minimum_required(VERSION 3.26)
project(common)

set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME}
        src/common.cpp
        src/common_literals.cpp
        src/common_ostream.cpp
        src/common_istream.cpp
        src/common_output.cpp
        src/common_exception.cpp
        src/common_math.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(test_${PROJECT_NAME} test/test_${PROJECT_NAME}.cpp)
target_link_libraries(test_${PROJECT_NAME} PUBLIC ${PROJECT_NAME})

add_executable(test_${PROJECT_NAME}_literals test/test_${PROJECT_NAME}_literals.cpp)
target_link_libraries(test_${PROJECT_NAME}_literals PUBLIC ${PROJECT_NAME})

add_executable(test_${PROJECT_NAME}_output test/test_${PROJECT_NAME}_output.cpp)
target_link_libraries(test_${PROJECT_NAME}_output PUBLIC ${PROJECT_NAME})

add_executable(test_${PROJECT_NAME}_math test/test_${PROJECT_NAME}_math.cpp)
target_link_libraries(test_${PROJECT_NAME}_math PUBLIC ${PROJECT_NAME})

add_executable(test_${PROJECT_NAME}_exception test/test_${PROJECT_NAME}_exception.cpp)
target_link_libraries(test_${PROJECT_NAME}_exception PUBLIC ${PROJECT_NAME})

add_executable(test_${PROJECT_NAME}_istream test/test_${PROJECT_NAME}_istream.cpp)
target_link_libraries(test_${PROJECT_NAME}_istream PUBLIC ${PROJECT_NAME})

enable_testing()

add_test(NAME ${PROJECT_NAME}_test COMMAND $<TARGET_FILE:test_${PROJECT_NAME}> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME ${PROJECT_NAME}_literals_test COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_literals> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME ${PROJECT_NAME}_output_test COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_output> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME ${PROJECT_NAME}_math_test COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_math> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME ${PROJECT_NAME}_exception_test COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_exception> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME ${PROJECT_NAME}_istream_test COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_istream> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})