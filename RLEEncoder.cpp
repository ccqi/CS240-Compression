#include "RLEEncoder.h"

using namespace std;

RLEEncoder::RLEEncoder(TextComponent * component):Decorator(component){
    id_ = RLE;
}

RLEEncoder::~RLEEncoder(){}

vector<bool> RLEEncoder::encode(){
    vector<bool> plainBits = Decorator::encode();
    vector<bool> cipherBits;
    if(plainBits.size()==0){
        throw("no string to encode");
    }

    cipherBits.push_back(plainBits[0]);
    int blockLength = 1;

//    copy(plainBits.begin(),plainBits.end(),ostream_iterator<bool>(cout));
//    cout<<endl;

    for(auto it = plainBits.begin()+1;it!=plainBits.end();++it){

        if(*it == *(it-1) && (it != plainBits.end()-1)){
            blockLength++;
        }
        else{
            if(it==plainBits.end()-1){
                blockLength++;
            }
            int temp = blockLength;
            vector<bool> binLength;
            while(temp>0){
                binLength.push_back(temp%2);
                temp/=2;
            }
            for(int i=0;i<binLength.size()-1;i++){
                cipherBits.push_back(false);
            }
            while(!binLength.empty()){
                cipherBits.push_back(binLength.back());
                binLength.pop_back();
            }
            blockLength = 1;
        }
    }

    encoding_ = cipherBits;
    assert(equal(plainBits.begin(),plainBits.end(),getDecode(encoding_).begin()));
    return cipherBits;
}
void RLEEncoder::print(ofstream& os){
    Decorator::print(os);
    cout<<endl<<"Run Length Encoding Cipher Text:"<<endl;
    copy(encoding_.begin(),encoding_.end(),ostream_iterator<bool>(cout));
    cout<<endl<<"Length of new encoding: "<<encoding_.size()<<endl;
}
//TextComponent * RLEEncoder::getComponent(){
//    return Decorator::getComponent();
//}
vector<bool> RLEEncoder::getDecode(vector<bool> cipherCode){

    vector<bool> plainCode;
    if(cipherCode.size()==0){
        throw("no string to decode");
    }

    bool curBit = cipherCode[0];

    auto it = cipherCode.begin()+1;

    int blockLength = 0;
    while(it!=cipherCode.end()){

        while(*it==0){
            blockLength++;
            it++;
        }
        int strLength = 0;

        for(int i=0;i<blockLength+1;i++){

            if(*it == 1){
                int pow2 = 1;
                for(int j=0;j<blockLength-i;j++){
                    pow2 *= 2;
                }
                strLength+=pow2;
            }
            ++it;
        }
        for(int i=0;i<strLength;i++){
            plainCode.push_back(curBit);
        }
        curBit = !curBit;
        blockLength = 0;

    }
    return plainCode;
}

