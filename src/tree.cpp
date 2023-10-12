#include "tree.h"

Tree::Tree(int numLeaf): array(16, 0)
{
    dim = 16;
    array[7] = numLeaf;
}

ostream& operator<<(ostream& os, const Tree& tree)
{
    for (int i = 0; i < tree.dim; i++)
    {
        os << tree.array[i] << " ";
    }
    return os;
}