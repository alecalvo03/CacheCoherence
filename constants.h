#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

enum class AccessType {
    WRITE, READ
};

enum class DistributionType {
    BINOMIAL, POISSON, GEOMETRIC, NORMAL
};

const int CPU_NUM = 4;
const size_t DATA_SIZE = 16;
const int BLOCK_SIZE = 8;
const DistributionType CURRENT_DISTRIBUTION = DistributionType::NORMAL;

//Delays
const int CLOCK_PERIOD = 4;
const int MEM_DELAY = 3*CLOCK_PERIOD;
extern bool CLK;
extern bool MANUAL_CLOCK;

//Bernoulli
const double MEM_ACCESS_PROB = 0.5;
const double WRITE_PROB = 0.5;

//Binomial
const double B_PROB_SUCC = 0.5;

//Poisson
const double P_MEAN = 11.0;

//Geometric
const double G_PROB_SUCC = 0.2;

//Normal
const double N_MEAN = 8.0;
const double N_STDDEV = 3.0;

#endif // CONSTANTS_H
