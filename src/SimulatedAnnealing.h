#pragma once
#include "DataLoader.h"
#include "TreeArray.h"
#include "Encoder.h"
#include "Constants.h"
#include <vector>

using namespace std;

struct Config
{
    string filePath;    // Path to input data
    int maxIter;        // Maximum number of iterations
    int stallIter;      // Maximum number of iterations without improvement
    double T;           // Initial temperature
    double Rt;          // Temperature reduction rate
    int initModRate;    // Initail modifcation rate
    double decayModRate;   // Decay rate of modification rate
    int maxTime;        // Maximum time in seconds
    bool deterministic; // Whether to use deterministic random number generator
    bool quiet;
    INIT_MODE initMode; // Initial code length mode
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
    int modRate;
    int maxLen;
    int minLen;
    int maxLenOfBest;
    int minLenOfBest;
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

        vector<History *> history;

        // utility functions
        void initTree(INIT_MODE mode);

    public:
        // constructor
        SimulatedAnnealing(Config *config);
        int run();
        void show();
        void show_compress_ratio();
        void show_history();
        void show_plot_info();
        Config *config;
        DataLoader *dl;
        TreeArray *tree;
        Encoder *encoder;
        TreeArray *bestTree;
    };
