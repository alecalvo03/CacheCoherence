#include "mainwindow.h"
#include "memory.h"
#include "processor.h"
//#include "constants.h"
#include <thread>
#include <chrono>
#include <unistd.h>
#include <iostream>
#include <QApplication>

bool CLK;

void init_processors(UpdateGUI *updateGUI){
    Memory *mem = new Memory();

    BusController busController(mem);

    std::thread tbus(&BusController::run, &busController);

    Processor CPU1("CPU1", &busController);
    Processor CPU2("CPU2", &busController);
    Processor CPU3("CPU3", &busController);
    Processor CPU4("CPU4", &busController);

    //GUI initialization
    updateGUI->setCPUs(&CPU1, &CPU2, &CPU3, &CPU4);
    updateGUI->setCacheControllers(CPU1.cacheController, CPU2.cacheController, CPU3.cacheController, CPU4.cacheController);
    updateGUI->setCaches(CPU1.getCacheBlock(), CPU2.getCacheBlock(), CPU3.getCacheBlock(), CPU4.getCacheBlock());
    updateGUI->setMemory(mem->getMemory());
    updateGUI->setBus(&busController);
    updateGUI->ready = true;

    std::condition_variable cv;

    std::thread t1(&Processor::run, &CPU1, &CLK, &cv);
    std::thread t2(&Processor::run, &CPU2, &CLK, &cv);
    std::thread t3(&Processor::run, &CPU3, &CLK, &cv);
    std::thread t4(&Processor::run, &CPU4, &CLK, &cv);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    bool prev_clk = false;

    //Clock loop
    while (1){
        if (!MANUAL_CLOCK) {
            CLK = true;
            cv.notify_all();
            std::this_thread::sleep_for(std::chrono::milliseconds(CLOCK_PERIOD/2));

            CLK = false;
            cv.notify_all();
            std::this_thread::sleep_for(std::chrono::milliseconds(CLOCK_PERIOD/2));
        } else {
            if (CLK != prev_clk){
                prev_clk = CLK;
                cv.notify_all();
            }
        }
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    UpdateGUI *updateGUI = new UpdateGUI();
    MainWindow w(updateGUI);
    w.show();

    std::thread t1(init_processors, updateGUI);
    updateGUI->start();

    return a.exec();
}
