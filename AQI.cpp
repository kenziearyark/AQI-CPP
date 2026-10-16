#include <cstdint>
#include <iostream>
#include <curl/curl.h>
#include "json.hpp"
#include "fstream"
#include <string>

struct Data {
    std::string API;
    std::string LocationID;
};

class Menu {
    private:
        void Setup(Data& data) {
            std::cout << "========== Setup ==========\nAPI?\n > "; std::cin >> data.API;
            std::cout << "Location ID?\n > "; std::cin >> data.LocationID;
        }
    public:
        void First(Data& data) {
            Setup(data);
        }
};

size_t saveResponse(char* piece, size_t size, size_t count, void* destination) {
    auto* response = static_cast<std::string*>(destination);
    size_t bytes = size * count;
    response->append(piece, bytes);
    return bytes;
}

int main() {

    CURL* curl = curl_easy_init();      //turn on
    if (curl == nullptr) {std::cout << "Check Connection."; return 1;}

    Data data; Menu menu;     // objects
    std::ifstream input("settings.txt");

    if (!(std::getline(input, data.API) &&
        std::getline(input, data.LocationID))) {
        menu.First(data);

        std::ofstream output("settings.txt");
        output << data.API << '\n' << data.LocationID << "\n\nThe first line contains your API key, and the second line contains your location ID.";
    }

    std::string apiHeader = "X-API-Key: " + data.API;
    const std::string URL = 
        "https://api.purpleair.com/v1/sensors/"+data.LocationID;


    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, saveResponse);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());     // config camera
    curl_slist* headers = curl_slist_append(nullptr, apiHeader.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    CURLcode result = curl_easy_perform(curl);     // press record


    long statusCode = 0;
    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &statusCode
    );                              // check connection (info)
    if (statusCode != 200) {
        std::cout << statusCode << " Error!";
    }

    nlohmann::json json = nlohmann::json::parse(response);
    double PM = json["sensor"]["pm2.5_atm"].get<double>();
    float lh, ll, ch, cl;
    if (PM <= 9.0) {
        lh = 50,
        ll = 0,
        ch = 9.0,
        cl = 0;
    } else if (PM >= 9.1 && PM <= 35.4) {
        lh = 100,
        ll = 51,
        ch = 35.4,
        cl = 9.1;
    } else if (PM >= 35.5 && PM <= 55.4) {
        lh = 150,
        ll = 101,
        ch = 55.4,
        cl = 35.5;
    } else if (PM >= 55.5 && PM <= 125.4) {
        lh = 200,
        ll = 151,
        ch = 125.4,
        cl = 55.5;
    } else if (PM >= 125.5 && PM <= 225.4) {
        lh = 300,
        ll = 201,
        ch = 225.4,
        cl = 125.5;
    } else if (PM >= 225.5 && PM <= 325.4) {
        lh = 500,
        ll = 301,
        ch = 325.4,
        cl = 225.5;
    } else {std::cout << "SYSTEM OVERLOAD.";}

    // std::cout << lh << ' ' << ll << ' ' << ch << ' ' << cl;
    // std::cout << "\n" << PM;

    int AQI = ll + ((PM -cl)/(ch-cl))*(lh-ll);
    while (true) {
        int8_t o;
        std::cout << "\033[2J\033[H";
        std::cout << "The current AQI is: " << AQI << "\nActions:\n1. Status\n0. Exit\n > ";
        std::cin >> o;
        if (o == '0') {return 0; curl_easy_cleanup(curl);}
    }
}
