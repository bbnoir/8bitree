#include <iostream>
#include <vector>
#include "tree.h"
#include "dl.h"
#include "sa.h"
#include <cmath>

using namespace std;

void SimulatedAnnealing()
{
    string filePath = "../data/ALBERT/ALBERT_word_emb.txt";
    int numLeaf = 256;
    DataLoader *dl = new DataLoader(filePath);
    Tree *tree = new Tree(numLeaf);
    tree->getHuffman(dl);

    const int maxIter = 10;
    int iter = 0;
    double T = 1000;
    double Rt = 0.99;

    int curMinWidth = tree->getMinWidth(dl);
    Tree *newTree = NULL;
    int newMinWidth = 0;
    while (iter++ < maxIter)
    {
        cout << "iter: " << iter << " T: " << T << endl;
        cout << "Current tree: " << *tree << endl;
        cout << "Current minWidth: " << curMinWidth << endl;
        newTree = tree->modify();
        newMinWidth = newTree->getMinWidth(dl);
        cout << "New tree: " << *newTree << endl;
        cout << "New minWidth: " << newMinWidth << endl;
        if (newMinWidth < curMinWidth || rand() % 10000 < exp((curMinWidth - newMinWidth) / T) * 10000)
        {
            cout << "Accept new tree" << endl;
            delete tree;
            tree = newTree;
            curMinWidth = newMinWidth;
        }
        else
        {
            cout << "Preserve current tree" << endl;
            delete newTree;
        }
        T *= Rt;
    }
}

// struct Config
// {
//     string filePath;
//     int numLeaf;
//     int maxIter;
//     int T;
//     int Rt;
//     Config(string filePath, int numLeaf, int maxIter, int T, int Rt) : filePath(filePath), numLeaf(numLeaf), maxIter(maxIter), T(T), Rt(Rt) {}
// };

int main(int argc, char *argv[])
{
    string filePath = "../data/ALBERT/ALBERT_word_emb.txt";
    DataLoader *dl = new DataLoader(filePath);
    cout << *dl << endl;
    // int numLeaf = 256;

    // Tree::testModify(10000);

    // SimulatedAnnealing();

    return 0;
}