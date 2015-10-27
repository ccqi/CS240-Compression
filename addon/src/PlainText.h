#ifndef PLAINTEXT_H
#define PLAINTEXT_H

#include "TextComponent.h"
#include <string>
#include <fstream>
#include <iostream>

class PlainText : public TextComponent
{
    public:
        PlainText(Encoding *);
        virtual ~PlainText();
        Encoding * encode();
        TextComponent * decode();
        virtual void print(std::ofstream&);
        State getID() const;
    private:
        std::string plainText_;
        Encoding * getDecode(Encoding *);

};

#endif // PLAINTEXT_H
