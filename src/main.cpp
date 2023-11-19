#include "DataLoader.h"
#include "TreeArray.h"
#include "SimulatedAnnealing.h"
#include <iostream>

#include "Decoder.h"
#include "Verification.h"

using namespace std;

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <input data>" << endl;
        return EXIT_FAILURE;
    }

    Config config = {
        .filePath = argv[1],
        .maxIter = 100,
        .T = 100,
        .Rt = 0.999,
        .modRate = 1000};

    cout << "Input file: " << argv[1] << endl;
    SimulatedAnnealing *sa = new SimulatedAnnealing(&config);
    sa->run();
    sa->show();

    // verify if needed
    if (argc == 3 && string(argv[2]) == "verify")
    {
        string dataFileName = argv[1];
        string encodedFileName = dataFileName.substr(0, dataFileName.find_last_of('.')) + "_encoded.txt";
        string decodedFileName = dataFileName.substr(0, dataFileName.find_last_of('.')) + "_decoded.txt";

        bool clean = true;
        if (argc == 3 && string(argv[2]) == "noclean")
            clean = false;

        cout << "\033[1m";
        cout << "Start verification with data file: " << dataFileName << endl;
        cout << "\033[0m";

        // encode data
        sa->encoder->encode(encodedFileName);

        // decode data
        Decoder decoder(encodedFileName, decodedFileName);
        decoder.decode(sa->dl->getNumLines());

        // verify
        bool success = verify(sa->dl, decodedFileName);

        // show max line width
        if (success)
            cout << "Max width: " << decoder.max_line_width << endl;

        // clean up
        if (clean)
        {
            remove(encodedFileName.c_str());
            remove(decodedFileName.c_str());
        }
    }

    return EXIT_SUCCESS;
}