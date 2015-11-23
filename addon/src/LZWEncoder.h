#ifndef LZWENCODER_H
#define LZWENCODER_H

#include "Decorator.h"



class LZWEncoder : public Decorator
{
    public:
        LZWEncoder(TextComponent *);
        LZWEncoder(BITS);
        virtual ~LZWEncoder();
        Encoding * encode();
        virtual void print(std::ofstream&);
        std::map<int, std::string> getTable() const;
        std::deque<std::tuple<std::string, int, BITS> > getOutput() const;

    private:
        std::map<std::string,int> table_;
        std::deque<std::tuple<std::string, int, BITS> > output_;
        Encoding * getDecode(Encoding*);
};

#endif // LZWENCODER_H
