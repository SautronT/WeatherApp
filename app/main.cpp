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

int main()
{

  std::string inputCity = "Paris";
  std::optional<WeatherData> currentWeatherData = std::nullopt;

  // Threading 
  std::mutex weatherMutex;
  std::future<std::optional<WeatherData>> weatherFuture;
  bool isLoading = false;
  bool hasRequestedOnce = false;

  WeatherService service = WeatherService(std::make_unique<RequestWeatherEngine>());
  
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  
  auto cityInput = ftxui::Input(&inputCity, "Entrer le nom de la ville ..." , ftxui::InputOption
  {

        .multiline = false,    
        .on_enter = [&]{
            if (isLoading || inputCity.empty()) return;
            
            isLoading = true;
            std::string city = inputCity;
            hasRequestedOnce = true; 
            weatherFuture = std::async(std::launch::async, [&, city]{
                auto result = service.cityWeatherRequest(city);
                screen.PostEvent(ftxui::Event::Custom);
                return result;
            });
        }
    
  });

  auto container = ftxui::Container::Vertical({cityInput});

  auto renderer = ftxui::Renderer(container, [&] {

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

    

    
    // right side panell
    ftxui::Element rightPanel;

    if (isLoading)
    {
        rightPanel = ftxui::vbox
        ({
            ftxui::text("Chargement...") | ftxui::bold | ftxui::center | ftxui::blink,
        }) | ftxui::flex;
    }
    else if (!hasRequestedOnce)
      {
          rightPanel = ftxui::vbox({
              ftxui::text("  Entrez une ville et appuyez sur Entrée.")
                  | ftxui::color(ftxui::Color::GrayDark),
          }) | ftxui::flex;
      }
      else if (!currentWeatherData.has_value())
      {
          rightPanel = ftxui::vbox({
              ftxui::text("  Ville introuvable, vérifiez l'orthographe.")
                  | ftxui::color(ftxui::Color::Red),
          }) | ftxui::flex;
      }    
    else
    {
        auto& weather = *currentWeatherData;

        // conditions panel 
        auto conditionsPanel = ftxui::vbox
        ({
           ftxui::text(" Conditions") | ftxui::bold | ftxui::color(ftxui::Color::Green),
            ftxui::separator(),
            ftxui::text(" " + weather.weatherDescription) | ftxui::bold | ftxui::color(ftxui::Color::Cyan),
        }) | ftxui::border | ftxui::xflex;

        // temperature panel
        auto tempPanel = ftxui::vbox
        ({
            ftxui::text(" Temperatures") | ftxui::bold| ftxui::color(ftxui::Color::Green),
            ftxui::separator(),
            ftxui::hbox
            ({
                ftxui::text("  Actuelle  ") | ftxui::color(ftxui::Color::White),
                ftxui::text(std::format("{:.1f} C", weather.temperature)) | ftxui::bold,
            }),
            ftxui::hbox
            ({
                ftxui::text("  Ressenti  ") | ftxui::color(ftxui::Color::White),
                ftxui::text(std::format("{:.1f} C", weather.feltTemperature)),
            }),
            ftxui::hbox
            ({
                ftxui::text("  Min / Max ") | ftxui::color(ftxui::Color::White),
                ftxui::text(std::format("{:.1f} / {:.1f} C", weather.minTemperature, weather.maxTemperature)),
            }),
        }) | ftxui::border | ftxui::xflex;

        // ambient data panel 
        auto atmosPanel = ftxui::vbox
        ({
            ftxui::text(" Données ambiantes") | ftxui::bold | ftxui::color(ftxui::Color::Green),
            ftxui::separator(),
          
            ftxui::hbox
            ({
                ftxui::text("  Humidité  ") | ftxui::color(ftxui::Color::White),
                ftxui::text(std::format("{}%", weather.humidityRate)),
            }),
            ftxui::hbox
            ({
                ftxui::text("  Nuages    ") | ftxui::color(ftxui::Color::White),
                ftxui::text(std::format("{}%", weather.cloudRate)),
            }),
        }) | ftxui::border | ftxui::xflex;

        rightPanel = ftxui::vbox
        ({
            ftxui::text(" " + inputCity) | ftxui::bold | ftxui::color(ftxui::Color::Yellow),
            ftxui::separator(),
            conditionsPanel,
            tempPanel,
            atmosPanel,
        }) | ftxui::flex;
    }

    return ftxui::vbox
    ({
        // Header
        ftxui::text("  Weather App") | ftxui::bold | ftxui::color(ftxui::Color::Cyan),
        ftxui::separator(),
        // body 
        ftxui::hbox
        ({
            ftxui::vbox
            ({
                ftxui::text(" Météo de la ville") | ftxui::bold,
                ftxui::separator(),
                cityInput ->Render(),
                ftxui::separator(),
                ftxui::text(" Appuyez sur Entree") | ftxui::color(ftxui::Color::GrayDark),
            }) | ftxui::border,
            
            rightPanel,
        
            }) | ftxui::flex,
    }) | ftxui::border | ftxui::flex;
});
    screen.Loop(renderer);


  return 0;
}


