#include "PlainText.h"

using namespace std;

PlainText::PlainText(string text):plainText_(text){
    id_ = NONE;
    encoding_ = getBits(text);
}

PlainText::PlainText(vector<bool> code){
    id_ = NONE;
    plainText_ = getString(code);
    encoding_ = code;
}
PlainText::~PlainText(){}

vector<bool> PlainText::encode(){
    return encoding_;
}

void PlainText::print(ofstream& os){
    TextComponent::print(os);
    cout<<"Original file length: "<<encoding_.size()<<endl;

}
TextComponent * PlainText::decode(){
    return this;
}
State PlainText::getID() const{
    return NONE;
}
vector<bool> PlainText::getDecode(vector<bool> cipherCode){
    return cipherCode;
}

