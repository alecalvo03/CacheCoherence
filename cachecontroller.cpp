#include "cachecontroller.h"

#include <iostream>
#include <math.h>
#include "constants.h"
#include "updategui.h"

CacheController::CacheController(std::string id, BusController* busController) {
    cache = new Cache();
    CacheController::busController = busController;
    CacheController::id = id;
    busController->subscribe(this);
}

CacheBlock* CacheController::getCacheBlock(){
    return cache->cacheBlock;
}

size_t getAddress(CacheAddress add){
    return (add.tag << int(log2(BLOCK_SIZE))) + add.index;
}

void CacheController::invalidate(CacheAddress add){
    size_t i = add.index;
    size_t tag = cache->cacheBlock[i].tag;
    if (tag == add.tag){
        if(cache->cacheBlock[i].state == CacheState::Modified){
            cache->cacheBlock[i].state = CacheState::Invalid;
            busController->writeBack(getAddress(add), cache->cacheBlock[i].data);
        } else {
            cache->cacheBlock[i].state = CacheState::Invalid;
        }
    }
}

void CacheController::share(CacheAddress add, std::string cacheid){
    size_t i = add.index;
    size_t tag = cache->cacheBlock[i].tag;
    if (tag == add.tag){
        if(cache->cacheBlock[i].state == CacheState::Modified){
            cache->cacheBlock[i].state = CacheState::Shared;
            guiout = "Migrating " + std::to_string(add.index) + " to " + cacheid;
        }
        //Migration
        if (cache->cacheBlock[i].state != CacheState::Invalid)
            busController->writeBackData = cache->cacheBlock[i].data;
    }
}

std::string CacheController::access(CacheAddress add, std::string data, AccessType a){
    size_t i = add.index;
    CacheState state = cache->cacheBlock[i].state;
    size_t tag = cache->cacheBlock[i].tag;
    requestDone = false;
    if (state == CacheState::Modified){
        //Local Write
        if(a == AccessType::WRITE){
            if (add.tag != tag){ //Write Miss
                guiout = "Write miss";
                size_t dir = (tag << int(log2(BLOCK_SIZE))) + add.index;
                busController->writeBack(dir, cache->cacheBlock[i].data);
                requestDone = false;
                busController->invalidate(add, this);
                while (!requestDone){
                    //Stall
                }
            } else
                guiout = "Write hit";
            cache->cacheBlock[i].data = data;
            cache->cacheBlock[i].tag = add.tag;
            return "";
        }
        //Local Read
        if(a == AccessType::READ){
            if (add.tag == tag) {
                guiout = "Read hit";
                return cache->cacheBlock[i].data;
            }
            else { //Read Miss
                guiout = "Read miss";
                //Read Request
                size_t dir = (tag << int(log2(BLOCK_SIZE))) + add.index;
                busController->writeBack(dir,cache->cacheBlock[i].data);
                requestDone = false;
                busController->requestRead(add, this);
                while (!requestDone){
                    //Stall until bus controller processes the request
                }
                cache->cacheBlock[i].data = requestedData;
                cache->cacheBlock[i].state = CacheState::Shared;
                cache->cacheBlock[i].tag = add.tag;
                return requestedData;
            }
        }
    } else if (state == CacheState::Shared){
        //Local Write and Invalidation Request
        if(a == AccessType::WRITE){
            if (add.tag == tag)
                guiout = "Write hit";
            else
                guiout = "Write miss";
            cache->cacheBlock[i].data = data;
            cache->cacheBlock[i].tag = add.tag;
            //Invalidation Request
            requestDone = false;
            busController->invalidate(add, this);
            while (!requestDone){
                //Stall
            }
            cache->cacheBlock[i].state = CacheState::Modified;
            return "";
        }
        //Local Read
        if(a == AccessType::READ){
            if (add.tag == tag) {
                guiout = "Read hit";
                return cache->cacheBlock[i].data;
            }
            else { //Read Miss
                guiout = "Read miss";
                //Read Request
                requestDone = false;
                busController->requestRead(add, this);
                while (!requestDone){
                    //Stall until bus controller processes the request
                }
                cache->cacheBlock[i].data = requestedData;
                cache->cacheBlock[i].state = CacheState::Shared;
                cache->cacheBlock[i].tag = add.tag;
                return requestedData;
            }
        }
    } else if (state == CacheState::Invalid){
        //Local Write and Write Request
        if(a == AccessType::WRITE){
            guiout = "Write miss";
            cache->cacheBlock[i].data = data;
            cache->cacheBlock[i].tag = add.tag;
            //Write Request
            requestDone = false;
            busController->invalidate(add, this);
            while (!requestDone){
                //Stall
            }
            cache->cacheBlock[i].state = CacheState::Modified;
            return "";
        }
        //Local Read and Read Request
        if(a == AccessType::READ){
            guiout = "Read miss";
            //Read Request
            requestDone = false;
            busController->requestRead(add, this);
            while (!requestDone){
                //Stall
            }
            cache->cacheBlock[i].data = requestedData;
            cache->cacheBlock[i].state = CacheState::Shared;
            cache->cacheBlock[i].tag = add.tag;
            return requestedData;
        }
    } else {
        return "";
    }
}
