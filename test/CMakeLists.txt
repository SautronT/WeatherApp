add_executable(weather_tests 
    weather_data_test.cpp
    TestWeatherEngine.cpp
)

target_link_libraries(weather_tests PRIVATE
    core
    Catch2::Catch2WithMain
    nlohmann_json::nlohmann_json
)

include(Catch)
catch_discover_tests(weather_tests)
