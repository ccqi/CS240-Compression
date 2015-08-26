#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <iostream>
#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <vector>
#include <deque>
#include <bitset>
#include <map>
#include <queue>
#include <iterator>
#include <set>
#include <assert.h>
#include <fstream>
#include "Encoding.h"

enum State {NONE, HUFFMAN, BWT, RLE, MTF, LZW,ENCODE, DECODE, QUIT};

class TextComponent
{
    public:
        TextComponent();
        virtual ~TextComponent();
        virtual void print(std::ofstream &);
        virtual std::vector<bool> encode() = 0;
        virtual TextComponent * decode() = 0;
        virtual State getID() const = 0;
        Encoding getEncodings() const;
        void setEncoding(std::vector<bool>);
        void setEncoding(std::string);
        std::vector<bool> getEncoding() const;
    protected:
        std::vector<bool> getBits(std::string);
        virtual std::vector<bool> getDecode(std::vector<bool>) = 0;
        std::string getString(std::vector<bool>);
        BYTE * getBytes(std::vector<bool>);
        void writeBits(std::vector<bool>&, int, int);
        int readBits(std::vector<bool>, std::vector<bool>::iterator&, int);
        int getBinarySize(int);
        std::vector<bool> encoding_;
        Encoding encoding;

        State id_;
    private:


};

#endif // TEXTCOMPONENT_H
