add_library(jungles_bme280_driver_internal STATIC 
    bme280_conversion.cpp bme280_conversion.hpp bme280_registers.hpp)
target_include_directories(jungles_bme280_driver_internal PUBLIC ${CMAKE_CURRENT_LIST_DIR})
