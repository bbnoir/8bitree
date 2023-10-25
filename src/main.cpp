#include <iostream>
#include <vector>
#include "tree.h"
#include "dl.h"
#include "sa.h"

using namespace std;

int main(int argc, char *argv[])
{
    string filePath = "../data/ALBERT/ALBERT_word_emb.txt";
    DataLoader *DL = new DataLoader(filePath);

    int numLeaf = 256;

    Tree *tree = new Tree(numLeaf);
    // tree->testModify(100000);

    return 0;
}
