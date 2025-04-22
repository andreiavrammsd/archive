/*
 https://medium.com/@kateolenya/encapsulation-in-c-and-c-20dca9097e17
 */

#include <string>
#include <cassert>

class Contact {
private:
    std::string phone_number;
public:
public:
    explicit Contact(std::string phone_number) : phone_number{phone_number} {}

    [[nodiscard]] std::string getPhoneNumber() const {
        return phone_number;
    }

    friend void change(Contact &, std::string);
};

void change(Contact &c, std::string phone_number) {
    c.phone_number = phone_number;
}

struct ContactBypass {
    std::string phone_number;
};

int main() {
    Contact contact{"40700123123"};
    assert(contact.getPhoneNumber() == "40700123123");

    change(contact, "40700456456");
    assert(contact.getPhoneNumber() == "40700456456");

    ContactBypass *bypass;
    bypass = (ContactBypass *) &contact;
    bypass->phone_number = "40700789789";
    assert(contact.getPhoneNumber() == "40700789789");
}
