#include <nan.h>
#include <string>

using namespace std;
using namespace v8;
using namespace Nan;
void Method(const Nan::FunctionCallbackInfo<v8::Value>& args) {
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string text = std::string(*param1);
  v8::Local<v8::String> jsText = Nan::New(text).ToLocalChecked();
  args.GetReturnValue().Set(jsText);
  
}

/*NAN_METHOD(Method) {
    String::Utf8Value cmd(args[0]);
    string text = string(*cmd);
    NanReturnValue(NanNew<String>(text));
}*/

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("hello").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Method)->GetFunction());
}

NODE_MODULE(compression, Init)
