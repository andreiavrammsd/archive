# Static task executor

Testing of template based task executor. Compared with a version based on dynamic polymorphism.

Read details in the [Executing tasks based on static polymorphism](https://blog.andreiavram.ro/cpp-tasks-compile-time-polymorphism/) article.

## Requirements
* C++11
* CMake 3.12+
* g++-9 (Ubuntu 9.3.0-11ubuntu0~18.04.1) 9.3.0

## Better tests with CPU scaling disabled
https://github.com/google/benchmark#disabling-cpu-frequency-scaling
```shell script
sudo cpupower frequency-set --governor performance
sudo cpupower frequency-set --governor powersave
```

## Results
See [benchmark results](./tests/results.csv).
