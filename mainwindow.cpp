#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "updategui.h"
#include "constants.h"
#include <iostream>
#include <string>

bool MANUAL_CLOCK;

MainWindow::MainWindow(UpdateGUI *ug, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qRegisterMetaType<std::vector<Processor*>>("std::vector<Processor*>");
    qRegisterMetaType<std::vector<CacheBlock*>>("std::vector<CacheBlock*>");
    qRegisterMetaType<std::vector<CacheController*>>("std::vector<CacheController*>");
    qRegisterMetaType<std::string>("std::string");
    ui->setupUi(this);
    updategui = ug;

    MANUAL_CLOCK = ui->cb_clk->isChecked();

    //Connect signals to slots
    connect(updategui, SIGNAL(updateCPUs(std::vector<Processor*>)), this , SLOT(updateCPUs(std::vector<Processor*>)));
    connect(updategui, SIGNAL(updateCache(std::vector<CacheBlock*>)), this, SLOT(updateCaches(std::vector<CacheBlock*>)));
    connect(updategui, SIGNAL(updateMemory(std::string*)), this, SLOT(updateMemory(std::string*)));
    connect(updategui, SIGNAL(updateBus(std::string)), this, SLOT(updateBus(std::string)));
    connect(updategui, SIGNAL(updateCC(std::vector<CacheController*>)), this, SLOT(updateCC(std::vector<CacheController*>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::string stringState(CacheState state){
    if(state == CacheState::Modified){
        return "M";
    } else if(state == CacheState::Shared) {
        return "S";
    } else if (state == CacheState::Invalid) {
        return "I";
    }
    return "";
}

void MainWindow::updateCPUs(std::vector<Processor*> CPUs){
    //Clear all previous elements in the list
    ui->lv_cpu1->clear();
    ui->lv_cpu2->clear();
    ui->lv_cpu3->clear();
    ui->lv_cpu4->clear();
    //Update CPU GUI
    if (CPUs.size() == 4){
        ui->lv_cpu1->addItem(CPUs.at(0)->guiout.c_str());
        ui->lv_cpu2->addItem(CPUs.at(1)->guiout.c_str());
        ui->lv_cpu3->addItem(CPUs.at(2)->guiout.c_str());
        ui->lv_cpu4->addItem(CPUs.at(3)->guiout.c_str());
    }
}

void MainWindow::updateCC(std::vector<CacheController*> cacheControllers){
    //Clear all previous elements in the list
    ui->lv_cc1->clear();
    ui->lv_cc2->clear();
    ui->lv_cc3->clear();
    ui->lv_cc4->clear();
    //Update CacheController GUI
    if (cacheControllers.size() == 4){
        ui->lv_cc1->addItem(cacheControllers.at(0)->guiout.c_str());
        ui->lv_cc2->addItem(cacheControllers.at(1)->guiout.c_str());
        ui->lv_cc3->addItem(cacheControllers.at(2)->guiout.c_str());
        ui->lv_cc4->addItem(cacheControllers.at(3)->guiout.c_str());
    }
}

void MainWindow::updateMemory(std::string *memory){
    ui->lv_memory->clear();
    if (memory != nullptr){
        for(int i = 0; i < DATA_SIZE; ++i){
            std::string str;
            if (i < 10) str += "0";
            str += std::to_string(i) + " |\t" + memory[i];
            ui->lv_memory->addItem(str.c_str());
        }
    }
}

void MainWindow::updateCaches(std::vector<CacheBlock*> cacheBlocks){
    //Clear all previous elements in the list
    ui->lv_cache1->clear();
    ui->lv_cache2->clear();
    ui->lv_cache3->clear();
    ui->lv_cache4->clear();
    //Update Cache GUI
    if(cacheBlocks.size() == 4){
        //Add headers
        std::string header = "i | V | t | data";
        ui->lv_cache1->addItem(header.c_str());
        ui->lv_cache2->addItem(header.c_str());
        ui->lv_cache3->addItem(header.c_str());
        ui->lv_cache4->addItem(header.c_str());

        header = "----------------";
        ui->lv_cache1->addItem(header.c_str());
        ui->lv_cache2->addItem(header.c_str());
        ui->lv_cache3->addItem(header.c_str());
        ui->lv_cache4->addItem(header.c_str());

        //Populate caches
        for (int i = 0; i < BLOCK_SIZE; ++i){
            std::string str = std::to_string(i) + " | " + stringState(cacheBlocks.at(0)[i].state) + " | " + std::to_string(cacheBlocks.at(0)[i].tag) + " | " + cacheBlocks.at(0)[i].data.c_str();
            ui->lv_cache1->addItem(str.c_str());
        }
        for (int i = 0; i < BLOCK_SIZE; ++i){
            std::string str = std::to_string(i) + " | " + stringState(cacheBlocks.at(1)[i].state) + " | " + std::to_string(cacheBlocks.at(1)[i].tag) + " | " + cacheBlocks.at(1)[i].data.c_str();
            ui->lv_cache2->addItem(str.c_str());
        }
        for (int i = 0; i < BLOCK_SIZE; ++i){
            std::string str = std::to_string(i) + " | " + stringState(cacheBlocks.at(2)[i].state) + " | " + std::to_string(cacheBlocks.at(2)[i].tag) + " | " + cacheBlocks.at(2)[i].data.c_str();
            ui->lv_cache3->addItem(str.c_str());
        }
        for (int i = 0; i < BLOCK_SIZE; ++i){
            std::string str = std::to_string(i) + " | " + stringState(cacheBlocks.at(3)[i].state) + " | " + std::to_string(cacheBlocks.at(3)[i].tag) + " | " + cacheBlocks.at(3)[i].data.c_str();
            ui->lv_cache4->addItem(str.c_str());
        }
    }
}

void MainWindow::updateBus(std::string str){
    ui->lb_bus->setText(str.c_str());
}

void MainWindow::on_cb_clk_clicked() {
    if (ui->cb_clk->isChecked()){
        MANUAL_CLOCK = true;
        ui->btn_clk->setEnabled(true);
    }
    else {
        MANUAL_CLOCK = false;
        ui->btn_clk->setEnabled(false);
    }
}

void MainWindow::on_btn_clk_clicked() {
    CLK = !CLK;
}
