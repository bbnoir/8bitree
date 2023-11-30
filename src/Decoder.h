#pragma once
#include "CodeTree.h"
#include "BitStream.h"
#include "Constants.h"
#include <iostream>
#include <fstream>
#include <vector>

#include "DataLoader.h"

using namespace std;

class CodeTree;

/*
 * Reads from a Huffman-coded bit stream and decodes symbols.
 */
class Decoder final
{

private:
    BitInputStream in;
    ofstream out;
    int intPerLine;
    int numLines;
    const CodeTree *codeTree;
    // helper for generating CodeTree
    CodeTree *CanonicalToCodeTree(vector<int> codeLengths);
    int8_t decodeOne(int &line_width);

public:
    explicit Decoder(string encodedFileName, string decodedFileName);
    void decode();
    int max_line_width;
};