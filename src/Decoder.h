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

    /*---- Fields ----*/

    // The underlying bit input stream.
private:
    ifstream &input;

private:
    CodeTree *CanonicalToCodeTree(vector<int> codeLengths);

    // The code tree to use in the next read() operation. Must be given a non-null value
    // before calling read(). The tree can be changed after each symbol decoded, as long
    // as the encoder and decoder have the same tree at the same point in the code stream.
public:
    const CodeTree *codeTree;

    /*---- Constructor ----*/

    // Constructs a Huffman decoder based on the given bit input stream.
public:
    explicit Decoder(ifstream &in);

    /*---- Method ----*/

    // Reads from the input stream to decode the next Huffman-coded symbol.
public:
    string readline();

    bool notEOF();
};