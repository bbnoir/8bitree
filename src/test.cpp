#include <iostream>
#include "DataLoader.h"
#include "CodeLength.h"
#include "Constants.h"
#include "Utils.h"
#include <random>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

int main()
{
    CodeLength *cl = new CodeLength();
    cout << "Initial code length: " << endl;
    cout << *cl << endl;
    int test_num = 10000;
    for (int i = 1; i <= test_num; i++)
    {
        int times = rand() % 10000 + 1;
        cl->modify(times);
        cout << "After modify " << times << " times: " << endl;
        cl->showInfo();
        cout << endl;
        cl->recover();
        cout << "After recover: " << endl;
        cl->showInfo();
        cout << endl;
    }
    return EXIT_SUCCESS;
}
