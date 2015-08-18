#include "HuffmanEncoder.h"

using namespace std;

HuffmanEncoder::HuffmanEncoder(TextComponent * component):Decorator(component){
    id_ = HUFFMAN;
}

HuffmanEncoder::~HuffmanEncoder(){}


struct TrieCompare{
    bool operator()(HuffmanEncoder::Trie* a, HuffmanEncoder::Trie* b){
        return a->freq > b->freq;
    }
};

void printTrie(HuffmanEncoder::Trie * trie,int level){
    if(trie==NULL){
        return;
    }

        for(int i=0;i<level;i++){
            cout<<'\t';
        }
        if(trie->c){
            cout<<trie->c;
        }
        else{
            cout<<'I';
        }
        cout<<endl;
    printTrie(trie->zero,level+1);
    printTrie(trie->one,level+1);
}

vector<bool> HuffmanEncoder::encode(){
    vector<bool> plainBits = Decorator::encode();
    string plainText = getString(plainBits);
    //make freq table
    for(auto it = plainText.begin();it!=plainText.end();++it){
        char c = *it;
        frequencyTable_[c]++;
    }
    //construct leaves
    priority_queue<Trie*, vector<Trie*>, TrieCompare> leaves;
    for(auto it = frequencyTable_.begin();it!=frequencyTable_.end();++it){
        char c = it->first;
        int freq = it->second;
        HuffmanEncoder::Trie * leaf = new Trie();
        leaf->freq = freq;
        leaf->c = c;
        leaf->isLeaf = true;
        leaves.push(leaf);
    }

    //construct huffman trie
    while(leaves.size() > 1){
        HuffmanEncoder::Trie * leaf1 = leaves.top();
        leaves.pop();
        HuffmanEncoder::Trie * leaf2 = leaves.top();
        leaves.pop();
        HuffmanEncoder::Trie * node = new Trie();
        node->freq = leaf1->freq + leaf2->freq;
        node->zero = leaf1;
        node->one = leaf2;
        node->isLeaf = false;
        leaves.push(node);
        //sort(leaves.begin(),leaves.end(),trieCompare);
    }
    huffmanTrie_ = leaves.top();

    Trie * iter = huffmanTrie_;
    HuffmanCode prefix;
    getHuffmanCodes(iter, prefix, huffmanCodes_);

    for(auto it = plainText.begin();it!=plainText.end();++it){
        char c = *it;
        HuffmanCode curCode = huffmanCodes_[c];
        encoding_.insert(encoding_.end(),curCode.begin(),curCode.end());

    }
    assert(equal(plainBits.begin(),plainBits.end(),getDecode(encoding_).begin()));
    return encoding_;
}
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
    cout<<endl<<"Huffman codes: "<<endl;
    for(auto iter = huffmanCodes_.begin();iter!=huffmanCodes_.end();++iter){
        char c = iter->first;
        if(isalpha(c)){
            cout<<c;
        }
        else{
            cout<<'('<<(int)c<<')';
        }
        cout<<":";
        HuffmanCode code = iter->second;
        copy(code.begin(),code.end(),ostream_iterator<bool>(cout));
        cout<<endl;
    }
    cout<<"New encoding: "<<endl;
    for(auto iter = encoding_.begin();iter!=encoding_.end();++iter){
            cout<<*iter;
    }
    cout<<endl;
    cout<<"Length of new encoding "<<encoding_.size()<<endl;

}
//TextComponent * HuffmanEncoder::getComponent(){
//    return Decorator::getComponent();
//}
vector<bool> HuffmanEncoder::getDecode(vector<bool> cipherCode){
    string plainText;
    Trie * tree_iter = huffmanTrie_;
    for(auto it = cipherCode.begin();it!=cipherCode.end();++it){
        if(*it){
            tree_iter = tree_iter->one;
        }
        else{
            tree_iter = tree_iter->zero;
        }
        if(tree_iter->isLeaf){
            plainText+=tree_iter->c;
            tree_iter = huffmanTrie_;
        }
    }
    return getBits(plainText);
}
