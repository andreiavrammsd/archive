/*
 The C++ Programming Language, 4th Edition
 */

#include <iostream>

class Storable {
public:
    explicit Storable(const std::string &s) : file_name{s} {}

    virtual void read() = 0;

    virtual void write() = 0;

    virtual ~Storable() = default;

    Storable(const Storable &) = delete;

    Storable &operator=(const Storable &) = delete;

protected:
    std::string file_name;
};

class Transmitter : public virtual Storable {
public:
    void write() override {
        std::cout << "Transmitter: " << file_name << "\n";
    }
};

class Receiver : public virtual Storable {
public:
    void write() override {
        std::cout << "Receiver: " << file_name << "\n";
    }
};

class Radio : public Transmitter, public Receiver {
public:
    explicit Radio(const std::string &s) : Storable{s} {}

    void write() override final {
        Transmitter::write();
        Receiver::write();
    }

    void read() override {}
};

class Window {
protected:
public:
    virtual void draw() {
        std::cout << "main\n";
    };
};

class Window_with_border : public virtual Window {
protected:
    void own_draw() { // display the border
        std::cout << "border\n";
    }

public:
    void draw() override;
};

class Window_with_menu : public virtual Window {
protected:
    void own_draw() { // display the menu
        std::cout << "menu\n";
    }

public:
    void draw() override;
};

class Clock final : public Window_with_border, public Window_with_menu {
protected:
    void own_draw() { // display the clock face and hands
        std::cout << "clock\n";
    }

public:
    void draw() override;
};

void Window_with_border::draw() {
    Window::draw();
    own_draw(); // display the border
}

void Window_with_menu::draw() {
    Window::draw();
    own_draw(); // display the menu
}

void Clock::draw() {
    Window::draw();
    Window_with_border::draw();
    Window_with_menu::draw();
    own_draw(); // display the clock face and hands
}

// class AnotherClock : public Clock {}; // error: Base 'Clock' is marked 'final'

class AnotherRadio : public Radio {
    // void write() override final {} // error: Declaration of 'write' overrides a 'final' function

    void read() override {}
};

int main() {
    Radio radio{"filename"};
    radio.write();

    Clock clock;
    clock.draw();
    static_assert(std::is_final<Clock>::value);
}
