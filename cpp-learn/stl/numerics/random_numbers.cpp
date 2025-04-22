/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <random>

int main()
{
    int from = 1;
    int to = 6;

    // step by step
    using my_engine = std::default_random_engine;             // type of engine
    using my_distribution = std::uniform_int_distribution<>;  // type of distribution

    my_engine re{};
    my_distribution one_to_six{from, to};
    auto die = bind(one_to_six, re);  // the default engine

    int x = die();  // roll the die: x becomes a value in [from:to]
    std::cout << x << std::endl;

    // compact
    auto random = std::bind(std::uniform_int_distribution<>{from, to}, std::default_random_engine{});
    std::cout << random() << std::endl;
    std::cout << random() << std::endl;
    std::cout << random() << std::endl;

    std::cout << '\n';

    /**
     * Engines
     */

    // Standard Random Number Engines
    // default_random_engine
    // linear_congruential_engine<UI,a,c,m>
    // mersenne_twister_engine<UI,w,n,m,r,a,u,d,s,t,c,l,f>
    // subtract_with_carry_engine<UI,w,s,r>

    std::map<int, int> m;
    std::linear_congruential_engine<unsigned int, 17, 5, 0> linc_eng;
    for (int i = 0; i < 1000000; ++i) {
        if (1 < ++m[linc_eng()]) {
            std::cerr << "linc_eng: " + std::to_string(i);
            exit(1);
        }
    }

    // Standard Random Number Engine Adaptors
    // discard_block_engine<E,p,r>
    // independent_bits_engine<E,w,UI>
    // shuffle_order_engine<E,k>

    std::independent_bits_engine<std::default_random_engine, 4, unsigned int> ibe;
    for (int i = 0; i < 100; ++i) {
        std::cout << '0' + ibe() << '\n';
    }

    std::cout << '\n';

    /**
     * Random Device
     *
     * If an implementation is able to offer a truly random number generator, that source of random
     * numbers is presented as a uniform random number generator called random_device:
     */

    // random_device rd {s};
    // d=rd.entropy()

    std::random_device rd;

    std::cout << rd.entropy() << '\n';
    std::cout << std::random_device::min() << '\n';
    std::cout << std::random_device::max() << '\n';

    std::cout << '\n';

    for (int i = 0; i <= 5; ++i) {
        std::cout << rd() << '\n';
    }

    std::cout << '\n';

    /**
     * Distributions
     *
     * A random number distribution is a function object that, when called with a random number
     * generator argument, produces a sequence of values_ of its result_type:
     */

    // Random Number Distribution
    // Uniform Distributions
    // Bernoulli Distributions
    // Poisson Distributions
    // Normal Distributions
    // Sampling Distributions

    auto gen = bind(std::normal_distribution<double>{15, 4.0}, std::default_random_engine{});
    for (int i = 0; i < 10; ++i) {
        std::cout << std::string(gen(), '*') << '\n';
    }

    std::cout << '\n';

    auto gen2 = bind(std::uniform_real_distribution<double>{15, 4.0}, std::default_random_engine{});
    for (int i = 0; i < 10; ++i) {
        std::cout << std::string(gen2(), '*') << '\n';
    }

    std::cout << '\n';

    /**
     * C-Style Random Numbers
     *
     * In <cstdlib> and <stdlib.h>, the standard library provides a simple basis for the generation of random numbers
     */

    std::cout << RAND_MAX << '\n';

    srand(22);  // seed random number generator by i

    std::cout << rand() << '\n';  // pseudo-random number between 0 and RAND_MAX
}