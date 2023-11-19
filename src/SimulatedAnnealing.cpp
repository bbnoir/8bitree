#include "SimulatedAnnealing.h"
#include "DataLoader.h"
#include "TreeArray.h"
#include "Constants.h"
#include <algorithm>
#include <cmath>

using namespace std;

SimulatedAnnealing::SimulatedAnnealing(Config *config) : config(config), maxIter(config->maxIter), T(config->T), Rt(config->Rt), minMaxWidth(0)
{
    config = config;
    dl = new DataLoader(config->filePath);
    tree = new TreeArray(dl->getNumInts());
    encoder = new Encoder(dl, tree);
    bestTree = new TreeArray(*tree);
    minMaxWidth = encoder->getMaxWidth();
}

int SimulatedAnnealing::run()
{
    cout << "Initial line width: " << dl->getIntPerLine() * 8 << endl;
    int MaxWidth = minMaxWidth;
    int newMaxWidth = 0;
    for (int iter = 0; iter < maxIter; iter++)
    {
        cout << "=======================================================================================" << endl;
        cout << "iter: " << iter << endl;
        cout << "T = " << T << endl;
        cout << "cur tree: " << *tree << endl;
        cout << "cur width: " << MaxWidth << endl;
        srand(time(NULL));
        tree->modify(rand() % 1000 + 1);
        newMaxWidth = encoder->getMaxWidth();
        cout << "new tree: " << *tree << endl;
        cout << "new width: " << newMaxWidth << endl;
        if (newMaxWidth < minMaxWidth)
        {
            minMaxWidth = newMaxWidth;
            delete bestTree;
            bestTree = new TreeArray(*tree);
        }
        // if (newMaxWidth < MaxWidth || rand() % 10000 < exp((MaxWidth - newMaxWidth) / T) * 10000)
        if (newMaxWidth < MaxWidth)
        {
            cout << "=> Accept new tree" << endl;
            MaxWidth = newMaxWidth;
        }
        else
        {
            cout << "=> Preserve cur tree" << endl;
            tree->recover();
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
    double initWidth = dl->getIntPerLine() * 8;
    cout << "Initial row width: " << initWidth << endl;
    cout << "Maximum row width: " << minMaxWidth << endl;
    cout << "Compression ratio: " << (initWidth - minMaxWidth) / initWidth * 100 << "%" << endl;
    cout << "=======================================================================================" << endl;
}