#include "buscontroller.h"
#include "math.h"

BusController::BusController(Memory *mem) {
    BusController::mem = mem;
}

void BusController::subscribe(CacheController *cacheController){
    CCSubscribed.push_back(cacheController);
}

void BusController::invalidate(CacheAddress add, CacheController* cacheController){
    write_queue_mtx.lock();
    Request request;
    request.add = add;
    request.cacheController = cacheController;
    writeRequest.push(request);
    write_queue_mtx.unlock();
}

void BusController::requestRead(CacheAddress add, CacheController* cacheController){
    read_queue_mtx.lock();
    Request request;
    request.add = add;
    request.cacheController = cacheController;
    readRequest.push(request);
    read_queue_mtx.unlock();
}

void BusController::writeBack(size_t add, std::string data){
    writeBackData = data;
    mem->access(add, data, AccessType::WRITE);
}

void BusController::run(){
    while (1) {
        while(!writeRequest.empty()){
            Request request = writeRequest.front();
            size_t dir = (request.add.tag << int(log2(BLOCK_SIZE))) + request.add.index;
            output = "Write Request from " + request.cacheController->id + ", invalidating caches";
            for(size_t i = 0; i < CCSubscribed.size(); ++i){
                output = request.cacheController->id + " Write-back to address " + std::to_string(dir);
                if(CCSubscribed.at(i) != request.cacheController){
                    CCSubscribed.at(i)->invalidate(request.add);
                }
            }
            request.cacheController->requestDone = true;
            writeRequest.pop();
        }
        if(!readRequest.empty()){
            Request request = readRequest.front();
            size_t dir = (request.add.tag << int(log2(BLOCK_SIZE))) + request.add.index;
            output = "Read Request from " + request.cacheController->id + " to read " + std::to_string(dir);
            //Notify to CC someone read
            for(size_t i = 0; i < CCSubscribed.size(); ++i){
                if(CCSubscribed.at(i) != request.cacheController){
                    CCSubscribed.at(i)->share(request.add, request.cacheController->id);
                }
            }
            //If someone migrated the data
            if (writeBackData != "") {
                request.cacheController->requestedData = writeBackData;
                writeBackData = "";
            }
            else //Read from memory
                request.cacheController->requestedData = mem->access(dir, "", AccessType::READ);
            request.cacheController->requestDone = true;
            readRequest.pop();
        }
        output = "Stand-by";
    }
}
