#include <iostream>
#include "Encoder.h"
#include "Decoder.h"
#include "DataLoader.h"
#include "Constants.h"
#include "Utils.h"
#include <random>
#include <iostream>
#include <fstream>

using namespace std;

bool verify(string dataFileName, string decodedFileName, int numLines)
{
    ifstream dataFile(dataFileName);
    ifstream decodedFile(decodedFileName);
    string dataLine, decodedLine;
    int curline = 1;
    while (getline(dataFile, dataLine) && getline(decodedFile, decodedLine))
    {
        progressBar(curline++, numLines);
        if (dataLine != decodedLine)
        {
            cout << endl;
            cout << "\033[31m"
                 << "     === Verification failed at line " << curline << " ==="
                 << "\033[0m" << endl;
            cout << "Data line: " << dataLine << endl;
            cout << "Decoded line: " << decodedLine << endl;
            cout << endl;
            return false;
        }
    }
    if (decodedFile.peek() != EOF)
    {
        cout << endl;
        cout << "\033[31m"
             << "     === Verification failed: Decoded file has more lines than data file ==="
             << "\033[0m" << endl;
        cout << endl;
        return false;
    }
    if (dataFile.peek() != EOF)
    {
        cout << endl;
        cout << "\033[31m"
             << "     === Verification failed: Data file has more lines than decoded file ==="
             << "\033[0m" << endl;
        cout << endl;
        return false;
    }
    cout << endl;
    cout << "\033[32m"
         << "     === Verification passed ==="
         << "\033[0m" << endl;
    cout << endl;
    return true;
}

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

    cout << "\033[1m";
    cout << "Start verification with data file: " << dataFileName << endl;
    cout << "\033[0m";

    // load data
    cout << "Loading data..." << endl;
    DataLoader *dl = new DataLoader(dataFileName);

    // setup tree array based on data
    cout << "Generating tree array..." << endl;
    TreeArray *tree = TreeArray::genHuffmanArray(dl);
    srand(time(NULL));
    tree->modify(rand() % 30000);
    cout << "Tree array: " << *tree << endl;

    // encode data based on tree array
    cout << "Encoding data..." << endl;
    Encoder encoder(dl, tree);
    encoder.encode(dataFileName, encodedFileName, dl->getNumLines());

    // decode data
    cout << "Decoding data..." << endl;
    Decoder decoder(encodedFileName, decodedFileName);
    decoder.decode(dl->getNumLines());

    // verify
    cout << "Verifying..." << endl;
    bool success = verify(dataFileName, decodedFileName, dl->getNumLines());

    // clean up
    if (clean)
    {
        remove(encodedFileName.c_str());
        remove(decodedFileName.c_str());
    }

    return EXIT_SUCCESS;
}