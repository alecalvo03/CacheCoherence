#include "processor.h"
#include <math.h>

Processor::Processor(std::string id, BusController *busController){
    cacheController = new CacheController(id, busController);
    Processor::id = id;
    std::random_device rd;
    generator.seed(rd());
}

size_t Processor::getRandomFromDistribution(DistributionType dist, size_t limit){
    size_t n;

    if (dist == DistributionType::BINOMIAL){
        std::binomial_distribution<int> distribution(limit,B_PROB_SUCC);
        n = distribution(generator);
    } else if (dist == DistributionType::GEOMETRIC){
        std::geometric_distribution<int> distribution(G_PROB_SUCC);
        n = distribution(generator);
    } else if (dist == DistributionType::POISSON){
        std::poisson_distribution<int> distribution(P_MEAN);
        n = distribution(generator);
    } else if (dist == DistributionType::NORMAL){
        std::normal_distribution<double> distribution(N_MEAN,N_STDDEV);
        n = int(distribution(generator));
    } else {
        n = 0;
    }

    //Limit conditions
    if (n > limit) return limit;
    if (n < 0) return 0;
    return n;
}

void Processor::previewDistribution(DistributionType dist){
    size_t p[16]={};

    for (int i=0; i<10000; ++i) {
        size_t n = getRandomFromDistribution(dist, 15);
        ++p[n];
    }

    for (int i=0; i<16; ++i){
        if (i < 10) std::cout << "0";
        std::cout << i << ": " << std::string(p[i]*100/10000,'*') << std::endl;
    }
}

CacheBlock* Processor::getCacheBlock(){
    return cacheController->getCacheBlock();
}

void Processor::run(bool *clk, std::condition_variable *cv){
    std::mutex m;
    std::mutex m2;

    //Bernoulli Distribution
    std::bernoulli_distribution mem_distribution(MEM_ACCESS_PROB);
    std::bernoulli_distribution write_distribution(WRITE_PROB);

    //Main processor loop
    while(1){
        //Lock and wait for clock
        std::unique_lock<std::mutex> lock(m);
        while (!*clk) cv->wait(lock);


        if(mem_distribution(generator)){ //Memory acess operation
            size_t add = getRandomFromDistribution(CURRENT_DISTRIBUTION, DATA_SIZE);

            CacheAddress c_add;
            //Direct map
            c_add.index = add%BLOCK_SIZE;
            c_add.tag = add >> int(log2(BLOCK_SIZE));

            if(write_distribution(generator)){ //Write to Memory
                guiout = "Write to address " + std::to_string(add);

                cacheController->access(c_add, id, AccessType::WRITE);
            } else { //Read from Memory
                guiout = "Read from address " + std::to_string(add);
                cacheController->access(c_add, id, AccessType::READ);
            }
        } else { //Process operation
            guiout = "Process";
            cacheController->guiout = "";
        }

        //Lock and wait for clock
        std::unique_lock<std::mutex> lock2(m2);
        while (*clk) cv->wait(lock2);
    }
}
