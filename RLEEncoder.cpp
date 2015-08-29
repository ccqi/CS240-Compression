#include "RLEEncoder.h"

using namespace std;

RLEEncoder::RLEEncoder(TextComponent * component):Decorator(component){
    id_ = RLE;
}

RLEEncoder::~RLEEncoder(){}

Encoding * RLEEncoder::encode(){
    Encoding * originalEncoding = Decorator::encode();
    BITS plainBits = originalEncoding->getBits();
    BITS cipherBits;
    if(plainBits.size()==0){
        throw("no string to encode");
    }
    encoding_->writeBits(plainBits[0],1);
    int blockLength = 1;
    for(auto it = plainBits.begin()+1;it!=plainBits.end();++it){

        if(*it == *(it-1) && (it != plainBits.end()-1)){
            blockLength++;
        }
        else{
            if(it==plainBits.end()-1){
                blockLength++;
            }
            int temp = blockLength;
            BITS binLength;
            while(temp>0){
                binLength.push_back(temp%2);
                temp/=2;
            }
            for(int i=0;i<binLength.size()-1;i++){
                encoding_->writeBits(0,1);
            }
            while(!binLength.empty()){
                encoding_->writeBits(binLength.back(),1);
                binLength.pop_back();
            }
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
    //stringstream ss;
    //ss<<(char)padding;
    encoding_->addToFront(Encoding::convertToBits(padding,8));
    encoding_->addToFront(Encoding::convertToBits(id_,8));

//    bits.insert(bits.end(),paddingBits.begin(),paddingBits.end());
//    encoding encodingBits = encoding_->getBits();
//    bits.insert(bits.end(),encodingBits.begin(),encodingBits.end());
//    encoding_ = new Encoding(bits);
    //Encoding * test = getDecode(encoding_);

    //encoding_ = cipherBits;
    //  assert(equal(plainBits.begin(),plainBits.end(),getDecode(encoding_).begin()));
    return encoding_;
}
void RLEEncoder::print(ofstream& os){
    Decorator::print(os);

//    BYTE * binary= getBytes(encoding_);
//    int size = encoding_.size()/8;
//
//    os.write(reinterpret_cast<const char*>(&binary[0]),size*sizeof(BYTE));
//    os.close();

    cout<<endl<<"After Run Length Encoding:";
    //copy(encoding_.begin(),encoding_.end(),ostream_iterator<bool>(cout));
    cout<<endl<<"Length of new encoding: "<<encoding_->getSize()<<endl;
//    double ratio = (double)encoding_.size()/getDecode(encoding_).size();
//    cout<<"Compression ratio: "<<ratio<<endl;
}
Encoding * RLEEncoder::getDecode(Encoding * encoding){

    BITS plainCode;
    BITS cipherCode = encoding->getBits();
    if(cipherCode.size()==0){
        throw("no string to decode");
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

