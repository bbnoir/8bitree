#include "BitStream.h"

BitInputStream::BitInputStream(string fileName) : in(*(new ifstream(fileName, ios::binary))), buf(0), nbits(0) {}

int BitInputStream::readBit()
{
    // check eof and read one bit
    if (in.eof())
        return -1;
    if (nbits == 0)
    {
        in.read(&buf, 1);
        nbits = 8;
    }
    nbits--;
    return (buf >> nbits) & 1;
}

int BitInputStream::readInt()
{
    int i;
    in.read((char *)&i, sizeof(int));
    return i;
}

BitOutputStream::BitOutputStream(string fileName) : out(*(new ofstream(fileName, ios::binary))), buf(0), nbits(0) {}

BitOutputStream::~BitOutputStream()
{
    flush();
    out.close();
}

void BitOutputStream::writeBit(int i)
{
    // write one bit
    buf = (buf << 1) | i;
    nbits++;
    if (nbits == 8)
        flush();
}

void BitOutputStream::writeInt(int i)
{
    out.write((char *)&i, sizeof(int));
}

void BitOutputStream::writeCode(int code, int length)
{
    for (int i = 0; i < length; i++)
        writeBit((code >> (length - 1 - i)) & 1);
}

void BitOutputStream::flush()
{
    // flush the buffer
    if (nbits == 0)
        return;
    buf <<= (8 - nbits);
    out.write(&buf, sizeof(char));
    buf = 0;
    nbits = 0;
}
