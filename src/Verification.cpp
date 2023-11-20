#include "Verification.h"

using namespace std;

bool verify(DataLoader *dl, string decodedFileName)
{
    ifstream decodedFile(decodedFileName);
    string decodedLine;
    int curline = 0;
    int intPerLine = dl->getIntPerLine();
    int numLines = dl->getNumLines();
    bool diff = false;
    while (getline(decodedFile, decodedLine))
    {
        progressBar("Verify", curline++, numLines - 1);
        stringstream ss(decodedLine);
        int n;
        for (int i = 0; i < intPerLine; i++)
        {
            ss >> n;
            if (n != dl->getDataAry()[curline - 1][i])
            {
                diff = true;
                break;
            }
        }
        if (diff)
        {
            cout << endl;
            cout << "\033[31m"
                 << "     === Verification failed at line " << curline << " ==="
                 << "\033[0m" << endl;
            cout << "Data line: ";
            for (int i = 0; i < intPerLine; i++)
                cout << (int)dl->getDataAry()[curline - 1][i] << " ";
            cout << endl;
            cout << "Decoded line: " << decodedLine << endl;
            cout << endl;
            return false;
        }
    }
    if (decodedFile.peek() != EOF)
    {
        cout << endl;
        cout << "\033[31m"
             << "     === Verification failed: Decoded file has more lines than data file ==="
             << "\033[0m" << endl;
        cout << endl;
        return false;
    }
    if (curline != numLines)
    {
        cout << endl;
        cout << "\033[31m"
             << "     === Verification failed: Data file has more lines than decoded file ==="
             << "\033[0m" << endl;
        cout << endl;
        return false;
    }
    cout << "\033[32m"
         << "     === Verification passed ==="
         << "\033[0m" << endl;
    return true;
}

bool verify_quiet(DataLoader *dl, string decodedFileName)
{
    ifstream decodedFile(decodedFileName);
    string decodedLine;
    int curline = 0;
    int intPerLine = dl->getIntPerLine();
    int numLines = dl->getNumLines();
    while (getline(decodedFile, decodedLine))
    {
        curline++;
        stringstream ss(decodedLine);
        int n;
        for (int i = 0; i < intPerLine; i++)
        {
            ss >> n;
            if (n != dl->getDataAry()[curline - 1][i])
                return false;
        }
    }
    if (decodedFile.peek() != EOF)
        return false;
    if (curline != numLines)
        return false;
    return true;
}