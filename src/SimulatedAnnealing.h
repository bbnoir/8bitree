#pragma once
#include "DataLoader.h"
#include "TreeArray.h"
#include "CodeLength.h"
#include "Encoder2.h"
#include <vector>

using namespace std;

enum class INIT_MODE
{
    BALANCED,
    HUFFMAN,
    FOURBIT
};

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
};

class SimulatedAnnealing
{

public:
    int minMaxWidth;
    int initMaxWidth;
    double nonCompressedWidth;
    const int maxIter = 10;
    double T = 1000;
    double Rt = 0.99;
    int modRate = 1000;

    vector<History *> history;

    // utility functions
    void initCodeLength(INIT_MODE mode);

public:
    // constructor
    SimulatedAnnealing(Config *config);
    int run();
    void show();
    void show_compress_ratio();
    void show_history();
    Config *config;
    DataLoader *dl;
    CodeLength *cl;
    vector<int> bestCodeLength;
    Encoder2 *encoder;
};