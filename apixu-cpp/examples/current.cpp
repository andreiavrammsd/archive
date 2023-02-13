// Copyright 2020 <Andrei Avram>
#include <iostream>

#include "apixu/apixu.h"
#include "apixu/exception/api_exception.h"

using std::cout;
using std::endl;

using apixu::exception::ApiException;
using apixu::exception::ApixuException;
using apixu::response::Current;
using apixu::response::CurrentWeather;
using apixu::response::Location;

int main()
{
    const char* api_key = getenv("APIXUKEY");
    if (!api_key) {
        cout << "APIXUKEY not set";
        return 1;
    }

    apixu::Apixu apixu{api_key};

    CurrentWeather current_weather;
    try {
        current_weather = apixu.Current("Paris");
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

    const Location& location = current_weather.location;

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

    cout << endl << "current weather" << endl;

    const Current& current = current_weather.current;

    cout << "\tlast updated epoch = " << *current.last_updated_epoch << endl;

    auto last_updated = current.last_updated;
    cout << "\tlast updated:" << endl;
    cout << "\t\tyear = " << last_updated.tm_year << ", month = " << last_updated.tm_mon
         << ", day = " << last_updated.tm_mday << endl;
    cout << "\t\thour = " << last_updated.tm_hour << ", minute = " << last_updated.tm_min << endl;

    cout << "\ttemp C = " << *current.temp_c << endl;
    cout << "\ttemp F = " << *current.temp_f << endl;
    cout << "\tis day = " << *current.is_day << endl;

    cout << "\tcondition " << endl;
    cout << "\t\ttext = " << *current.condition.text << endl;
    cout << "\t\ticon = " << *current.condition.icon << endl;
    cout << "\t\tcode = " << *current.condition.code << endl;

    cout << "\twind MPH = " << *current.wind_mph << endl;
    cout << "\twind KPH = " << *current.wind_kph << endl;
    cout << "\twind degree = " << *current.wind_degree << endl;
    cout << "\twind direction = " << *current.wind_dir << endl;
    cout << "\tpressure MB = " << *current.pressure_mb << endl;
    cout << "\tpressure IN = " << *current.pressure_in << endl;
    cout << "\tprecip MM = " << *current.precip_mm << endl;
    cout << "\tprecip IN = " << *current.precip_in << endl;
    cout << "\thumidity = " << *current.humidity << endl;
    cout << "\tcloud = " << *current.cloud << endl;
    cout << "\tfeels like C = " << *current.feelslike_c << endl;
    cout << "\tfeels like F = " << *current.feelslike_f << endl;
    cout << "\tvis KM = " << *current.vis_km << endl;
    cout << "\tvis Miles = " << *current.vis_miles << endl;
    cout << "\t_u_v = " << *current.uv << endl;
    cout << "\tgust MPH = " << *current.gust_mph << endl;
    cout << "\tgust KPH = " << *current.gust_kph << endl;

    return 0;
}
