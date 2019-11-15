#ifndef _OSPF_MINI_LIB_H_
#define _OSPF_MINI_LIB_H_

#include "ospf_mini_dep.h"
#include <list>
#include <set>
#include <map>

using namespace std;
static const long STALE_PEER_DEVICE_MS = 2000;

typedef string Address;

class PeerDevice {
private:
    unsigned long birth_ms;
public:
    PeerDevice(){
        refresh();
        //printf("%s NEW\n",to_string().c_str());
    }
    ~PeerDevice() {
//        printf("%s DEL\n",to_string().c_str());
    }

    Address address;
    int cost;


    bool operator<(const PeerDevice &other) const {
        return address < other.address;
    }

    void debug() {
        printf("  PeerDevice %s\n", to_string().c_str());
    }

    string to_string() {
        PeerDevice *p = this;
        return address + " cost=" + std::to_string(cost) + " age=" + std::to_string(age()) + " p=" +
               std::to_string((long) p);
    }

    void refresh() {
        birth_ms = millis();
    }

    unsigned long age() const {
        return millis() - birth_ms;
    }

    bool stale() const {
        return age() > STALE_PEER_DEVICE_MS;
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

template<class K, class V>
bool mapContainsKey(std::map<K, V> &map, K &key) {
    return map.find(key) != map.end();
}

class OspfNano {
protected:
    LinkState linkState;
    std::map<Address, PeerDevice*> peers;
    set<LinkState> database;

public:

    virtual void detectPeerDevices() = 0;

    virtual void poll() = 0;

    void detectStalePeerDevices() {
        set<Address> stale;
        for (const auto &any : peers) {
            if (any.second->stale()) {
                stale.insert(any.second->address);
                auto peer = peers[any.second->address];
                printf("%s GONE\n", peer->to_string().c_str());
                delete peer;
            }
        }

        for (const auto &addr : stale) {
            peers.erase(addr);
        }

    }

    int handle() {
//        delay(100);
        //delay(1000);
        //printf("ms,%lu\n",ms());
        poll();
        //debug();
        detectPeerDevices();
        detectStalePeerDevices();
        return 0;
    }

    void addPeerDevice(PeerDevice device) {
        bool present = mapContainsKey(peers, device.address);
        if (!present) {
//            new element
            auto p= new PeerDevice();
            p->address = device.address;
            p->cost = device.cost;
            printf("%s NEW\n", p->to_string().c_str());
            peers[device.address] = p;
        }
        peers[device.address]->refresh();
    }

    int debug() {
        printf("\nDebug linkState.adjacency msec %lu\n", millis());
        for (auto peer : linkState.adjacency) {
            peer.debug();
        }
    }

};


#endif //_OSPF_MINI_LIB_H_
