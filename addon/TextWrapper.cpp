#include "TextWrapper.h"

using namespace std;

Nan::Persistent<v8::Function> TextWrapper::constructor;

TextWrapper::TextWrapper(){
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

TextComponent * TextWrapper::parseEncoding(string type, BITS bits){
  if (type == "Huffman") {
      return new HuffmanEncoder(bits);
  }
  else if (type == "BWT") {
      return new BWTransform(bits);
  }
  else if (type == "RLE") {
      return new RLEEncoder(bits);
  }
  else if (type == "MTF") {
      return new MTFEncoder(bits);
  }
  else if (type == "LZW" || type =="symbol" || type == "output") {
      return new LZWEncoder(bits);
  }
  else {
      return NULL;
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
  
  // path
  v8::String::Utf8Value param1(args[0]->ToString());
  string filePath = string(*param1);
  
  // method
  v8::String::Utf8Value param2(args[1]->ToString());
  string type = string(*param2);
  
  // get max values from config
  v8Object arg1  = args[2]->ToObject();
  int maxData = arg1->Get(Nan::New("data").ToLocalChecked())->Int32Value();
  maxData = (maxData == 0) ? 50: maxData;
  int maxTree = arg1->Get(Nan::New("tree").ToLocalChecked())->Int32Value();
  maxTree = (maxTree == 0) ? 3: maxTree;
  int maxTable = arg1->Get(Nan::New("table").ToLocalChecked())->Int32Value();
  maxTable = (maxTable == 0) ? 50: maxTable;
  

  // input type
  v8::String::Utf8Value param4(args[3]->ToString());
  std::string inputType = std::string(*param4);
  
  // content
  v8::String::Utf8Value param5(args[4]->ToString());
  std::string content = std::string(*param5);
  
  TextWrapper * wrapper = ObjectWrap::Unwrap<TextWrapper>(args.Holder());
  Encoding * pt;
  
  if (inputType == "TEXT") {
    pt = new Encoding(content, TEXT);
  } else {
    stringstream path;
    path << "uploads/" << content;
    ifstream file(path.str(), ios::binary);
    BITS whole_data;
    char buffer;
    while (file.read(&buffer,1)){
      int byte = (unsigned char)buffer;
      for (int i = 0; i < 8; i++){
        whole_data.push_back(((byte >> i) & 1) != 0);
      }
    }
    pt = new Encoding(whole_data); 
  }
  
  wrapper->setEncoding(pt);
  wrapper->component_ = setDecorator(type, wrapper->component_);
  TextComponent * component = wrapper->component_; 
   
  // encode
  Encoding * encoding = wrapper->component_->encode();  
  // write to file
  std::ofstream myFile;
  myFile.open(filePath, ios::binary);
  encoding->writeBinary(myFile);
  
  // convert result to json
  v8Object result = Nan::New<v8::Object>();
  vector<string> format = component->getFormat();
  result->Set(Nan::New("formats").ToLocalChecked(), getFormats(format));
  result->Set(Nan::New("data").ToLocalChecked(), formatData(type, encoding, component, 0, maxData));
  double percent = (double)maxData / encoding->size();
  percent = (percent > 1) ? 1 : percent;
  result->Set(Nan::New("percent").ToLocalChecked(), Nan::New(percent));
  result->Set(Nan::New("compression_ratio").ToLocalChecked(), Nan::New(component->getCompressionRatio()));
  
  // get additional properties
  if (type == "LZW") {
    map<int, string> symbolTable = reinterpret_cast<LZWEncoder*>(component)->getTable();
    deque<tuple<string,int, BITS> > outputTable = reinterpret_cast<LZWEncoder*>(component)->getOutput();
    v8Object table = Nan::New<v8::Object>();
    table->Set(Nan::New("symbol").ToLocalChecked(), formatSymbolTable(symbolTable, 128, maxTable));
    table->Set(Nan::New("output").ToLocalChecked(), formatOutputTable(outputTable, 0, maxTable));
    result->Set(Nan::New("table").ToLocalChecked(), table);
  }
  else if (type == "RLE") {
    deque<tuple<string,BITS,BITS> > table = reinterpret_cast<RLEEncoder*>(component)->getTable();
    result->Set(Nan::New("table").ToLocalChecked(), formatRLETable(table, 0, maxTable));
  }
  else if (type == "Huffman") {
    Trie * huffmanTrie = reinterpret_cast<HuffmanEncoder*>(component)->getHuffmanTrie();
    int depth = reinterpret_cast<HuffmanEncoder*>(component)->getTreeDepth();
    double percentage = (depth == 0) ? 0: (double)maxTree / depth;
    Trie * iter = huffmanTrie;
    v8Object huffmanTree = formatHuffmanTrie(iter, 0, maxTree);
    result->Set(Nan::New("treePercentage").ToLocalChecked(), Nan::New(percentage));
    result->Set(Nan::New("huffmanTrie").ToLocalChecked(), huffmanTree);
  }

  // set return value to object
  args.GetReturnValue().Set(result);
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
    
  // convert argument to string
  //v8::String::Utf8Value param1(args[0]->ToString());
  //string filename = string(*param1);
  
  //v8::String::Utf8Value param2(args[1]->ToString());
  //string type = string(*param2);
}

void TextWrapper::GetData(const Nan::FunctionCallbackInfo<v8::Value>& args) {
  
  // convert argument to string
  v8::String::Utf8Value param1(args[0]->ToString());
  string filename = string(*param1);
  v8::String::Utf8Value param2(args[1]->ToString());
  string type = string(*param2);
  // get max value
  int start  = args[2]->Int32Value();
  int increment  = args[3]->Int32Value();
  if (increment == 0) {
    increment = 50;
  }
  
  // parse encoding from file
  TextWrapper * wrapper = ObjectWrap::Unwrap<TextWrapper>(args.Holder()); 
  ifstream file(filename, ios::binary);
  BITS whole_data;
  char buffer;
  while (file.read(&buffer,1)){
    int byte = (unsigned char)buffer;
    for (int i = 0; i < 8; i++){
      whole_data.push_back(((byte >> i) & 1) != 0);
    }
  }
  TextComponent * component = parseEncoding(type, std::vector<bool>(whole_data.begin()+8, whole_data.end()));
  Encoding * encoding = component->getEncoding();
 
  // make json object
  v8Object result = Nan::New<v8::Object>();  
  result->Set(Nan::New("data").ToLocalChecked(), formatData(type, encoding, component, start, increment));
  double percent = (double)(start + increment)  / encoding->size();
  percent = (percent > 1) ? 1 : percent;
  result->Set(Nan::New("percent").ToLocalChecked(), Nan::New(percent));
  // set return value to object
  args.GetReturnValue().Set(result);
}

void TextWrapper::GetTable(const Nan::FunctionCallbackInfo<v8::Value>& args) {
  
   
  // convert argument to string
  v8::String::Utf8Value param1(args[0]->ToString());
  string filename = string(*param1);

  v8::String::Utf8Value param2(args[1]->ToString());
  string type = string(*param2);

  int start  = args[2]->Int32Value();
  int increment  = args[3]->Int32Value();
  if (increment == 0) {
    increment = 50;
  }
  
  // parse encoding from file
  TextWrapper * wrapper = ObjectWrap::Unwrap<TextWrapper>(args.Holder()); 
  ifstream file(filename, ios::binary);
  BITS whole_data;
  char buffer;
  while (file.read(&buffer,1)){
    int byte = (unsigned char)buffer;
    for (int i = 0; i < 8; i++){
      whole_data.push_back(((byte >> i) & 1) != 0);
    }
  }
  TextComponent * component = parseEncoding(type, std::vector<bool>(whole_data.begin()+8, whole_data.end()));
  Encoding * encoding = component->getEncoding();

  v8Object result;
  
  if (type == "symbol") {
    map<int, string> symbolTable = reinterpret_cast<LZWEncoder*>(component)->getTable();
    result = formatSymbolTable(symbolTable, start + 128, increment);
  }
  if (type == "output") {
    deque<tuple<string,int, BITS> > outputTable = reinterpret_cast<LZWEncoder*>(component)->getOutput();
    result = formatOutputTable(outputTable, start, increment);
  }
  else if (type == "RLE") {
    deque<tuple<string,BITS,BITS> > table = reinterpret_cast<RLEEncoder*>(component)->getTable();
    result = formatRLETable(table, start, increment);
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
v8Array TextWrapper::formatData(string type, Encoding* encoding, TextComponent * component, int start, int increment) {
  v8Array data = Nan::New<v8::Array>();
  deque<tuple<string,int, BITS> > outputTable;
  if (type == "LZW") {
      outputTable = reinterpret_cast<LZWEncoder*>(component)->getOutput();
  }
  int iter = 0;
  int dataMax = ((start + increment) < encoding->size()) ? (start + increment) : encoding->size();
  for (int i = start; i < dataMax; i++) {
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
  
  return data;
}

v8Object TextWrapper::formatSymbolTable(map<int,string> sTable, int start, int increment) {
  
  v8Object symbolTable = Nan::New<v8::Object>(); 
  v8Array sCols = Nan::New<v8::Array>();
  sCols->Set(0, Nan::New("Decimal").ToLocalChecked());
  sCols->Set(1, Nan::New("Symbol").ToLocalChecked());
  symbolTable->Set(Nan::New("columns").ToLocalChecked(), sCols);
  v8Array sList = Nan::New<v8::Array>();
  int i = 0;
  int lzwMax = ((start + increment) < sTable.size() + 128) ? (start + increment) : (sTable.size() + 128);
  for (auto it = sTable.begin(); it!=sTable.end(); ++it) {
    if(it->first <= lzwMax && it->first > start) {
      v8Object entry = Nan::New<v8::Object>();
      string s = Encoding::convertToText(it->second);
      entry->Set(Nan::New("Decimal").ToLocalChecked(), Nan::New(it->first));
      entry->Set(Nan::New("Symbol").ToLocalChecked(), Nan::New(s).ToLocalChecked());
      sList->Set(i, entry);
      i++;
    }
  }
  symbolTable->Set(Nan::New("entries").ToLocalChecked(), sList);
  double percent = (double)lzwMax / (sTable.size() + 128);
  percent = (percent > 1) ? 1 : percent;
  symbolTable->Set(Nan::New("percent").ToLocalChecked(), Nan::New(percent));
  return symbolTable;
}

v8Object TextWrapper::formatOutputTable(deque<tuple<string,int, BITS> > oTable, int start, int increment) {
  
  // output table
  v8Object outputTable = Nan::New<v8::Object>(); 
  v8Array oCols = Nan::New<v8::Array>();
  oCols->Set(0, Nan::New("Binary").ToLocalChecked());
  oCols->Set(1, Nan::New("Code").ToLocalChecked());
  outputTable->Set(Nan::New("columns").ToLocalChecked(), oCols);
  v8Array oList = Nan::New<v8::Array>();
  int iter = 0;
  int lzwMax = ((start + increment) < oTable.size()) ? (start + increment): oTable.size();
  for (int i = start; i < lzwMax; i++) {
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
  double percent = (double)lzwMax / oTable.size();
  percent = (percent > 1) ? 1 : percent;
  outputTable->Set(Nan::New("percent").ToLocalChecked(), Nan::New(percent));
  outputTable->Set(Nan::New("entries").ToLocalChecked(), oList);
  return outputTable;
}

v8Object TextWrapper::formatRLETable(deque<tuple<string,BITS,BITS> > table, int start, int increment) {
  // output table
  v8Object rleTable = Nan::New<v8::Object>(); 
  v8Array cols = Nan::New<v8::Array>();
  
  cols->Set(0, Nan::New("Run").ToLocalChecked());
  cols->Set(1, Nan::New("RunLength").ToLocalChecked());
  rleTable->Set(Nan::New("columns").ToLocalChecked(), cols);
  v8Array entries = Nan::New<v8::Array>();
  int iter = 0;
  int rleMax = ((start + increment) < table.size()) ? (start + increment) : table.size();
  for (int i = start; i < rleMax; i++) {
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
  double percent = (double)rleMax / table.size();
  percent = (percent > 1) ? 1 : percent;
  rleTable->Set(Nan::New("percent").ToLocalChecked(), Nan::New(percent));
  rleTable->Set(Nan::New("entries").ToLocalChecked(), entries);
  return rleTable;
}

v8Object TextWrapper::formatHuffmanTrie(Trie * trie, int level, int maxLevel) {
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
  if (level < maxLevel) {
    huffmanTrie->Set(Nan::New("zero").ToLocalChecked(), formatHuffmanTrie(trie->zero, level + 1, maxLevel));
    huffmanTrie->Set(Nan::New("one").ToLocalChecked(), formatHuffmanTrie(trie->one, level + 1, maxLevel));
  }
  return huffmanTrie;
}
void TextWrapper::Decode(const Nan::FunctionCallbackInfo<v8::Value>& args) {

}
