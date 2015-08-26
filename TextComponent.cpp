#include "TextComponent.h"

using namespace std;


TextComponent::TextComponent(){
    id_ = NONE;
}

TextComponent::~TextComponent(){}

vector<bool> TextComponent::getBits(string plainText){
    vector<bool> binaryText;
    int count = 0;
    for(auto it = plainText.begin();it!=plainText.end();++it){
        char c = *it;
        for (int i = 0; i < 8; ++i) {
            binaryText.push_back((c>>i)&1);
        }
        count ++;
    }
    return binaryText;
}
string TextComponent::getString(vector<bool> bits){
    int count = 0;
    string text;

    while(count<bits.size()){

        vector<bool> bitChar(8);
        copy(bits.begin()+count,bits.begin()+count+8,bitChar.begin());
        //reverse(bitChar.begin(),bitChar.end());

        unsigned char c = 0;
        for (int i=0; i < 8; i++){
            c += (bitChar[i] << i);
        }
        text += c;
        count += 8;
    }
    return text;
}

BYTE * TextComponent::getBytes(vector<bool> bits){
    int count = bits.size()/8;
    BYTE * bytes = new BYTE[count];
    auto it = bits.begin();
    for(int i=0;i<count;i++){
        BYTE c = 0;
        for (int i=0; i < 8; i++){
            c += (*it << i);
            ++it;
        }
        bytes[i] = c;
    }
    return bytes;
}

void TextComponent::setEncoding(vector<bool> encoding){
    encoding_ = encoding;
}
void TextComponent::setEncoding(string encoding){
    encoding_ = getBits(encoding);
}
vector<bool> TextComponent::getEncoding() const{
    return encoding_;
}
//convert from index to binary bits
void TextComponent::writeBits(vector<bool>& bits, int code,int bit_size){
//    int bitSize = getBinarySize(max_size);
    for(int i=0;i<bit_size;i++){
        bits.push_back(code%2);
        code/=2;
    }
}

//convert a string of bits into an index
int TextComponent::readBits(vector<bool> bits, vector<bool>::iterator& it,int bit_size){

//    int bit_size = getBinarySize(max_size);
    int acc = 0;
    for(int i=0;i<bit_size;i++){
        if(*it == 1){
            int pow2 = 1;
            for(int j=0;j<i;j++){
                pow2 *= 2;
            }
            acc+=pow2;
        }
        ++it;
    }
    return acc;
}
int TextComponent::getBinarySize(int max_size){
    int bitSize = 0;
    int iter=1;
    while(iter<=max_size){
        bitSize++;
        iter *=2;
    }
    return bitSize;
}
void TextComponent::print(ofstream& os){
    BYTE * binary= getBytes(encoding_);
    int size = encoding_.size()/8;

    os.write(reinterpret_cast<const char*>(&binary[0]),size*sizeof(BYTE));
    os.close();
}
