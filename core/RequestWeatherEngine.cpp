#include "RequestWeatherEngine.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <iostream>

namespace WeatherEngine
{

  std::optional<WeatherData> RequestWeatherEngine::fetchWeather(const std::string& city)
  {
    const char* apiKey = std::getenv("OPENWEATHER_KEY");
    if (!apiKey) {
        std::cerr << "Erreur : variable d'environnement OPENWEATHER_KEY non definie.\n";
        return std::nullopt;
    }

    cpr::Response response = cpr::Get(
        cpr::Url{"https://api.openweathermap.org/data/2.5/weather"},
        cpr::Parameters{{"appid", apiKey}, {"q", city}, {"units", "metric"}, {"lang", "fr"}}
    );


    if (response.status_code == 200)
    {
        nlohmann::json data = nlohmann::json::parse(response.text);
        return WeatherData::getDataFromJSON(data);
    }
    else 
        return std::nullopt;

  }
  
}
