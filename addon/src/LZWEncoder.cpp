#include "LZWEncoder.h"

using namespace std;

LZWEncoder::LZWEncoder(TextComponent * component_):Decorator(component_){
    id_ = LZW;
    format_.push_back("binary");
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
    
    encoding_->writeBits("header", id_, 8);
    string w;
    int counter = 0;
    for(int i=0;i<plainText.size();i++){
       stringstream ss;
       ss<<(char)plainText[i];
       string key = ss.str();

       if(table.find(w+key)== table.end()){
            int code = table[w];
            BITS binCode = Encoding::convertToBits(code, Encoding::getBinarySize(table.size()-1));
            stringstream ss;
            ss << "lzw";
            if (counter > 0) {
              ss << "_" << (counter + 1);
            }
            encoding_->add(ss.str(), binCode);
            output_.push_back(make_tuple(ss.str(),code, binCode));
            table[w+key] = table.size()-1;
            w = key;
            counter++;
       }
       else{
            w = w+key;
       }
    }
    BITS binCode = Encoding::convertToBits(table[w], Encoding::getBinarySize(table.size()-1));
    stringstream ss;
    ss << "lzw";
    if (counter > 0) {
      ss << "_" << (counter + 1);
    }
    encoding_->add(ss.str(),binCode);
    output_.push_back(make_tuple(ss.str(),table[w], binCode));

    //write stop bites
    binCode = Encoding::convertToBits(0, Encoding::getBinarySize(table.size()-1));
    encoding_->add("stop", binCode);
    output_.push_back(make_tuple("stop", 0, binCode));

    //pad a non-multiple of 8 with zeros
    int bitsize =encoding_->getSize();
    cout<<"size"<<bitsize<<endl;
    if(bitsize % 8 > 0){
        int padding = 8 - bitsize % 8;
        for(int i=0;i<padding;i++){
            encoding_->writeBits("padding",0,1);
        }
    }
    table_ = table;
    compressionRatio_ = *encoding_ / *originalEncoding_;
    return encoding_;
}

map<int, string> LZWEncoder::getTable() const {
  map<int, string> extendedTable;
  for(auto it = table_.begin(); it!=table_.end(); ++it) {
    if (it->second > 127) {
      extendedTable[it->second] = it->first;
    }
  }
  return extendedTable;
}

deque<tuple<string, int, BITS> > LZWEncoder::getOutput() const {
  return output_;
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
