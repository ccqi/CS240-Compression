#ifndef PLAINTEXT_H
#define PLAINTEXT_H

#include "TextComponent.h"
#include <string>
#include <fstream>
#include <iostream>

class PlainText : public TextComponent
{
    public:
        PlainText(std::string);
        PlainText(std::vector<bool>);
        virtual ~PlainText();
        std::vector<bool> encode();
        TextComponent * decode();
        virtual void print(std::ofstream&);
        State getID() const;
    private:
        std::string plainText_;
        std::vector<bool> getDecode(std::vector<bool>);

};

#endif // PLAINTEXT_H
