#pragma once
#include <istream>
#include <ostream>
#include <fstream>

using namespace std;

class BitInputStream
{
private:
    istream &in;
    char buf;
    int nbits;

public:
    BitInputStream(string fileName);
    int readBit();
    int readInt();
};

class BitOutputStream
{
private:
    ofstream &out;
    char buf;
    int nbits;

public:
    BitOutputStream(string fileName);
    ~BitOutputStream();
    void writeBit(int i);
    void writeInt(int i);
    void writeCode(string code);
    void flush();
};