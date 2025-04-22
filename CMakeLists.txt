cmake_minimum_required(VERSION 3.14)
project(RunRecord VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

# FITSDK
file(GLOB FitSDK "${PROJECT_SOURCE_DIR}/third_party/FitSDKRelease_21.158.00/cpp/*.cpp")
# net
file(GLOB RRNet "${PROJECT_SOURCE_DIR}/net/*.cc")

#
add_library(run_record "")
target_sources(run_record
  PUBLIC
    ${FitSDK}
    ${RRNet}
)

target_include_directories(run_record
  PUBLIC
    "${PROJECT_SOURCE_DIR}/third_party/FitSDKRelease_21.158.00/cpp/"
    "."
)

target_link_libraries(run_record workflow)


# TEST

enable_testing()
set(install_gtest OFF)

add_subdirectory("third_party/googletest")

add_executable(rr-tests "")
target_sources(rr-tests
  PUBLIC
    "tests/test.cc"
)
target_link_libraries(rr-tests run_record gtest gtest_main)
add_test(NAME "rr-tests" COMMAND "rr-tests")

# app
add_executable(rr-app "")
target_sources(rr-app
  PUBLIC
    "launch/app.cc"
)
target_link_libraries(rr-app run_record)