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
        //TextComponent * getComponent();

    private:
        std::vector<std::string> table_;
        std::vector<bool> getDecode(std::vector<bool>);
        void writeBits(std::vector<bool>&, int, int);
        int readBits(std::vector<bool>, std::vector<bool>::iterator&, int);
        //std::vector<bit_byte> toBytes(std::vector<bool>);
        int getBinarySize(int);
};

#endif // LZWENCODER_H
