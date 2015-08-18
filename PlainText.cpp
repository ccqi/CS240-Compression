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
    cout<<"Original file length: "<<encoding_.size()<<endl;
    plainText_ = getString(encoding_);
    os<<plainText_;
    os.close();
}
TextComponent * PlainText::decode(){
    return this;
}
//void PlainText::recurseDecode(State id){
//    cerr<<"The string has not been encoded with this algorithm."<<endl;
//}
State PlainText::getID() const{
    return NONE;
}
//TextComponent * PlainText::getComponent(){
//
//}
vector<bool> PlainText::getDecode(vector<bool> cipherCode){
    return cipherCode;
}

