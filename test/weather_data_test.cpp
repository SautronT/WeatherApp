#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>
#include "WeatherData.hpp"
#include "TestWeatherEngine.hpp"
#include "WeatherService.hpp"
using namespace WeatherEngine;

const std::string VALID_JSON = R"({
    "weather": [{"id": 804, "main": "Clouds", "description": "ciel couvert", "icon": "04d"}],
    "main": {
        "temp": 29.82,
        "feels_like": 30.79,
        "temp_min": 27.34,
        "temp_max": 30.6,
        "humidity": 50
    },
    "clouds": {"all": 99},
    "name": "Paris"
})";

TEST_CASE("WeatherData - parsing JSON valide", "[WeatherData]")
{
    auto result = WeatherData::getDataFromJSON(nlohmann::json::parse(VALID_JSON));

    REQUIRE(result.has_value());

    CHECK(result->cityName           == "Paris");
    CHECK(result->weatherType        == "Clouds");
    CHECK(result->weatherDescription == "overcast clouds");
    CHECK(result->weatherIconCode    == "04d");
    CHECK(result->temperature        == 29.82);
    CHECK(result->feltTemperature    == 30.79);
    CHECK(result->minTemperature     == 27.34);
    CHECK(result->maxTemperature     == 30.6);
    CHECK(result->humidityRate       == 50);
    CHECK(result->cloudRate          == 99);
}

TEST_CASE("WeatherData - missing information returns nullopt", "[WeatherData]")
{
    const std::string MISSING_FIELD_JSON = R"({
        "weather": [{"id": 804, "main": "Clouds", "description": "ciel couvert", "icon": "04d"}],
        "main": {
            "temp": 29.82,
            "feels_like": 30.79,
            "temp_min": 27.34,
            "temp_max": 30.6,
            "humidity": 50
        }
    })";
    // "clouds" and "name" are missing 

    auto result = WeatherData::getDataFromJSON(nlohmann::json::parse(MISSING_FIELD_JSON));
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("WeatherData - empty JSON returns nullopt", "[WeatherData]")
{
    auto result = WeatherData::getDataFromJSON(nlohmann::json::parse("{}"));
    REQUIRE_FALSE(result.has_value());
}


TEST_CASE("Weather Engine - fetch Weather Success", "[WeatherEngine]")
{
  auto weatherEngine = TestWeatherEngine(true);
  REQUIRE(weatherEngine.fetchWeather("").has_value());
}

TEST_CASE("Weather Engine - fetch Weather Error", "[WeatherEngine]")
{
  auto weatherEngine = TestWeatherEngine(false);
  REQUIRE_FALSE(weatherEngine.fetchWeather("").has_value());
}

TEST_CASE("Weather Service - cityWeatherRequest returns a result", "[WeatherService]")
{
    auto service = WeatherService(std::make_unique<TestWeatherEngine>(true));
    auto result = service.cityWeatherRequest("Paris");
    REQUIRE(result.has_value());
    CHECK(result->cityName == "Paris");
}

TEST_CASE("WeatherService - cityWeatherRequest returns nullopt if engine fails to get data", "[WeatherService]")
{
    auto service = WeatherService(std::make_unique<TestWeatherEngine>(false));
    auto result = service.cityWeatherRequest("Paris");
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("WeatherService - checks if cache is working", "[WeatherService]")
{
    auto* fake = new TestWeatherEngine(true);
    auto service = WeatherService(std::unique_ptr<TestWeatherEngine>(fake));

    service.cityWeatherRequest("Paris");
    service.cityWeatherRequest("Paris");

    CHECK(fake->getFetchCount() == 1);
}
