# BME280 Driver

This is a fully portable BME280 driver. It can be used on any platform. The platform-dependent modules are abstracted
out.

The driver configures BME280 for a single shot measurements. No configuration layer is provided.

## Usage

```
#include "bme280_driver.hpp"

// ...

jungles::BME280Driver bme280_driver{i2c_master, millisecond_delayer};

auto [temperature, pressure, humidity] = bme280_driver.read();

// Use temperature, pressure, humidity ...

``` 

The `i2c_master` must implement `jungles::I2CMaster` interface for your platform.

`millisecond_delayer` is a callable that delays for millisecond. It takes `std::chrono::millisecond` as a parameter. 

For NRF52 SDK that might be:

```
auto millisecond_delayer{[](std::chrono::milliseconds delay) {
    auto delay_ms{static_cast<unsigned>(delay.count())};
    nrf_delay_ms(delay_ms);
}};
```

For FreeRTOS:

```
auto millisecond_delayer{[](std::chrono::milliseconds delay) {
    auto delay_ms{static_cast<unsigned>(delay.count())};
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
}};
```

## Incorporating the library to your project

CMake is supported only. One can add the sources to the codebase manually when using non-CMake project.

C++17 features are used, so make sure that corresponding compile flags are used.

One can import the library like that:

    include(FetchContent)
    FetchContent_Declare(
        JunglesBME280Driver
        GIT_REPOSITORY https://github.com/KKoovalsky/BME280Driver.git
        # GIT_TAG <Here the latest commit hash for this repo's "main" branch>
    )
    FetchContent_MakeAvailable(JunglesBME280Driver)

    # consume "jungles::bme280_driver" target, e.g.:
    add_executable(some_exec main.cpp)
    target_link_libraries(some_exec PRIVATE jungles::bme280_driver)

