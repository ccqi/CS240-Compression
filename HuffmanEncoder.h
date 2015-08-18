#ifndef HUFFMANENCODER_H
#define HUFFMANENCODER_H

#include "Decorator.h"
#include <utility>

typedef std::vector<bool> HuffmanCode;
typedef std::map<char, HuffmanCode > HuffmanMap;


class HuffmanEncoder : public Decorator
{
    public:
        HuffmanEncoder(TextComponent*);
        virtual ~HuffmanEncoder();
        std::vector<bool> encode();
        virtual void print(std::ofstream&);

        struct Trie{
            int freq;
            char c;
            bool isLeaf;
            Trie * zero;
            Trie * one;
        };
        //TextComponent * getComponent();

    private:
        std::map<char, int> frequencyTable_;
        Trie * huffmanTrie_;
        HuffmanMap huffmanCodes_;
        void getHuffmanCodes(Trie*, HuffmanCode&,HuffmanMap&);
        std::vector<bool> getDecode(std::vector<bool>);

};
bool trieCompare(HuffmanEncoder::Trie*, HuffmanEncoder::Trie*);
#endif // HUFFMANENCODER_H
