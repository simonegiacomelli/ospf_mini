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

using namespace std;

struct OspfNanoFile : OspfNano {

    void detectPeerDevices() override {
        ifstream infile("o_link_state.txt");
        string line;
        while (getline(infile, line)) {
            stringstream ss(line);
            auto p = PeerDevice();
            ss >> p.address;
            if (p.address.empty())
                break; //a blank line interrupts parsing
            if (p.address.rfind("#", 0) == 0)
                continue; //it's a comment
            ss >> p.cost;
            p.cost = -p.cost;
            addPeerDevice(p);
        }
    }

    void poll() override {

    }
};


int main() {
    PeerDevice p;

    OspfNanoFile ospf;
    using namespace this_thread; // sleep_for, sleep_until
//    using namespace chrono; // nanoseconds, system_clock, seconds
    while (true) {
//    for (int i = 0; i < 1000; i++) {
        ospf.handle();
    }

}