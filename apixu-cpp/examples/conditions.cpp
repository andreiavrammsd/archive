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
using apixu::response::Condition;

int main()
{
    const char* api_key = getenv("APIXUKEY");
    if (!api_key) {
        cout << "APIXUKEY not set";
        return 1;
    }

    apixu::Apixu apixu{api_key};

    vector<Condition> conditions;
    try {
        conditions = apixu.Conditions();
    }
    catch (ApiException& e) {
        cout << "ApiException: " << e.what() << " (code: " << e.getCode() << ")";
        return 1;
    }
    catch (ApixuException& e) {
        cout << "ApixuException: " << e.what();
        return 1;
    }

    for (const auto& c : conditions) {
        cout << "code: " << c.code << endl;
        cout << "day: " << c.day << endl;
        cout << "night: " << c.night << endl;
        cout << "icon: " << c.icon << endl << endl;
    }

    return 0;
}
