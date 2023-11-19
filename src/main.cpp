#include "DataLoader.h"
#include "TreeArray.h"
#include "SimulatedAnnealing.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <input data>" << endl;
        return EXIT_FAILURE;
    }

    Config config = {
        .filePath = argv[1],
        .maxIter = 100,
        .T = 100,
        .Rt = 0.999,
    };

    cout << "Input file: " << argv[1] << endl;
    SimulatedAnnealing *sa = new SimulatedAnnealing(&config);
    sa->run();
    sa->show();

    return EXIT_SUCCESS;
}