#include "WeatherService.hpp"

namespace WeatherEngine
{
    WeatherService::WeatherService(std::unique_ptr<IWeatherEngine> engine)
      : m_weatherEngine(std::move(engine))
      , m_cachedWeather(std::nullopt)
      , m_cacheDuration(60)
      , m_cachedCity("")
      , m_lastFetchTime(std::chrono::steady_clock::time_point{})
    {
    }

    std::optional<WeatherData> WeatherService::cityWeatherRequest(const std::string& city)
    {
      auto now = std::chrono::steady_clock::now();

      if (m_cachedWeather.has_value() && now - m_lastFetchTime < m_cacheDuration && m_cachedCity == city)
        return m_cachedWeather;
      
      m_cachedWeather = m_weatherEngine->fetchWeather(city);
      m_lastFetchTime = now;
      m_cachedCity = city;
      return m_cachedWeather;

    }
} 
