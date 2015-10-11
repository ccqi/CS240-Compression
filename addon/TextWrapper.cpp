#include "TextWrapper.h"

using namespace std;

Nan::Persistent<v8::Function> TextWrapper::constructor;

TextWrapper::TextWrapper(Encoding * encoding){
  component_ = new PlainText(encoding);
}

TextWrapper::~TextWrapper() {

}

TextComponent * TextWrapper::setDecorator(string type, TextComponent * text){
  if (type == "Huffman") {
      return new HuffmanEncoder(text);
  }
  else if (type == "BWT") {
      return new BWTransform(text);
  }
  else if (type == "RLE") {
      return new RLEEncoder(text);
  }
  else if (type == "MTF") {
      return new MTFEncoder(text);
  }
  else if (type == "LZW") {
      return new LZWEncoder(text);
  }
  else {
      return text;
  }
}
void TextWrapper::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("TextWrapper").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "encode", Encode);
  Nan::SetPrototypeMethod(tpl, "decode", Decode);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("TextWrapper").ToLocalChecked(), tpl->GetFunction());
}

void TextWrapper::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new TextWrapper(...)`
    v8::String::Utf8Value param1(info[0]->ToString());
    std::string plainText = std::string(*param1);
    Encoding * encoding = new Encoding(plainText, TEXT);
    TextWrapper * wrapper = new TextWrapper(encoding);
    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = { info[0] };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void TextWrapper::Encode(const Nan::FunctionCallbackInfo<v8::Value>& args) {
  v8::String::Utf8Value param2(args[1]->ToString());
  std::string type = std::string(*param2);
  TextWrapper * wrapper = ObjectWrap::Unwrap<TextWrapper>(args.Holder());
  
  wrapper->component_ = setDecorator(type, wrapper->component_);
  Encoding * encoding = wrapper->component_->encode();
  string cipherText = Encoding::convertToString(encoding->getBits());
  
  v8::Local<v8::String> jsText = Nan::New(cipherText).ToLocalChecked();
  args.GetReturnValue().Set(jsText);
}

void TextWrapper::Decode(const Nan::FunctionCallbackInfo<v8::Value>& args) {
  //MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());
  //obj->value_ += 1;
  //info.GetReturnValue().Set(Nan::New(obj->value_));
}
