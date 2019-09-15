#ifndef BUSCONTROLLER_H
#define BUSCONTROLLER_H

#include "cachecontroller.h"
#include "memory.h"
#include <string>
#include <queue>

class CacheController;

struct BusRequest{
    CacheController *cacheController;
    size_t add;
    std::string data;
    AccessType accessType;
    CacheState cacheState;
};

struct Request{
    CacheController *cacheController;
    CacheAddress add;
};

class BusController {
private:
    Memory* mem;
    std::queue<BusRequest> requestQ;
    std::vector<CacheController*> CCSubscribed;
    std::queue<Request> writeRequest;
    std::queue<Request> readRequest;

public:
    std::string writeBackData;
    std::string output = "";

    BusController(Memory *mem);
    void requestBus(CacheController *cacheController, size_t add, std::string data, AccessType accessType, CacheState cacheState);
    void subscribe(CacheController *cacheController);
    void invalidate(CacheAddress add, CacheController *cacheController);
    void requestRead(CacheAddress add, CacheController *cacheController);
    void writeBack(size_t add, std::string data);
    void run();
};

#endif // BUSCONTROLLER_H
