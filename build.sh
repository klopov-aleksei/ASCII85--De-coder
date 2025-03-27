#!/bin/bash

build_app() 
{
    g++ -o ascii85 main.cpp
}

build_tests() 
{
    g++ -DUNIT_TEST -o test test.cpp main.cpp -lgtest -lgtest_main -lpthread
}
