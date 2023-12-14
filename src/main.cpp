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

    Config config =
        {
            .filePath = argv[1],
            .maxIter = 200,
            .stallIter = 100,
            .T = 100,
            .Rt = 0.95,
            .modRate = 10000,
            .maxTime = 5 * 60,
            .deterministic = false,
            .initMode = INIT_MODE::BALANCED};

    cout << "Data: " << config.filePath << endl;
    SimulatedAnnealing *sa = new SimulatedAnnealing(&config);
    sa->run();
    // sa->show();
    sa->show_history();
    sa->show_compress_ratio();

    // verify
    if (argc >= 3 && string(argv[2]) == "verify")
    {
        string dataFileName = argv[1];
        string encodedFileName = dataFileName.substr(0, dataFileName.find_last_of('.')) + "_encoded.bin";
        string decodedFileName = dataFileName.substr(0, dataFileName.find_last_of('.')) + "_decoded.bin";

        // encode data
        sa->encoder->encode(encodedFileName);

        // decode data
        Decoder *decoder = new Decoder(encodedFileName, decodedFileName);
        decoder->decode();
        int max_line_width = decoder->max_line_width;
        delete decoder;

        // verify
        // bool success = verify_bin_quiet(sa->dl, decodedFileName);
        bool success = verify_bin(sa->dl, decodedFileName);

        // if (!success)
        //     cout << "FAILED: verification failed" << endl;
        cout << "max line width: " << max_line_width << endl;
        cout << "min max line width: " << sa->minMaxWidth << endl;
        if (max_line_width != sa->minMaxWidth)
            cout << "FAILED: max line width mismatch" << endl;

        // clean up
        if (!(argc == 4 && string(argv[3]) == "noclean"))
        {
            remove(encodedFileName.c_str());
            remove(decodedFileName.c_str());
        }
    }
    cout << endl;

    return EXIT_SUCCESS;
}