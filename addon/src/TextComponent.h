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

class TextComponent
{
    public:
        TextComponent();
        virtual ~TextComponent();
        virtual void print(std::ofstream &);
        virtual Encoding * encode() = 0;
        virtual TextComponent * decode() = 0;
        virtual State getID() const = 0;
        void setEncoding(Encoding*);
        Encoding * getEncoding() const;
        double getCompressionRatio() const;
    protected:
        virtual Encoding * getDecode(Encoding *) = 0;
        Encoding * encoding_;
        Encoding * originalEncoding_;
        double compressionRatio_;
        State id_;
    private:


};

#endif // TEXTCOMPONENT_H
