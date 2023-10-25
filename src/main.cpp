#include <iostream>
#include <vector>
#include "tree.h"
#include "dl.h"
#include "sa.h"

using namespace std;

// void SimulatedAnnealing()
// {
//     string filePath = "../data/ALBERT/ALBERT_word_emb.txt";
//     int numLeaf = 256;
//     DataLoader *dl = new DataLoader(filePath);
//     Tree *tree = new Tree(numLeaf);

//     const int maxIter = 100000;
//     int iter = 0;
//     int T = 1000;
//     int Rt = 0.99;

//     int energy = tree->getEnergy(dl);
//     while (iter++ < maxIter)
//     {
//         Tree *newTree = tree->getNeighbor();
//         int newEnergy = newTree->getEnergy(dl);
//         if (newEnergy < energy || rand() % 10000 < exp((energy - newEnergy) / T) * 10000)
//         {
//             delete tree;
//             tree = newTree;
//             energy = newEnergy;
//         }
//         else
//         {
//             delete newTree;
//         }
//         T *= Rt;
//     }
// }

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
    int numLeaf = 256;

    Tree::testModify(20);

    // SimulatedAnnealing();

    return 0;
}