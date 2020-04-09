#include <napi.h>
#include <vector>
#include <string>
#include "main.hpp"
#include "include/util.hpp"
#include "include/parserSimulatorAPI.hpp"
#include "simulator/include/simulator.hpp"


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
    catch (RunErr &e)
    {
        throw Napi::Error::New(env, e.message);
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
        res = compile(input);
    }
    catch (retException &e)
    {
        throw Napi::Error::New(env, e.msg);
    }
    catch (RunErr &e)
    {
        throw Napi::Error::New(env, e.message);
    }


    auto ret = Napi::Array::New(env, res.size());

    for (int i = 0; i < res.size(); ++i)
    {
        ret[i] = res[i];
    }

    return ret;
}

Napi::String playground(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    std::string input = (std::string)info[0].ToString();

    std::string res;

    try
    {
        res = testBoost(input);
    }
    catch (retException &e)
    {
        throw Napi::Error::New(env, e.msg);
    }
    catch (RunErr &e)
    {
        throw Napi::Error::New(env, e.message);
    }
    catch (const exception& e)
    {
        throw Napi::Error::New(env, e.what());
    }

    auto ret = Napi::String::New(env, res);

    return ret;
}


Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "formatMIPS"),
        Napi::Function::New(env, formatMIPS));
    exports.Set(
        Napi::String::New(env, "compileMIPS"),
        Napi::Function::New(env, compileMIPS));
    exports.Set(
        Napi::String::New(env, "playground"),
        Napi::Function::New(env, playground));
    return exports;
}

NODE_API_MODULE(mips_parser_node, Init);