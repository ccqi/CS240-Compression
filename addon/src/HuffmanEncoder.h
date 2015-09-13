#ifndef HUFFMANENCODER_H
#define HUFFMANENCODER_H

#include "Decorator.h"
#include <utility>

typedef std::map<BYTE, BITS > HuffmanMap;
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
        Encoding * encode();
        virtual void print(std::ofstream&);
    private:
        void getHuffmanCodes(Trie*, BITS&,HuffmanMap&);
        Encoding * getDecode(Encoding*);
        void writeTrie(Trie*, BITS&);
        Trie * readTrie(BITS::iterator&);

};
bool trieCompare(Trie*, Trie*);
#endif // HUFFMANENCODER_H
