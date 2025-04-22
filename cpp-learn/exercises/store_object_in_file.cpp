/*
 https://youtu.be/j0_u26Vpb4w?t=980
 */

#include <any>
#include <cassert>
#include <fstream>

struct Test {
    int a;
    float b;
};

template <typename T>
void DumpToFile(void *a, const std::string &file)
{
    std::ofstream f;
    f.exceptions(std::ios::badbit | std::ios::failbit);
    f.open(file, std::ios::binary);

    f.write(static_cast<const char *>(a), sizeof(T));
}

template <typename T>
void DumpToFile(const std::any &a, const std::string &file)
{
    std::ofstream f;
    f.exceptions(std::ios::badbit | std::ios::failbit);
    f.open(file, std::ios::binary);

    auto object = std::any_cast<T>(a);
    f.write(reinterpret_cast<const char *>(&object), sizeof(T));
}

template <typename T>
T LoadFromFile(const std::string &file)
{
    std::ifstream f;
    f.exceptions(std::ios::badbit | std::ios::failbit);
    f.open(file, std::ios::binary);

    char data[sizeof(T)];
    f.read(data, sizeof(T));

    return reinterpret_cast<T &>(data);
}

int main()
{
    const std::string file = "dumpy.bin";

    Test t1{5, 10.0f};
    DumpToFile<Test>(&t1, file);
    DumpToFile<Test>(t1, file);

    auto t2 = LoadFromFile<Test>(file);
    assert(t1.a == t2.a);
    assert(t1.b == t2.b);
}
