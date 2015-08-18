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
        //TextComponent * getComponent();
    private:
        std::vector<bool> getDecode(std::vector<bool>);
        std::string alpha_;
};

#endif // MTFENCODER_H
