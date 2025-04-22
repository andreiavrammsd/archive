#include <cassert>
#include <iostream>

#include "header.h"

int main()
{
    using Header::EmptyInput;
    using Header::Header;

    std::string init = "a";
    Header h{init};

    std::string result;

    try {
        result = h.concat("");
        std::cout << result;
    }
    catch (const EmptyInput &ex) {
        std::cerr << "input is empty" << std::endl;
    }
    catch (const std::exception &ex) {
        std::cerr << "other error = " << ex.what() << std::endl;
    }
    catch (...) {
        std::cerr << "unknown error " << std::endl;
    }

    result = h.concat("b");
    assert(result == "ab");
}
