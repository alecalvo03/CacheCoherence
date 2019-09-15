#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <string>
#include "constants.h"

enum class CacheState {
    Modified, Shared, Invalid
};

struct CacheAddress{
    size_t tag;
    size_t index;
};

struct CacheBlock {
    volatile CacheState state;
    size_t tag;
    std::string data;
};

class Cache {

public:
    CacheBlock cacheBlock[BLOCK_SIZE];
    Cache();
};

#endif // CACHE_H
