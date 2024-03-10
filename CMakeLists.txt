cmake_minimum_required(VERSION 3.28)
project(common)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
        INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)

add_executable(${PROJECT_NAME}_test test/${PROJECT_NAME}_test.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_literals_test test/${PROJECT_NAME}_literals_test.cpp)
target_link_libraries(${PROJECT_NAME}_literals_test PRIVATE ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_output_test test/${PROJECT_NAME}_output_test.cpp)
target_link_libraries(${PROJECT_NAME}_output_test PRIVATE ${PROJECT_NAME})

enable_testing()

add_test(
        NAME ${PROJECT_NAME}_test
        COMMAND $<TARGET_FILE:${PROJECT_NAME}_test>
)
add_test(
        NAME ${PROJECT_NAME}_literals_test
        COMMAND $<TARGET_FILE:${PROJECT_NAME}_literals_test>
)
add_test(
        NAME ${PROJECT_NAME}_output_test
        COMMAND $<TARGET_FILE:${PROJECT_NAME}_output_test>
)