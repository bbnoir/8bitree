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
        .maxIter = 10,
        .T = 1000,
        .Rt = 0.99,
    };

    SimulatedAnnealing *sa = new SimulatedAnnealing(&config);
    sa->run();

    return EXIT_SUCCESS;
}