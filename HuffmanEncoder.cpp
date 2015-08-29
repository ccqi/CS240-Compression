#include "HuffmanEncoder.h"

using namespace std;

HuffmanEncoder::HuffmanEncoder(TextComponent * component):Decorator(component){
    id_ = HUFFMAN;
}

HuffmanEncoder::~HuffmanEncoder(){}


struct TrieCompare{
    bool operator()(Trie* a, Trie* b){
        return a->freq > b->freq;
    }
};

//void printTrie(Trie * trie,int level){
//    if(trie==NULL){
//        return;
//    }
//
//        for(int i=0;i<level;i++){
//            cout<<'\t';
//        }
//        if(trie->c){
//            cout<<trie->c;
//        }
//        else{
//            cout<<'I';
//        }
//        cout<<endl;
//    printTrie(trie->zero,level+1);
//    printTrie(trie->one,level+1);
//}

Encoding * HuffmanEncoder::encode(){
    Encoding * encoding = Decorator::encode();
    BITS plainBits = encoding->getBits();
    int size = encoding->getSize()/8;
    BYTE * plainText = encoding->getBinary();
    //make freq table
    map<BYTE,int> frequencyTable;
    for(int i=0;i<size;i++){
        frequencyTable[plainText[i]]++;
    }
    //construct leaves
    priority_queue<Trie*, vector<Trie*>, TrieCompare> leaves;
    for(auto it = frequencyTable.begin();it!=frequencyTable.end();++it){
        char c = it->first;
        int freq = it->second;
        Trie * leaf = new Trie();
        leaf->freq = freq;
        leaf->c = c;
        leaf->isLeaf = true;
        leaves.push(leaf);
    }

    //construct huffman trie
    while(leaves.size() > 1){
        Trie * leaf1 = leaves.top();
        leaves.pop();
        Trie * leaf2 = leaves.top();
        leaves.pop();
        Trie * node = new Trie();
        node->freq = leaf1->freq + leaf2->freq;
        node->zero = leaf1;
        node->one = leaf2;
        node->isLeaf = false;
        leaves.push(node);
    }


    //write trie to beginning of file for decoding
    BITS cipherBits;
    Trie * huffmanTrie = leaves.top();
    Trie * iter = huffmanTrie;
    writeTrie(iter,cipherBits);
    iter = huffmanTrie;
    encoding_->add(cipherBits);
    //get huffman codes from trie and encode
    BITS prefix;
    HuffmanMap huffmanCodes;
    getHuffmanCodes(iter, prefix, huffmanCodes);


    for(int i=0;i<size;i++){
        BYTE c = plainText[i];
        BITS curCode = huffmanCodes[c];
        encoding_->add(curCode);
    }
    //pad a non-multiple of 8 with zeros
    int bitsize =cipherBits.size();
    int padding = 0;
    if(bitsize % 8 > 0){
        padding = 8 - bitsize % 8;
        for(int i=0;i<padding;i++){
            encoding_->writeBits(0,1);
        }
    }
    encoding_->addToFront(Encoding::convertToBits(padding,8));
    encoding_->addToFront(Encoding::convertToBits(id_,8));
//    encoding_->add(cipherBits);
    return encoding_;
}
//convert tree to map
void HuffmanEncoder::getHuffmanCodes(Trie * trie, BITS & prefix , HuffmanMap & map){
    if(trie ==NULL){
        return;
    }
    if(trie->isLeaf){
        map[trie->c] = prefix;
    }
    else{
        BITS leftPrefix = prefix;
        leftPrefix.push_back(false);
        getHuffmanCodes(trie->zero,leftPrefix,map);
        BITS rightPrefix = prefix;
        rightPrefix.push_back(true);
        getHuffmanCodes(trie->one,rightPrefix,map);
    }
}
void HuffmanEncoder::print(ofstream& os){
    Decorator::print(os);
    cout<<"After huffman encoding"
    <<endl<<"Length of new encoding "<<encoding_->getSize()<<endl;

}

Encoding * HuffmanEncoder::getDecode(Encoding * encoding){
    string plainText;

    BITS cipherCode = encoding->getBits();
    //read padding
    BITS first(8);
    copy(cipherCode.begin(),cipherCode.begin()+8,first.begin());
    BYTE * padding = Encoding::convertToBinary(first);
    //read tree
    auto it = cipherCode.begin()+8;
    Trie * head = readTrie(it);
    Trie * tree_iter = head;

    //decrpyt
    ++it;
    for(;it!=cipherCode.end()-*padding;++it){
        if(tree_iter==NULL){
            continue;
        }
        if(*it){
            tree_iter = tree_iter->one;
        }
        else{
            tree_iter = tree_iter->zero;
        }
        if(tree_iter->isLeaf){
            plainText+=tree_iter->c;
            tree_iter = head;
        }
    }
    return new Encoding(plainText,TEXT);
}
//tree encryption: 1 bit + data for leaf nodes; 0 bit for internal nodes
void HuffmanEncoder::writeTrie(Trie* trie, BITS& bits){
    if(trie ==NULL){
        return;
    }
    if(trie->isLeaf){
        stringstream ss;
        ss<<trie->c;
        BITS newByte = Encoding::convertToBits(ss.str());
        bits.push_back(true);
        bits.insert(bits.end(),newByte.begin(),newByte.end());
        return;
    }
    else{
        bits.push_back(false);
        writeTrie(trie->zero,bits);
        writeTrie(trie->one,bits);
        return;
    }
}
Trie* HuffmanEncoder::readTrie(BITS::iterator& it){
    if(*it){
        Trie * trie = new Trie();
        trie->isLeaf = true;
        char c;
        for (int i=0; i < 8; i++){
            c += (*(++it) << i);
        }
        trie->c = c;
        return trie;
    }
    else{
        Trie * trie = new Trie();
        trie->isLeaf = false;
        Trie * zero = readTrie(++it);
        Trie * one = readTrie(++it);
        trie->zero = zero;
        trie->one = one;
        return trie;
    }
}
