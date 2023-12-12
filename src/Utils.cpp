#include "Utils.h"
#include "Constants.h"
#include <iomanip>
#include <algorithm>

using namespace std;

void progressBar(int progress, int total)
{
    int barWidth = 50;
    float progressRatio = (float)progress / total;
    int pos = barWidth * progressRatio;
    cout.flush();
    cout << "[";
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            cout << "=";
        else if (i == pos)
            cout << ">";
        else
            cout << " ";
    }
    cout << "] " << int(progressRatio * 100.0) << " %\r";
    if (progress == total)
        cout << endl;
}

void progressBar(string title, int progress, int total)
{
    int barWidth = 50;
    float progressRatio = (float)progress / total;
    int pos = barWidth * progressRatio;
    cout.flush();
    cout << setw(15) << title << " ";
    cout << "[";
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            cout << "=";
        else if (i == pos)
            cout << ">";
        else
            cout << " ";
    }
    cout << "] " << int(progressRatio * 100.0) << " %\r";
    if (progress == total)
        cout << endl;
}

bool cmp(const pair<int, int> &a, const pair<int, int> &b)
{
    if (a.second == b.second)
        return a.first < b.first;
    return a.second < b.second;
}

// Function to sort the map according to value in a (key-value) pairs
vector<pair<int, int>> sort_cl(vector<int> M)
{
    vector<pair<int, int>> A;
    for (int i = 0; i < M.size(); i++)
        A.push_back(make_pair(i + SYM_MIN, M[i]));
    sort(A.begin(), A.end(), cmp);
    return A;
}

bool checkTreeValid(vector<int> codeLengths)
{
    std::sort(codeLengths.begin(), codeLengths.end(), std::greater<int>());
    int currentLevel = codeLengths.front();
    int numNodesAtLevel = 0;
    for (int cl : codeLengths)
    {
        if (cl == 0)
            break;
        while (cl < currentLevel)
        {
            if (numNodesAtLevel % 2 != 0)
                throw std::invalid_argument("Under-full Huffman code tree");
            numNodesAtLevel /= 2;
            currentLevel--;
        }
        numNodesAtLevel++;
    }
    while (currentLevel > 0)
    {
        if (numNodesAtLevel % 2 != 0)
            throw std::invalid_argument("Under-full Huffman code tree");
        numNodesAtLevel /= 2;
        currentLevel--;
    }
    if (numNodesAtLevel < 1)
        throw std::invalid_argument("Under-full Huffman code tree");
    if (numNodesAtLevel > 1)
        throw std::invalid_argument("Over-full Huffman code tree");
    return true;
}