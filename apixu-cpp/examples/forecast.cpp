// Copyright 2020 <Andrei Avram>
#include <iostream>
#include <vector>

#include "apixu/apixu.h"
#include "apixu/exception/api_exception.h"

using std::cout;
using std::endl;
using std::vector;

using apixu::exception::ApiException;
using apixu::exception::ApixuException;
using apixu::response::Current;
using apixu::response::Location;
using apixu::response::forecast::Astro;
using apixu::response::forecast::Day;
using apixu::response::forecast::ForecastDay;
using apixu::response::forecast::Hour;
using apixu::response::forecast::WeatherForecast;

int main()
{
    const char* api_key = getenv("APIXUKEY");
    if (!api_key) {
        cout << "APIXUKEY not set";
        return 1;
    }

    apixu::Apixu apixu{api_key};

    WeatherForecast forecast;
    try {
        int h = 12;
        forecast = apixu.Forecast("Paris", 2, &h);
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

    const Location& location = forecast.location;

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

    const Current& current = forecast.current;

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

    cout << endl << "day forecast" << endl;

    vector<ForecastDay> days = forecast.forecast.forecast_day;
    for (auto const& day : days) {
        cout << "\tdate = " << day.date << endl;
        cout << "\tdate epoch = " << day.date_epoch << endl;

        const Day& d = day.day;
        cout << "\tday" << endl;
        cout << "\t\tmax temp C = " << d.max_temp_celsius << endl;
        cout << "\t\tmax temp F = " << d.max_temp_fahrenheit << endl;
        cout << "\t\tmin temp C = " << d.min_temp_celsius << endl;
        cout << "\t\tmin temp F = " << d.min_temp_fahrenheit << endl;
        cout << "\t\tavg temp C = " << d.avg_temp_celsius << endl;
        cout << "\t\tavg temp F = " << d.avg_temp_fahrenheit << endl;
        cout << "\t\tmax wind MPH = " << d.max_wind_mph << endl;
        cout << "\t\tmax wind KMH = " << d.max_wind_kph << endl;
        cout << "\t\ttotal precip MM = " << d.total_precip_mm << endl;
        cout << "\t\ttotal precip IN = " << d.total_precip_in << endl;
        cout << "\t\tavg vis KM = " << d.avg_vis_km << endl;
        cout << "\t\tavg vis Miles = " << d.avg_vis_miles << endl;
        cout << "\t\tavg humidity = " << d.avg_humidity << endl;
        cout << "\t\tcondition " << endl;
        cout << "\t\t\ttext = " << *d.condition.text << endl;
        cout << "\t\t\ticon = " << *d.condition.icon << endl;
        cout << "\t\t\tcode = " << *d.condition.code << endl;
        cout << "\t\tuv = " << d.uv << endl;

        const Astro& a = day.astro;
        cout << "\tastro" << endl;
        cout << "\t\tsunrise = " << a.sunrise << endl;
        cout << "\t\tsunset = " << a.sunset << endl;
        cout << "\t\tmoonrise = " << a.moonrise << endl;
        cout << "\t\tmoonset = " << a.moonset << endl;
        cout << "\t\tmoon phase = " << a.moon_phase << endl;
        cout << "\t\tmoon illumination = " << a.moon_illumination << endl;

        const vector<Hour>& hour = day.hour;
        for (auto const& h : hour) {
            cout << "\thour" << endl;
            cout << "\t\ttime epoch = " << h.time_epoch << endl;
            cout << "\t\ttime = " << h.time << endl;
            cout << "\t\ttemp C = " << h.temp_c << endl;
            cout << "\t\ttemp f = " << h.temp_f << endl;
            cout << "\t\tday = " << h.is_day << endl;
            cout << "\t\tcondition " << endl;
            cout << "\t\t\ttext = " << h.condition.text << endl;
            cout << "\t\t\ticon = " << h.condition.icon << endl;
            cout << "\t\t\tcode = " << h.condition.code << endl;
            cout << "\t\twind MPH = " << h.wind_mph << endl;
            cout << "\t\twind KPH = " << h.wind_kph << endl;
            cout << "\t\twind degree = " << h.wind_degree << endl;
            cout << "\t\twind direction = " << h.wind_dir << endl;
            cout << "\t\tpressure MB = " << h.pressure_mb << endl;
            cout << "\t\tpressure IN = " << h.pressure_in << endl;
            cout << "\t\tprecip MM = " << h.precip_mm << endl;
            cout << "\t\tprecip IN = " << h.precip_in << endl;
            cout << "\t\thumidity = " << h.humidity << endl;
            cout << "\t\tcloud = " << h.cloud << endl;
            cout << "\t\tfeels like C = " << h.feels_like_c << endl;
            cout << "\t\tfeels like F = " << h.feels_like_f << endl;
            cout << "\t\twind chill C = " << h.wind_chill_c << endl;
            cout << "\t\twind chill F = " << h.wind_chill_f << endl;
            cout << "\t\theat index C = " << h.heat_index_c << endl;
            cout << "\t\theat index F = " << h.heat_index_f << endl;
            cout << "\t\tdew point C = " << h.dew_point_c << endl;
            cout << "\t\tdew point F = " << h.dew_point_f << endl;
            cout << "\t\twill it rain? = " << h.will_rain << endl;
            cout << "\t\tchance of rain = " << h.chance_of_rain << endl;
            cout << "\t\twill it snow? = " << h.will_snow << endl;
            cout << "\t\tchance of snow = " << h.chance_of_snow << endl;
            cout << "\t\tvis KM = " << h.vis_km << endl;
            cout << "\t\tvis Miles = " << h.vis_miles << endl;
            cout << "\t\tgust MPH = " << h.gust_mph << endl;
            cout << "\t\tgust KPH = " << h.gust_kph << endl;
            cout << endl;
        }

        cout << endl;
    }

    return 0;
}
