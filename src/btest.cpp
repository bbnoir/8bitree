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
        cout << "Usage: ./bin/btest <txtFilePath>" << endl;
        return EXIT_FAILURE;
    }

    // load data of txt
    string txtFileName = argv[1];
    // cout << "load txt data: " << txtFileName << endl;
    // auto start2 = chrono::high_resolution_clock::now();
    // DataLoader *dl2 = new DataLoader(txtFileName);
    // auto stop2 = chrono::high_resolution_clock::now();
    // auto duration2 = chrono::duration_cast<chrono::milliseconds>(stop2 - start2);
    // cout << "data loaded in " << duration2.count() << " ms" << endl;

    // // load data of bin
    string binFileName = txtFileName.substr(0, txtFileName.find_last_of('.')) + ".bin";
    cout << "load bin data: " << binFileName << endl;
    auto start = chrono::high_resolution_clock::now();
    DataLoader *dl = new DataLoader(1, binFileName);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "data loaded in " << duration.count() << " ms" << endl;

    // // compare 2 data loaders
    // cout << "Comparing data loaders..." << endl;
    // if (dl->getNumLines() != dl2->getNumLines())
    // {
    //     cout << "=> Error: numLines not match" << endl;
    //     return EXIT_FAILURE;
    // }
    // if (dl->getIntPerLine() != dl2->getIntPerLine())
    // {
    //     cout << "=> Error: intPerLine not match" << endl;
    //     return EXIT_FAILURE;
    // }
    // if (dl->getNumInts() != dl2->getNumInts())
    // {
    //     cout << "=> Error: numInts not match" << endl;
    //     cout << "txt: " << dl->getNumInts() << endl;
    //     cout << "bin: " << dl2->getNumInts() << endl;
    //     return EXIT_FAILURE;
    // }
    // vector<int> freqMap = dl->getFreqMap();
    // vector<int> freqMap2 = dl2->getFreqMap();
    // for (int i = 0; i < 256; i++)
    // {
    //     if (freqMap[i] != freqMap2[i])
    //     {
    //         cout << "=> Error: freqMap not match" << endl;
    //         cout << "i: " << i << " txt: " << freqMap[i] << " bin: " << freqMap2[i] << endl;
    //         return EXIT_FAILURE;
    //     }
    // }
    // for (int i = 0; i < dl->getNumLines(); i++)
    // {
    //     for (int j = 0; j < dl->getIntPerLine(); j++)
    //     {
    //         if (dl->getDataAry()[i][j] != dl2->getDataAry()[i][j])
    //         {
    //             cout << "=> Error: dataAry not match" << endl;
    //             return EXIT_FAILURE;
    //         }
    //     }
    // }
    // cout << "=> Data loaders match" << endl;

    return EXIT_SUCCESS;
}