#include "BWTransform.h"

using namespace std;

BWTransform::BWTransform(TextComponent * component):Decorator(component){
    id_ = BWT;
}

BWTransform::~BWTransform(){}

Encoding * BWTransform::encode(){
    //encoding_ = Decorator::encode();
    Encoding * originalEncoding = Decorator::encode();

     //plainCode = Decorator::encode();
    BYTES bytes = originalEncoding->getBytes();
    int size = bytes.size()+1;
    //cout<<"Size"<<size<<endl;
//    for(int i=0;i<size-1;i++){
//        plainText.push_back(bytes[i]);
//    }
    bytes.push_back((BYTE)0);
    //cout<<(int)plainText[plainText.size()];
    vector<BYTES> shifts;
    BYTES oldShift = bytes;
    for(int i=0;i<size;i++){
        BYTES curShift;
        curShift.push_back(oldShift[size-1]);
        for(int j=0;j<size-1;j++){
            curShift.push_back(oldShift[j]);
        }
        shifts.push_back(curShift);
        oldShift = curShift;
    }
    sort(shifts.begin(),shifts.end());

    encoding_->writeBits(id_,8);
    for(int i=0;i<size;i++){
        //encoding.push_back(shifts[i][size - 1]);
        //cout<<(int)(shifts[i][size-1])<<" ";
        encoding_->writeBits((int)(shifts[i][size-1]),8);
    }
//    for(int i=0;i<encoding_->getBytes().size();i++){
//        cout<<(int)encoding_->getBytes()[i]<<" ";
//    }
//    cout<<endl;
    //encoding_ = getBits(encoding);
    //Encoding test = getDecode(encoding_);
    //assert(equal(plainCode.begin(),plainCode.end(),getDecode(encoding_).begin()));
    return encoding_;

}
Encoding * BWTransform::getDecode(Encoding * encoding){
    BYTES bytes = encoding->getBytes();
    int size = encoding->getSize()/8;
    //cout<<"Size:"<<size<<endl;
    BYTES cipherText;
    //cout<<"Decode"<<endl;
    for(int i=0;i<size;i++){
        //cout<<(int)bytes[i]<<" ";
        cipherText.push_back(bytes[i]);
    }
    vector< pair<char,int> > A;
    for(int i=0;i<cipherText.size();i++){
        pair<char,int> curPair(cipherText[i],i);
        A.push_back(curPair);
    }
    sort(A.begin(),A.end());
    vector<int> N;
    for(auto it = A.begin();it!=A.end();++it){
        N.push_back(get<1>(*it));
    }

    BYTES pt;
    int j = distance(cipherText.begin(),find(cipherText.begin(),cipherText.end(),'\0'));
    while(true){
        j = N[j];
        if(cipherText[j]=='\0')
            break;
        pt.push_back(cipherText[j]);
    }
//    cout<<"Byte:";
//    for(int i=0;i<pt.size();i++){
//        cout<<(char)pt[i];
//    }
//    cout<<endl;
    return new Encoding(pt);
}


void BWTransform::print(ofstream& os){
    Decorator::print(os);

//    string text = getString(encoding_);
//
//    cout<<text<<endl;
//    BYTE * binary= encoding_.getBinary(encoding_);
//    int size = encoding_.size()/8;
//
//    os.write(reinterpret_cast<const char*>(&binary[0]),size*sizeof(BYTE));
//    os.close();

    cout<<endl<<"After bwt encoding:"<<endl;
    cout<<"Length of bits: "<<encoding_->getSize()<<endl;
//    cout<<"New string: "<<text<<endl;
//    cout<<"New binary: ";
//    for(auto it = text.begin();it!=text.end();++it){
//        cout<<(int)*it<<" ";
//    }
//    cout<<endl;
//    for(auto it = text.begin();it!=text.end();++it){
//        cout<<*it;
//    }
//    cout<<endl;
    //os<<text;
    //copy(text.begin(),text.end(),ostreambuf_iterator<char>(os));
    //cout<<endl;

}
