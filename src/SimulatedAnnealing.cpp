#include "SimulatedAnnealing.h"
#include "DataLoader.h"
#include "TreeArray.h"
#include "Constants.h"
#include <algorithm>
#include <cmath>

using namespace std;

SimulatedAnnealing::SimulatedAnnealing(Config *config) : config(config), maxIter(config->maxIter), T(config->T), Rt(config->Rt)
{
    config = config;
    dl = new DataLoader(0, config->filePath);
    tree = new TreeArray(dl->getNumInts());
}

vector<int> SimulatedAnnealing::genCodeLength()
{
    vector<int> codeLength(SYM_NUM, 0);
    vector<int> freqMap = dl->getFreqMap();
    // sort the freqMap based on the frequency
    vector<pair<int, int8>> freqMapWithIndex;
    // <frequency, number>
    for (int i = 0; i < SYM_NUM; i++)
        freqMapWithIndex.push_back(make_pair(freqMap[i], (int8)i));
    sort(freqMapWithIndex.begin(), freqMapWithIndex.end(), [](pair<int, int8> a, pair<int, int8> b)
         { return a.first > b.first; });
    // build the table
    unordered_map<int8, int> lengthTable;
    vector<short> treeAry = tree->getCodeArray();
    // [nums of 0 bits code, nums of 1 bits code, ...] sum = 16
    int cl = 0;
    // let higher frequency symbol have shorter code length
    for (int j = 0; j < SYM_NUM; j++)
    {
        while (treeAry[cl] == 0)
            cl++;
        lengthTable[freqMapWithIndex[j].second] = cl;
        treeAry[cl]--;
    }
    for (int i = 0; i < SYM_NUM; i++)
        codeLength[i] = lengthTable[i];
    return codeLength;
}

int SimulatedAnnealing::getMaxWidth()
{
}

int SimulatedAnnealing::run()
{
    int minMaxWidth = getMaxWidth();
    int newMaxWidth = 0;
    for (int iter = 0; iter < maxIter; iter++)
    {
        cout << "iter: " << iter << " T: " << T << endl;
        cout << "Current tree: " << *tree << endl;
        cout << "Current minWidth: " << minMaxWidth << endl;
        tree->modify(rand() % 6 + 1);
        newMaxWidth = getMaxWidth();
        cout << "New tree: " << *tree << endl;
        cout << "New minWidth: " << newMaxWidth << endl;
        if (newMaxWidth < minMaxWidth || rand() % 10000 < exp((minMaxWidth - newMaxWidth) / T) * 10000)
        {
            cout << "Accept new tree" << endl;
            tree->recover();
            minMaxWidth = newMaxWidth;
        }
        else
        {
            cout << "Preserve current tree" << endl;
        }
        T *= Rt;
    }
    return minMaxWidth;
}

void SimulatedAnnealing::show()
{
    cout << "Simulated Annealing" << endl;
    cout << "==================" << endl;
    cout << "Input file: " << config->filePath << endl;
    cout << "Maximum number of iterations: " << config->maxIter << endl;
    cout << "Initial temperature: " << config->T << endl;
    cout << "Temperature reduction rate: " << config->Rt << endl;
    cout << "==================" << endl;
    cout << "Result Tree Array:" << endl;
    cout << *tree << endl;
    cout << "==================" << endl;
    cout << "Minimum maximum width: " << getMaxWidth() << endl;
    cout << "==================" << endl;
}