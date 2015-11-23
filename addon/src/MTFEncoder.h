#ifndef MTFENCODER_H
#define MTFENCODER_H

#include "Decorator.h"


class MTFEncoder : public Decorator
{
    public:
        MTFEncoder(TextComponent*);
        MTFEncoder(BITS);
        virtual ~MTFEncoder();
        virtual void print(std::ofstream&);
        Encoding * encode();
    private:
        Encoding * getDecode(Encoding*);
};

#endif // MTFENCODER_H
