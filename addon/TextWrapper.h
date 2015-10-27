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
  explicit TextWrapper(Encoding * encoding);
  ~TextWrapper();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Encode(const Nan::FunctionCallbackInfo<v8::Value>& args); 
  static void Decode(const Nan::FunctionCallbackInfo<v8::Value>& args);
  static Nan::Persistent<v8::Function> constructor;
  static TextComponent * setDecorator(std::string type, TextComponent * text);
  static v8Object getData(Encoding *, std::vector<std::string>);
  static v8Array getLZWTable(std::map<int,std::string>);
  TextComponent * component_;
};

#endif
