# Learning C++

C++11 information based on [The C++ Programming Language (4th Edition)](http://www.stroustrup.com/4th.html) book by
Bjarne Stroustrup.

C++14/17/20 from other sources.

Tested on Ubuntu 18.04.

## [Algorithms](./algorithms)

## [Data structures](./data_structures)

## [Exercises](./exercises)

## [Numbers](./numbers)

## [Standard Library](./stl)

## [Templates](./templates)

## [Tests](./tests)

### Requirements

* For C++17:
    * GCC-10/G++-10
        ```shell script
        sudo add-apt-repository ppa:ubuntu-toolchain-r/test
        sudo apt update
        sudo apt install -y gcc-10 g++-10
        ```
    * Threading Building Blocks for Parallel execution policies 2018+ (https://github.com/oneapi-src/oneTBB)
        ```shell script
        echo "deb http://cz.archive.ubuntu.com/ubuntu eoan main universe" | sudo tee -a  /etc/apt/sources.list
        sudo apt install -y libtbb-dev
        ```

### Resources

* https://en.cppreference.com/w/cpp/compiler_support
* https://github.com/AnthonyCalandra/modern-cpp-features
* https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md
* https://github.com/sinairv/Cpp-Tutorial-Samples
* https://www.walletfox.com/course/cheatsheets_cpp.php
* https://github.com/joboccara/pipes/blob/master/.travis.yml