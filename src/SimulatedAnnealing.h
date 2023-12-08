#pragma once
#include "DataLoader.h"
#include "TreeArray.h"
#include "Encoder.h"
#include <vector>

using namespace std;

struct Config
{
    string filePath;    // Path to input data
    int maxIter;        // Maximum number of iterations
    int stallIter;      // Maximum number of iterations without improvement
    double T;           // Initial temperature
    double Rt;          // Temperature reduction rate
    int modRate;        // Modification rate
    int maxTime;        // Maximum time in seconds
    bool deterministic; // Whether to use deterministic random number generator
};

struct History
{
    int iter;
    double T;
    int stall_count;
    int maxWidth;
    int newMaxWidth;
    int minMaxWidth;
    double time;
    double compress_ratio;
    double accept_prob;
};

class SimulatedAnnealing
{

public:
    int minMaxWidth;
    int balanceWidth;
    double initWidth;
    const int maxIter = 10;
    double T = 1000;
    double Rt = 0.99;
    int modRate = 1000;

    vector<History *> history;

    // utility functions

public:
    // constructor
    SimulatedAnnealing(Config *config);
    int run();
    void show();
    void show_compress_ratio();
    void show_history();
    Config *config;
    DataLoader *dl;
    TreeArray *tree;
    Encoder *encoder;
    TreeArray *bestTree;
};