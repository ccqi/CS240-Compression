#include "BWTransform.h"

using namespace std;

BWTransform::BWTransform(TextComponent * component):Decorator(component){
    id_ = BWT;
    format_.push_back("binary");
    format_.push_back("hex");
    format_.push_back("text");
}

BWTransform::~BWTransform(){}

Encoding * BWTransform::encode(){
    originalEncoding_ = Decorator::encode();
    BYTES bytes = originalEncoding_->getBytes();
    int size = bytes.size() + 1;
    bytes.push_back((BYTE)0);
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
    encoding_->writeBits("header",id_,8);
    for(int i=0;i<size;i++){
        stringstream ss;
        ss << "bwt";
        if (i > 0) {
          ss<<"_"<<(i+1);
        }
        encoding_->writeBits(ss.str(),(int)(shifts[i][size-1]),8);
    }
    //Encoding test = getDecode(encoding_);
    //assert(equal(plainCode.begin(),plainCode.end(),getDecode(encoding_).begin()));
    compressionRatio_ = *encoding_ / *originalEncoding_;
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

    cout<<endl<<"BWT ENCODING:"<<endl;
    cout<<"Length of input: "<<originalEncoding_->getSize()<<endl;
    cout<<"Length of encoding: "<<encoding_->getSize()<<endl;
    cout<<"Compression ratio: "<<compressionRatio_<<endl;
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
