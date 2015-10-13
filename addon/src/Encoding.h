#ifndef ENCODING_H
#define ENCODING_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

typedef std::vector<bool> BITS;
typedef unsigned char BYTE;
typedef std::vector<BYTE> BYTES;
enum State {NONE, PLAIN, HUFFMAN, BWT, RLE, MTF, LZW,ENCODE, DECODE, QUIT};
enum TYPE {TEXT, BINARY, HEX};

class Encoding
{
    public:
        explicit Encoding();
        Encoding(std::string,TYPE);
        Encoding(BITS);
        Encoding(BYTES);
        virtual ~Encoding();

        //getters
        BITS getBits() const;
        int getSize() const;
        BYTE * getBinary() const;
        BYTES getBytes() const;

        //setters
        void setBits(BITS);
        State readState();
        void reset();

        //static conversion methods
        static BITS convertToBits(int,int);
        static BITS convertToBits(std::string);
        static BITS convertToBits(BYTES);
        static std::string convertToHexString(BITS);
        static std::string convertToString(BITS);
        static std::string convertToBinaryString(BITS);
        static BYTES convertToBytes(BITS);
        static int getBinarySize(int);
        static BYTE * convertToBinary(BITS);

        //mutators
        void writeBits(int, int);
        int readBits(int);
        void writeBinary(std::ofstream&);
        void add(const BITS& b);
        void addToFront(const BITS& b);

        friend double operator /(const Encoding& a,const Encoding& b);
    private:
        BITS bits_;
        BYTE * text_;
        TYPE type_;
        BITS::iterator it_;

};

#endif // ENCODING_H
