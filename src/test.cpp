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
        cout << "Modify " << i << " times" << endl;
        cl->modify(i);
        cout << *cl << endl;
    }
    return EXIT_SUCCESS;
}