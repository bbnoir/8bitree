#include "SimulatedAnnealing.h"
#include "DataLoader.h"
#include "TreeArray.h"
#include "Constants.h"
#include "Encoder.h"
#include "Utils.h"
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <chrono>

using namespace std;

void SimulatedAnnealing::initCodeLength(INIT_MODE mode)
{
    switch (mode)
    {
    case INIT_MODE::BALANCED:
        TreeArray *tmpTree = new TreeArray(dl->getNumInts());
        Encoder *tmpEncoder = new Encoder(dl, tmpTree);
        minMaxWidth = tmpEncoder->getBestWidth();
        cl = new CodeLength(tmpEncoder->getCodeLength());
        cl->showInfo();
        break;
    }
}

SimulatedAnnealing::SimulatedAnnealing(Config *config) : config(config), maxIter(config->maxIter), T(config->T), Rt(config->Rt), minMaxWidth(0), modRate(config->modRate)
{
    config = config;
    dl = new DataLoader(config->filePath);
    initCodeLength(config->initMode);
    encoder = new Encoder2(dl, cl->getCodeLengthRef());
    initMaxWidth = minMaxWidth;
    bestCodeLength = cl->getCodeLength();
    nonCompressedWidth = dl->getElementPerLine() * 8;
}

bool accept(int newMaxWidth, int MaxWidth, double nonCompressedWidth, double T)
{
    if (newMaxWidth < MaxWidth)
        return true;
    else if (exp((MaxWidth - newMaxWidth) / nonCompressedWidth * 10000 / T) > (double)rand() / RAND_MAX)
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
    for (int iter = 0; iter < maxIter; iter++)
    {
        History *h = new History();
        h->iter = iter;
        h->T = T;
        h->stall_count = stall_count;
        h->time = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start).count();
        h->maxWidth = MaxWidth;
        cl->modify(rand() % modRate + 1);
        cout << endl;
        cout << "After modify " << rand() % modRate + 1 << " times: " << endl;
        cl->showInfo();
        cout << endl;
        newMaxWidth = encoder->getMaxWidth();
        h->newMaxWidth = newMaxWidth;
        h->compress_ratio = (nonCompressedWidth - newMaxWidth) / nonCompressedWidth * 100;

        stall_count++;
        if (newMaxWidth < minMaxWidth) // early stop
        {
            minMaxWidth = newMaxWidth;
            bestCodeLength = cl->getCodeLength();
        }
        h->minMaxWidth = minMaxWidth;
        h->accept_prob = exp((MaxWidth - newMaxWidth) / nonCompressedWidth * 10000 / T) * 100;
        if (h->accept_prob > 100)
            h->accept_prob = 100;

        if (accept(newMaxWidth, MaxWidth, nonCompressedWidth, T))
        {
            MaxWidth = newMaxWidth;
            stall_count = 0;
        }
        cl->recover();
        // else
        // {
        //     cl->recover();
        // }

        cout << "After accept: " << endl;
        cl->showInfo();
        cout << endl;

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

        // cout.flush();
        // cout << "\r"
        //      << "Iter: " << iter << " / " << maxIter << " -> Compress ratio: " << fixed << setprecision(2) << h->compress_ratio << "%";
        cout << "Iter: " << iter << " / " << maxIter << " -> Compress ratio: " << fixed << setprecision(2) << h->compress_ratio << "%" << endl;
    }
    delete encoder;
    encoder = new Encoder2(dl, bestCodeLength);
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
    cout << "Result Code Length: " << endl;
    for (int i = 0; i < SYM_NUM; i++)
    {
        cout << bestCodeLength[i] << " ";
        if (i % 64 == 63)
            cout << endl;
    }
    cout << "=======================================================================================" << endl;
    cout << "Integers used: " << dl->getNumInts() << "/256" << endl;
    cout << "Non-compressed row width: " << nonCompressedWidth << endl;
    cout << "Initial row width: " << initMaxWidth << endl;
    cout << "Compression ratio: " << fixed << setprecision(2) << (nonCompressedWidth - initMaxWidth) / nonCompressedWidth * 100 << "%" << endl;
    cout << "Maximum row width: " << minMaxWidth << endl;
    cout << "Compression ratio: " << fixed << setprecision(2) << (nonCompressedWidth - minMaxWidth) / nonCompressedWidth * 100 << "%" << endl;
    cout << "=======================================================================================" << endl;
}

void SimulatedAnnealing::show_compress_ratio()
{
    cout << "Integers used: " << dl->getNumInts() << "/256" << endl;
    cout << "Non-compressed row width: " << nonCompressedWidth << endl;
    cout << "Initial row width: " << initMaxWidth << endl;
    cout << "Compression ratio: " << fixed << setprecision(2) << (nonCompressedWidth - initMaxWidth) / nonCompressedWidth * 100 << "%" << endl;
    cout << "Maximum row width: " << minMaxWidth << endl;
    cout << "Compression ratio: " << fixed << setprecision(2) << (nonCompressedWidth - minMaxWidth) / nonCompressedWidth * 100 << "%" << endl;
}

void SimulatedAnnealing::show_history()
{
    cout << "=======================================================================================" << endl;
    cout << "======================================= History =======================================" << endl;
    cout << "=======================================================================================" << endl;
    cout << "Iter\tT\tTime\tStall\tMaxWidth\tNewMaxWidth\tMinMaxWidth\tacceptProb\tCompress" << endl;
    for (auto h : history)
    {
        cout << h->iter << "\t" << h->T << "\t" << h->time << "\t";
        cout << h->stall_count << "\t" << h->maxWidth << "\t\t" << h->newMaxWidth << "\t\t" << h->minMaxWidth << "\t\t";
        cout << fixed << setprecision(2) << h->accept_prob << "%\t\t";
        cout << fixed << setprecision(2) << h->compress_ratio << "%" << endl;
    }
    cout << "=======================================================================================" << endl;
}