from flask import Flask

app = Flask(__name__)


@app.route('/conditions.json')
def conditions():
    res = '''
        [ 
           { 
              "code":1,
              "day":"day",
              "night":"night",
              "icon":22
           }
        ]
    '''
    return res


@app.route('/current.json')
def current():
    res = '''
        {
          "location": {
            "name": "ABCDEFGHIJKLMNOPQRST",
            "region": "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
            "country": "ABCDEFGHIJKLMNOPQRS",
            "lat": 934.25,
            "lon": 617.0,
            "tz_id": "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
            "localtime_epoch": 1574708193,
            "localtime": "2019-11-25 18:56:33"
          },
          "current": {
            "last_updated_epoch": 1574708193,
            "last_updated": "2019-11-25 18:56:33",
            "temp_c": 335.5,
            "temp_f": 327.0,
            "is_day": 1,
            "condition": {
              "text": "ABCDE",
              "icon": "ABCDEFGHIJKLMNOPQRSTUVW",
              "code": 623
            },
            "wind_mph": 159.5,
            "wind_kph": -91.0,
            "wind_degree": -6,
            "wind_dir": "ABCDEFGHIJKLMNOPQRSTUVWXYZAB",
            "pressure_mb": -68.25,
            "pressure_in": 2.5,
            "precip_mm": 785.0,
            "precip_in": 588.75,
            "humidity": -11,
            "cloud": 509,
            "feelslike_c": 284.25,
            "feelslike_f": 413.25,
            "vis_km": 356.75,
            "vis_miles": 440.5,
            "uv": -8.0,
            "gust_mph": 0.0,
            "gust_kph": 0.0
          }
        }
    '''
    return res


@app.route('/forecast.json')
def forecast():
    res = '''
        {
              "location": {
                "name": "ABCDEFGHIJKLMNOPQRST",
                "region": "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                "country": "ABCDEFGHIJKLMNOPQRS",
                "lat": 934.25,
                "lon": 617.0,
                "tz_id": "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                "localtime_epoch": 1574708193,
                "localtime": "2019-11-25 18:56:33"
              },
              "current": {
                "last_updated_epoch": 1574708193,
                "last_updated": "2019-11-25 18:56:33",
                "temp_c": 335.5,
                "temp_f": 327.0,
                "is_day": 1,
                "condition": {
                  "text": "ABCDE",
                  "icon": "ABCDEFGHIJKLMNOPQRSTUVW",
                  "code": 623
                },
                "wind_mph": 159.5,
                "wind_kph": -91.0,
                "wind_degree": -6,
                "wind_dir": "ABCDEFGHIJKLMNOPQRSTUVWXYZAB",
                "pressure_mb": -68.25,
                "pressure_in": 2.5,
                "precip_mm": 785.0,
                "precip_in": 588.75,
                "humidity": -11,
                "cloud": 509,
                "feelslike_c": 284.25,
                "feelslike_f": 413.25,
                "vis_km": 356.75,
                "vis_miles": 440.5,
                "uv": -8.0,
                "gust_mph": 0.0,
                "gust_kph": 0.0
              },
              "forecast": {
                "forecastday": [
                  {
                    "date": "ABCDEFGHI",
                    "date_epoch": 1574708193,
                    "day": {
                      "maxtemp_c": 570.75,
                      "maxtemp_f": 920.0,
                      "mintemp_c": 484.0,
                      "mintemp_f": 544.0,
                      "avgtemp_c": 518.25,
                      "avgtemp_f": 645.5,
                      "maxwind_mph": 244.25,
                      "maxwind_kph": 497.0,
                      "totalprecip_mm": 582.75,
                      "totalprecip_in": 298.0,
                      "avgvis_km": 377.0,
                      "avgvis_miles": 576.25,
                      "avghumidity": 406.5,
                      "condition": {
                        "text": "ABCDEFGHIJKLM",
                        "icon": "ABCDEFGHIJKLMNO",
                        "code": 430
                      },
                      "uv": 978.25
                    },
                    "astro": {
                      "sunrise": "ABCDEFGHIJKLMNOPQRSTUVWX",
                      "sunset": "ABCDEFGHIJKLMNOPQRST",
                      "moonrise": "ABCDEFGHIJKLMNOPQRSTUVWXYZAB",
                      "moonset": "ABCDEFG"
                    },
                    "hour": [
                        {
                              "time_epoch": 1574708193,
                              "time": "2019-11-25 18:56:33",
                              "temp_c": 519.25,
                              "temp_f": 282.75,
                              "is_day": 1,
                              "condition": {
                                "text": "ABCDEFGHIJKLMNOPQRSTU",
                                "icon": "ABCDEFGHIJKLMN",
                                "code": 861
                              },
                              "wind_mph": 499.0,
                              "wind_kph": 860.25,
                              "wind_degree": 818,
                              "wind_dir": "ABCDEFGHIJKLMNOPQRSTUV",
                              "pressure_mb": 360.75,
                              "pressure_in": 793.5,
                              "precip_mm": 373.5,
                              "precip_in": 474.75,
                              "humidity": 397,
                              "cloud": 164,
                              "feelslike_c": 922.25,
                              "feelslike_f": 426.75,
                              "vis_km": 861.0,
                              "vis_miles": 723.0,
                              "gust_mph": 688.0,
                              "gust_kph": 286.75,
                              "uv": 377.25
                        }
                    ]
                  }
                ]
              }
            }
    '''
    return res


@app.route('/history.json')
def history():
    res = '''
        {
              "location": {
                "name": "ABCDEFGHIJK",
                "region": "ABCDEFGHIJKLMNOP",
                "country": "ABCDEFGHIJKL",
                "lat": 504.0,
                "lon": 70.0,
                "tz_id": "ABCD",
                "localtime_epoch": -24,
                "localtime": "ABCDEFG"
              },
              "forecast": {
                "forecastday": [
                  {
                    "date": "ABCDEFGHIJKLMNOPQRSTUVWXYZABC",
                    "date_epoch": 319,
                    "day": {
                      "maxtemp_c": 66.5,
                      "maxtemp_f": 324.75,
                      "mintemp_c": 320.25,
                      "mintemp_f": 178.25,
                      "avgtemp_c": 50.5,
                      "avgtemp_f": -30.5,
                      "maxwind_mph": 29.0,
                      "maxwind_kph": 45.0,
                      "totalprecip_mm": 711.25,
                      "totalprecip_in": 300.0,
                      "avgvis_km": 84.25,
                      "avgvis_miles": 768.0,
                      "avghumidity": 775.0,
                      "condition": {
                        "text": "ABCDEFGHIJKLMNOPQRSTUVWX",
                        "icon": "ABCDEFG",
                        "code": 174
                      },
                      "uv": 128.5
                    },
                    "astro": {
                      "sunrise": "ABCDEFGHIJKLMNOPQRSTU",
                      "sunset": "ABCDEFGHIJKL",
                      "moonrise": "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                      "moonset": "ABCDEFGHIJKLMN",
                      "moon_phase": "ABCDEFGHIJKLMNOPQRSTU",
                      "moon_illumination": "ABCDEFGHIJKL"
                    },
                    "hour":[
                       {
                          "time_epoch":1574708193,
                          "time":"2019-11-25 18:56:33",
                          "temp_c":519.25,
                          "temp_f":282.75,
                          "is_day":0,
                          "condition":{
                             "text":"ABCDEFGHIJKLMNOPQRSTU",
                             "icon":"ABCDEFGHIJKLMN",
                             "code":861
                          },
                          "wind_mph":499.0,
                          "wind_kph":860.25,
                          "wind_degree":818,
                          "wind_dir":"ABCDEFGHIJKLMNOPQRSTUV",
                          "pressure_mb":360.75,
                          "pressure_in":793.5,
                          "precip_mm":373.5,
                          "precip_in":474.75,
                          "humidity":397,
                          "cloud":164,
                          "feelslike_c":922.25,
                          "feelslike_f":426.75,
                          "vis_km":861.0,
                          "vis_miles":723.0,
                          "gust_mph":688.0,
                          "gust_kph":286.75,
                          "uv":377.25
                       }
                    ]
                  }
                ]
              }
            }
    '''
    return res


@app.route('/search.json')
def search():
    res = '''
       [
          {
            "id": 988,
            "name": "ABCDEFG",
            "region": "ABCDEFGHI",
            "country": "ABCDEFGHIJKLMNOPQRSTUVW",
            "lat": 861.75,
            "lon": -72.75,
            "url": "ABCDEFGHIJKLMNOPQ",
            "tz_id": "Europe/Paris",
            "localtime_epoch": 1576174998,
            "localtime": "2019-11-25 18:56:33"
          }
        ]
    '''
    return res


if __name__ == '__main__':
    app.run(host='0.0.0.0')
