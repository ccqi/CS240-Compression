#include "Encoding.h"

using namespace std;

//helper function
char toHexChar(int i){
  i %= 16;
  if (i>=0 && i<10) {
    return i+48;
  }
  else {
    return i+55;
  }
}

//member function
Encoding::Encoding(){
    bits_ = BITS();
    it_ =bits_.begin();
    type_ = BINARY;
    text_ = NULL;
}
Encoding::Encoding(string data,TYPE type):type_(type){
    if(type==TEXT){
        text_ = new BYTE[data.size()];
        for(int i=0;i<data.size();i++){
            text_[i] = data[i];
        }
        bits_ = Encoding::convertToBits(data);
        reset();
    }
}
Encoding::Encoding(BITS data):bits_(data),type_(BINARY){
    reset();
    text_ = getBinary();

}
Encoding::Encoding(BYTES data):type_(BINARY){
    bits_ = convertToBits(data);
    reset();
    text_ = getBinary();

}
Encoding::~Encoding(){
}
BITS Encoding::getBits() const{
    return bits_;
}
BYTE * Encoding::getBinary() const{
    int count = bits_.size()/8;
    BYTE * bytes = new BYTE[count];
    auto it = bits_.begin();
    for(int i=0;i<count;i++){
        BYTE c = 0;
        for (int i=0; i < 8; i++){
            c += (*it << i);
            ++it;
        }
        bytes[i] = c;
    }
    return bytes;
}


BYTES Encoding::getBytes() const{
    int count = bits_.size()/8;
    BYTES bytes;
    auto it = bits_.begin();
    for(int i=0;i<count;i++){
        BYTE c = 0;
        for (int i=0; i < 8; i++){
            c += (*it << i);
            ++it;
        }
        bytes.push_back(c);
    }
    return bytes;
}
int Encoding::getSize() const{
    return bits_.size();
}

void Encoding::add(const BITS& b){
    bits_.insert(bits_.end(),b.begin(),b.end());
    BITS bits;
    bits.insert(bits.end(),b.begin(),b.end());
    data_.push_back(make_pair("data", bits));
}

void Encoding::add(string field, const BITS& b){
    bits_.insert(bits_.end(),b.begin(),b.end());
    BITS bits;
    bits.insert(bits.end(),b.begin(),b.end());
    data_.push_back(make_pair(field, bits));
}

void Encoding::add(bool b){
  bits_.push_back(b);
}

void Encoding::add(string field, bool b){
  bits_.push_back(b);
}

BITS Encoding::convertToBits(int code, int bit_size) {
  BITS bits;
  for (int i = 0; i < bit_size; i++) {
    bits.push_back(code % 2);
    code /= 2;
  }
  return bits;
}


void Encoding::addToFront(const BITS& b){
    bits_.insert(bits_.begin(),b.begin(),b.end());
    BITS bits; 
    bits.insert(bits.begin(),b.begin(),b.end());
    data_.push_front(make_pair("data", bits));
}
void Encoding::addToFront(string field, const BITS& b){
    bits_.insert(bits_.begin(),b.begin(),b.end());
    BITS bits;
    bits.insert(bits.begin(),b.begin(),b.end());
    data_.push_front(make_pair(field, bits));
}
State Encoding::readState(){
    int state = readBits(8);
    bits_.erase(bits_.begin(),bits_.begin()+8);
    reset();
    text_ = getBinary();
    return (State)state;
}
BITS Encoding::convertToBits(string plainText){
    BITS binaryText;
    int count = 0;
    for(auto it = plainText.begin();it!=plainText.end();++it){
        char c = *it;
        for (int i = 0; i < 8; ++i) {
            binaryText.push_back((c>>i)&1);
        }
        count ++;
    }
    return binaryText;
}
BITS Encoding::convertToBits(BYTES plainText){
    BITS binaryText;
    int count = 0;
    for(auto it = plainText.begin();it!=plainText.end();++it){
        BYTE c = *it;
        for (int i = 0; i < 8; ++i) {
            binaryText.push_back((c>>i)&1);
        }
        count ++;
    }
    return binaryText;
}

string Encoding::convertToString(BITS bits){
    int count = 0;
    string text;

    while(count<bits.size()){

        BITS bitChar(8);
        copy(bits.begin()+count,bits.begin()+count+8,bitChar.begin());
        unsigned char c = 0;
        for (int i=0; i < 8; i++){
            c += (bitChar[i] << i);
        }
        text += c;
        count += 8;
    }
    return text;
}

string Encoding::convertToText(BITS bits){
    int count = 0;
    string text;

    while(count<bits.size()){

        BITS bitChar(8);
        copy(bits.begin()+count,bits.begin()+count+8,bitChar.begin());
        unsigned char c = 0;
        for (int i=0; i < 8; i++){
            c += (bitChar[i] << i);
        }
        if (c > 31 && c < 127 ) {
          text += c;
        }
        else {
          stringstream ss;
          ss << "(" << (int)c << ")";
          text+=ss.str();
        }
        count += 8;
    }
    return text;
}

string Encoding::convertToText(string s){
    string text;
    for(int i = 0; i < s.size(); i++) {
      char c = s[i];
      if (c == 32) {
        text += "_";
      }
      else if (c > 31 && c < 127 ) {
        text += c;
      }
      else {
        stringstream ss;
        ss << "(" << (int)c << ")";
        text+=ss.str();
      }
    }
    return text;
}

string Encoding::convertToBinaryString(BITS bits){
  string binaryString;
  for(int i=0;i<bits.size();i++){
    string bit = (bits[i]) ? "1" : "0";
    binaryString += bit;
    
  }
  return binaryString;
}

BYTES Encoding::convertToBytes(BITS bits){
    int count = bits.size()/8;
    BYTES bytes;
    auto it = bits.begin();
    for(int i=0;i<count;i++){
        BYTE c = 0;
        for (int i=0; i < 8; i++){
            c += (*it << i);
            ++it;
        }
        bytes.push_back(c);
    }
    return bytes;
}
BYTE * Encoding::convertToBinary(BITS bits){
    int count = bits.size()/8;
    BYTE * bytes = new BYTE[count];
    auto it = bits.begin();
    for(int i=0;i<count;i++){
        BYTE c = 0;
        for (int i=0; i < 8; i++){
            c += (*it << i);
            ++it;
        }
        bytes[i] = c;
    }
    return bytes;
}

string Encoding::convertToHexString(BITS bits){
    int count = bits.size()/8;
    BYTE * bytes = new BYTE[count];
    auto it = bits.begin();
    string hexString;
    for(int i=0;i<count;i++){
        BYTE c2 = 0;
        for (int j = 0; j < 4; j++) {
            c2 += (*it << j);
            ++it;
        }
        BYTE c1 = 0;
        for (int j = 0; j < 4; j++) {
            c1 += (*it << j);
            ++it;
        }
        hexString+=toHexChar(c1);
        hexString+=toHexChar(c2);
        hexString+=" ";
    }
    return hexString;
}


void Encoding::setBits(BITS encoding){
    bits_ = encoding;
}

void Encoding::setFields(string field) {
  fields_.push_back(field);
}

//convert from index to binary bits
void Encoding::writeBits(int code,int bit_size){
    BITS bits;
    for(int i=0;i<bit_size;i++){
        bits_.push_back(code % 2);
        bits.push_back(code % 2);
        code/=2;
    }
    data_.push_back(make_pair("data", bits));
}

//convert from index to binary bits
void Encoding::writeBits(string field, int code,int bit_size){
    BITS bits;
    for(int i=0;i<bit_size;i++){
        bits_.push_back(code % 2);
        bits.push_back(code % 2);
        code/=2;
    } 
    data_.push_back(make_pair(field,bits));
}
//convert a string of bits into an index
int Encoding::readBits(int bit_size){
    int acc = 0;
    for(int i=0;i<bit_size;i++){
        if(*it_ == 1){
            int pow2 = 1;
            for(int j=0;j<i;j++){
                pow2 *= 2;
            }
            acc+=pow2;
        }
        ++it_;
    }
    return acc;
}

int Encoding::getBinarySize(int max_size){
    int bitSize = 0;
    int iter=1;
    while(iter<=max_size){
        bitSize++;
        iter *=2;
    }
    return bitSize;
}
void Encoding::reset(){
    it_ = bits_.begin();
}
double operator /(const Encoding& a,const Encoding& b){
    return (double)a.bits_.size()/(double)b.bits_.size();
}

void Encoding::writeBinary(ofstream& os){
    text_ = convertToBinary(bits_);
    int size = bits_.size()/8;
    os.write(reinterpret_cast<const char*>(&text_[0]),size*sizeof(BYTE));
    os.close();
}

vector<string> Encoding::getFields() const {
  return fields_;
}

pair<string,BITS> Encoding::get(int i) const{
  return data_[i];
}

int Encoding::size() const {
  return data_.size();
}
