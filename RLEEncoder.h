#ifndef RLEENCODER_H
#define RLEENCODER_H

#include "Decorator.h"


class RLEEncoder : public Decorator
{
    public:
        RLEEncoder(TextComponent*);
        virtual ~RLEEncoder();
        std::vector<bool> encode();
        virtual void print(std::ofstream&);
        //TextComponent * getComponent();
    private:
        std::vector<bool> getDecode(std::vector<bool>);
};

#endif // RLEENCODER_H
