#include "BitStream.h"
#include <limits>

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

void BitOutputStream::writeCode(string code)
{
    for (int i = 0; i < code.length(); i++)
        writeBit(code[i] - '0');
}

void BitOutputStream::flush()
{
    // flush the buffer
    if (nbits == 0)
        return;
    buf <<= (8 - nbits);
    if (std::numeric_limits<char>::is_signed)
        buf -= (buf >> 7) << 8;
    out.put(static_cast<char>(buf));
    buf = 0;
    nbits = 0;
}
