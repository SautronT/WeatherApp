#pragma once
#include "IWeatherEngine.hpp"
#include "WeatherData.hpp"
#include <nlohmann/json.hpp>

namespace WeatherEngine
{
    class TestWeatherEngine : public IWeatherEngine
    {
    public:
        explicit TestWeatherEngine(bool success = true) : m_success(success) {}
        std::optional<WeatherData> fetchWeather(const std::string& city) override;

        int getFetchCount() const { return m_fetchCounter;};

    private:
        bool m_success;
        int m_fetchCounter = 0;
    };
}
