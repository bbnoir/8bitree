#include <iostream>
#include "DataLoader.h"
#include "Constants.h"
#include "Utils.h"
#include <random>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: ./test <dataFileName>" << endl;
        return EXIT_FAILURE;
    }
    string dataFileName = argv[1];
    string encodedFileName = dataFileName.substr(0, dataFileName.find_last_of('.')) + "_encoded.txt";
    string decodedFileName = dataFileName.substr(0, dataFileName.find_last_of('.')) + "_decoded.txt";

    bool clean = true;
    if (argc == 3 && string(argv[2]) == "noclean")
        clean = false;

    // calculate data load time
    // auto start = chrono::high_resolution_clock::now();

    // load data by method 0
    // cout << "Loading binary data by method 0..." << endl;
    // DataLoader *dl = new DataLoader(0, dataFileName);

    // auto stop = chrono::high_resolution_clock::now();
    // auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    // cout << "Data loaded in " << duration.count() << " ms" << endl;

    // cout << "numLines: " << dl->getNumLines() << endl;

    auto start2 = chrono::high_resolution_clock::now();
    // load data by method 1
    cout << "Loading binary data by method 1..." << endl;
    DataLoader *dl2 = new DataLoader(1, dataFileName);

    auto stop2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(stop2 - start2);
    cout << "Data loaded in " << duration2.count() << " ms" << endl;

    cout << "numLines: " << dl2->getNumLines() << endl;

    return EXIT_SUCCESS;
}