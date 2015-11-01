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

void TextWrapper::Init(v8Object exports) {
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
  
  // convert argument to string
  v8::String::Utf8Value param2(args[1]->ToString());
  string type = string(*param2);

  // unwrap the object
  TextWrapper * wrapper = ObjectWrap::Unwrap<TextWrapper>(args.Holder());
  wrapper->component_ = setDecorator(type, wrapper->component_);
  
  TextComponent * component = wrapper->component_;
  
  // encode
  Encoding * encoding = component->encode();
  
  // convert result to json
  v8Object result = Nan::New<v8::Object>();
  vector<string> fields = encoding->getFields();
  result->Set(Nan::New("fields").ToLocalChecked(), getFields(fields));
  result->Set(Nan::New("data").ToLocalChecked(), getData(type,encoding, fields));
  result->Set(Nan::New("compression_ratio").ToLocalChecked(), Nan::New(component->getCompressionRatio()));
  // get additional properties
  if (type == "LZW") {
    map<int, string> table = reinterpret_cast<LZWEncoder*>(component)->getTable();
    result->Set(Nan::New("table").ToLocalChecked(), formatLZWTable(table));
  }
  else if (type == "RLE") {
    map<BITS,BITS> table = reinterpret_cast<RLEEncoder*>(component)->getTable();
    vector<BITS> order = reinterpret_cast<RLEEncoder*>(component)->getRunOrder();
    result->Set(Nan::New("table").ToLocalChecked(), formatRLETable(table, order));
  }
  else if (type == "Huffman") {
    Trie * huffmanTrie = reinterpret_cast<HuffmanEncoder*>(component)->getHuffmanTrie();
    Trie * iter = huffmanTrie;
    result->Set(Nan::New("huffmanTrie").ToLocalChecked(), formatHuffmanTrie(iter));
  }

  // set return value to object
  args.GetReturnValue().Set(result);
}

v8Array TextWrapper::getFields(vector<string> fields) {
  v8Array fieldArray = Nan::New<v8::Array>();
  for(int i=0;i<fields.size();i++){
    fieldArray->Set(i, Nan::New(fields[i]).ToLocalChecked());
  }
  return fieldArray;
}
v8Object TextWrapper::getData(string type, Encoding* encoding, vector<string> fields) {
  v8Object entry = Nan::New<v8::Object>();
  for (auto it = fields.begin();it!=fields.end();++it) {
    string field = *it;
    v8Object subentry = Nan::New<v8::Object>();
    
    BITS bits = encoding->get(field);
    string binary = Encoding::convertToBinaryString(bits);
    string hex = Encoding::convertToHexString(bits);
    subentry->Set(Nan::New("binary").ToLocalChecked(), Nan::New(binary).ToLocalChecked());
    subentry->Set(Nan::New("hex").ToLocalChecked(), Nan::New(hex).ToLocalChecked());
    if (type == "BWT") {
      string text = Encoding::convertToText(bits);
      subentry->Set(Nan::New("text").ToLocalChecked(), Nan::New(text).ToLocalChecked());
    }
    entry->Set(Nan::New(field).ToLocalChecked(), subentry);
  }
  return entry;
}

v8Array TextWrapper::formatLZWTable(map<int,string> table) {
  v8Array list = Nan::New<v8::Array>();
  int i = 0;
  for (auto it = table.begin(); it!=table.end(); ++it) {
    v8Object entry = Nan::New<v8::Object>();
    string s = it->second;
    entry->Set(Nan::New("index").ToLocalChecked(), Nan::New(it->first));
    entry->Set(Nan::New("value").ToLocalChecked(), Nan::New(it->second).ToLocalChecked());
    list->Set(i, entry);
    i++;
  }
  return list;
}

v8Array TextWrapper::formatRLETable(map<BITS,BITS> table, vector<BITS> order) {
  v8Array list = Nan::New<v8::Array>();
  int i = 0;
  for (auto it = order.begin(); it!=order.end(); ++it) {
    v8Object entry = Nan::New<v8::Object>();
    string run = Encoding::convertToBinaryString(*it);
    string runLength = Encoding::convertToBinaryString(table[*it]);
    entry->Set(Nan::New("run").ToLocalChecked(), Nan::New(run).ToLocalChecked());
    entry->Set(Nan::New("runLength").ToLocalChecked(), Nan::New(runLength).ToLocalChecked());
    list->Set(i, entry);
    i++;
  }
  return list;
}

v8Object TextWrapper::formatHuffmanTrie(Trie * trie) {
  v8Object huffmanTrie = Nan::New<v8::Object>();
  if(trie->isLeaf){
    v8Object node = Nan::New<v8::Object>();
    stringstream ss;
    ss<< trie->c;
    node->Set(Nan::New("char").ToLocalChecked(), Nan::New(ss.str()).ToLocalChecked());
    node->Set(Nan::New("freq").ToLocalChecked(), Nan::New(trie->freq));
    return node;
  }
  huffmanTrie->Set(Nan::New("freq").ToLocalChecked(), Nan::New(trie->freq));
  huffmanTrie->Set(Nan::New("zero").ToLocalChecked(), formatHuffmanTrie(trie->zero));
  huffmanTrie->Set(Nan::New("one").ToLocalChecked(), formatHuffmanTrie(trie->one));
  return huffmanTrie;
}
void TextWrapper::Decode(const Nan::FunctionCallbackInfo<v8::Value>& args) {

}
