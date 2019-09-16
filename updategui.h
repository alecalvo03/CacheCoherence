#ifndef UPDATEGUI_H
#define UPDATEGUI_H

#include <QThread>
#include <QMetaType>
#include "constants.h"
#include "cache.h"
#include "buscontroller.h"
#include "processor.h"
#include "cachecontroller.h"
#include <thread>
#include <iostream>

class UpdateGUI : public QThread{
    Q_OBJECT
public:
    UpdateGUI(){

    }
    bool ready = false;
    void run(){
        while(1){
            if (ready) {
                if(!cacheBlocks.empty()) emit updateCache(cacheBlocks);
                if(memory != nullptr) emit updateMemory(memory);
                if(bus != nullptr) emit updateBus(bus->output);
                if(!CPUs.empty()) emit updateCPUs(CPUs);
                if(!cacheControllers.empty()) emit updateCC(cacheControllers);
                std::this_thread::sleep_for(std::chrono::milliseconds(CLOCK_PERIOD/4));
            }
        }
    }
    void setCaches(CacheBlock *cacheBlock1, CacheBlock *cacheBlock2, CacheBlock *cacheBlock3, CacheBlock *cacheBlock4){
        cacheBlocks.push_back(cacheBlock1);
        cacheBlocks.push_back(cacheBlock2);
        cacheBlocks.push_back(cacheBlock3);
        cacheBlocks.push_back(cacheBlock4);
    }

    void setCPUs(Processor *cpu1, Processor *cpu2, Processor *cpu3, Processor *cpu4){
        CPUs.push_back(cpu1);
        CPUs.push_back(cpu2);
        CPUs.push_back(cpu3);
        CPUs.push_back(cpu4);
    }

    void setCacheControllers(CacheController *cc1, CacheController *cc2, CacheController *cc3, CacheController *cc4){
        cacheControllers.push_back(cc1);
        cacheControllers.push_back(cc2);
        cacheControllers.push_back(cc3);
        cacheControllers.push_back(cc4);
    }

    void setMemory(std::string *mem){
        memory = mem;
    }

    void setBus(BusController *bus){
        UpdateGUI::bus = bus;
    }

private:
    std::vector<Processor*> CPUs;
    std::vector<CacheBlock*> cacheBlocks;
    std::vector<CacheController*> cacheControllers;
    std::string *memory;
    BusController *bus;

signals:
    void updateCPUs(std::vector<Processor*> CPUs);
    void updateCache(std::vector<CacheBlock*> cacheBlocks);
    void updateCC(std::vector<CacheController*> cacheControllers);
    void updateMemory(std::string *memory);
    void updateBus(std::string str);
};

#endif // UPDATEGUI_H
