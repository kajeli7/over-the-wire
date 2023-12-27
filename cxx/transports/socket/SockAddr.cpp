#include "SockAddr.hpp"

namespace OverTheWire::Transports::Socket {

Napi::Object SockAddr::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "SockAddr", {
    InstanceMethod<&SockAddr::toString>("toString", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    InstanceMethod<&SockAddr::toBuffer>("toBuffer", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    InstanceMethod<&SockAddr::toHuman>(Napi::Symbol::For(env, "nodejs.util.inspect.custom"), static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    InstanceAccessor<&SockAddr::getPort, &SockAddr::setPort>("port"),
    InstanceAccessor<&SockAddr::getIp, &SockAddr::setIp>("ip"),
    InstanceAccessor<&SockAddr::getDomain, &SockAddr::setDomain>("domain"),
  });

  env.GetInstanceData<AddonData>()->SetClass(typeid(SockAddr), func);

  exports.Set("SockAddr", func);

  exports.Set("inetPton", Napi::Function::New<inetPton>(env, "inetPton"));
  exports.Set("inetNtop", Napi::Function::New<inetNtop>(env, "inetNtop"));

  exports.Set("htonl", Napi::Function::New<jsHtonl>(env, "htonl"));
  exports.Set("ntohl", Napi::Function::New<jsNtohl>(env, "ntohl"));
  exports.Set("htons", Napi::Function::New<jsHtons>(env, "htons"));
  exports.Set("ntohs", Napi::Function::New<jsNtohs>(env, "ntohs"));

  return exports;
}

SockAddr::SockAddr(const Napi::CallbackInfo& info) : Napi::ObjectWrap<SockAddr>{info} {
  if (info.Length() > 0) {
    Napi::Object obj = info[0].As<Napi::Object>();
    if (obj.Has("ip")) {
      ip = obj.Get("ip").As<Napi::String>().Utf8Value();
    }
    if (obj.Has("port")) {
      port = obj.Get("port").As<Napi::Number>().Int32Value();
    }
    if (obj.Has("domain")) {
      domain = obj.Get("domain").As<Napi::Number>().Int32Value();
    }
  }
}

SockAddr::~SockAddr() { }

std::pair<sockaddr_ptr_t, size_t> SockAddr::addr() {
  auto res = std::make_pair(sockaddr_ptr_t{(sockaddr*)nullptr}, size_t{});
  if (port == -1) {
    err = "Invalid port";
    return res;
  }
  if (ip.size() == 0) {
    err = "Invalid destination";
    return res;
  }
  if (domain == AF_INET6){
    res.second = sizeof(sockaddr_in6);
    res.first = sockaddr_ptr_t{(sockaddr*)new sockaddr_in6};
    int code = uv_ip6_addr(ip.c_str(), port, (sockaddr_in6*)res.first.get());
    if (code < 0) {
      err = getLibuvError(code);
    }
    else {
      err = "";
    }
  }
  else if (domain == AF_INET) {
    res.second = sizeof(sockaddr_in);
    res.first = sockaddr_ptr_t{(sockaddr*)new sockaddr_in};
    int code =  uv_ip4_addr(ip.c_str(), port, (sockaddr_in*)res.first.get());
    if (code < 0) {
      err = getLibuvError(code);
    }
    else {
      err = "";
    }
  }
  else {
    err = "Unknown domain";
  }

  return res;
}

//from https://github.com/libuv/libuv/pull/3368/files
int uv_ip_name(const struct sockaddr *src, char *dst, size_t size) {
  switch (src->sa_family) {
  case AF_INET:
    return uv_inet_ntop(AF_INET, &((struct sockaddr_in *)src)->sin_addr,
                        dst, size);
  case AF_INET6:
    return uv_inet_ntop(AF_INET6, &((struct sockaddr_in6 *)src)->sin6_addr,
                        dst, size);
  default:
    return UV_EAFNOSUPPORT;
  }
}

bool SockAddr::genName(Napi::Env env, bool toThrow = true) {
  sockaddr_ptr_t target;
  size_t size;
  std::tie(target, size) = addr();
  if (err.size() > 0) {
    if (toThrow) {
      Napi::Error::New(env, err).ThrowAsJavaScriptException();
    }
    return false;
  }
  int code = uv_ip_name(target.get(), name, INET6_ADDRSTRLEN);
  if (code < 0) {
    if (toThrow) {
      Napi::Error::New(env, getLibuvError(code)).ThrowAsJavaScriptException();
    }
    return false;
  }

  return true;
}

Napi::Value SockAddr::toString(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  bool ok = genName(env);
  if (!ok) {
    return env.Undefined();
  }
  return Napi::String::New(env, name);
}

Napi::Value SockAddr::toBuffer(const Napi::CallbackInfo& info) {
  sockaddr_ptr_t target;
  size_t size;
  std::tie(target, size) = addr();
  return js_buffer_t::NewOrCopy(info.Env(), (uint8_t*)target.release(), size, [](Napi::Env env, uint8_t* data) { 
    DEBUG_OUTPUT("Deleting SockAddr buffer");
    delete data; 
  });
}

Napi::Value SockAddr::toHuman(const Napi::CallbackInfo& info) {
  std::stringstream ss;
  Napi::Env env = info.Env();
  bool ok = genName(env, false);
  if (!ok) {
    ss << "<SockAddr | " << "invalid" << ">";
  }
  else {
    ss << "<SockAddr | IP: " << name;
    if (port > 0) {
      ss << " port: " << port;
    }
    ss << '>';
  }
  return Napi::String::New(env, ss.str());
}

Napi::Value SockAddr::getPort(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), port);
}

void SockAddr::setPort(const Napi::CallbackInfo&, const Napi::Value& val) {
  port = val.As<Napi::Number>().Int32Value();
}

Napi::Value SockAddr::getIp(const Napi::CallbackInfo& info) {
  if (ip.size() == 0) {
    return info.Env().Null();
  }
  return Napi::String::New(info.Env(), ip);
}

void SockAddr::setIp(const Napi::CallbackInfo&, const Napi::Value& val) {
  ip = val.As<Napi::String>().Utf8Value();
}

Napi::Value SockAddr::getDomain(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), domain);
}

void SockAddr::setDomain(const Napi::CallbackInfo&, const Napi::Value& val) {
  domain = val.As<Napi::Number>().Int32Value();
}

Napi::Value inetPton(const Napi::CallbackInfo& info) {
  checkLength(info, 2);
  Napi::Env env = info.Env();
  int domain = info[0].As<Napi::Number>().Uint32Value();
  std::string src = info[1].As<Napi::String>().Utf8Value();
  js_buffer_t res = js_buffer_t::New(env, sizeof(in6_addr));

  int s = inet_pton(domain, src.c_str(), res.Data());

  if (s <= 0) {
    if (s == 0) {
      Napi::Error::New(env, "Not in presentation format").ThrowAsJavaScriptException();
      return env.Undefined();
    }
    else {
      Napi::Error::New(env, getSystemError()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }

  return res;
}

Napi::Value inetNtop(const Napi::CallbackInfo& info) {
  checkLength(info, 2);
  Napi::Env env = info.Env();
  int domain = info[0].As<Napi::Number>().Uint32Value();
  js_buffer_t buf = info[1].As<js_buffer_t>();

  char str[INET6_ADDRSTRLEN];

  if (inet_ntop(domain, buf.Data(), str, INET6_ADDRSTRLEN) == NULL) {
    Napi::Error::New(env, "Not in presentation format").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  return Napi::String::New(env, str);
}

Napi::Value jsHtonl(const Napi::CallbackInfo& info) {
  checkLength(info, 1);
  return Napi::Number::New(info.Env(), htonl(info[0].As<Napi::Number>().Uint32Value()));
}

Napi::Value jsNtohl(const Napi::CallbackInfo& info) {
  checkLength(info, 1);
  return Napi::Number::New(info.Env(), ntohl(info[0].As<Napi::Number>().Uint32Value()));
}

Napi::Value jsHtons(const Napi::CallbackInfo& info) {
  checkLength(info, 1);
  return Napi::Number::New(info.Env(), htons(info[0].As<Napi::Number>().Uint32Value()));
}

Napi::Value jsNtohs(const Napi::CallbackInfo& info) {
  checkLength(info, 1);
  return Napi::Number::New(info.Env(), ntohs(info[0].As<Napi::Number>().Uint32Value()));
}

};
