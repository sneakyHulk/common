cmake_minimum_required(VERSION 3.26)
project(common)

set(CPM_DOWNLOAD_VERSION 0.38.7)
set(CPM_HASH_SUM "83e5eb71b2bbb8b1f2ad38f1950287a057624e385c238f6087f94cdfc44af9c5")

if (CPM_SOURCE_CACHE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif (DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else ()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif ()

# Expand relative path. This is important if the provided path contains a tilde (~)
get_filename_component(CPM_DOWNLOAD_LOCATION ${CPM_DOWNLOAD_LOCATION} ABSOLUTE)

file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
        ${CPM_DOWNLOAD_LOCATION} EXPECTED_HASH SHA256=${CPM_HASH_SUM}
)

include(${CPM_DOWNLOAD_LOCATION})

#CPMAddPackage(
#        NAME matplotplusplus
#        GITHUB_REPOSITORY alandefreitas/matplotplusplus
#        GIT_TAG origin/master # or whatever tag you want
#        OPTIONS "MATPLOTPP_BUILD_EXPERIMENTAL_OPENGL_BACKEND ON"
#)

CPMAddPackage(
        NAME matplotlibcpp17
        GITHUB_REPOSITORY soblin/matplotlibcpp17
        GIT_TAG origin/master
)


CPMAddPackage(
        NAME Boost
        VERSION 1.84.0
        URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz
        URL_HASH SHA256=2e64e5d79a738d0fa6fb546c6e5c2bd28f88d268a2a080546f74e5ff98f29d0e
        OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_INCLUDE_LIBRARIES container\\\;asio" # Note the escapes!
)


# matplotlib-cpp
#find_package(Python3 COMPONENTS Interpreter Development.Embed)
#if (${Python3_FOUND})
#    CPMAddPackage(
#            NAME matplotlib_cpp
#            GITHUB_REPOSITORY lava/matplotlib-cpp
#            GIT_TAG origin/master
#            DOWNLOAD_ONLY YES
#    )
#    add_library(matplotlib_cpp INTERFACE)
#    target_include_directories(matplotlib_cpp INTERFACE ${matplotlib_cpp_SOURCE_DIR})
#    target_link_libraries(matplotlib_cpp INTERFACE Python3::Python Python3::Module)
#    target_compile_features(matplotlib_cpp INTERFACE cxx_std_11)
#
#    find_package(Python3 COMPONENTS NumPy)
#    if (${Python3_NumPy_FOUND})
#        target_link_libraries(matplotlib_cpp INTERFACE Python3::NumPy)
#    else ()
#        message(WARNING "Python3 NumPy not found, proceeding with -DWITHOUT_NUMPY."
#                " Some functions of `matplotlibcpp.h` might not work as expected.")
#        message(${Python3_EXECUTABLE})
#        target_compile_definitions(matplotlib_cpp INTERFACE WITHOUT_NUMPY)
#    endif ()
#else ()
#    message(WARNING "Python3 not found."
#            "\nPlease install it to use any function of `matplotlibcpp.h`.")
#endif ()


add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
        INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

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

add_executable(${PROJECT_NAME}_graph_test test/${PROJECT_NAME}_graph_test.cpp)
target_include_directories(${PROJECT_NAME}_graph_test PRIVATE)
target_link_libraries(${PROJECT_NAME}_graph_test PRIVATE ${PROJECT_NAME} matplotlibcpp17::matplotlibcpp17 Boost::asio Boost::container)

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
add_test(
        NAME ${PROJECT_NAME}_math_test
        COMMAND $<TARGET_FILE:${PROJECT_NAME}_math_test>
)
add_test(
        NAME ${PROJECT_NAME}_graph_test
        COMMAND $<TARGET_FILE:${PROJECT_NAME}_graph_test>
)
add_test(
        NAME ${PROJECT_NAME}_exception_test
        COMMAND $<TARGET_FILE:${PROJECT_NAME}_exception_test>
)