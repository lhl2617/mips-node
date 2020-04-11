#include <napi.h>
#include <vector>
#include <string>
#include "main.hpp"
#include "include/util.hpp"
#include "include/parserSimulatorAPI.hpp"
#include "simulator/include/simulator.hpp"

using namespace std;

Napi::Array NAPI_format(const Napi::CallbackInfo &info)
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
    catch (...)
    {
        throw Napi::Error::New(env, "Unknown error!");
    }

    auto ret = Napi::Array::New(env, res.size());
    for (int i = 0; i < res.size(); ++i)
    {
        ret[i] = res[i];
    }

    return ret;
}

Napi::Array NAPI_compile(const Napi::CallbackInfo &info)
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
    catch (...)
    {
        throw Napi::Error::New(env, "Unknown error!");
    }

    auto ret = Napi::Array::New(env, res.size());
    for (int i = 0; i < res.size(); ++i)
    {
        ret[i] = res[i];
    }

    return ret;
}

Napi::String NAPI_intellisense(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    std::string input = (std::string)info[0].ToString();

    try 
    {
        intellisense(input);
    }
    catch (retException &e)
    {
        /// this is what we want - we have error here!
        return Napi::String::New(env, e.msg);
    }
    catch (RunErr &e)
    {
        /// should not happen!
        throw Napi::Error::New(env, e.message);
    }
    catch (...)
    {
        throw Napi::Error::New(env, "Unknown error!");
    }

    return Napi::String::New(env, "");
}

Napi::String NAPI_getPickledRunInfo(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    std::string input = (std::string)info[0].ToString();
    std::string res;

    try
    {
        res = getPickledRunInfo(input);
    }
    catch (retException &e)
    {
        throw Napi::Error::New(env, e.msg);
    }
    catch (RunErr &e)
    {
        throw Napi::Error::New(env, e.message);
    }
    catch (...)
    {
        throw Napi::Error::New(env, "Unknown error!");
    }

    auto ret = Napi::String::New(env, res);

    return ret;
}

Napi::Object VSCodePayloadToObject(Napi::Env env, const VSCodePayload &v)
{
    auto ret = Napi::Object::New(env);
    ret.Set("highlightLineNo", v.highlightLineNo);
    ret.Set("highlightClassName", v.highlightClassName);
    ret.Set("notificationMsg", v.notificationMsg);
    ret.Set("notificationType", v.notificationType);

    return ret;
}

Napi::Object UIPayloadToObject(Napi::Env env, const UIPayload &u)
{
    auto ret = Napi::Object::New(env);

    auto regs = Napi::Object::New(env);
    for (auto &x : u.regs)
        regs.Set(x.first, x.second);
    ret.Set("regs", regs);

    auto mem = Napi::Object::New(env);
    for (auto &x : u.mem)
        mem.Set(x.first, x.second);
    ret.Set("mem", mem);

    ret.Set("stepsDone", u.stepsDone);
    ret.Set("statusBarMsg", u.statusBarMsg);
    ret.Set("statusBarClassName", u.statusBarClassName);

    return ret;
}

Napi::Object NAPI_stepCode(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    std::string pickledRI = (std::string)info[0].ToString();
    int steps = (int)info[1].ToNumber();
    Payload res;

    try
    {
        res = stepCode(pickledRI, steps);
    }
    catch (retException &e)
    {
        throw Napi::Error::New(env, e.msg);
    }
    catch (RunErr &e)
    {
        throw Napi::Error::New(env, e.message);
    }
    catch (...)
    {
        throw Napi::Error::New(env, "Unknown error!");
    }

    auto ret = Napi::Object::New(env);

    ret.Set("pickledRunInfo", res.ri);
    ret.Set("vscodePayload", VSCodePayloadToObject(env, res.v));
    ret.Set("uiPayload", UIPayloadToObject(env, res.u));

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
    catch (...)
    {
        throw Napi::Error::New(env, "Unknown error!");
    }

    auto ret = Napi::String::New(env, res);

    return ret;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "format"),
        Napi::Function::New(env, NAPI_format));
    exports.Set(
        Napi::String::New(env, "compile"),
        Napi::Function::New(env, NAPI_compile));
    exports.Set(
        Napi::String::New(env, "intellisense"),
        Napi::Function::New(env, NAPI_intellisense));
    exports.Set(
        Napi::String::New(env, "getPickledRunInfo"),
        Napi::Function::New(env, NAPI_getPickledRunInfo));
    exports.Set(
        Napi::String::New(env, "stepCode"),
        Napi::Function::New(env, NAPI_stepCode));
    exports.Set(
        Napi::String::New(env, "playground"),
        Napi::Function::New(env, playground));
    return exports;
}

NODE_API_MODULE(mips_parser_node, Init);