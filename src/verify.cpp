#include "Verification.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
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
    DataLoader *dl = new DataLoader(dataFileName);

    // setup tree array based on data
    cout << "Generating tree array..." << endl;
    // TreeArray *tree = TreeArray::genHuffmanArray(dl);
    cout << "numInts: " << dl->getNumInts() << endl;
    TreeArray *tree = new TreeArray(dl->getNumInts());
    srand(time(NULL));
    // tree->modify(rand() % 30000);
    cout << "Tree array: " << *tree << endl;

    // encode data based on tree array
    Encoder encoder(dl, tree);
    encoder.encode(encodedFileName);

    // decode data
    Decoder decoder(encodedFileName, decodedFileName);
    decoder.decode(dl->getNumLines());

    // verify
    bool success = verify(dl, decodedFileName);

    // clean up
    if (clean)
    {
        remove(encodedFileName.c_str());
        remove(decodedFileName.c_str());
    }

    return EXIT_SUCCESS;
}