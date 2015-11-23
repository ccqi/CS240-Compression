#ifndef BWTRANSFORM_H
#define BWTRANSFORM_H

#include "Decorator.h"



class BWTransform : public Decorator
{
    public:
        BWTransform(TextComponent*);
        BWTransform(BITS bits);
        virtual ~BWTransform();
        Encoding * encode();
        void print(std::ofstream&);
        Encoding * parseData();
    private:
        Encoding * getDecode(Encoding*);

};

#endif // BWTRANSFORM_H
