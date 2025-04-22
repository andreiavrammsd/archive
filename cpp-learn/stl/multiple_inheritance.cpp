/*
 https://www.geeksforgeeks.org/multiple-inheritance-in-c/
 */

#include <iostream>

class Person {
public:
    Person() {
        std::cout << "Person()\n";
    }

    explicit Person(int) {
        std::cout << "Person(int)\n";
    }
};

class Faculty : virtual public Person {
public:
    explicit Faculty(int x) : Person(x) {
        std::cout << "Faculty(int)\n";
    }
};

class Student : virtual public Person {
public:
    explicit Student(int x) : Person(x) {
        std::cout << "Student(int)\n";
    }
};

class TA : public Faculty, public Student {
public:
    explicit TA(int x) : Person(x), Faculty(x), Student(x) {
        std::cout << "TA(int)\n";
    }
};

int main() {
    TA ta(1);
}
