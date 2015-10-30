#include "PlainText.h"

using namespace std;

PlainText::PlainText(Encoding * encoding){
    id_ = PLAIN;
    encoding_ = encoding;
    BITS type = Encoding::convertToBits(id_,8);
    //encoding_->set("type", type);
    //encoding_->set("data", encoding_->getBits());
    encoding_->addToFront(type);
    compressionRatio_ = 1.0;
}

PlainText::~PlainText(){}

Encoding * PlainText::encode(){
    return encoding_;
}

void PlainText::print(ofstream& os){
    cout<<"Original file length: "<<encoding_->getSize()<<endl;
}
TextComponent * PlainText::decode(){
    return this;
}
State PlainText::getID() const {
    return NONE;
}
Encoding * PlainText::getDecode(Encoding * cipherCode){
    return cipherCode;
}
