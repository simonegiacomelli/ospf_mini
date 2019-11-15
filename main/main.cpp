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
    void newAdjacency() override {
        updateset(linkState.adjacency);
    }

    void updateset(set<PeerDevice> &adjacency) {
        ifstream infile("o_link_state.txt");
        string line;
        while (getline(infile, line)) {
            stringstream ss(line);
            auto p = PeerDevice();
            ss >> p.address;
            if (p.address == "")
                break;
            ss >> p.cost;
            p.cost = -p.cost;
            p.millis = millis();
            adjacency.insert(p);
        }
    }

    void poll() override {

    }
};


int main() {
    OspfNanoFile ospf;
    using namespace this_thread; // sleep_for, sleep_until
//    using namespace chrono; // nanoseconds, system_clock, seconds
    while (true) {
        ospf.handle();
    }

}