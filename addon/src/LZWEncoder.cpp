#include "LZWEncoder.h"

using namespace std;

LZWEncoder::LZWEncoder(TextComponent * component_):Decorator(component_){
    id_ = LZW;
}

LZWEncoder::~LZWEncoder(){
}
Encoding * LZWEncoder::encode(){

    originalEncoding_ = Decorator::encode();
    BITS plainBits = originalEncoding_->getBits();
    string plainText = Encoding::convertToString(plainBits);

    //table of ASCII values 0-127
    map<string,int> table;
    for(int i=0;i<128;i++){
        stringstream ss;
        ss<<(char)i;
        table[ss.str()]=i;
    }

    encoding_->writeBits(id_,8);

    string w;
    //BITS bits;
    for(int i=0;i<plainText.size();i++){
       stringstream ss;
       ss<<(char)plainText[i];
       string key = ss.str();

       if(table.find(w+key)== table.end()){
            int code = table[w];
            encoding_->writeBits(code,Encoding::getBinarySize(table.size()-1));

            table[w+key] = table.size()-1;
            w = key;
       }
       else{
            w = w+key;
       }
    }
    encoding_->writeBits(table[w],Encoding::getBinarySize(table.size()-1));

    //write stop bites
    encoding_->writeBits(0,Encoding::getBinarySize(table.size()-1));

    //pad a non-multiple of 8 with zeros
    int bitsize =encoding_->getSize();
    if(bitsize % 8 > 0){
        int padding = 8 - bitsize % 8;
        for(int i=0;i<padding;i++){
            encoding_->writeBits(0,1);
        }
    }
    //encoding_->reset();
    //encoding_ = bits;

    //check invariant
    //Encoding test = getDecode(encoding_);
    //assert(equal(plainBits.begin(),plainBits.end(),getDecode(encoding_).begin()));
    compressionRatio_ = *encoding_ / *originalEncoding_;
    return encoding_;
}

Encoding * LZWEncoder::getDecode(Encoding * encoding){

    if(encoding->getSize()==0){
        //throw new string("No code to decode");
        return NULL;
    }
    //table of ASCII values 0-127
    vector<string> table;
    for(int i=0;i<128;i++){
        stringstream ss;
        ss<<(char)i;
        table.push_back(ss.str());
    }

    string plainText;
    //auto it = cipherCode.begin();

    int prev_code = encoding->readBits(Encoding::getBinarySize(table.size()-1));
    int first_code = prev_code;
    plainText+=table[prev_code];
    int cur_code;
    while(cur_code = encoding->readBits(Encoding::getBinarySize(table.size()))){
        string decode;
        if(cur_code>=table.size()){
            decode = table[prev_code];
            decode += first_code;
        }
        else{
            decode = table[cur_code];
        }
        plainText+=decode;
        first_code = decode[0];
        stringstream ss;
        ss<<table[prev_code]<<table[first_code];
        table.push_back(ss.str());
        prev_code = cur_code;
    }
    return new Encoding(plainText,TEXT);
}

void LZWEncoder::print(ofstream& os){
    Decorator::print(os);

    cout<<endl<<"LZW ENCODING:"<<endl;
    cout<<"Length of input: "<<originalEncoding_->getSize()<<endl;
    cout<<"Length of encoding: "<<encoding_->getSize()<<endl;
    cout<<"Compression ratio: "<<compressionRatio_<<endl;
    //double ratio = (double)encoding_.size()/getDecode(encoding_).size();
    //cout<<"Compression ratio: "<<ratio<<endl;

}
