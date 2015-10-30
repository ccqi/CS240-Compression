#ifndef DECORATOR_H
#define DECORATOR_H

#include "TextComponent.h"
#include <iostream>
#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <vector>
#include <deque>
#include <bitset>


//enum DecorateClass

class Decorator : public TextComponent
{
    public:
        Decorator(TextComponent*);
        virtual ~Decorator();
    protected:
        virtual Encoding * encode();
        virtual void print(std::ofstream&);
        virtual TextComponent * decode();
        State getID() const;
    private:
        TextComponent * component_;
        virtual Encoding * getDecode(Encoding*);

};

#endif // DECORATOR_H
