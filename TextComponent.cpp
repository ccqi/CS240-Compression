#include "TextComponent.h"

using namespace std;

TextComponent::TextComponent(){
    //id_ = NONE;
}

TextComponent::~TextComponent(){
    delete encoding_;
}

//void TextComponent::setEncoding(BITS encoding){
//    encoding_ = encoding;
//}
//void TextComponent::setEncoding(string encoding){
//    encoding_ = getBits(encoding);
//}
void TextComponent::setEncoding(Encoding * encoding){
    encoding_ = encoding;
}
Encoding * TextComponent::getEncoding() const{
    return encoding_;
}
////convert from index to binary bits
//void TextComponent::writeBits(BITS& bits, int code,int bit_size){
//    for(int i=0;i<bit_size;i++){
//        bits.push_back(code%2);
//        code/=2;
//    }
//}
//
////convert a string of bits into an index
//int TextComponent::readBits(BITS bits, BITS::iterator& it,int bit_size){
//    int acc = 0;
//    for(int i=0;i<bit_size;i++){
//        if(*it == 1){
//            int pow2 = 1;
//            for(int j=0;j<i;j++){
//                pow2 *= 2;
//            }
//            acc+=pow2;
//        }
//        ++it;
//    }
//    return acc;
//}
//int TextComponent::getBinarySize(int max_size){
//    int bitSize = 0;
//    int iter=1;
//    while(iter<=max_size){
//        bitSize++;
//        iter *=2;
//    }
//    return bitSize;
//}
void TextComponent::print(ofstream& os){
    BYTE * binary= encoding_->getBinary();
    int size = encoding_->getSize()/8;

    os.write(reinterpret_cast<const char*>(&binary[0]),size*sizeof(BYTE));
    os.close();
}
