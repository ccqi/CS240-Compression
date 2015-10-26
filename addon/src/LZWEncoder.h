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
        std::map<int, std::string> getTable() const;

    private:
        std::map<std::string,int> table_;
        Encoding * getDecode(Encoding*);
};

#endif // LZWENCODER_H
