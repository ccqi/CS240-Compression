#include "MTFEncoder.h"

using namespace std;


MTFEncoder::MTFEncoder(TextComponent * component):Decorator(component){
    id_ = MTF;
}

MTFEncoder::~MTFEncoder(){}


vector<bool> MTFEncoder::encode(){
    vector<bool> plainBits = Decorator::encode();
    string plainText = getString(plainBits);
    string alpha;
    for(int i=0;i<128;i++){
        alpha.push_back((char)i);
    }


//    priority_queue< char, vector<char>, greater<char> > pq;
//    for(auto it = plainText.begin();it!=plainText.end();++it){
//        pq.push(*it);
//    }
//    set<char> s;
//
//    while(!pq.empty()){
//        s.insert(pq.top());
//        pq.pop();
//    }
//    string alpha;
//    copy(s.begin(),s.end(),back_inserter(alpha));
//    alpha_ = alpha;

    //string encoding;
    for(int i=0;i<plainText.length();i++){
        char c = plainText[i];
        auto it = find_if(alpha.begin(),alpha.end(),[c](char ch)->bool{
                                return ch == c;
                            });
        int index = (std::distance(alpha.begin(),it));
        writeBits(encoding_,index,8);
        char first = alpha.front();
        string newAlpha = alpha;
        newAlpha.erase(remove_copy_if(alpha.begin(),alpha.end(),newAlpha.begin(),[c](char ch)->bool{
                                return ch == c;
                            }),newAlpha.end());
        stringstream ss;
        ss<<c<<newAlpha;
        alpha = ss.str();
    }
    assert(equal(plainBits.begin(),plainBits.end(),getDecode(encoding_).begin()));
    return encoding_;
}
vector<bool> MTFEncoder::getDecode(vector<bool> cipherCode){
    vector<bool> plainCode;
    string plainText;
    string cipherText = getString(cipherCode);

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
    return getBits(plainText);

}
void MTFEncoder::print(ofstream& os){
    Decorator::print(os);
    BYTE * binary= getBytes(encoding_);
    int size = encoding_.size()/8;
    os.write(reinterpret_cast<const char*>(&binary[0]),size*sizeof(BYTE));
    os.close();


    cout<<endl<<"After move to front encoding:"<<endl;
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
    cout<<"Length of bits: "<<encoding_.size()<<endl;
    double ratio = (double)encoding_.size()/getDecode(encoding_).size();
    cout<<"Compression ratio: "<<ratio<<endl;
}
