#include "TestWeatherEngine.hpp"

namespace WeatherEngine
{
    std::optional<WeatherData> TestWeatherEngine::fetchWeather(const std::string& city)
    {
        m_fetchCounter ++;

        if (!m_success)
            return std::nullopt;

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


        return WeatherData::getDataFromJSON(nlohmann::json::parse(VALID_JSON));
    }
}
