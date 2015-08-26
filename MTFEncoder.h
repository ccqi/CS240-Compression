#ifndef MTFENCODER_H
#define MTFENCODER_H

#include "Decorator.h"


class MTFEncoder : public Decorator
{
    public:
        MTFEncoder(TextComponent*);
        virtual ~MTFEncoder();
        virtual void print(std::ofstream&);
        std::vector<bool> encode();
    private:
        std::vector<bool> getDecode(std::vector<bool>);
};

#endif // MTFENCODER_H
