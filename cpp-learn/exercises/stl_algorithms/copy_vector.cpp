/*
 https://www.fluentcpp.com/2017/01/05/the-importance-of-knowing-stl-algorithms/
 */

#include <vector>
#include <cassert>
#include <algorithm>

struct Employee {
    int id;
};

int main() {
    std::vector<Employee> employees{
            {1},
            {2},
            {3},
            {4},
    };
    std::vector<Employee> employeeRegister;

    // For loop
    for (std::vector<Employee>::const_iterator it = employees.begin(); it != employees.end(); ++it) {
        employeeRegister.push_back(*it);
    }
    assert(employeeRegister.size() == 4);

    employeeRegister.clear();

    // Algorithm
    std::copy(employees.begin(), employees.end(), std::back_inserter(employeeRegister));
    assert(employeeRegister.size() == 4);
}
