#include "TextWrapper.h"

using namespace std;

Nan::Persistent<v8::Function> TextWrapper::constructor;

TextWrapper::TextWrapper(){
  rleIndex_ = 0;
  symbolIndex_ = 128;
  outputIndex_ = 0;
  huffmanIndex_ = 0;
  dataIndex_ = 0;
  max_ = 50;
}

TextWrapper::~TextWrapper() {

}

void TextWrapper::setEncoding(Encoding * encoding) {
  component_ = new PlainText(encoding);
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
  Nan::SetPrototypeMethod(tpl, "get", Get);
  Nan::SetPrototypeMethod(tpl, "getData", GetData);
  Nan::SetPrototypeMethod(tpl, "getTable", GetTable);
  Nan::SetPrototypeMethod(tpl, "set", Set);
  Nan::SetPrototypeMethod(tpl, "write", Write);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("TextWrapper").ToLocalChecked(), tpl->GetFunction());
}

void TextWrapper::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new TextWrapper(...)`
    TextWrapper * wrapper = new TextWrapper();
    
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

void TextWrapper::Set(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // unwrap the object
  TextWrapper * wrapper = ObjectWrap::Unwrap<TextWrapper>(info.Holder());
  
  // get the plain text
  v8::String::Utf8Value param1(info[0]->ToString());
  std::string plainText = std::string(*param1);
  Encoding * encoding = new Encoding(plainText, TEXT);
  wrapper->setEncoding(encoding);
  
  info.GetReturnValue().Set(info.This());
}

void TextWrapper::Encode(const Nan::FunctionCallbackInfo<v8::Value>& args) {
  
  // convert argument to string
  v8::String::Utf8Value param1(args[0]->ToString());
  string type = string(*param1);

  // unwrap the object
  TextWrapper * wrapper = ObjectWrap::Unwrap<TextWrapper>(args.Holder());
  wrapper->component_ = setDecorator(type, wrapper->component_);
  
  // encode
  Encoding * encoding = wrapper->component_->encode(); 
  wrapper->rleIndex_ = 0;
  wrapper->symbolIndex_ = 128;
  wrapper->outputIndex_ = 0;
  wrapper->huffmanIndex_ = 0;
  wrapper->dataIndex_ = 0;
  args.GetReturnValue().Set(args.This());
}

void TextWrapper::Write(const Nan::FunctionCallbackInfo<v8::Value>& args) {
  
  if (args.Length() < 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }
  
  // convert argument to string
  v8::String::Utf8Value param1(args[0]->ToString());
  string filePath = string(*param1);
  // unwrap the object
  TextWrapper * wrapper = ObjectWrap::Unwrap<TextWrapper>(args.Holder());
  TextComponent * component = wrapper->component_;
  Encoding * encoding = component->getEncoding();

  std::ofstream myFile;
  myFile.open(filePath, ios::binary);
  encoding->writeBinary(myFile);
  
  args.GetReturnValue().Set(args.This());
}

void TextWrapper::Get(const Nan::FunctionCallbackInfo<v8::Value>& args) {
   
  // unwrap the object
  TextWrapper * wrapper = ObjectWrap::Unwrap<TextWrapper>(args.Holder());
  TextComponent * component = wrapper->component_;
  Encoding * encoding = component->getEncoding();
  
  // convert argument to string
  v8::String::Utf8Value param1(args[0]->ToString());
  string type = string(*param1);
  wrapper->max_ = args[1]->Int32Value();
  
  // convert result to json
  v8Object result = Nan::New<v8::Object>();
  vector<string> format = component->getFormat();
  result->Set(Nan::New("formats").ToLocalChecked(), getFormats(format));
  result->Set(Nan::New("data").ToLocalChecked(), formatData(type, encoding, component, wrapper));
  double percent = (double)wrapper->dataIndex_ / encoding->size();
  percent = (percent > 1) ? 1 : percent;
  result->Set(Nan::New("percent").ToLocalChecked(), Nan::New(percent));
  result->Set(Nan::New("compression_ratio").ToLocalChecked(), Nan::New(component->getCompressionRatio()));
  
  // get additional properties
  if (type == "LZW") {
    map<int, string> symbolTable = reinterpret_cast<LZWEncoder*>(component)->getTable();
    deque<tuple<string,int, BITS> > outputTable = reinterpret_cast<LZWEncoder*>(component)->getOutput();
    v8Object table = Nan::New<v8::Object>();
    table->Set(Nan::New("symbol").ToLocalChecked(), formatSymbolTable(symbolTable, wrapper));
    table->Set(Nan::New("output").ToLocalChecked(), formatOutputTable(outputTable, wrapper));
    result->Set(Nan::New("table").ToLocalChecked(), table);
  }
  else if (type == "RLE") {
    deque<tuple<string,BITS,BITS> > table = reinterpret_cast<RLEEncoder*>(component)->getTable();
    result->Set(Nan::New("table").ToLocalChecked(), formatRLETable(table, wrapper));
  }
  else if (type == "Huffman") {
    Trie * huffmanTrie = reinterpret_cast<HuffmanEncoder*>(component)->getHuffmanTrie();
    Trie * iter = huffmanTrie;
    result->Set(Nan::New("huffmanTrie").ToLocalChecked(), formatHuffmanTrie(iter));
  }

  // set return value to object
  args.GetReturnValue().Set(result);
}

void TextWrapper::GetData(const Nan::FunctionCallbackInfo<v8::Value>& args) {
  // unwrap the object
  TextWrapper * wrapper = ObjectWrap::Unwrap<TextWrapper>(args.Holder());
  TextComponent * component = wrapper->component_;
  Encoding * encoding = component->getEncoding();
  // convert argument to string
  v8::String::Utf8Value param1(args[0]->ToString());
  string type = string(*param1);
  
  v8Object result = Nan::New<v8::Object>();  
  result->Set(Nan::New("data").ToLocalChecked(), formatData(type, encoding, component, wrapper));
  double percent = (double)wrapper->dataIndex_ / encoding->size();
  percent = (percent > 1) ? 1 : percent;
  result->Set(Nan::New("percent").ToLocalChecked(), Nan::New(percent));
  // set return value to object
  args.GetReturnValue().Set(result);
}

void TextWrapper::GetTable(const Nan::FunctionCallbackInfo<v8::Value>& args) {
  // unwrap the object
  TextWrapper * wrapper = ObjectWrap::Unwrap<TextWrapper>(args.Holder());
  TextComponent * component = wrapper->component_;
  Encoding * encoding = component->getEncoding();
  
  // convert argument to string
  v8::String::Utf8Value param1(args[0]->ToString());
  string type = string(*param1);
  v8Object result;
  if (type == "symbol") {
    map<int, string> symbolTable = reinterpret_cast<LZWEncoder*>(component)->getTable();
    result = formatSymbolTable(symbolTable, wrapper);
  }
  if (type == "output") {
    deque<tuple<string,int, BITS> > outputTable = reinterpret_cast<LZWEncoder*>(component)->getOutput();
    result = formatOutputTable(outputTable, wrapper);
  }
  else if (type == "RLE") {
    deque<tuple<string,BITS,BITS> > table = reinterpret_cast<RLEEncoder*>(component)->getTable();
    result = formatRLETable(table, wrapper);
  }
  
  // set return value to object
  args.GetReturnValue().Set(result);
}

v8Array TextWrapper::getFormats(vector<string> formats) {
  v8Array fieldArray = Nan::New<v8::Array>();
  for(int i=0;i<formats.size();i++){
    fieldArray->Set(i, Nan::New(formats[i]).ToLocalChecked());
  }
  return fieldArray;
}
v8Array TextWrapper::formatData(string type, Encoding* encoding, TextComponent * component, TextWrapper * wrapper) {
  v8Array data = Nan::New<v8::Array>();
  deque<tuple<string,int, BITS> > outputTable;
  if (type == "LZW") {
      outputTable = reinterpret_cast<LZWEncoder*>(component)->getOutput();
  }
  int iter = 0;
  int dataMax = (wrapper->dataIndex_ + wrapper->max_ < encoding->size()) ? wrapper->dataIndex_ + wrapper->max_ : encoding->size();
  for (int i = wrapper->dataIndex_; i < dataMax; i++) {
    v8Object entry = Nan::New<v8::Object>();
    pair<string, BITS> entryData = encoding->get(i);
    stringstream ss;
    ss << entryData.first;
    string field = ss.str();
    BITS bits = entryData.second;
    string binary = Encoding::convertToBinaryString(bits);
    entry->Set(Nan::New("binary").ToLocalChecked(), Nan::New(binary).ToLocalChecked());
    if (type == "BWT" || type =="MTF") {
      string hex = Encoding::convertToHexString(bits);
      entry->Set(Nan::New("hex").ToLocalChecked(), Nan::New(hex).ToLocalChecked());
    }
    if (type == "BWT") {
      string text = Encoding::convertToText(bits);
      entry->Set(Nan::New("text").ToLocalChecked(), Nan::New(text).ToLocalChecked());
    }
    if (type == "LZW") {
      for(int i=0;i<outputTable.size();i++) {
        tuple<string, int, BITS> tuple = outputTable[i];
        if(binary == Encoding::convertToBinaryString(get<2>(tuple))) {
          int code = get<1>(tuple);
          entry->Set(Nan::New("Code").ToLocalChecked(), Nan::New(code));
          break;
        }
      }
    } 
    entry->Set(Nan::New("field").ToLocalChecked(), Nan::New(field).ToLocalChecked());
    data->Set(iter, entry);
    iter++;
  }
  wrapper->dataIndex_ = dataMax;
  
  return data;
}

v8Object TextWrapper::formatSymbolTable(map<int,string> sTable, TextWrapper * wrapper) {
  
  v8Object symbolTable = Nan::New<v8::Object>(); 
  v8Array sCols = Nan::New<v8::Array>();
  sCols->Set(0, Nan::New("Decimal").ToLocalChecked());
  sCols->Set(1, Nan::New("Symbol").ToLocalChecked());
  symbolTable->Set(Nan::New("columns").ToLocalChecked(), sCols);
  v8Array sList = Nan::New<v8::Array>();
  int i = 0;
  int lzwMax = (wrapper->symbolIndex_ + wrapper->max_ < sTable.size() + 128) ? wrapper->symbolIndex_ + wrapper->max_ : sTable.size() + 128;
  for (auto it = sTable.begin(); it!=sTable.end(); ++it) {
    if(it->first <= lzwMax && it->first > wrapper->symbolIndex_) {
      v8Object entry = Nan::New<v8::Object>();
      string s = Encoding::convertToText(it->second);
      entry->Set(Nan::New("Decimal").ToLocalChecked(), Nan::New(it->first));
      entry->Set(Nan::New("Symbol").ToLocalChecked(), Nan::New(s).ToLocalChecked());
      sList->Set(i, entry);
      i++;
    }
  }
  symbolTable->Set(Nan::New("entries").ToLocalChecked(), sList);
  wrapper->symbolIndex_ = lzwMax;
  double percent = (double)wrapper->symbolIndex_ / (sTable.size() + 128);
  percent = (percent > 1) ? 1 : percent;
  symbolTable->Set(Nan::New("percent").ToLocalChecked(), Nan::New(percent));
  return symbolTable;
}

v8Object TextWrapper::formatOutputTable(deque<tuple<string,int, BITS> > oTable, TextWrapper * wrapper) {
  
  // output table
  v8Object outputTable = Nan::New<v8::Object>(); 
  v8Array oCols = Nan::New<v8::Array>();
  oCols->Set(0, Nan::New("Binary").ToLocalChecked());
  oCols->Set(1, Nan::New("Code").ToLocalChecked());
  outputTable->Set(Nan::New("columns").ToLocalChecked(), oCols);
  v8Array oList = Nan::New<v8::Array>();
  int iter = 0;
  int lzwMax = (wrapper->outputIndex_ + wrapper->max_ < oTable.size()) ? wrapper->outputIndex_ + wrapper->max_ : oTable.size();
  for (int i = wrapper->outputIndex_; i < lzwMax; i++) {
    v8Object entry = Nan::New<v8::Object>();
    tuple<string, int, BITS> tuple = oTable[i];
    string key = get<0>(tuple);
    int dec = get<1>(tuple);
    string binString = Encoding::convertToBinaryString(get<2>(tuple));
    entry->Set(Nan::New("field").ToLocalChecked(), Nan::New(key).ToLocalChecked());
    entry->Set(Nan::New("Binary").ToLocalChecked(), Nan::New(binString).ToLocalChecked());
    entry->Set(Nan::New("Code").ToLocalChecked(), Nan::New(dec));
    oList->Set(iter, entry);
    iter++;
  }
  wrapper->outputIndex_ = lzwMax;
  double percent = (double)wrapper->outputIndex_ / oTable.size();
  percent = (percent > 1) ? 1 : percent;
  outputTable->Set(Nan::New("percent").ToLocalChecked(), Nan::New(percent));
  outputTable->Set(Nan::New("entries").ToLocalChecked(), oList);
  return outputTable;
}

v8Object TextWrapper::formatRLETable(deque<tuple<string,BITS,BITS> > table, TextWrapper * wrapper) {
  // output table
  v8Object rleTable = Nan::New<v8::Object>(); 
  v8Array cols = Nan::New<v8::Array>();
  
  cols->Set(0, Nan::New("Run").ToLocalChecked());
  cols->Set(1, Nan::New("RunLength").ToLocalChecked());
  rleTable->Set(Nan::New("columns").ToLocalChecked(), cols);
  v8Array entries = Nan::New<v8::Array>();
  int iter = 0;
  int rleMax = (wrapper->rleIndex_ + wrapper->max_ < table.size()) ? wrapper->rleIndex_ + wrapper->max_ : table.size();
  for (int i = wrapper->rleIndex_; i < rleMax; i++) {
    v8Object entry = Nan::New<v8::Object>();
    tuple<string, BITS, BITS> tuple = table[i];
    string key = get<0>(tuple);
    string run = Encoding::convertToBinaryString(get<1>(tuple));
    string runLength = Encoding::convertToBinaryString(get<2>(tuple));
    entry->Set(Nan::New("field").ToLocalChecked(), Nan::New(key).ToLocalChecked());
    entry->Set(Nan::New("Run").ToLocalChecked(), Nan::New(run).ToLocalChecked());
    entry->Set(Nan::New("RunLength").ToLocalChecked(), Nan::New(runLength).ToLocalChecked());
    entries->Set(iter, entry);
    iter++;
  }
  wrapper->rleIndex_ = rleMax;
  double percent = (double)wrapper->rleIndex_ / table.size();
  percent = (percent > 1) ? 1 : percent;
  rleTable->Set(Nan::New("percent").ToLocalChecked(), Nan::New(percent));
  rleTable->Set(Nan::New("entries").ToLocalChecked(), entries);
  return rleTable;
}

v8Object TextWrapper::formatHuffmanTrie(Trie * trie) {
  v8Object huffmanTrie = Nan::New<v8::Object>();
  if(trie->isLeaf){
    v8Object node = Nan::New<v8::Object>();
    stringstream ss;
    ss<< trie->c;
    string c = Encoding::convertToText(ss.str());
    node->Set(Nan::New("char").ToLocalChecked(), Nan::New(c).ToLocalChecked());
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
