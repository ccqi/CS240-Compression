#include <nan.h>
#include <string>
#include "TextWrapper.h"

using namespace std;
using namespace v8;

void Init(v8::Local<v8::Object> exports) {
  TextWrapper::Init(exports);
}

NODE_MODULE(compression, Init)
