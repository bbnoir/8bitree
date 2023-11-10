#include <iostream>
#include "Encoder.h"
#include "Decoder.h"
#include "DataLoader.h"
#include "Constants.h"
#include <random>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    string dataFileName = "/home/U114ysyang/project/8bitree/data/ALBERT/ALBERT_word_emb.txt";
    string encodedFileName = "/home/U114ysyang/project/8bitree/data/ALBERT/ALBERT_word_emb_encoded.txt";
    string decodedFileName = "/home/U114ysyang/project/8bitree/data/ALBERT/ALBERT_word_emb_decoded.txt";

    // given data, output encoded data
    DataLoader *dl = new DataLoader(dataFileName);
    TreeArray *tree = TreeArray::genHuffmanArray(dl);
    tree->modify(100);
    cout << *tree << endl;
    Encoder encoder(dl, tree);
    encoder.encode(dataFileName, encodedFileName);

    // given encoded data, output decoded data
    ifstream in(encodedFileName);
    ofstream out(decodedFileName);
    Decoder decoder(in);
    while (decoder.notEOF())
    {
        out << decoder.readline();
        if (decoder.notEOF())
            out << endl;
    }

    return EXIT_SUCCESS;
}