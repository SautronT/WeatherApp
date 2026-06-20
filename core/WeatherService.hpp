#pragma once 

#include <memory>
#include <optional>
#include <string>
#include <chrono>

#include "IWeatherEngine.hpp"
namespace WeatherEngine
{
  class WeatherService
  {
    private : 
      std::unique_ptr<IWeatherEngine> m_weatherEngine;

      std::string m_cachedCity;
      std::optional<WeatherData> m_cachedWeather;
      std::chrono::steady_clock::time_point m_lastFetchTime;
      const std::chrono::seconds m_cacheDuration;

    public : 
      WeatherService(std::unique_ptr<IWeatherEngine> engine);
      std::optional<WeatherData> cityWeatherRequest(const std::string& city);

      

  };
}
