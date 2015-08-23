#include "LZWEncoder.h"

using namespace std;





LZWEncoder::LZWEncoder(TextComponent * component_):Decorator(component_){
    id_ = LZW;
}

LZWEncoder::~LZWEncoder(){
}
vector<bool> LZWEncoder::encode(){

    vector<bool> plainBits = Decorator::encode();
    string plainText = getString(plainBits);

    //table of ASCII values 0-127
    map<string,int> table;
    for(int i=0;i<128;i++){
        stringstream ss;
        ss<<(char)i;
        table[ss.str()]=i;
    }
    string w;
    vector<bool> bits;
    for(int i=0;i<plainText.size();i++){
       stringstream ss;
       ss<<(char)plainText[i];
       string key = ss.str();

       if(table.find(w+key)== table.end()){
            int code = table[w];
            writeBits(bits,code,table.size()-1);

            table[w+key] = table.size()-1;
            w = key;
       }
       else{
            w = w+key;
       }
    }
    writeBits(bits,table[w],table.size()-1);

    //write stop bites
    writeBits(bits,0,table.size()-1);

    //pad a non-multiple of 8 with zeros
    int bitsize =bits.size();
    if(bitsize % 8 > 0){
        for(int i=0;i<(8-bitsize%8);i++){
            bits.push_back(false);
        }
    }
    encoding_ = bits;

    //check invariant
    assert(equal(plainBits.begin(),plainBits.end(),getDecode(encoding_).begin()));
    return encoding_;
}

vector<bool> LZWEncoder::getDecode(vector<bool> cipherCode){

    if(cipherCode.size()==0){
        throw new string("No code to decode");
    }
    //table of ASCII values 0-127
    vector<string> table;
    for(int i=0;i<128;i++){
        stringstream ss;
        ss<<(char)i;
        table.push_back(ss.str());
    }

    string plainText;
    auto it = cipherCode.begin();

    int prev_code = readBits(cipherCode,it,table.size()-1);
    int first_code = prev_code;
    plainText+=table[prev_code];
    int cur_code;
    while(cur_code = readBits(cipherCode,it,table.size())){
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
    return getBits(plainText);
}

void LZWEncoder::print(ofstream& os){
    //Decorator::print(os);
    BYTE * binary= getBytes(encoding_);
    int size = encoding_.size()/8;

    os.write(reinterpret_cast<const char*>(&binary[0]),size*sizeof(BYTE));
    os.close();

    cout<<endl<<"After LZW encoding:"<<endl;
    cout<<"Length of bits: "<<encoding_.size()<<endl;
    double ratio = (double)encoding_.size()/getDecode(encoding_).size();
    cout<<"Compression ratio: "<<ratio<<endl;

}


//convert from index to binary bits
void LZWEncoder::writeBits(vector<bool>& bits, int code,int max_size){
    int bitSize = getBinarySize(max_size);
    for(int i=0;i<bitSize;i++){
        bits.push_back(code%2);
        code/=2;
    }
}

//convert a string of bits into an index
int LZWEncoder::readBits(vector<bool> bits, vector<bool>::iterator& it,int max_size){

    int bit_size = getBinarySize(max_size);
    int acc = 0;
    for(int i=0;i<bit_size;i++){
        if(*it == 1){
            int pow2 = 1;
            for(int j=0;j<i;j++){
                pow2 *= 2;
            }
            acc+=pow2;
        }
        ++it;
    }
    return acc;
}
int LZWEncoder::getBinarySize(int max_size){
    int bitSize = 0;
    int iter=1;
    while(iter<=max_size){
        bitSize++;
        iter *=2;
    }
    return bitSize;
}
