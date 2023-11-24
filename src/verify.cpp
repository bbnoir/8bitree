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
    cout << "Loading data..." << endl;
    DataLoader *dl = new DataLoader(dataFileName);

    // setup tree array based on data
    // cout << "Generating tree array..." << endl;
    // TreeArray *tree = TreeArray::genHuffmanArray(dl);
    // cout << "numInts: " << dl->getNumInts() << endl;
    TreeArray *tree = new TreeArray(dl->getNumInts());
    srand(time(NULL));
    // tree->modify(rand() % 30000);
    // cout << "Tree array: " << *tree << endl;

    // encode data based on tree array
    cout << "Encoding data..." << endl;
    auto start = chrono::high_resolution_clock::now();
    Encoder *encoder = new Encoder(dl, tree);
    encoder->encode(encodedFileName);
    delete encoder;
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Data encoded in " << duration.count() << " ms" << endl;

    // decode data
    cout << "Decoding data..." << endl;
    auto start2 = chrono::high_resolution_clock::now();
    Decoder *decoder = new Decoder(encodedFileName, decodedFileName);
    decoder->decode(dl->getNumLines());
    delete decoder;
    auto stop2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(stop2 - start2);
    cout << "Data decoded in " << duration2.count() << " ms" << endl;

    // verify
    cout << "Verifying..." << endl;
    auto start3 = chrono::high_resolution_clock::now();
    bool success = verify(dl, decodedFileName);
    auto stop3 = chrono::high_resolution_clock::now();
    auto duration3 = chrono::duration_cast<chrono::milliseconds>(stop3 - start3);
    cout << "Data verified in " << duration3.count() << " ms" << endl;

    // clean up
    if (clean)
    {
        remove(encodedFileName.c_str());
        remove(decodedFileName.c_str());
    }

    return EXIT_SUCCESS;
}