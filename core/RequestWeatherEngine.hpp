#pragma once

#include "IWeatherEngine.hpp"
#include "WeatherData.hpp"
namespace WeatherEngine
{
  class RequestWeatherEngine : public IWeatherEngine
  {

    public:
    std::optional<WeatherData> fetchWeather(const std::string& city) override;

  };
}
