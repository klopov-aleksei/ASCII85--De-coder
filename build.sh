#!/bin/bash

build_app() 
{
    g++ -o ascii85 main.cpp
}

build_tests() 
{
    g++ -DUNIT_TEST -o test test.cpp main.cpp -lgtest -lgtest_main -lpthread
}

build_python_test()
{
    g++ -o ascii85 main.cpp
    python3 test.py
}

case "$1" in
    "test") build_tests ;;
    "pytest") build_python_test ;;
    *) build_app ;;
esac