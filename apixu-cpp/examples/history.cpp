// Copyright 2020 <Andrei Avram>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

#include "apixu/apixu.h"
#include "apixu/exception/api_exception.h"

using std::cout;
using std::endl;

using apixu::exception::ApiException;
using apixu::exception::ApixuException;
using apixu::response::Location;
using apixu::response::WeatherHistory;
using apixu::response::forecast::ForecastWeather;

int main()
{
    const char* api_key = getenv("APIXUKEY");
    if (!api_key) {
        cout << "APIXUKEY not set";
        return 1;
    }

    apixu::Apixu apixu{api_key};

    class WeatherHistory history;
    try {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream since;
        since << std::put_time(&tm, "%Y-%m-%d");

        history = apixu.History("Prague", since.str());
    }
    catch (ApiException& e) {
        cout << "ApiException: " << e.what() << " (code: " << e.getCode() << ")";
        return 1;
    }
    catch (ApixuException& e) {
        cout << "ApixuException: " << e.what();
        return 1;
    }

    cout << "location" << endl;

    const Location& location = history.location;

    cout << "\tname = " << location.name << endl;
    cout << "\tregion = " << location.region << endl;
    cout << "\tcountry = " << location.country << endl;
    cout << "\tlat = " << location.lat << endl;
    cout << "\tlon = " << location.lon << endl;
    cout << "\ttimezone = " << *location.timezone << endl;
    cout << "\tlocaltime epoch = " << *location.localtime_epoch << endl;

    auto localtime = location.localtime;
    cout << "\tlocaltime:" << endl;
    cout << "\t\tyear = " << localtime.tm_year << ", month = " << localtime.tm_mon << ", day = " << localtime.tm_mday
         << endl;
    cout << "\t\thour = " << localtime.tm_hour << ", minute = " << localtime.tm_min << endl;

    cout << endl << "forecast weather" << endl;

    const ForecastWeather& forecast = history.forecast;

    for (const auto& f : forecast.forecast_day) {
        cout << "\tdate = " << f.date << endl;
        cout << "\tdate epoch = " << f.date_epoch << endl;

        cout << "\tday" << endl;
        cout << "\t\tmax temp C = " << f.day.max_temp_celsius << endl;
        cout << "\t\tmax temp F = " << f.day.max_temp_fahrenheit << endl;

        cout << "\tastro" << endl;
        cout << "\t\tsunrise = " << f.astro.sunrise << endl;
        cout << "\t\tsunset = " << f.astro.sunset << endl;

        cout << "\thour" << endl;
        for (const auto& h : f.hour) {
            cout << "\t\ttime epoch = " << h.time_epoch << endl;
            cout << "\t\ttime = " << h.time << endl;
            cout << "\t\ttemp C = " << h.temp_c << endl;
            cout << "\t\ttemp F = " << h.temp_f << endl;
            cout << endl;
        }
    }

    return 0;
}
