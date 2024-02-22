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
    string encodedFileName = dataFileName.substr(0, dataFileName.find_last_of('.')) + "_encoded.bin";
    string decodedFileName = dataFileName.substr(0, dataFileName.find_last_of('.')) + "_decoded.bin";

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
    TreeArray *tree = new TreeArray(dl->getNumInts());

    // encode data based on tree array
    auto start = chrono::high_resolution_clock::now();
    Encoder *encoder = new Encoder(dl, tree);
    encoder->encode(encodedFileName);
    delete encoder;
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Data encoded in " << duration.count() << " ms" << endl;

    // decode data
    auto start2 = chrono::high_resolution_clock::now();
    Decoder *decoder = new Decoder(encodedFileName, decodedFileName);
    // decoder->decode();
    decoder->decode();
    delete decoder;
    auto stop2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(stop2 - start2);
    cout << "Data decoded in " << duration2.count() << " ms" << endl;

    // verify
    auto start3 = chrono::high_resolution_clock::now();
    bool success = verify_bin_quiet(dl, decodedFileName);
    auto stop3 = chrono::high_resolution_clock::now();
    auto duration3 = chrono::duration_cast<chrono::milliseconds>(stop3 - start3);
    cout << "Data verified in " << duration3.count() << " ms" << endl;
    cout << endl;

    // clean up
    if (clean)
    {
        remove(encodedFileName.c_str());
        remove(decodedFileName.c_str());
    }

    return EXIT_SUCCESS;
}
