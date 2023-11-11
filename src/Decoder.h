#pragma once
#include "CodeTree.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class CodeTree;

/*
 * Reads from a Huffman-coded bit stream and decodes symbols.
 */
class Decoder final
{

private:
    ifstream in;
    ofstream out;
    const CodeTree *codeTree;
    // helper for generating CodeTree
    CodeTree *CanonicalToCodeTree(vector<int> codeLengths);
    string readline();
    bool notEOF();

public:
    explicit Decoder(string encodedFileName, string decodedFileName);
    void decode();
    void decode(int numLines);
};