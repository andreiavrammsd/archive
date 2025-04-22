/**
 * Mask an email address: johndoe@emailprovider.tld -> j*****e@emailprovider.tld
 */

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

using func = std::function<std::string(const std::string, const char)>;

// no STL algorithms (verbose)
std::string mask(const std::string &email, const char mask)
{
    //    if (email.empty() || email == "@" || email[0] == '@') {
    //        return email;
    //    }
    //    vs
    if (email[0] == '@') {
        return email;
    }
    //    vs
    //    no if statement

    std::string masked;
    masked.reserve(email.size());

    bool hide = true;
    bool is_email = false;

    for (size_t i = 0; i < email.size(); ++i) {
        if (email[i] == '@') {
            is_email = true;
            hide = false;

            if (i > 2) {
                masked[0] = email[0];
                masked[i - 1] = email[i - 1];
            }
        }

        masked += hide ? mask : email[i];
    }

    if (!is_email && masked.size() > 2) {
        masked[0] = email[0];
        masked[masked.size() - 1] = email[masked.size() - 1];
    }

    return masked;
}

// STL find/transform (compact)
std::string mask_with_find_and_transform(const std::string &email, const char mask = '*')
{
    std::string masked = email;

    auto p = find(masked.begin(), masked.end(), '@');
    auto offset = (p - masked.begin() > 2) ? 1 : 0;
    auto begin = masked.begin() + offset;
    auto end = p - offset;

    transform(begin, end, begin, [mask](const char &) { return mask; });

    return masked;
}

// STL find/replace_if (the predicate feels like a hack)
std::string mask_with_find_and_replace_if(const std::string &email, const char mask = '*')
{
    std::string masked = email;

    auto p = find(masked.begin(), masked.end(), '@');
    auto offset = (p - masked.begin() > 2) ? 1 : 0;
    auto begin = masked.begin() + offset;
    auto end = p - offset;

    replace_if(
        begin, end, [](const char &) { return true; }, mask);

    return masked;
}

// STL strings find_first_of/replace
std::string mask_with_find_first_of_and_replace(const std::string &email, const char mask = '*')
{
    std::string masked = email;
    if (masked.empty()) {
        return masked;
    }

    auto pos = masked.find_first_of('@');
    auto start = 1;
    auto n = pos - 2;

    if (pos < 3) {
        start = 0;
        n = pos;
    }

    if (pos == std::string::npos) {
        start = 1;
        n = masked.size() - 2;

        if (n < 1 || n == std::string::npos) {
            start = 0;
            n = masked.size();
        }
    }

    masked.replace(start, n, n, mask);

    return masked;
}

long call(const func &f, const std::string &input, const std::string &expected)
{
    auto start = std::chrono::high_resolution_clock::now();

    auto result = f(input, '*');

    auto duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

    if (result != expected) {
        char message[100];
        sprintf(message, R"(actual: "%s"; expected: "%s")", result.c_str(), expected.c_str());
        throw std::runtime_error(message);
    }

    return duration.count();
}

enum class measure {
    // show execution time for each function
    function,

    // show execution time for each function and input
    input,
};

int main()
{
    const auto measure_type = measure::function;

    const std::map<std::string, std::string> tests{
        {"johndoe@emailprovider.tld", "j*****e@emailprovider.tld"},
        {"jde@emailprovider.tld", "j*e@emailprovider.tld"},
        {"jd@emailprovider.tld", "**@emailprovider.tld"},
        {"j@emailprovider.tld", "*@emailprovider.tld"},
        {"@emailprovider.tld", "@emailprovider.tld"},
        {"wrong", "w***g"},
        {"wro", "w*o"},
        {"wr", "**"},
        {"w", "*"},
        {"", ""},
        {"@", "@"},
    };

    std::map<std::string, std::vector<long>> results;
    for (const auto &test : tests) {
        for (int i = 1; i <= 100; ++i) {
            if (measure_type == measure::function) {
                results["mask"].push_back(call(&mask, test.first, test.second));
                results["mask_with_find_and_transform"].push_back(
                    call(&mask_with_find_and_transform, test.first, test.second));
                results["mask_with_find_and_replace_if"].push_back(
                    call(&mask_with_find_and_replace_if, test.first, test.second));
                results["mask_with_find_first_of_and_replace"].push_back(
                    call(&mask_with_find_first_of_and_replace, test.first, test.second));
            }
            else {
                results["mask(" + test.first + ")"].push_back(call(&mask, test.first, test.second));
                results["mask_with_find_and_transform(" + test.first + ")"].push_back(
                    call(&mask_with_find_and_transform, test.first, test.second));
                results["mask_with_find_and_replace_if(" + test.first + ")"].push_back(
                    call(&mask_with_find_and_replace_if, test.first, test.second));
                results["mask_with_find_first_of_and_replace(" + test.first + ")"].push_back(
                    call(&mask_with_find_first_of_and_replace, test.first, test.second));
            }
        }
    }

    std::cout << "\nExecution time (nanoseconds)\n\n";

    for (const auto &result : results) {
        auto data = result.second;

        auto all = std::accumulate(data.cbegin(), data.cend(), std::string{},
                                   [](const std::string &a, long v) { return a + std::to_string(v) + ", "; });

        sort(data.begin(), data.end());

        auto max = *std::max_element(data.cbegin(), data.cend());
        auto min = *std::min_element(data.cbegin(), data.cend());
        auto avg = std::accumulate(data.cbegin(), data.cend(), 0.0) / data.size();
        auto med = data[data.size() == 1 ? 0 : ((data.size() + 1) / 2)];

        std::cout << "min: " << std::setw(3) << min << "\tmax: " << std::setw(3) << max << "\tavg: " << std::setw(3)
                  << std::fixed << std::setprecision(2) << avg << "\t\tmed: " << std::setw(3) << med << "\t\t"
                  << result.first << " (" + all.substr(0, all.size() - 2) + ")"
                  << "\n\n";
    }
}
