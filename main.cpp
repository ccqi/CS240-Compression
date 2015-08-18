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


int main(int argc, char * argv[]){
    cout << "CS240 Compression Simulation with decorator pattern:" << endl << endl;

    //read from command line or input
    TextComponent * text;
    if(argc ==1){
        string plainText;
        cout << "Please enter your string to be encoded: ";
        getline(cin,plainText);
        text = new PlainText(plainText);
    }
    else{

        vector<bool> whole_data;
        ifstream file(argv[1], ios::binary);
        char buffer;
        while (file.read(&buffer,1)){
            int byte = (unsigned char)buffer;
            for (int i = 0; i < 8; i++){
                whole_data.push_back(((byte >> i) & 1) != 0);
            }
        }
        text = new PlainText(whole_data);
    }

    //make output file
    string filename ="output.bin";

    if(argc == 3){
        filename = argv[2];
    }
    ofstream myFile(filename,ios::binary);
    //myFile.open(filename);
    //text->print(myFile);

    //get input from command line
    cout<<"[m = move to front, b = burrows wheeler transform, h = huffman encoding, r = run length encoding, l = lzw compression p = print, d = decode, q= quit ]"<<endl<<"Enter actions seperated by spaces: ";
    string command;
    cin >> command;

    State op = convertState(command);

    while ( !cin.eof() ) {
        switch ( op ) {
            case HUFFMAN: {
                TextComponent * huffman = new HuffmanEncoder(text);
                text = huffman;
                break;
            }
            case BWT: {
                TextComponent * bwt = new BWTransform(text);
                text = bwt;
                break;
            }
            case RLE: {
                TextComponent * rle = new RLEEncoder(text);
                text = rle;
                break;
            }
            case MTF: {
                TextComponent * mtf = new MTFEncoder(text);
                text = mtf;
                break;
            }
            case LZW: {
                TextComponent * lzw = new LZWEncoder(text);
                text = lzw;
                break;
            }
//            case PRINT: {
//                text->print(myFile);
//                break;
//            }
            case ENCODE: {
                vector<bool> encoding = text->encode();
                text->print(myFile);
                break;
            }
            case DECODE: {
//                cout<<"What algorithm do you want to decode: [m = move to front, b = burrows wheeler transform, h = huffman encoding, r = run length encoding, l = lzw compression]";
//                cout<<endl;
//                string command2;
//                cin >> command2;
//                State op2;
//                op2 = convertState(command2);
                text = text->decode();
                text->print(myFile);
                break;
            }
            case QUIT:
                break;
            default: {
                break;
            }
        } // switch
        if(op == QUIT){
            break;
        }
        cout<<endl<<"[m = move to front, b = burrows wheeler transform, h = huffman encoding, r = run length encoding, l = lzw compression, p = print, d = decode, q = quit ]"<<endl<<"Enter Action: ";
        cin >> command;
        op = convertState(command);

    } // while cin OK
    myFile.close();
    return 0;
}
