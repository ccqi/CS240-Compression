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
            writeBits(bits,code,getBinarySize(table.size()-1));

            table[w+key] = table.size()-1;
            w = key;
       }
       else{
            w = w+key;
       }
    }
    writeBits(bits,table[w],getBinarySize(table.size()-1));

    //write stop bites
    writeBits(bits,0,getBinarySize(table.size()-1));

    //pad a non-multiple of 8 with zeros
    int bitsize =bits.size();
    if(bitsize % 8 > 0){
        int padding = 8 - bitsize % 8;
        for(int i=0;i<padding;i++){
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

    int prev_code = readBits(cipherCode,it,getBinarySize(table.size()-1));
    int first_code = prev_code;
    plainText+=table[prev_code];
    int cur_code;
    while(cur_code = readBits(cipherCode,it,getBinarySize(table.size()))){
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
    Decorator::print(os);
//    BYTE * binary= getBytes(encoding_);
//    int size = encoding_.size()/8;
//
//    os.write(reinterpret_cast<const char*>(&binary[0]),size*sizeof(BYTE));
//    os.close();

    cout<<endl<<"After LZW encoding:"<<endl;
    cout<<"Length of bits: "<<encoding_.size()<<endl;
    double ratio = (double)encoding_.size()/getDecode(encoding_).size();
    cout<<"Compression ratio: "<<ratio<<endl;

}
