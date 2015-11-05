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
        std::vector<std::pair<int, BITS> > getOutput() const;

    private:
        std::map<std::string,int> table_;
        std::vector<std::pair<int, BITS> > output_;
        Encoding * getDecode(Encoding*);
};

#endif // LZWENCODER_H
