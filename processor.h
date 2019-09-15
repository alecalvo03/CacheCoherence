#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "cachecontroller.h"
#include "memory.h"
#include "constants.h"
#include <string>
#include <condition_variable>
#include <mutex>
#include <random>
#include <QThread>
#include <iostream>

class Processor {
private:
    std::string id;
    std::default_random_engine generator;
    size_t getRandomFromDistribution(DistributionType dist, size_t limit);

public:
    std::string guiout = "";
    CacheController* cacheController;
    Processor(std::string id, BusController *busController);
    void previewDistribution(DistributionType dist);
    void run(bool *clk, std::condition_variable *cv);
    CacheBlock* getCacheBlock();
};

#endif // PROCESSOR_H
