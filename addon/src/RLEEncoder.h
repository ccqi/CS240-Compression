#ifndef RLEENCODER_H
#define RLEENCODER_H

#include "Decorator.h"


class RLEEncoder : public Decorator
{
    public:
        RLEEncoder(TextComponent*);
        virtual ~RLEEncoder();
        Encoding * encode();
        std::map<BITS, BITS> getTable() const;
        std::vector<BITS> getRunOrder() const;
        virtual void print(std::ofstream&);
    private:
        Encoding * getDecode(Encoding *);
        std::map<BITS, BITS> runTable_;
        std::vector<BITS> runOrder_;
};

#endif // RLEENCODER_H
