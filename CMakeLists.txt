cmake_minimum_required(VERSION 3.28)
project(common)

set(CMAKE_CXX_STANDARD 23)

find_package(nlohmann_json CONFIG)
find_package(Eigen3 CONFIG)

if (nlohmann_json_FOUND AND Eigen3_FOUND)
    add_library(eigen_utils
            src/EigenJsonUtils.cpp
    )
    target_include_directories(eigen_utils PUBLIC include)
    target_link_libraries(eigen_utils PUBLIC nlohmann_json::nlohmann_json)
    target_link_libraries(eigen_utils PUBLIC Eigen3::Eigen)
endif ()

add_library(${PROJECT_NAME}
        src/common.cpp
        src/common_literals.cpp
        src/common_ostream.cpp
        src/common_istream.cpp
        src/common_output.cpp
        src/common_exception.cpp
        src/common_math.cpp
        src/common_linspace.cpp
        src/common_error.cpp
        src/common_ring_buffer.cpp
        src/common_time.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC include)

if (PROJECT_IS_TOP_LEVEL)
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

    add_executable(test_${PROJECT_NAME}_linspace test/test_${PROJECT_NAME}_linspace.cpp)
    target_link_libraries(test_${PROJECT_NAME}_linspace PUBLIC ${PROJECT_NAME})

    add_executable(test_${PROJECT_NAME}_error test/test_${PROJECT_NAME}_error.cpp)
    target_link_libraries(test_${PROJECT_NAME}_error PUBLIC ${PROJECT_NAME})

    add_executable(test_${PROJECT_NAME}_ring_buffer test/test_${PROJECT_NAME}_ring_buffer.cpp)
    target_link_libraries(test_${PROJECT_NAME}_ring_buffer PUBLIC ${PROJECT_NAME})

    add_executable(test_${PROJECT_NAME}_time test/test_${PROJECT_NAME}_time.cpp)
    target_link_libraries(test_${PROJECT_NAME}_time PUBLIC ${PROJECT_NAME})

    if (TARGET eigen_utils)
        add_executable(test_eigen_utils test/test_eigen_utils.cpp)
        target_link_libraries(test_eigen_utils PUBLIC eigen_utils)
    endif ()

    enable_testing()

    add_test(NAME test_${PROJECT_NAME} COMMAND $<TARGET_FILE:test_${PROJECT_NAME}> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_test(NAME test_${PROJECT_NAME}_literals COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_literals> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_test(NAME test_${PROJECT_NAME}_output COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_output> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_test(NAME test_${PROJECT_NAME}_math COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_math> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_test(NAME test_${PROJECT_NAME}_exception COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_exception> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_test(NAME test_${PROJECT_NAME}_istream COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_istream> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_test(NAME test_${PROJECT_NAME}_linspace COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_linspace> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_test(NAME test_${PROJECT_NAME}_error COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_error> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_test(NAME test_${PROJECT_NAME}_ring_buffer COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_ring_buffer> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_test(NAME test_${PROJECT_NAME}_time COMMAND $<TARGET_FILE:test_${PROJECT_NAME}_time> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    if (TARGET eigen_utils)
        add_test(NAME test_eigen_utils COMMAND $<TARGET_FILE:test_eigen_utils> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif ()
endif ()