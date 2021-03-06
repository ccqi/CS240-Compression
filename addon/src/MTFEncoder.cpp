#include "MTFEncoder.h"

using namespace std;


MTFEncoder::MTFEncoder(TextComponent * component):Decorator(component){
    id_ = MTF;
    format_.push_back("binary");
    format_.push_back("hex");
}

MTFEncoder::MTFEncoder(BITS bits):Decorator(bits){
    id_ = MTF;
    format_.push_back("binary");
    format_.push_back("hex");
    encoding_->writeBits("header",id_,8);
    string cipherText = Encoding::convertToString(bits);
    for (int i = 0; i < cipherText.size(); i++) {
      stringstream key;
      key << "mtf";
      if (i > 0) {
        key << "_" << (i + 1);
      }
      encoding_->writeBits(key.str(),cipherText[i],8);
    }
}

MTFEncoder::~MTFEncoder(){}


Encoding * MTFEncoder::encode(){
    originalEncoding_ = Decorator::encode();
    BITS plainBits = originalEncoding_->getBits();
    string plainText = Encoding::convertToString(plainBits);
    string alpha;
    for(int i=0;i<128;i++){
        alpha.push_back((char)i);
    }
    encoding_->writeBits("header",id_,8);
    for(int i=0;i<plainText.length();i++){
        char c = plainText[i];
        auto it = find_if(alpha.begin(),alpha.end(),[c](char ch)->bool{
                                return ch == c;
                            });
        int index = (std::distance(alpha.begin(),it));
        stringstream key;
        key << "mtf";
        if (i > 0) {
          key << "_" << (i + 1);
        }
        encoding_->writeBits(key.str(),index,8);
        char first = alpha.front();
        string newAlpha = alpha;
        newAlpha.erase(remove_copy_if(alpha.begin(),alpha.end(),newAlpha.begin(),[c](char ch)->bool{
                                return ch == c;
                            }),newAlpha.end());
        stringstream ss;
        ss<<c<<newAlpha;
        alpha = ss.str();
    }
    Encoding * test =getDecode(encoding_);
    //assert(equal(plainBits.begin(),plainBits.end(),getDecode(encoding_).begin()));
    compressionRatio_ = *encoding_ / *originalEncoding_;
    return encoding_;
}
Encoding * MTFEncoder::getDecode(Encoding * encoding){
    BITS cipherCode = encoding->getBits();
    BITS plainCode;
    string plainText;
    string cipherText = Encoding::convertToString(cipherCode);

    //table of ASCII values 0-127
    string alpha;
    for(int i=0;i<128;i++){
        alpha.push_back((char)i);
    }
    for(int i=0;i<cipherText.length();i++){
        int pos = cipherText[i];

        char foundChar = alpha[pos];
        plainText += foundChar;
        string newAlpha = alpha;
        newAlpha.erase(remove_copy_if(alpha.begin(),alpha.end(),newAlpha.begin(),[foundChar](char ch)->bool{
                                return ch == foundChar;
                            }),newAlpha.end());
        stringstream ss;
        ss<<foundChar<<newAlpha;
        alpha = ss.str();
    }
    return new Encoding(plainText,TEXT);

}
void MTFEncoder::print(ofstream& os){
    Decorator::print(os);
    cout<<endl<<"MTF ENCODING:"<<endl;
    cout<<"Length of input: "<<originalEncoding_->getSize()<<endl;
    cout<<"Length of encoding: "<<encoding_->getSize()<<endl;
    cout<<"Compression ratio: "<<compressionRatio_<<endl;
//    string text = getString(encoding_);
//    cout<<"Bytes: ";
//    for(auto it = text.begin();it!=text.end();++it){
//        cout<<(int)*it<<" ";
//    }
//    cout<<endl;
//    cout<<"New encoding: ";
//    for(auto it = encoding_.begin();it!=encoding_.end();++it){
//        cout<<*it;
//    }
//    cout<<endl;
//    double ratio = (double)encoding_.size()/getDecode(encoding_).size();
//    cout<<"Compression ratio: "<<ratio<<endl;
}
