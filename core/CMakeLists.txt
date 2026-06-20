add_library(core STATIC
    RequestWeatherEngine.cpp
    WeatherService.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(core PUBLIC
    nlohmann_json::nlohmann_json
    cpr::cpr
)
