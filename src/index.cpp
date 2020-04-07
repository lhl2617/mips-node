#include <napi.h>
#include <vector>
#include <string>
#include "main.hpp"
#include "include/util.hpp"

Napi::Array formatMIPS(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    std::string input = (std::string)info[0].ToString();

    std::vector<std::string> res;

    try
    {
        res = format(input);
    }
    catch (retException &e)
    {
        throw Napi::Error::New(env, e.msg);
    }

    auto ret = Napi::Array::New(env, res.size());

    for (int i = 0; i < res.size(); ++i)
    {
        ret[i] = res[i];
    }

    return ret;
}

Napi::Array compileMIPS(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    std::string input = (std::string)info[0].ToString();

    std::vector<uint32_t> res;

    try
    {
        res = parse(input);
    }
    catch (retException &e)
    {
        throw Napi::Error::New(env, e.msg);
    }

    auto ret = Napi::Array::New(env, res.size());

    for (int i = 0; i < res.size(); ++i)
    {
        ret[i] = res[i];
    }

    return ret;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "formatMIPS"),
        Napi::Function::New(env, formatMIPS));
    exports.Set(
        Napi::String::New(env, "parseMIPS"),
        Napi::Function::New(env, parseMIPS));
    return exports;
}

NODE_API_MODULE(mips_parser_node, Init);