#include <map>
#include <iostream>
#include <string>
#include <cstring>

#include <functional>
#include <tuple>


long f1(long a)
{
    std::cout << "f1(" << a << ")" << std::endl;
    throw 99;
    return 0;
}

long f2(long a, double b)
{
    std::cout << "f2(" << a << "," << b << ")" << std::endl;
    return 0;
}

long f3(long a, long b)
{
    std::cout << "f3(" << a << "," << b << ")" << std::endl;
    return 0;
}

long f5(long a, long* b)
{
    std::cout << "f3(" << a << "," << *b << ")" << std::endl;
    return 0;
}


struct ScriptArgs
{
public:
    char data[512] = { 0 };

    template<typename T>
    T GetArgValue()
    {
        return *(reinterpret_cast<T*>(data));
    }

    template<typename T>
    T GetArgPtr()
    {
        return reinterpret_cast<T>(data);
    }

    template<typename T>
    T GetArg()
    {
        if(std::is_pointer<T>())
        {
            return GetArgPtr<T>();
        }
        return GetArgValue<T>();
    }
};

namespace user_scripts
{
    std::map<std::string, std::function<long(ScriptArgs&)> > functionMap;
    std::map<std::string, std::function<long(long)> > m;
}

// macro defines a lambda that extracts the args in the correct format
#define MAKE_LAMBDA(fname) [&](long) { long a; return a; }

// 1 arg case
#define POLARIS_REGISTER_USER_FUNCTION_1(fname, a0) long w##fname(ScriptArgs& args) { return fname(args.GetArg<a0>()); }

// 2 arg case
#define POLARIS_REGISTER_USER_FUNCTION_2(fname, a0, a1) long w##fname(ScriptArgs& args) { return fname(args.GetArg<a0>(), args.GetArg<a1>()); }

POLARIS_REGISTER_USER_FUNCTION_1(f1, long)
POLARIS_REGISTER_USER_FUNCTION_2(f2, long, double);


#define MAKE_FUNCTION_WRAPPER_1(fname, a0) [&](ScriptArgs& args) { return fname(args.GetArg<a0>()); }
#define MAKE_FUNCTION_WRAPPER_2(fname, a0, a1) [&](ScriptArgs& args) { return fname(args.GetArg<a0>(), args.GetArg<a1>()); }

long Call(std::string fName, ScriptArgs& args)
{
    if(user_scripts::functionMap.find(fName) != user_scripts::functionMap.end())
    {
        long ret = 0;
        try {
            ret = user_scripts::functionMap[fName](args);
        }
        catch (std::exception& e)
        {
            std::cout << "Caught exception" << std::endl;
        }
        return ret;
    }
    std::cout << "No function matches name provided" << std::endl;
    return -1; // error no function matching name registered
}

int main(int argc, char** argv)
{
    ScriptArgs a;

    user_scripts::functionMap.insert({"f1", MAKE_FUNCTION_WRAPPER_1(f1, long)});
    user_scripts::functionMap.insert({"f2", MAKE_FUNCTION_WRAPPER_2(f2, long, double)});
    user_scripts::functionMap.insert({"f3", MAKE_FUNCTION_WRAPPER_2(f3, long, long)});

    Call("f1",a);
    Call("f2",a);
    Call("f3",a);
    Call("f4",a);

    user_scripts::functionMap.insert({"f4", MAKE_FUNCTION_WRAPPER_1(f1, long)});
    
    Call("f1",a);
    Call("f2",a);
    Call("f3",a);
    Call("f4",a);

    return 0;
}