#pragma once
#include "DataLoader.h"
#include "TreeArray.h"

using namespace std;

struct Config
{
    string filePath; // Path to input data
    int maxIter;     // Maximum number of iterations
    int T;           // Initial temperature
    int Rt;          // Temperature reduction rate
};

class SimulatedAnnealing
{

private:
    // config data
    DataLoader *dl;
    TreeArray *tree;
    const int maxIter = 10;
    double T = 1000;
    double Rt = 0.99;
    // utility functions
    vector<int> genCodeLength();

public:
    // constructor
    SimulatedAnnealing(Config *config);
    int run();
    void show();
    Config *config;

    int getMaxWidth();
};