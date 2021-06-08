add_library(jungles_bme280_driver STATIC 
    bme280_driver.cpp bme280_driver.hpp bme280_measurement.hpp i2c_master.hpp)
target_include_directories(jungles_bme280_driver PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(internal)
target_link_libraries(jungles_bme280_driver PUBLIC jungles_bme280_driver_internal)

include(FetchContent)
FetchContent_Declare(
    JunglesOsHelpersRepo
    GIT_REPOSITORY https://github.com/KKoovalsky/JunglesOsHelpers.git
    GIT_TAG d0644fbb6eacbef5190a1c85f5640a3df60abb9b)
FetchContent_MakeAvailable(JunglesOsHelpersRepo)

target_link_libraries(jungles_bme280_driver PRIVATE jungles::os_helpers)
