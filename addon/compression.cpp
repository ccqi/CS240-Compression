#include <nan.h>
#include <string>
#include "TextWrapper.h"

using namespace std;
using namespace v8;

/*void Encode (const Nan::FunctionCallbackInfo<v8::Value>& args) {
  
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string plainText = std::string(*param1);
  
  TextComponent * text;
  BITS whole_data;
  Encoding * encoding;
  encoding = new Encoding(plainText, TEXT);
  text = new PlainText(encoding);
  text = new BWTransform(text);
  Encoding * curEncoding = text->encode();
  string cipherText = Encoding::convertToString(curEncoding->getBits());

  v8::Local<v8::String> jsText = Nan::New(cipherText).ToLocalChecked();
  args.GetReturnValue().Set(jsText);
}*/

/*void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("encode").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Encode)->GetFunction());
}*/

void Init(v8::Local<v8::Object> exports) {
  TextWrapper::Init(exports);
}

NODE_MODULE(compression, Init)
