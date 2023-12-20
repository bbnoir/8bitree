#include "Constants.h"
#include "DataLoader.h"
#include "TreeArray.h"
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <input data>" << endl;
        return EXIT_FAILURE;
    }

    // open file
    int fd = open(argv[1], O_RDONLY);
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
    int elementPerLine = *(int *)addr;
    int numLines = *((int *)addr + 1);
    int8 *cur = (int8 *)(addr + 2 * sizeof(int));
    vector<int8> *line;
    vector<freq_t> *freq;
    int8 num = 0;
    int limitLineWidth = 0;
    int tempWidth = 0;
    TreeArray *tree;
    for (int i = 0; i < numLines; i++)
    {
        line = new vector<int8>(elementPerLine, 0);
        freq = new vector<freq_t>(SYM_NUM, 0);
        for (int j = 0; j < elementPerLine; j++)
        {
            num = *(cur++);
            line->at(j) = num;
            freq->at(num + 128)++;
        }
        tree = new TreeArray(*freq);
        tempWidth = 0;
        for (int j = 0; j < elementPerLine; j++)
            tempWidth += tree->cl[line->at(j) + 128];

        if (tempWidth > limitLineWidth)
            limitLineWidth = tempWidth;

        delete freq;
        delete tree;
    }
    munmap(addr, length);
    close(fd);

    cout << "Data: " << argv[1] << endl;
    cout << "Compression ratio: " << (1 - (double)limitLineWidth / ((double)elementPerLine * 8)) * 100.0 << "%" << endl;
    cout << endl;

    return EXIT_SUCCESS;
}
