#include "SimulatedAnnealing.h"
#include "DataLoader.h"
#include "TreeArray.h"
#include "Constants.h"
#include "Utils.h"
#include <iomanip>
#include <algorithm>
#include <cmath>

using namespace std;

SimulatedAnnealing::SimulatedAnnealing(Config *config) : config(config), maxIter(config->maxIter), T(config->T), Rt(config->Rt), minMaxWidth(0), modRate(config->modRate)
{
    config = config;
    dl = new DataLoader(config->filePath);
    tree = new TreeArray(dl->getNumInts());
    encoder = new Encoder(dl, tree);
    bestTree = new TreeArray(*tree);
    minMaxWidth = encoder->getBestWidth();
    balanceWidth = minMaxWidth;
}

int SimulatedAnnealing::run()
{
    // cout << "Initial line width: " << dl->getIntPerLine() * 8 << endl;
    int MaxWidth = minMaxWidth;
    int newMaxWidth = 0;
    int stall_count = 0;
    for (int iter = 0; iter < maxIter; iter++)
    {
        // progressBar("SA", iter, maxIter - 1);
        // cout << "=======================================================================================" << endl;
        // cout << "iter: " << iter << endl;
        // cout << "T = " << T << endl;
        // cout << "cur tree: " << *tree << endl;
        // cout << "cur width: " << MaxWidth << endl;
        srand(time(NULL));
        tree->modify(rand() % modRate + 1);
        newMaxWidth = encoder->getBestWidth();
        // cout << "new tree: " << *tree << endl;
        // cout << "new width: " << newMaxWidth << endl;
        stall_count++;
        if (newMaxWidth < minMaxWidth)
        {
            minMaxWidth = newMaxWidth;
            delete bestTree;
            bestTree = new TreeArray(*tree);
            stall_count = 0;
        }
        // if (newMaxWidth < MaxWidth || rand() % 10000 < exp((MaxWidth - newMaxWidth) / T) * 10000)
        if (newMaxWidth < MaxWidth)
        {
            // cout << "=> Accept new tree" << endl;
            MaxWidth = newMaxWidth;
        }
        else
        {
            // cout << "=> Preserve cur tree" << endl;
            tree->recover();
        }
        // cout << "Stop SA at iter " << iter << "\r" << flush;
        if (stall_count > config->stallIter)
        {
            // cout << endl;
            cout << "Stop SA at iter " << iter << endl;
            break;
        }
        T *= Rt;
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
    double initWidth = dl->getElementPerLine() * 8;
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
    double initWidth = dl->getElementPerLine() * 8;
    cout << "Integers used: " << dl->getNumInts() << "/256" << endl;
    cout << "Assignment mode: ";
    int bestWay = encoder->getBestWay();
    if (bestWay == FQ)
        cout << "FQ" << endl;
    else if (bestWay == EX)
        cout << "EX" << endl;
    else if (bestWay == OC)
        cout << "OC" << endl;
    cout << "Initial row width: " << initWidth << endl;
    cout << "Balance row width: " << balanceWidth << endl;
    cout << "Compression ratio: " << fixed << setprecision(2) << (initWidth - balanceWidth) / initWidth * 100 << "%" << endl;
    cout << "Maximum row width: " << minMaxWidth << endl;
    cout << "Compression ratio: " << fixed << setprecision(2) << (initWidth - minMaxWidth) / initWidth * 100 << "%" << endl;
}
