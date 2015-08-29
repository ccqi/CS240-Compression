#include "Encoding.h"

using namespace std;

Encoding::Encoding(){
    bits_ = BITS();
    it_ =bits_.begin();
    type_ = BINARY;
    text_ = NULL;
}
Encoding::Encoding(string data,TYPE type):type_(type){
    if(type==TEXT){
        text_ = new BYTE[data.size()];
        for(int i=0;i<data.size();i++){
            text_[i] = data[i];
        }
        bits_ = Encoding::convertToBits(data);
        reset();
    }
}
Encoding::Encoding(BITS data):bits_(data),type_(BINARY){
    reset();
    text_ = getBinary();

}
Encoding::Encoding(BYTES data):type_(BINARY){
    bits_ = convertToBits(data);
    reset();
    text_ = getBinary();

}
Encoding::~Encoding(){
//    if(text_!=NULL){
//            delete[] text_;
//    }
//    for(int i=0;i<getSize()/8;i++){
//        delete text_[i];
//    }
}
BITS Encoding::getBits() const{
    return bits_;
}
BYTE * Encoding::getBinary() const{
    int count = bits_.size()/8;
    BYTE * bytes = new BYTE[count];
    auto it = bits_.begin();
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
BYTES Encoding::getBytes() const{
    int count = bits_.size()/8;
    BYTES bytes;
    auto it = bits_.begin();
    for(int i=0;i<count;i++){
        BYTE c = 0;
        for (int i=0; i < 8; i++){
            c += (*it << i);
            ++it;
        }
        bytes.push_back(c);
    }
    return bytes;
}
int Encoding::getSize() const{
    return bits_.size();
}
void Encoding::add(const BITS& b){
    bits_.insert(bits_.end(),b.begin(),b.end());
}
void Encoding::addToFront(const BITS& b){
    bits_.insert(bits_.begin(),b.begin(),b.end());
}
State Encoding::readState(){
    int state = readBits(8);
    bits_.erase(bits_.begin(),bits_.begin()+8);
    reset();
    text_ = getBinary();
    return (State)state;
}
BITS Encoding::convertToBits(string plainText){
    BITS binaryText;
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
BITS Encoding::convertToBits(BYTES plainText){
    BITS binaryText;
    int count = 0;
    for(auto it = plainText.begin();it!=plainText.end();++it){
        BYTE c = *it;
        for (int i = 0; i < 8; ++i) {
            binaryText.push_back((c>>i)&1);
        }
        count ++;
    }
    return binaryText;
}
BITS Encoding::convertToBits(int code, int bit_size){
    BITS bits;
    for(int i=0;i<bit_size;i++){
        bits.push_back(code%2);
        code/=2;
    }
    return bits;
}
string Encoding::convertToString(BITS bits){
    int count = 0;
    string text;

    while(count<bits.size()){

        BITS bitChar(8);
        copy(bits.begin()+count,bits.begin()+count+8,bitChar.begin());
        unsigned char c = 0;
        for (int i=0; i < 8; i++){
            c += (bitChar[i] << i);
        }
        text += c;
        count += 8;
    }
    return text;
}
BYTES Encoding::convertToBytes(BITS bits){
    int count = bits.size()/8;
    BYTES bytes;
    auto it = bits.begin();
    for(int i=0;i<count;i++){
        BYTE c = 0;
        for (int i=0; i < 8; i++){
            c += (*it << i);
            ++it;
        }
        bytes.push_back(c);
    }
    return bytes;
}
BYTE * Encoding::convertToBinary(BITS bits){
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
void Encoding::setBits(BITS encoding){
    bits_ = encoding;
}
//convert from index to binary bits
void Encoding::writeBits(int code,int bit_size){
    for(int i=0;i<bit_size;i++){
        bits_.push_back(code%2);
        code/=2;
    }
}
//convert a string of bits into an index
int Encoding::readBits(int bit_size){
    int acc = 0;
    for(int i=0;i<bit_size;i++){
        if(*it_ == 1){
            int pow2 = 1;
            for(int j=0;j<i;j++){
                pow2 *= 2;
            }
            acc+=pow2;
        }
        ++it_;
    }
    return acc;
}
int Encoding::getBinarySize(int max_size){
    int bitSize = 0;
    int iter=1;
    while(iter<=max_size){
        bitSize++;
        iter *=2;
    }
    return bitSize;
}
void Encoding::reset(){
    it_ = bits_.begin();
}
void Encoding::writeBinary(ofstream& os){
    text_ = convertToBinary(bits_);
    int size = bits_.size()/8;
    os.write(reinterpret_cast<const char*>(&text_[0]),size*sizeof(BYTE));
    os.close();
}

