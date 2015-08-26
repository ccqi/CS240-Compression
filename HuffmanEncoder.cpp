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

vector<bool> HuffmanEncoder::encode(){
    vector<bool> plainBits = Decorator::encode();
    int size = plainBits.size()/8;
    BYTE * plainText = getBytes(plainBits);
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
    Trie * huffmanTrie = leaves.top();
    Trie * iter = huffmanTrie;
    writeTrie(iter,encoding_);
    iter = huffmanTrie;

    //get huffman codes from trie and encode
    HuffmanCode prefix;
    HuffmanMap huffmanCodes;
    getHuffmanCodes(iter, prefix, huffmanCodes);
    for(int i=0;i<size;i++){
        BYTE c = plainText[i];
        HuffmanCode curCode = huffmanCodes[c];
        encoding_.insert(encoding_.end(),curCode.begin(),curCode.end());
    }
    //pad a non-multiple of 8 with zeros
    BYTE bitsize =encoding_.size();
    int padding = 0;
    if(bitsize % 8 > 0){
        padding = 8 - bitsize % 8;
        for(int i=0;i<padding;i++){
            encoding_.push_back(false);
        }
    }
    stringstream ss;
    ss<<(char)padding;
    vector<bool> bits = getBits(ss.str());
    bits.insert(bits.end(),encoding_.begin(),encoding_.end());
    encoding_ = bits;

    double ratio = (double)encoding_.size()/plainBits.size();
    cout<<"Compression ratio: "<<ratio<<endl;
    //assert(equal(plainBits.begin(),plainBits.end(),getDecode(encoding_).begin()));
    return encoding_;
}
//convert tree to map
void HuffmanEncoder::getHuffmanCodes(Trie * trie, HuffmanCode & prefix , HuffmanMap & map){
    if(trie ==NULL){
        return;
    }
    if(trie->isLeaf){
        map[trie->c] = prefix;
    }
    else{
        HuffmanCode leftPrefix = prefix;
        leftPrefix.push_back(false);
        getHuffmanCodes(trie->zero,leftPrefix,map);
        HuffmanCode rightPrefix = prefix;
        rightPrefix.push_back(true);
        getHuffmanCodes(trie->one,rightPrefix,map);
    }
}
void HuffmanEncoder::print(ofstream& os){
    Decorator::print(os);
    BYTE * binary= getBytes(encoding_);
    int size = encoding_.size()/8;
    os.write(reinterpret_cast<const char*>(&binary[0]),size*sizeof(BYTE));
    os.close();
    cout<<"After huffman encoding"
    <<endl<<"Length of new encoding "<<encoding_.size()<<endl;
//    double ratio = (double)encoding_.size()/getDecode(encoding_).size();
//    cout<<"Compression ratio: "<<ratio<<endl;

        //Decorator::print(os);
//    cout<<endl<<"Huffman codes: "<<endl;
//    for(auto iter = huffmanCodes_.begin();iter!=huffmanCodes_.end();++iter){
//        char c = iter->first;
//        if(isalpha(c)){
//            cout<<c;
//        }
//        else{
//            cout<<'('<<(int)c<<')';
//        }
//        cout<<":";
//        HuffmanCode code = iter->second;
//        copy(code.begin(),code.end(),ostream_iterator<bool>(cout));
//        cout<<endl;
//    }
//    cout<<"New encoding: "<<endl;
//    for(auto iter = encoding_.begin();iter!=encoding_.end();++iter){
//            cout<<*iter;
//    }
//    cout<<endl;

}

vector<bool> HuffmanEncoder::getDecode(vector<bool> cipherCode){
    string plainText;

    //read padding
    vector<bool> first(8);
    copy(cipherCode.begin(),cipherCode.begin()+8,first.begin());
    BYTE * padding = getBytes(first);
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
    return getBits(plainText);
}
//tree encryption: 1 bit + data for leaf nodes; 0 bit for internal nodes
void HuffmanEncoder::writeTrie(Trie* trie, vector<bool>& bits){
    if(trie ==NULL){
        return;
    }
    if(trie->isLeaf){
        stringstream ss;
        ss<<trie->c;
        vector<bool> newByte = getBits(ss.str());
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
Trie* HuffmanEncoder::readTrie(vector<bool>::iterator& it){
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
