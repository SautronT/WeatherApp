add_executable(weather_app main.cpp)

target_link_libraries(weather_app PRIVATE
  core
  cpr::cpr
  nlohmann_json::nlohmann_json
  ftxui::screen
  ftxui::dom
  ftxui::component
)
