#include <iostream>

#include <stdio.h>
#include "../ospf_mini_lib.h"
//
#include <chrono>
#include <thread>

#include <sstream>
#include <string>
#include <fstream>
#include <list>
#include <set>

class AntiSpin {
public:
    AntiSpin(int window_ms) {
        this->window_ms = window_ms;
        last_execute_millis = 0;
    }

    AntiSpin &setOnExecuteLambda(std::function<void()> l) {
        this->onExecuteLambda = l;
        return *this;
    }

    bool spin() {
        auto ms = millis();
        auto delta = ms - last_execute_millis;
        if (counter == 0 || delta > window_ms) {
            counter++;
            last_execute_millis = ms;
            onExecuteLambda();
        }

    }

    unsigned long counter = 0;
protected:
    std::function<void()> onExecuteLambda;
    unsigned long last_execute_millis;

    int window_ms;
};

int main() {
    AntiSpin x(1000);
    x.setOnExecuteLambda([&x]() {
        printf("ciao %lu\n", x.counter);
    });
    while (1) {
        x.spin();
    }
}