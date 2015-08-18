#include "Decorator.h"

using namespace std;

Decorator::Decorator(TextComponent * component):component_(component){
    encoding_ = component_->getEncoding();
}

Decorator::~Decorator()
{
    //delete component_;
}
vector<bool> Decorator::encode(){
    return component_->encode();
}
void Decorator::print(ofstream& os){
    return component_->print(os);
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

//TextComponent * Decorator::getComponent() const{
//    //cout<<component_->getID()+" ";
//    return component_;
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
vector<bool> Decorator::getDecode(vector<bool> cipherCode){

}
