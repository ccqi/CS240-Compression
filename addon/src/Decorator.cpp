#include "Decorator.h"

using namespace std;

Decorator::Decorator(TextComponent * component):component_(component),compressionRatio_(1.0){
    //encoding_ = component_->getEncoding();
    encoding_ = new Encoding();
}

Decorator::~Decorator()
{
    //delete component_;
}
Encoding * Decorator::encode(){
    return component_->encode();
}
void Decorator::print(ofstream& os){
    TextComponent::print(os);
}
TextComponent * Decorator::decode(){
    TextComponent * childRef = component_;
    childRef->setEncoding(this->getDecode(encoding_));
    component_ = NULL;
    delete this;
    return childRef;
}

//TextComponent * Decorator::decode(State id){
//    if(this->getID() == id){
//        TextComponent * childRef = component_;
//        childRef->setEncoding(this->getDecode(encoding_));
//        component_ = NULL;
//        delete this;
//        return childRef;
//    }
//    else{
//        recurseDecode(id);
//        return this;
//    }
//}

//void Decorator::recurseDecode(State id){
//    if(component_->getID() == id){
//        TextComponent * childRef = component_;
//        //childRef = this->getDecode(cipherText_);
//        component_ = childRef -> getComponent();
//        this->encode();
//        delete childRef;
//    }
//    else{
//        component_->recurseDecode(id);
//    }
//    return;
//
//}
State Decorator::getID() const{
    return id_;
}
Encoding * Decorator::getDecode(Encoding * cipherCode){

}
