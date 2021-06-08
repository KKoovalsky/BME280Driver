cmake_minimum_required(VERSION 3.16)

project(JunglesBME280Driver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(bme280_driver)

set(JUNGLES_BME280_DRIVER_ENABLE_TESTING OFF CACHE BOOL "Enables self-testing of the library")
if(JUNGLES_BME280_DRIVER_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

