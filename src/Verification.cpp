#include "Verification.h"
#include "sys/mman.h"
#include "sys/stat.h"
#include "fcntl.h"

using namespace std;

bool verify(DataLoader *dl, string decodedFileName)
{
    ifstream decodedFile(decodedFileName);
    string decodedLine;
    int curline = 0;
    int intPerLine = dl->getElementPerLine();
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
    int intPerLine = dl->getElementPerLine();
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
    return true;
}

bool verify_bin(DataLoader *dl, string decodedFileName)
{
    int fd = open(decodedFileName.c_str(), O_RDONLY);
    if (fd == -1)
    {
        cout << "Error: can't open file" << endl;
        exit(1);
    }
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        cout << "Error: can't get file size" << endl;
        exit(1);
    }
    size_t length = sb.st_size;
    int8 *addr = (int8 *)mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED)
    {
        cout << "Error: mmap failed" << endl;
        exit(1);
    }
    int intPerLine = *(int *)addr;
    int numLines = *((int *)addr + 1);
    if (intPerLine != dl->getElementPerLine())
    {
        cout << "=== Verification failed: Decoded file has different intPerLine ===" << endl;
        return false;
    }
    if (numLines != dl->getNumLines())
    {
        cout << "=== Verification failed: Decoded file has different numLines ===" << endl;
        return false;
    }
    int8 *cur = (int8 *)(addr + 2 * sizeof(int));
    bool diff = false;
    int error_element = 0;
    for (int i = 0; i < numLines; i++)
    {
        for (int j = 0; j < intPerLine; j++)
        {
            if (*(cur++) != dl->getDataAry()[i][j])
            {
                diff = true;
                if (error_element == 0)
                    error_element = j;
            }
        }
        if (diff)
        {
            cout << "=== Verification failed at line " << i + 1 << " ===" << endl;
            cout << "Data / Decoded: " << endl;
            if (error_element > 2)
                error_element -= 2;
            for (int j = error_element; j < intPerLine; j++)
                cout << (int)dl->getDataAry()[i][j] << " / " << (int)*(cur - intPerLine + j) << endl;
            return false;
        }
    }
    cout << "=== Verification passed ===" << endl;
    return true;
}

bool verify_bin_quiet(DataLoader *dl, string decodedFileName)
{
    int fd = open(decodedFileName.c_str(), O_RDONLY);
    if (fd == -1)
    {
        cout << "Error: can't open file" << endl;
        exit(1);
    }
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        cout << "Error: can't get file size" << endl;
        exit(1);
    }
    size_t length = sb.st_size;
    int8 *addr = (int8 *)mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED)
    {
        cout << "Error: mmap failed" << endl;
        exit(1);
    }
    int intPerLine = *(int *)addr;
    int numLines = *((int *)addr + 1);
    if (intPerLine != dl->getElementPerLine())
    {
        cout << "=== Verification failed: Decoded file has different intPerLine ===" << endl;
        return false;
    }
    if (numLines != dl->getNumLines())
    {
        cout << "=== Verification failed: Decoded file has different numLines ===" << endl;
        return false;
    }
    int8 *cur = (int8 *)(addr + 2 * sizeof(int));
    bool diff = false;
    int error_element = 0;
    for (int i = 0; i < numLines; i++)
    {
        for (int j = 0; j < intPerLine; j++)
        {
            if (*(cur++) != dl->getDataAry()[i][j])
            {
                diff = true;
                if (error_element == 0)
                    error_element = j;
            }
        }
        if (diff)
        {
            cout << "=== Verification failed at line " << i + 1 << " ===" << endl;
            cout << "Data / Decoded: " << endl;
            if (error_element > 2)
                error_element -= 2;
            for (int j = error_element; j < intPerLine; j++)
                cout << (int)dl->getDataAry()[i][j] << " / " << (int)*(cur - intPerLine + j) << endl;
            return false;
        }
    }
    cout << "=== Verification passed ===" << endl;
    return true;
}