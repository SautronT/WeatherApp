#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>
#include "WeatherData.hpp"
#include "TestWeatherEngine.hpp"
#include "WeatherService.hpp"
using namespace WeatherEngine;

const std::string VALID_JSON = R"({
    "weather": [{"id": 804, "main": "Clouds", "description": "overcast clouds", "icon": "04d"}],
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

TEST_CASE("WeatherData - champ manquant retourne nullopt", "[WeatherData]")
{
    const std::string MISSING_FIELD_JSON = R"({
        "weather": [{"id": 804, "main": "Clouds", "description": "overcast clouds", "icon": "04d"}],
        "main": {
            "temp": 29.82,
            "feels_like": 30.79,
            "temp_min": 27.34,
            "temp_max": 30.6,
            "humidity": 50
        }
    })";
    // "clouds" et "name" sont pas là 

    auto result = WeatherData::getDataFromJSON(nlohmann::json::parse(MISSING_FIELD_JSON));
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("WeatherData - JSON vide retourne nullopt", "[WeatherData]")
{
    auto result = WeatherData::getDataFromJSON(nlohmann::json::parse("{}"));
    REQUIRE_FALSE(result.has_value());
}


TEST_CASE("Weather Engine - Fetch Weather Success", "[WeatherEngine]")
{
  auto weatherEngine = TestWeatherEngine(true);
  REQUIRE(weatherEngine.fetchWeather("").has_value());
}

TEST_CASE("Weather Engine - Fetch Weather Error", "[WeatherEngine]")
{
  auto weatherEngine = TestWeatherEngine(false);
  REQUIRE_FALSE(weatherEngine.fetchWeather("").has_value());
}

TEST_CASE("Weather Service - cityWeatherRequest retourne bien un resultat", "[WeatherService]")
{
    auto service = WeatherService(std::make_unique<TestWeatherEngine>(true));
    auto result = service.cityWeatherRequest("Paris");
    REQUIRE(result.has_value());
    CHECK(result->cityName == "Paris");
}

TEST_CASE("WeatherService - cityWeatherRequest retourne nullopt si engine echoue", "[WeatherService]")
{
    auto service = WeatherService(std::make_unique<TestWeatherEngine>(false));
    auto result = service.cityWeatherRequest("Paris");
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("WeatherService - cache evite un deuxieme appel reseau", "[WeatherService]")
{
    auto* fake = new TestWeatherEngine(true);
    auto service = WeatherService(std::unique_ptr<TestWeatherEngine>(fake));

    service.cityWeatherRequest("Paris");
    service.cityWeatherRequest("Paris");

    CHECK(fake->getFetchCount() == 1);
}
