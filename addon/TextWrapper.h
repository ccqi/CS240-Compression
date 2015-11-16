#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <nan.h>
#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <vector>
#include <deque>
#include <map>
#include "src/Decorator.h"
#include "src/TextComponent.h"
#include "src/PlainText.h"
#include "src/MTFEncoder.h"
#include "src/HuffmanEncoder.h"
#include "src/RLEEncoder.h"
#include "src/BWTransform.h"
#include "src/LZWEncoder.h"
#include "src/Encoding.h"

typedef v8::Local<v8::Array> v8Array;
typedef v8::Local<v8::Object> v8Object;

class TextWrapper : public Nan::ObjectWrap {
 public:
  static void Init(v8Object exports);

 private:
  explicit TextWrapper();
  ~TextWrapper();
  void setEncoding(Encoding * encoding);

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Set(const Nan::FunctionCallbackInfo<v8::Value>& info); 
  static void Get(const Nan::FunctionCallbackInfo<v8::Value>& args); 
  static void GetTable(const Nan::FunctionCallbackInfo<v8::Value>& args); 
  static void GetData(const Nan::FunctionCallbackInfo<v8::Value>& args); 
  static void Encode(const Nan::FunctionCallbackInfo<v8::Value>& args); 
  static void Decode(const Nan::FunctionCallbackInfo<v8::Value>& args);
  static Nan::Persistent<v8::Function> constructor;
  static TextComponent * setDecorator(std::string type, TextComponent * text);
  static v8Array formatData(std::string, Encoding *, TextComponent *, TextWrapper*);
  static v8Array getFormats(std::vector<std::string>);
  static v8Object formatSymbolTable(std::map<int,std::string>);
  static v8Object formatOutputTable(std::deque<std::tuple<std::string, int, BITS> >, TextWrapper*);
  static v8Object formatRLETable(std::deque<std::tuple<std::string,BITS, BITS> >, TextWrapper*);
  static v8Object formatHuffmanTrie(Trie*);
  TextComponent * component_;
  int rleIndex_;
  int lzwIndex_;
  int huffmanIndex_;
  int dataIndex_;
  int max_;
};

#endif
