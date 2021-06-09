################################################################################
# Macros
################################################################################


macro(DownloadAndPopulateCatch2)
    set(CATCH_BUILD_TESTING OFF CACHE BOOL "Internal Catch2's option to disable Catch2 self-test")
    set(BUILD_TESTING OFF CACHE BOOL "Internal Catch2's option to disable Catch2 self-test")

    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG 0fa133a0c5e065065ef96ac2b6c0284cf5da265d
    )
    FetchContent_MakeAvailable(Catch2)
endmacro()


macro(CreateTests)
    add_executable(jungles_bme280_driver_tests test_conversion.cpp)
    target_link_libraries(jungles_bme280_driver_tests PRIVATE Catch2::Catch2WithMain jungles::bme280_driver)
    target_compile_options(jungles_bme280_driver_tests PRIVATE -Wall -Wextra)
    add_test(NAME test_jungles_bme280_driver COMMAND 
        valgrind --leak-check=full $<TARGET_FILE:jungles_bme280_driver_tests>)
endmacro()


################################################################################
# Main script
################################################################################


DownloadAndPopulateCatch2()
CreateTests()
