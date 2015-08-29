#include "PlainText.h"

using namespace std;

//PlainText::PlainText(string text):plainText_(text){
//    id_ = NONE;
//    encoding_ = Encoding::convertToBits(text)getBits(text);
//}
//
//PlainText::PlainText(BITS code){
//    id_ = NONE;
//    plainText_ = getString(code);
//    encoding_ = code;
//}
PlainText::PlainText(Encoding * encoding){
    id_ = PLAIN;
    encoding_ = encoding;
    encoding_->addToFront(Encoding::convertToBits(id_,8));
//    BITS bits = Encoding::convertToBits(id_,8);
//    BITS encodingBits = encoding->getBits();
//    bits.insert(bits.end(),encodingBits.begin(),encodingBits.end());
//    encoding_ = new Encoding(bits);
}

PlainText::~PlainText(){}

Encoding * PlainText::encode(){
    return encoding_;
}

void PlainText::print(ofstream& os){
    TextComponent::print(os);
    cout<<"Original file length: "<<encoding_->getSize()<<endl;

}
TextComponent * PlainText::decode(){
    return this;
}
State PlainText::getID() const{
    return NONE;
}
Encoding * PlainText::getDecode(Encoding * cipherCode){
    return cipherCode;
}

