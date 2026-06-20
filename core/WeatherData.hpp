#pragma once
#include <nlohmann/json.hpp>
#include <optional>

namespace WeatherEngine
{
  struct WeatherData
  {
    std::string cityName;
    std::string weatherType;
    std::string weatherDescription;
    std::string weatherIconCode;

    double temperature;
    double feltTemperature;
    double minTemperature;
    double maxTemperature;
    int   humidityRate;
    int   cloudRate;

    static std::optional<WeatherData> getDataFromJSON(const nlohmann::json& json)
    {
        if (!json.is_object() || json.empty())
          return std::nullopt;

        try 
        {
            WeatherData data;
            data.cityName = json.at("name");
            data.weatherType = json.at("weather").at(0).at("main");
            data.weatherDescription = json.at("weather").at(0).at("description");
            data.weatherIconCode = json.at("weather").at(0).at("icon");

            data.temperature = json.at("main").at("temp");
            data.feltTemperature = json.at("main").at("feels_like");
            data.maxTemperature = json.at("main").at("temp_max");
            data.minTemperature = json.at("main").at("temp_min");
            data.humidityRate = json.at("main").at("humidity");
            data.cloudRate = json.at("clouds").at("all");

            return data;
        }
        catch (const nlohmann::json::exception&)
        {
            return std::nullopt;
        }

    }
  };
}
