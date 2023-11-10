#include "DataLoader.h"
#include "TreeArray.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    // if (argc != 2)
    // {
    //     cout << "Usage: ./main <input_data.txt>" << endl;
    //     return EXIT_FAILURE;
    // }
    // string filePath = argv[1];
    // DataLoader *DL = new DataLoader(filePath);
    // cout << *DL << endl;

    TreeArray::testModify(100000);

    return EXIT_SUCCESS;
}