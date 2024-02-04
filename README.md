<p align="center">
  <a href="https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/">
    <img src="https://files.seeedstudio.com/wiki/wiki-platform/SeeedStudio.png" width="auto" height="auto" alt="Xiao ESP32C3">
  </a>
</p>

<div align="center">

# **Xiao Prism Buddy**

This project is built on the ESP32 IoT Development Framework (IDF) and utilizes the C++ programming language for development.

</div>

<p align="center">
  <a href="https://raw.githubusercontent.com/timo614/xiao-prism-buddy/main/LICENSE">
    <img src="https://img.shields.io/github/license/timo614/xiao-prism-buddy" alt="license">
  </a>
  <a href="https://github.com/timo614/xiao-prism-buddy/releases">
    <img src="https://img.shields.io/github/v/release/timo614/xiao-prism-buddy?include_prereleases&color=blueviolet" alt="release">
  </a>
    <img src="https://img.shields.io/github/repo-size/timo614/xiao-prism-buddy" alt="repo-size">
</p>

This project demonstrates the use of a splitter prism to create a three-dimensional display. Configuration is provided for connecting with a local home assistant install for fetching weather data for display on the device via animations. 

## Function
- [x] Time and animated weather display
- [x] Activity timers and presence detection for sleep behavior (hiding text)
- [x] Gesture sensor based control scheme 
- [x] Animated gif browser
- [x] Cryptocurrency watcher

## Third Party APIs

This application uses several third party APIs that provide free plans, see their sites for their respective license and rate limit information.

- [Weather data by Open-Meteo.com](https://open-meteo.com/)
- Cryptocurrency data [Powered by CoinGecko](https://www.coingecko.com)


### Dependencies

The following library versions were used for this example:
- esp-idf: v5.0.4
- LovyanGFX: v1.1.9
- LVGL: v8.3.11

#### Preparing your environment

1. Install esp-idf 5.0.4
4. Export your environment for esp-idf
5. Build, flash, and (optionally) monitor the deployment

#### Build and Flash

Run `idf.py -p PORT flash monitor` to build, flash and monitor the project.

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.


### References

The SenseCAP Indicator ESP32 example indicator_basis project was used as a reference for the structure of this application with several files used and modified for this project: https://github.com/Seeed-Solution/SenseCAP_Indicator_ESP32/tree/main/examples/indicator_basis