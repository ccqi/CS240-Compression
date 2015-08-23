#include "BWTransform.h"

using namespace std;

BWTransform::BWTransform(TextComponent * component):Decorator(component){
    id_ = BWT;
}

BWTransform::~BWTransform(){}

vector<bool> BWTransform::encode(){
    vector<bool> plainCode = Decorator::encode();
    string plainText = getString(plainCode);
    plainText = plainText;
    //cout<<plainText<<endl;
    int size = plainText.size()+1;
    //cout<<(int)plainText[plainText.size()];
    vector<string> shifts;
    string oldShift = plainText;
    for(int i=0;i<size;i++){
        string curShift;
        curShift.push_back(oldShift[size-1]);
        for(int j=0;j<size-1;j++){
            curShift.push_back(oldShift[j]);
        }
        shifts.push_back(curShift);
        oldShift = curShift;
    }
    sort(shifts.begin(),shifts.end());
    string encoding;
    for(int i=0;i<size;i++){
        encoding+=shifts[i][size - 1];
    }

    encoding_ = getBits(encoding);
    vector<bool> test = getDecode(encoding_);
    assert(equal(plainCode.begin(),plainCode.end(),getDecode(encoding_).begin()));
    return encoding_;

}
vector<bool> BWTransform::getDecode(vector<bool> cipherCode){
    string cipherText = getString(cipherCode);
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

    string pt;
    int j = distance(cipherText.begin(),find(cipherText.begin(),cipherText.end(),'\0'));
    while(true){
        j = N[j];
        pt +=cipherText[j];
        if(cipherText[j]=='\0')
            break;
    }
    return getBits(pt);
}


void BWTransform::print(ofstream& os){
    Decorator::print(os);
    cout<<endl<<"After bwt encoding:"<<endl;
    cout<<"Length of bits: "<<encoding_.size()<<endl;
    string text = getString(encoding_);

    cout<<text<<endl;
    os<<text;

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
//TextComponent * BWTransform::getComponent(){
//    return Decorator::getComponent();
//}
