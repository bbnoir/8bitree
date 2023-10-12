#include <iostream>
#include <vector>
#include "tree.h"
#include "dl.h"
#include "sa.h"

using namespace std;

int main(int argc, char *argv[])
{
    int numLeaf = 256;
    Tree *tree = new Tree(numLeaf);
    cout << *tree << endl;

    string filePath = "/home/U114ysyang/project/8bitree/data/ALBERT/ALBERT_word_emb.txt";
    DataLoader *DL = new DataLoader(filePath);

    return 0;
}
