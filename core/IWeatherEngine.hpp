#pragma once
#include <string>
#include <optional>

#include "WeatherData.hpp"

namespace WeatherEngine
{
  class IWeatherEngine 
  {
    public :
    virtual std::optional<WeatherData> fetchWeather(const std::string& city) = 0;
    virtual ~IWeatherEngine() = default;
  };


}
