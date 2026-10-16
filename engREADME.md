# AQI Look Up

A small C++ terminal app that requests a PurpleAir sensor reading and estimates a US EPA PM2.5 AQI value.

## Why I made this

I came up with this app because my hometown is dealing with a smoke problem. At its worst, the AQI there has reached around **230**. I wanted a simple way to check an air sensor and understand how bad the air quality is.

## What it does

- Asks for a PurpleAir API read key and sensor index on the first run.
- Saves them in a local `settings.txt` file so they do not need to be entered again.
- Fetches the sensor's latest `pm2.5_atm` reading with libcurl.
- Converts that reading to an AQI estimate using US EPA PM2.5 breakpoints.

The app fetches **one reading when it starts**. Its menu does not refresh the reading yet. The displayed number is an estimate from a recent raw sensor reading, not an official daily or NowCast AQI.

## Requirements

- A C++17 compiler, such as `g++`
- libcurl development headers and library
- A [PurpleAir API read key](https://develop.purpleair.com/) and a sensor index

The project includes `json.hpp` for parsing the API response, so no separate JSON library installation is needed.

## Build and run

From the project directory:

```sh
g++ -std=c++17 -O2 AQI.cpp -o AQI -lcurl
./AQI
```

On the first run, enter your API read key and sensor index when prompted. The app writes them to `settings.txt`, with the key on the first line and the sensor index on the second. On later runs, it reads those values automatically. Run the program from the project directory so it can find the settings file.

## Keep your key private

`settings.txt` contains the API key as plain text. It is excluded by `.gitignore`; do not commit or share it. On Linux, you can restrict access to your account with:

```sh
chmod 600 settings.txt
```

## How the estimate is calculated

The app takes the latest outdoor `pm2.5_atm` concentration, finds its range in the [US EPA PM2.5 AQI breakpoint table](https://document.airnow.gov/technical-assistance-document-for-the-reporting-of-daily-air-quailty.pdf), and scales the concentration into that range's AQI values:

```text
AQI = I_low + (C - C_low) / (C_high - C_low) * (I_high - I_low)
```

Here, `C` is the PM2.5 concentration; the four low/high values come from the matching breakpoint row. The current implementation uses the raw reading without a PurpleAir correction and does not calculate an EPA NowCast.

## Sources

- [PurpleAir API guide](https://community.purpleair.com/t/making-api-calls-with-the-purpleair-api/180)
- [US EPA AQI calculation guide](https://document.airnow.gov/technical-assistance-document-for-the-reporting-of-daily-air-quailty.pdf)
- [libcurl documentation](https://curl.se/libcurl/c/)
