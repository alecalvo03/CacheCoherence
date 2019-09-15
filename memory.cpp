#include "memory.h"
#include <thread>
#include <string>
#include <chrono>

Memory::Memory(){
    for(size_t i = 0; i < DATA_SIZE; ++i){
        mem[i] = "0";
    }
}

std::string* Memory::getMemory(){
    return mem;
}

std::string Memory::access(size_t dir, std::string data = "", AccessType a = AccessType::READ){
    //Make sure direction is within boundaries
    if (dir < DATA_SIZE){
        std::this_thread::sleep_for(std::chrono::seconds(MEM_DELAY));
        //Write operation
        if(a == AccessType::WRITE){
            mem[dir] = data;
            return "";
        }
        //Read operation
        if(a == AccessType::READ){
            return mem[dir];
        }
        std::cout << "Access Type Error" << std::endl;
        return "";
    }
    std::cout << "Direction Error" << std::endl;
    return "";
}
