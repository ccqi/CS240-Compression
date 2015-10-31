#include "RLEEncoder.h"

using namespace std;

RLEEncoder::RLEEncoder(TextComponent * component):Decorator(component){
    id_ = RLE;
}

RLEEncoder::~RLEEncoder(){}

Encoding * RLEEncoder::encode(){
    originalEncoding_ = Decorator::encode();
    BITS plainBits = originalEncoding_->getBits();
    BITS cipherBits;
    if(plainBits.size()==0){
        return NULL;
    }
    encoding_->writeBits(plainBits[0],1);
    bool curBit = plainBits[0];
    int blockLength = 1;
    for(auto it = plainBits.begin()+1;it!=plainBits.end();++it){
        BITS run;
        if(*it == *(it-1) && (it != plainBits.end()-1)){
            blockLength++;
        }
        else{
            if(it==plainBits.end()-1){
                blockLength++;
            }
            BITS run;
            for(int i=0;i<blockLength;i++){
              run.push_back(curBit); 
            }
            
            int temp = blockLength;
            BITS binLength;
            while(temp>0){
                binLength.push_back(temp%2);
                temp/=2;
            }

            BITS runLength;
            for(int i=0;i<binLength.size()-1;i++){
                encoding_->writeBits(0,1);
                runLength.push_back(0);
            }

            while(!binLength.empty()){
                encoding_->writeBits(binLength.back(),1);
                runLength.push_back(binLength.back());
                binLength.pop_back();
            }

            runTable_[run] = runLength;
            runOrder_.push_back(run);
            curBit = !curBit;
            blockLength = 1;
        }
    }
    //pad a non-multiple of 8 with zeros
    int bitsize =encoding_->getSize();
    int padding = 0;
    if(bitsize % 8 > 0){
        padding = 8 - bitsize % 8;
        for(int i=0;i<padding;i++){
            encoding_->writeBits(0,1);
        }
    }
    encoding_->addToFront(Encoding::convertToBits(padding,8));
    encoding_->addToFront("header",Encoding::convertToBits(id_,8));
    
    encoding_->setFields("header");
    encoding_->setFields("data");
    //Encoding * test = getDecode(encoding_);
    //  assert(equal(plainBits.begin(),plainBits.end(),getDecode(encoding_).begin()));
    compressionRatio_ = *encoding_ / *originalEncoding_;
    return encoding_;
}

map<BITS,BITS> RLEEncoder::getTable() const {
  return runTable_;
}

vector<BITS> RLEEncoder::getRunOrder() const {
  return runOrder_;
}
void RLEEncoder::print(ofstream& os){
    Decorator::print(os);
    cout<<endl<<"RLE ENCODING:"<<endl;
    cout<<"Length of input: "<<originalEncoding_->getSize()<<endl;
    cout<<"Length of encoding: "<<encoding_->getSize()<<endl;
    cout<<"Compression ratio: "<<compressionRatio_<<endl;

    //copy(encoding_.begin(),encoding_.end(),ostream_iterator<bool>(cout));
//    double ratio = (double)encoding_.size()/getDecode(encoding_).size();
//    cout<<"Compression ratio: "<<ratio<<endl;
}
Encoding * RLEEncoder::getDecode(Encoding * encoding){

    BITS plainCode;
    BITS cipherCode = encoding->getBits();
    if(cipherCode.size()==0){
        //throw("no string to decode");
        return NULL;
    }
    //read padding
    BITS first(8);
    copy(cipherCode.begin(),cipherCode.begin()+8,first.begin());
    BYTE * padding = Encoding::convertToBinary(first);

    bool curBit = cipherCode[8];
    auto it = cipherCode.begin()+9;

    int blockLength = 0;
    while(it!=cipherCode.end()-*padding){

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
    return new Encoding(plainCode);
}

