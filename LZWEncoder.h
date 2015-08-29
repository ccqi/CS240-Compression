#ifndef LZWENCODER_H
#define LZWENCODER_H

#include "Decorator.h"



class LZWEncoder : public Decorator
{
    public:
        LZWEncoder(TextComponent *);
        virtual ~LZWEncoder();
        Encoding * encode();
        virtual void print(std::ofstream&);

    private:
        std::vector<std::string> table_;
        Encoding * getDecode(Encoding*);
};

#endif // LZWENCODER_H
