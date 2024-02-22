#include "SimulatedAnnealing.h"
#include <iostream>
#include "Encoder.h"
#include "Decoder.h"
#include "Verification.h"

using std::cout;

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
            .maxIter = 5000,
            .stallIter = 100,
            .T = 100,
            .Rt = 0.95,
            .initModRate = 60,
            .decayModRate = 0.95,
            .maxTime = 60 * 60,
            .deterministic = false,
            .quiet = true,
            .initMode = INIT_MODE::BALANCED};

    SimulatedAnnealing *sa = new SimulatedAnnealing(&config);
    cout << "Data: " << config.filePath << endl;
    sa->run();
    // uncomment to show history
    // sa->show();
    // sa->show_history();
    cout << endl;
    sa->show_plot_info();
    cout << endl;
    sa->show_compress_ratio();
    cout << endl;
    cout << "Best tree: " << endl;
    cout << *sa->bestTree << endl;
    cout << "Best tree max length: " << sa->bestTree->getMaxLen() << endl;
    cout << "Best tree min length: " << sa->bestTree->getMinLen() << endl;

    // verify
    if (argc >= 3 && string(argv[2]) == "verify")
    {
        string dataFileName = argv[1];
        string encodedFileName = dataFileName.substr(0, dataFileName.find_last_of('.')) + "_encoded.bin";
        string decodedFileName = dataFileName.substr(0, dataFileName.find_last_of('.')) + "_decoded.bin";

        // encode data
        Encoder* best_encoder = new Encoder(sa->dl, sa->bestTree);
        best_encoder->encode(encodedFileName);

        // decode data
        Decoder *decoder = new Decoder(encodedFileName, decodedFileName);
        decoder->decode();
        int max_line_width = decoder->max_line_width;
        delete decoder;

        // verify
        bool success = verify_bin(sa->dl, decodedFileName);

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
