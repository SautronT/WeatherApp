# Weather App

This is a terminal weather application made with C++23 and FTXUI.

## Functionnalities

- Search weather by city names
- Display weather information (condition, temperature and ambient data)
- Fullscreen terminal interface made via FTXUI

## Tech Stack 
| Dependency | Version | Role |
|---|---|---|
| C++ | 23 | Project standard |
| CMake | 3.21+ | Build system |
| [FTXUI](https://github.com/ArthurSonzogni/FTXUI) | 5.0.0 | Terminal UI (TUI) |
| [cpr](https://github.com/libcpr/cpr) | 1.10.5 | HTTP requests |
| [nlohmann/json](https://github.com/nlohmann/json) | 3.11.3 | JSON parsing |
| [Catch2](https://github.com/catchorg/Catch2) | 3.6.0 | Unit tests |
 
All dependencies are managed via **CMake FetchContent** — no manual installation required.

## Architecture 

```
WeatherApp/
├── app/
│   ├── CMakeLists.txt
│   └── main.cpp               # Entry point, FTXUI UI
├── core/                      # Static library (business logic)
│   ├── CMakeLists.txt
│   ├── WeatherData.hpp        # Data model for weather + JSON parsing
│   ├── IWeatherEngine.hpp     # Abstract interface
│   ├── RequestWeatherEngine   # HTTP implementation via cpr
│   └── WeatherService         # Cache + layer coordination
└── test/
    ├── CMakeLists.txt
    ├── TestWeatherEngine      # Fake client for tests (no network)
    └── weather_data_test.cpp  # 8 Catch2 tests
```

## Prerequisites

- **Windows** with Visual Studio 2022 (MSVC) or any C++23-compatible compiler
- **CMake** 3.21 or higher
- **Git** (required by FetchContent)
- Internet connection (first build only, to download dependencies)
- [OpenWeatherMap](https://openweathermap.org/api) API key (free tier)

## Getting Started
 
### 1. Clone the repository
 
```bash
git clone https://github.com/SautronT/WeatherApp.git
cd WeatherApp
```
 
### 2. Set up the API key
 
Create a free account at [openweathermap.org](https://openweathermap.org), retrieve your key from "API Keys", then set it as an environment variable:
 
**Windows (PowerShell):**
```powershell
setx OPENWEATHER_KEY "your_api_key_here"
```
 
Close and reopen your terminal for the variable to take effect.
 
**Linux / macOS:**
```bash
export OPENWEATHER_KEY="your_api_key_here"
```
 
### 3. Configure CMake
 
```bash
cmake -S . -B build
```
 
> The first build downloads and compiles all dependencies (FTXUI, cpr, nlohmann/json, Catch2). Expect 5 to 15 minutes depending on your connection and machine.
 
### 4. Build
 
```bash
cmake --build build
```
 
### 5. Run
 
**Windows:**
```powershell
.\build\app\Debug\weather_app.exe
```
 
**Linux / macOS:**
```bash
./build/app/weather_app
```
 
## Running Tests
 
```bash
ctest --test-dir build -C Debug
```
 
Expected output: **8 tests pass**, with no network connection required (tests use `TestWeatherEngine`, a fake client with static data).
 
```
100% tests passed, 0 tests failed out of 8
```
 
## Usage
 
- Type a city name in the search field
- Press **Enter** to fetch the weather
City names are accepted in any language (`Paris`, `Marseille`, `Saint-Denis`, `Londres`...).
 
