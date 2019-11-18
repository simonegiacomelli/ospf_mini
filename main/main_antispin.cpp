#include <iostream>

#include <stdio.h>
#include "../src/ospf_mini_lib.h"
//
#include <chrono>
#include <thread>

#include <sstream>
#include <string>
#include <fstream>
#include <list>
#include <set>


int main() {
    AntiSpin x(1000);
    x.setOnExecuteLambda([&x]() {
        printf("ciao %lu\n", x.counter);
    });
    while (1) {
        x.spin();
    }
}