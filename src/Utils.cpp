#include "Utils.h"

using namespace std;

void progressBar(int progress, int total)
{
    int barWidth = 50;
    float progressRatio = (float)progress / total;
    int pos = barWidth * progressRatio;

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
    cout.flush();
    if (progress == total)
        cout << endl;
}
