#include "SimulatedAnnealing.h"
#include "DataLoader.h"
#include "TreeArray.h"
#include <iomanip>
#include <cmath>
#include <chrono>

using namespace std;

void SimulatedAnnealing::initTree(INIT_MODE mode)
{
    switch (mode)
    {
    case INIT_MODE::BALANCED:
        tree = new TreeArray(dl->getNumInts());
        break;
    case INIT_MODE::HUFFMAN:
        tree = new TreeArray(dl->getFreqMap());
        break;
    case INIT_MODE::FOURBIT:    // not finished
        tree = new TreeArray(dl->getNumInts());
        break;
    default:
        tree = new TreeArray(dl->getNumInts());
        break;
    }
}

SimulatedAnnealing::SimulatedAnnealing(Config *config) : config(config), maxIter(config->maxIter), T(config->T), Rt(config->Rt), minMaxWidth(0)
{
    this->config = config;
    dl = new DataLoader(config->filePath);
    initTree(config->initMode);
    encoder = new Encoder(dl, tree);
    bestTree = new TreeArray(*tree);
    minMaxWidth = encoder->getBestWidth();
    balanceWidth = minMaxWidth;
    initWidth = dl->getElementPerLine() * 8;
}

bool accept(int newMaxWidth, int MaxWidth, double initWidth, double T)
{
    if (newMaxWidth < MaxWidth)
        return true;
    else if (newMaxWidth == MaxWidth)
        return (double)rand() / RAND_MAX < 0.4;
    else if (exp((MaxWidth - newMaxWidth) / initWidth * 10000 / T) > (double)rand() / RAND_MAX)
        return true;
    else
        return false;
}

int SimulatedAnnealing::run()
{
    int MaxWidth = minMaxWidth;
    int newMaxWidth = 0;
    int stall_count = 0;
    int maxTime = config->maxTime;
    auto start = chrono::steady_clock::now();
    unsigned seed = 42;
    if (!config->deterministic)
        seed = time(NULL);
    srand(seed);
    double modRate = config->initModRate;
    int roundModRate;
    double decayModRate = config->decayModRate;
    for (int iter = 0; iter < maxIter; iter++)
    {
        History *h = new History();
        h->iter = iter;
        h->T = T;
        h->stall_count = stall_count;
        h->time = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start).count();
        h->maxWidth = MaxWidth;
        roundModRate = int(modRate);
        h->modRate = roundModRate;
        // tree->modify(rand() % int(modRate) + 1);
        if (roundModRate > 5)
            tree->modify(roundModRate);
        else
            tree->modify(rand() % roundModRate + 1);
        if (roundModRate > 5)
            modRate *= decayModRate;
        newMaxWidth = encoder->getBestWidth(); // get new energy
        h->newMaxWidth = newMaxWidth;
        h->compress_ratio = (initWidth - newMaxWidth) / initWidth * 100;

        // if (modRate < 0.1 * config->initModRate)
        if (modRate < 15)
            stall_count++;
        if (newMaxWidth < minMaxWidth) // early stop
        {
            minMaxWidth = newMaxWidth;
            delete bestTree;
            bestTree = new TreeArray(*tree);
        }
        h->maxLen = tree->getMaxLen();
        h->minLen = tree->getMinLen();
        h->maxLenOfBest = bestTree->getMaxLen();
        h->minLenOfBest = bestTree->getMinLen();
        h->minMaxWidth = minMaxWidth;
        h->accept_prob = exp((MaxWidth - newMaxWidth) / initWidth * 10000 / T) * 100;
        if (h->accept_prob > 100)
            h->accept_prob = 100;

        if (accept(newMaxWidth, MaxWidth, initWidth, T))
        {
            MaxWidth = newMaxWidth;
            stall_count = 0;
        }
        else
    {
            tree->recover();
        }

        if (stall_count > config->stallIter)
        {
            cout << "Stop SA at iter " << iter << " since exceeding stall limit" << endl;
            break;
        }
        else if (chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start).count() > maxTime)
        {
            cout << "Stop SA at iter " << iter << " since exceeding time limit" << endl;
            break;
        }

        T *= Rt; // reduce temperature

        history.push_back(h);

        if (!config->quiet)
        {
            cout.flush();
            cout << "\r"
                << "Iter: " << iter << " / " << maxIter << " -> Compress ratio: " << fixed << setprecision(2) << h->compress_ratio << "%";
        }
    }
    return minMaxWidth;
}

void SimulatedAnnealing::show()
{
    cout << "=======================================================================================" << endl;
    cout << "======================================= Result ========================================" << endl;
    cout << "=======================================================================================" << endl;
    cout << "Input file: " << config->filePath << endl;
    cout << "Maximum number of iterations: " << config->maxIter << endl;
    cout << "Initial temperature: " << config->T << endl;
    cout << "Temperature reduction rate: " << config->Rt << endl;
    cout << "=======================================================================================" << endl;
    cout << "Result Tree Array:" << endl;
    cout << *bestTree << endl;
    cout << "=======================================================================================" << endl;
    cout << "Assignment mode: ";
    int bestWay = encoder->getBestWay();
    if (bestWay == FQ)
        cout << "FQ" << endl;
    else if (bestWay == EX)
        cout << "EX" << endl;
    else if (bestWay == OC)
        cout << "OC" << endl;
    cout << "Initial row width: " << initWidth << endl;
    cout << "Maximum row width: " << minMaxWidth << endl;
    cout << "Compression ratio: " << (initWidth - minMaxWidth) / initWidth * 100 << "%" << endl;
    cout << "=======================================================================================" << endl;
}

void SimulatedAnnealing::show_compress_ratio()
{
    cout << "Integers used: " << dl->getNumInts() << "/256" << endl;
    cout << "Initial row width: " << initWidth << endl;
    cout << "Balance row width: " << balanceWidth << endl;
    cout << "Compression ratio: " << fixed << setprecision(2) << (initWidth - balanceWidth) / initWidth * 100 << "%" << endl;
    cout << "Maximum row width: " << minMaxWidth << endl;
    cout << "Compression ratio: " << fixed << setprecision(2) << (initWidth - minMaxWidth) / initWidth * 100 << "%" << endl;
}

void SimulatedAnnealing::show_history()
{
    cout << "===========================================================================================" << endl;
    cout << "========================================= History =========================================" << endl;
    cout << "===========================================================================================" << endl;
    cout << "Iter\tT\tTime\tStall\tNewMaxWidth\tMinMaxWidth\tmodRate\tacceptProb\tCompress" << endl;
    for (auto h : history)
    {
        cout << h->iter << "\t" << h->T << "\t" << h->time << "\t";
        cout << h->stall_count << "\t" << h->newMaxWidth << "\t\t" << h->minMaxWidth << "\t\t";
        cout << h->modRate << "\t";
        cout << fixed << setprecision(2) << h->accept_prob << "%\t\t";
        cout << fixed << setprecision(2) << h->compress_ratio << "%" << endl;
    }
    cout << "===========================================================================================" << endl;
}

void SimulatedAnnealing::show_plot_info()
{
   cout << "Iter\tMaxLen\tMinLen\tMaxLenB\tMinLenB\tCompress(%)\tBestCompress(%)" << endl;
    for (auto h : history)
    {
        cout << h->iter << "\t" << h->maxLen << "\t" << h->minLen << "\t";
        cout << h->maxLenOfBest << "\t" << h->minLenOfBest << "\t";
        cout << fixed << setprecision(2) << h->compress_ratio << "\t";
        cout << fixed << setprecision(2) << (initWidth - h->minMaxWidth) / initWidth * 100 << endl;
    }
}