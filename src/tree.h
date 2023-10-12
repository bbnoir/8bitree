#include <iostream>
#include <vector>

using namespace std;

class Tree
{
private:
    vector<int> array;
    int dim;
    // void modify();
    // bool checkKraft();
public:
    Tree(int numLeaf);
    friend ostream& operator<<(ostream& os, const Tree& tree);
};
