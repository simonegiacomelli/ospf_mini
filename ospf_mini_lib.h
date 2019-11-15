#ifndef _OSPF_MINI_LIB_H_
#define _OSPF_MINI_LIB_H_

#include "ospf_mini_dep.h"
#include <list>
#include <set>

using namespace std;

typedef string Address;

class PeerDevice {
public:
    ~PeerDevice() {
    }

    Address address;
    int cost;
    unsigned long millis;

    bool operator<(const PeerDevice &other) const {
        return address < other.address;
    }

    void debug() {
        printf("  PeerDevice address:");
        printf("%s", address.c_str());
        printf(" cost: %d", cost);
        printf("\n");
    }
};

class LinkState {
public:
    Address address;
    set<PeerDevice> adjacency;
    unsigned long sequence;
};

class Message {
    Address from;
    int len;
    char *body;
};

class OspfNano {
protected:
    LinkState linkState;
    LinkState prev_linkState;
    set<LinkState> database;

public:

    virtual void newAdjacency() = 0;

    virtual void poll() = 0;

    int handle() {
        //delay(1000);
        //printf("millis,%lu\n",millis());
        poll();
        //debug();
        newAdjacency();

        return 0;
    }

    void lsaReceived(LinkState &ls) {

    }

    int debug() {
        printf("\nDebug linkState.adjacency msec %d\n", millis());
        for (auto peer : linkState.adjacency) {
            peer.debug();
        }
    }

};


#endif //_OSPF_MINI_LIB_H_
