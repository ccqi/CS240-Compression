#ifndef ENCODING_H
#define ENCODING_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

typedef unsigned char BYTE;

class Encoding
{
    public:
        Encoding();
        virtual ~Encoding();
        std::vector<bool> getEncodedBits() const;
        std::string getEncodedString() const;
        std::vector<bool> getOriginalBits() const;
        std::string getOriginalString() const;
        double getRatio() const;

    private:
        std::vector<bool> bits_;
        std::string text_;
        std::string originalText_;
        std::vector<bool> originalBits_;
        double compressionRatio_;

        std::vector<bool> getBits(std::string);
        std::string getString(std::vector<bool>);
        BYTE * getBytes(std::vector<bool>);
};

#endif // ENCODING_H
