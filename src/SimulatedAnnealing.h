#pragma once
#include "DataLoader.h"
#include "TreeArray.h"
#include "Encoder.h"

using namespace std;

struct Config
{
    string filePath; // Path to input data
    int maxIter;     // Maximum number of iterations
    double T;        // Initial temperature
    double Rt;       // Temperature reduction rate
    int modRate;     // Modification rate
};

class SimulatedAnnealing
{

private:
    // config data
    int minMaxWidth;
    const int maxIter = 10;
    double T = 1000;
    double Rt = 0.99;
    int modRate = 1000;

    // utility functions

public:
    // constructor
    SimulatedAnnealing(Config *config);
    int run();
    void show();
    Config *config;
    DataLoader *dl;
    TreeArray *tree;
    Encoder *encoder;
    TreeArray *bestTree;
};