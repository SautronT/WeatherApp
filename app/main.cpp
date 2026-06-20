#include <ftxui/dom/elements.hpp>

#include <cstdlib>
#include <iostream>
#include <vector>
#include <optional>
#include <format>
#include <future>
#include <mutex>

#include "WeatherService.hpp"
#include "WeatherData.hpp"
#include "RequestWeatherEngine.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>

using namespace WeatherEngine;

const std::vector<std::string> cities = {
    "Paris", "Lyon", "Marseille", "Bordeaux",
    "Lille", "Toulouse", "Nantes", "Strasbourg"
};

int main()
{

  int cityIndex = 0;
  std::optional<WeatherData> currentWeatherData = std::nullopt;
  int previousCityIndex = -1;

  // Threading 
  std::mutex weatherMutex;
  std::future<std::optional<WeatherData>> weatherFuture;
  bool isLoading = false;

  WeatherService service = WeatherService(std::make_unique<RequestWeatherEngine>());
  
  auto screen = ftxui::ScreenInteractive::Fullscreen();

  auto menu = ftxui::Menu(&cities, &cityIndex);

  auto container = ftxui::Container::Vertical({menu});

  auto renderer = ftxui::Renderer(container, [&] {

    if (cityIndex != previousCityIndex && !isLoading)
    {
        previousCityIndex = cityIndex; 
        isLoading = true;
        weatherFuture = std::async(std::launch::async, [&]{
            auto result = service.cityWeatherRequest(cities[cityIndex]);
            screen.PostEvent(ftxui::Event::Custom);
            return result;
        });
    }
    

    // Check thread is done
    if (isLoading && weatherFuture.valid())
    {
        auto status = weatherFuture.wait_for(std::chrono::milliseconds(0));
        if (status == std::future_status::ready)
        {
            std::lock_guard<std::mutex> lock(weatherMutex);
            currentWeatherData = weatherFuture.get();
            isLoading = false;
        }
    }

    // 
    ftxui::Element rightPanel;

    if (isLoading)
    {
        rightPanel = ftxui::vbox({
            ftxui::text("Chargement...") | ftxui::bold | ftxui::center | ftxui::blink,
        }) | ftxui::flex;
    }
    else if (!currentWeatherData.has_value())
    {
        rightPanel = ftxui::vbox({
            ftxui::text("Erreur : impossible de recuperer la meteo.")
                | ftxui::color(ftxui::Color::Red) | ftxui::center,
        }) | ftxui::flex;
    }
    else
    {
        auto& weather = *currentWeatherData;

        // Panneau Conditions
        auto conditionsPanel = ftxui::vbox({
           ftxui::text(" Conditions") | ftxui::bold | ftxui::color(ftxui::Color::Green),
            ftxui::separator(),
            ftxui::text(" " + weather.weatherDescription) | ftxui::bold | ftxui::color(ftxui::Color::Cyan),
        }) | ftxui::border | ftxui::xflex;

        // Panneau Temperatures
        auto tempPanel = ftxui::vbox({
            ftxui::text(" Temperatures") | ftxui::bold| ftxui::color(ftxui::Color::Green),
            ftxui::separator(),
            ftxui::hbox({
                ftxui::text("  Actuelle  ") | ftxui::color(ftxui::Color::White),
                ftxui::text(std::format("{:.1f} C", weather.temperature)) | ftxui::bold,
            }),
            ftxui::hbox({
                ftxui::text("  Ressenti  ") | ftxui::color(ftxui::Color::White),
                ftxui::text(std::format("{:.1f} C", weather.feltTemperature)),
            }),
            ftxui::hbox({
                ftxui::text("  Min / Max ") | ftxui::color(ftxui::Color::White),
                ftxui::text(std::format("{:.1f} / {:.1f} C", weather.minTemperature, weather.maxTemperature)),
            }),
        }) | ftxui::border | ftxui::xflex;

        // Panneau Atmospherique
        auto atmosPanel = ftxui::vbox({
            ftxui::text(" Données ambiantes") | ftxui::bold | ftxui::color(ftxui::Color::Green),
            ftxui::separator(),
          
            ftxui::hbox({
                ftxui::text("  Humidité  ") | ftxui::color(ftxui::Color::White),
                ftxui::text(std::format("{}%", weather.humidityRate)),
            }),
            ftxui::hbox({
                ftxui::text("  Nuages    ") | ftxui::color(ftxui::Color::White),
                ftxui::text(std::format("{}%", weather.cloudRate)),
            }),
        }) | ftxui::border | ftxui::xflex;

        rightPanel = ftxui::vbox({
            ftxui::text(" " + cities[cityIndex]) | ftxui::bold | ftxui::color(ftxui::Color::Yellow),
            ftxui::separator(),
            conditionsPanel,
            tempPanel,
            atmosPanel,
        }) | ftxui::flex;
    }

    return ftxui::vbox({
        // Header
        ftxui::text("  Weather App") | ftxui::bold | ftxui::color(ftxui::Color::Cyan),
        ftxui::separator(),
        // Corps
        ftxui::hbox({
            // Panneau gauche
            ftxui::vbox({
                ftxui::text(" Villes") | ftxui::bold,
                ftxui::separator(),
                menu->Render(),
            }) | ftxui::border,
            // Panneau droit
            rightPanel,
        }) | ftxui::flex,
    }) | ftxui::border | ftxui::flex;
});
    screen.Loop(renderer);


  return 0;
}


