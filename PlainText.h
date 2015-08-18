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
        std::vector<bool> encode();              //does nothing, decorators do the encoding
        TextComponent * decode();
        //void recurseDecode(State);
        //friend std::ostream& operator <<(std::ostream&,PlainText&);
        virtual void print(std::ofstream&);
        State getID() const;
        //TextComponent * getComponent();
    private:
        std::string plainText_;
        std::vector<bool> getDecode(std::vector<bool>);

};

#endif // PLAINTEXT_H
