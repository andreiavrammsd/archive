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
using apixu::response::Location;

int main()
{
    const char* api_key = getenv("APIXUKEY");
    if (!api_key) {
        cout << "APIXUKEY not set";
        return 1;
    }

    apixu::Apixu apixu{api_key};

    vector<Location> locations = apixu.Search("Koln, Germany");

    for (const auto& loc : locations) {
        if (loc.id) {
            cout << "id = " << *loc.id << endl;
        }
        cout << "name = " << loc.name << endl;
        cout << "region = " << loc.region << endl;
        cout << "country = " << loc.country << endl;
        cout << "lat = " << loc.lat << endl;
        cout << "lon = " << loc.lon << endl;
        cout << "url = " << *loc.url << endl;
        cout << "timezone = " << *loc.timezone << endl;
        cout << "localtime epoch = " << *loc.localtime_epoch << endl;
        cout << "localtime = " << loc.localtime.tm_min << endl;
        cout << endl;
    }

    return 0;
}
