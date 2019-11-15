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
    ~PeerDevice() {
    }

    Address address;
    int cost;


    bool operator<(const PeerDevice &other) const {
        return address < other.address;
    }

    void debug() {
        printf("  PeerDevice %s\n", to_string().c_str());
    }

    string to_string() const {
        return address + " cost=" + std::to_string(cost) + " age=" + std::to_string(millis() - birth_ms);
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
    map<Address, PeerDevice> peers;
    set<LinkState> database;

public:

    virtual void detectPeerDevices() = 0;

    virtual void poll() = 0;

    void detectStalePeerDevices() {
        set<Address> stale;
        for (const auto &any : peers) {
            if (any.second.stale()) {
                stale.insert(any.second.address);
                printf("%s STALE\n", any.second.to_string().c_str());
            }
        }

        for (const auto& addr : stale)
        {
            peers.erase(addr);
        }

    }

    int handle() {
        delay(100);
        //delay(1000);
        //printf("ms,%lu\n",ms());
        poll();
        //debug();
        detectPeerDevices();
        detectStalePeerDevices();
        return 0;
    }

    void addPeerDevice(PeerDevice device) {
        if (!mapContainsKey(peers, device.address)) {
//            new element
            printf("%s NEW\n", device.to_string().c_str());
            peers[device.address] = device;
        }
        peers[device.address].refresh();
    }

    int debug() {
        printf("\nDebug linkState.adjacency msec %lu\n", millis());
        for (auto peer : linkState.adjacency) {
            peer.debug();
        }
    }

};


#endif //_OSPF_MINI_LIB_H_
