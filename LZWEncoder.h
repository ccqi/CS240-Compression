#ifndef LZWENCODER_H
#define LZWENCODER_H

#include "Decorator.h"



class LZWEncoder : public Decorator
{
    public:
        LZWEncoder(TextComponent *);
        virtual ~LZWEncoder();
        std::vector<bool> encode();
        virtual void print(std::ofstream&);

    private:
        std::vector<std::string> table_;
        std::vector<bool> getDecode(std::vector<bool>);
};

#endif // LZWENCODER_H
