#ifndef ENCODING_H
#define ENCODING_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <utility>
#include <algorithm>

typedef std::vector<bool> BITS;
typedef unsigned char BYTE;
typedef std::vector<BYTE> BYTES;
typedef std::deque <std::pair<std::string, BITS> > DATA;
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
        std::pair<std::string, BITS> get(int) const;
        int size() const;
        std::vector<std::string> getFields() const;
        BITS getBits() const;
        int getSize() const;
        BYTE * getBinary() const;
        BYTES getBytes() const;
        
        //setters
        void setBits(BITS);
        void setFields(std::string);
        State readState();
        void reset();
        void set(std::map<std::string, BITS>);

        //static conversion methods
        static BITS convertToBits(std::string);
        static BITS convertToBits(int,int);
        static BITS convertToBits(BYTES);
        static std::string convertToHexString(BITS);
        static std::string convertToString(BITS);
        static std::string convertToText(BITS);
        static std::string convertToText(std::string);
        static std::string convertToBinaryString(BITS);
        static BYTES convertToBytes(BITS);
        static int getBinarySize(int);
        static BYTE * convertToBinary(BITS);

        //mutators
        void writeBits(int, int);
        void writeBits(std::string, int, int);
        int readBits(int);
        void writeBinary(std::ofstream&);
        void add(const BITS& b);
        void add(std::string, const BITS& b);
        void add(bool b);
        void add(std::string, bool b);
        void addToFront(const BITS& b);
        void addToFront(std::string, const BITS& b);
        // operator overload
        friend double operator /(const Encoding& a,const Encoding& b);
    private:
        BITS bits_;
        BYTE * text_;
        TYPE type_;
        DATA data_;
        std::vector<std::string> fields_;
        BITS::iterator it_;

};

#endif // ENCODING_H
