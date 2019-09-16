#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <string>
#include <mutex>
#include "constants.h"

class Memory{
private:
    std::string mem[DATA_SIZE];

public:
    Memory();
    std::string* getMemory();

    std::string access(size_t dir, std::string data, AccessType a);
};

#endif // MEMORY_H
