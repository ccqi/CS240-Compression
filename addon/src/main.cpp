#include <iostream>
#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <vector>
#include <deque>
#include <bitset>
#include "Decorator.h"
#include "TextComponent.h"
#include "PlainText.h"
#include "MTFEncoder.h"
#include "HuffmanEncoder.h"
#include "RLEEncoder.h"
#include "BWTransform.h"
#include "LZWEncoder.h"
#include "Encoding.h"
using namespace std;


State convertState( string opStr ) {
    switch( opStr[0] ) {
    case 'H':
    case 'h':
        return HUFFMAN;
    case 'B':
    case 'b':
        return BWT;
    case 'R':
    case 'r':
        return RLE;
    case 'M':
    case 'm':
        return MTF;
    case 'D':
    case 'd':
        return DECODE;
    case 'E':
    case 'e':
        return ENCODE;
    case 'l':
    case 'L':
        return LZW;
    case 'Q':
    case 'q':
        return QUIT;
    default:{
        cerr << "Invalid operation " << opStr << endl;
        return NONE;
    }
    }
}
TextComponent * setDecorator(State state,TextComponent * text){
switch(state){
    case HUFFMAN: {
        return new HuffmanEncoder(text);
    }
    case BWT: {
        return new BWTransform(text);
    }
    case RLE: {
        return new RLEEncoder(text);
    }
    case MTF: {
        return new MTFEncoder(text);
    }
    case LZW: {
        return new LZWEncoder(text);
    }
    default: {
        return text;
    }
}
}

int main(int argc, char * argv[]){
    cout << "CS240 Compression Simulation with decorator pattern:" << endl << endl;

    //read from command line or input
    TextComponent * text;
    BITS whole_data;
    string plainText;
    Encoding * encoding;
    string command;
    State op;
    if(argc ==1){

        cout << "Please enter your string to be encoded: ";
        getline(cin,plainText);
        encoding = new Encoding(plainText,TEXT);
        op = ENCODE;
    }
    else{
        ifstream file(argv[1], ios::binary);
        char buffer;
        while (file.read(&buffer,1)){
            int byte = (unsigned char)buffer;
            for (int i = 0; i < 8; i++){
                whole_data.push_back(((byte >> i) & 1) != 0);
            }
        }
        encoding = new Encoding(whole_data);
        cout<<"Do you wish to encode[e] or decode[d]: ";
        cin >> command;
        op = convertState(command);
    }
    text = new PlainText(encoding);

    //make output file
    std::ofstream myFile;
    string filename;
    if(argc >= 3){
        filename = argv[2];
    }
    else{
        filename = "output.bin";
    }
    text->print(myFile);

    //get input from command line

    while(!cin.eof()&& op!=QUIT){
        if(op == ENCODE){
            cout<<"[b = burrows wheeler transform, h = huffman encoding, l = lzw compression, m = move to front r = run length encoding, q= quit ]"
            <<endl<<"Enter actions seperated by spaces: ";
            cin >> command;
            op = convertState(command);
            while ( op!=QUIT) {
                text = setDecorator(op,text);
                Encoding * encoding = text->encode();
                myFile.open(filename,ios::binary);
                encoding->writeBinary(myFile);
                text->print(myFile);
                cout<<"[b = burrows wheeler transform, h = huffman encoding, l = lzw compression, m = move to front r = run length encoding, q= quit ]"
                <<endl<<"Enter actions seperated by spaces: ";
                cin>>command;
                op = convertState(command);
            }
            cout<<endl;
            break;
        }
        if(op == DECODE){
            Encoding * curEncoding = encoding;
            op = curEncoding->readState();
            //cout<<op<<endl;
            while(op!=PLAIN){
                myFile.open(filename,ios::binary);
                text = setDecorator(op,text);
                text->setEncoding(curEncoding);
                text = text->decode();
                curEncoding = text->getEncoding();
                op = curEncoding->readState();
                curEncoding->writeBinary(myFile);
            }
            break;
        }
        cout<<"Do you wish to encode[e] or decode[d]: ";
        cin >> command;
        op = convertState(command);
    } // while cin OK
    myFile.close();
    return 0;
}
