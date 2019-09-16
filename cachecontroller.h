#ifndef CACHECONTROLLER_H
#define CACHECONTROLLER_H

#include <stdio.h>
#include "cache.h"
#include "memory.h"
#include "buscontroller.h"
#include "constants.h"

class BusController;

class CacheController{
private:
    Cache* cache;
    BusController* busController;

public:
    volatile bool requestDone = false;
    std::string requestedData;
    std::string id;
    std::string guiout = "";
    CacheController(std:: string, BusController *busController);
    std::string access(CacheAddress add, std::string data, AccessType a);
    CacheBlock* getCacheBlock();
    void invalidate(CacheAddress add);
    void share(CacheAddress add, std::string cacheid);
};

#endif // CACHECONTROLLER_H
