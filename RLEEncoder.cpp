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

    encoding_.push_back(plainBits[0]);
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
            vector<bool> binLength;
            while(temp>0){
                binLength.push_back(temp%2);
                temp/=2;
            }
            for(int i=0;i<binLength.size()-1;i++){
                encoding_.push_back(false);
            }
            while(!binLength.empty()){
                encoding_.push_back(binLength.back());
                binLength.pop_back();
            }
            blockLength = 1;
        }
    }
    //pad a non-multiple of 8 with zeros
    BYTE bitsize =encoding_.size();
    int padding = 0;
    if(bitsize % 8 > 0){
        padding = 8 - bitsize % 8;
        for(int i=0;i<padding;i++){
            encoding_.push_back(false);
        }
    }
    stringstream ss;
    ss<<(char)padding;
    vector<bool> bits = getBits(ss.str());
    bits.insert(bits.end(),encoding_.begin(),encoding_.end());
    encoding_ = bits;


    //encoding_ = cipherBits;
    //  assert(equal(plainBits.begin(),plainBits.end(),getDecode(encoding_).begin()));
    return encoding_;
}
void RLEEncoder::print(ofstream& os){
    Decorator::print(os);

    BYTE * binary= getBytes(encoding_);
    int size = encoding_.size()/8;

    os.write(reinterpret_cast<const char*>(&binary[0]),size*sizeof(BYTE));
    os.close();

    cout<<endl<<"After Run Length Encoding:";
    //copy(encoding_.begin(),encoding_.end(),ostream_iterator<bool>(cout));
    cout<<endl<<"Length of new encoding: "<<encoding_.size()<<endl;
    double ratio = (double)encoding_.size()/getDecode(encoding_).size();
    cout<<"Compression ratio: "<<ratio<<endl;
}
vector<bool> RLEEncoder::getDecode(vector<bool> cipherCode){

    vector<bool> plainCode;
    if(cipherCode.size()==0){
        throw("no string to decode");
    }
    //read padding
    vector<bool> first(8);
    copy(cipherCode.begin(),cipherCode.begin()+8,first.begin());
    BYTE * padding = getBytes(first);

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
    return plainCode;
}

