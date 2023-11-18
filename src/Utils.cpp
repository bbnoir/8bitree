#include "Utils.h"
#include <iomanip>

using namespace std;

void progressBar(int progress, int total)
{
    int barWidth = 50;
    float progressRatio = (float)progress / total;
    int pos = barWidth * progressRatio;
    cout.flush();
    cout << "[";
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            cout << "=";
        else if (i == pos)
            cout << ">";
        else
            cout << " ";
    }
    cout << "] " << int(progressRatio * 100.0) << " %\r";
    if (progress == total)
        cout << endl;
}

void progressBar(string title, int progress, int total)
{
    int barWidth = 50;
    float progressRatio = (float)progress / total;
    int pos = barWidth * progressRatio;
    cout.flush();
    cout << setw(15) << title << " ";
    cout << "[";
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            cout << "=";
        else if (i == pos)
            cout << ">";
        else
            cout << " ";
    }
    cout << "] " << int(progressRatio * 100.0) << " %\r";
    if (progress == total)
        cout << endl;
}
