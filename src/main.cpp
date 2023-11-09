#include <iostream>
#include "Encoder.h"
#include "Decoder.h"
#include "DataLoader.h"
#include "Constants.h"
#include <random>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: ./main <input_data.txt>" << endl;
        return EXIT_FAILURE;
    }
    string filePath = argv[1];
    DataLoader *DL = new DataLoader(filePath);
    cout << *DL << endl;

    return EXIT_SUCCESS;
}