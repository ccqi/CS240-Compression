#ifndef RLEENCODER_H
#define RLEENCODER_H

#include "Decorator.h"


class RLEEncoder : public Decorator
{
    public:
        RLEEncoder(TextComponent*);
        virtual ~RLEEncoder();
        Encoding * encode();
        virtual void print(std::ofstream&);
    private:
        Encoding * getDecode(Encoding *);
};

#endif // RLEENCODER_H
