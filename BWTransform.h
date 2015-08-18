#ifndef BWTRANSFORM_H
#define BWTRANSFORM_H

#include "Decorator.h"


class BWTransform : public Decorator
{
    public:
        BWTransform(TextComponent*);
        virtual ~BWTransform();
        std::vector<bool> encode();
        void print(std::ofstream&);
        //TextComponent * getComponent();
    private:
        std::vector<bool> getDecode(std::vector<bool>);

};

#endif // BWTRANSFORM_H
