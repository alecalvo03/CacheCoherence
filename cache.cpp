#include "cache.h"
#include <iostream>

Cache::Cache(){
    for(size_t i = 0; i < BLOCK_SIZE; ++i){
        cacheBlock[i].state = CacheState::Invalid;
        cacheBlock[i].tag = 0;
        cacheBlock[i].data = "0";
    }
}


