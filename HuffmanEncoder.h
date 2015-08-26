#ifndef HUFFMANENCODER_H
#define HUFFMANENCODER_H

#include "Decorator.h"
#include <utility>

typedef std::vector<bool> HuffmanCode;
typedef std::map<BYTE, HuffmanCode > HuffmanMap;
struct Trie{
    int freq;
    BYTE c;
    bool isLeaf;
    Trie * zero;
    Trie * one;
};

class HuffmanEncoder : public Decorator
{
    public:
        HuffmanEncoder(TextComponent*);
        virtual ~HuffmanEncoder();
        std::vector<bool> encode();
        virtual void print(std::ofstream&);
    private:
        void getHuffmanCodes(Trie*, HuffmanCode&,HuffmanMap&);
        std::vector<bool> getDecode(std::vector<bool>);
        void writeTrie(Trie*, std::vector<bool>&);
        Trie * readTrie(std::vector<bool>::iterator&);

};
bool trieCompare(Trie*, Trie*);
#endif // HUFFMANENCODER_H
