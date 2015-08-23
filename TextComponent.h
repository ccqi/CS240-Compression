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

enum State {NONE, HUFFMAN, BWT, RLE, MTF, LZW,ENCODE, DECODE, QUIT};

typedef unsigned char BYTE;

class TextComponent
{
    public:
        TextComponent();
        virtual ~TextComponent();
        virtual void print(std::ofstream &) = 0;
        virtual std::vector<bool> encode() = 0;
        virtual TextComponent * decode() = 0;
        virtual State getID() const = 0;
        void setEncoding(std::vector<bool>);
        void setEncoding(std::string);
        std::vector<bool> getEncoding() const;
    protected:
        std::vector<bool> getBits(std::string);
        virtual std::vector<bool> getDecode(std::vector<bool>) = 0;
        std::string getString(std::vector<bool>);
        BYTE * getBytes(std::vector<bool>);
        std::vector<bool> encoding_;

        State id_;
    private:


};

#endif // TEXTCOMPONENT_H
