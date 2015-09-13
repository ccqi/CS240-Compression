#ifndef BWTRANSFORM_H
#define BWTRANSFORM_H

#include "Decorator.h"



class BWTransform : public Decorator
{
    public:
        BWTransform(TextComponent*);
        virtual ~BWTransform();
        Encoding * encode();
        void print(std::ofstream&);
    private:
        Encoding * getDecode(Encoding*);

};

#endif // BWTRANSFORM_H
