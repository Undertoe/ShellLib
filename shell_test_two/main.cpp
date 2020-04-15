#include <iostream>

#include "refl.hh"
#include "reflected_function_collection.h"
#include "reflected_object_collection.h"
#include "shell_runtime.h"

using namespace std;


struct Struc1
{
    Struc1() = default;

    int a = 1;
    int b = 2;

    int Print1(const std::string & args)
    {
        std::cout << "1" << std::endl;
        return 0;
    }

    int Print2(const std::string & args)
    {
        std::cout << "2" << std::endl;
        return 0;
    }
};

struct Struc2
{
    Struc2() = default;

    int c = 3;
    float d = 4.4;
    int Print3(const std::string & args)
    {
        std::cout << "3" << std::endl;
        return 0;
    }

    int Print4(const std::string & args)
    {
        std::cout << "4 " << args << std::endl;
        return 0;
    }
};

struct Struc3
{

    Struc3() = default;

    double five = 5.5;
    double six = 6.6;
    int Print5(const std::string & args)
    {
        std::cout << "5" << std::endl;
        return 0;
    }

    int Print6(const std::string & args)
    {
        std::cout << "6 " << args << std::endl;
        return 0;
    }
};


REFL_AUTO(
        type(Struc1),
        field(a),
        field(b),
        func(Print1),
        func(Print2)
        )

REFL_AUTO(
        type(Struc2),
        field(c),
        field(d),
        func(Print3),
        func(Print4)
        )

REFL_AUTO(
        type(Struc3),
        field(five),
        field(six),
        func(Print5),
        func(Print6)
        )

class CustomCallback : public ShellCallbacks
{
private:
    Terryn::ShellRuntime *_localRuntime;
public:
    CustomCallback() = default;

    void SetRuntime(Terryn::ShellRuntime *runtime)
    {
        _localRuntime = runtime;
    }


    virtual void OutputCallback(const std::string &output) override
    {
        std::cout << "Output: " << output << std::endl;
    }

    virtual void ScritpLinePlayback(const std::string &output) override
    {
        std::cout << "line: " << output << std::endl;
    }

    virtual void VarAssignCallback(bool success) override
    {
        if(success)
        {
            std::cout << "Value assigned" << std::endl;
        }
    }

    virtual void LSCallBack(const std::string &output) override
    {
        std::cout << "\t" << output;
        if(std::experimental::filesystem::is_directory(_localRuntime->CurrentPath() /= output))
        {
            std::cout << "/";
        }
        std::cout << std::endl;
    }

    virtual void CDCallBack(const std::string &output, bool successful) override
    {

    }
};


int main()
{
    CustomCallback callback;
    Terryn::ShellRuntime runtime(&callback);
    callback.SetRuntime(&runtime);

    constexpr uint32_t CECount = 4;

    std::array<Struc1, CECount> data1 =
    {
        Struc1(), Struc1(), Struc1(), Struc1()
    };
    std::array<Struc2, CECount> data2 =
    {
        Struc2(), Struc2(), Struc2(), Struc2()
    };
    std::array<Struc3, CECount> data3 =
    {
        Struc3(), Struc3(), Struc3(), Struc3()
    };

    for(uint32_t i = 0; i < CECount; i ++)
    {
        runtime.AddCEData(i, &data1[i], &data2[i], &data3[i]);
    }

    for(uint32_t i = 0; i < CECount; i ++)
    {
        runtime.AddCEFunctions(&data1[i], &data2[i], &data3[i]);
    }



    /// test output of the various lines we want to check
    std::cout << "Checking all ces" << std::endl;
    std::string line = "mcva a";
    if(runtime.ExecuteLine(line) < 0)
    {
        std::cout << "error: " << runtime.error.GetError().value_or("Unknown error");
    }

    std::cout << "Checking all CEs again" << std::endl;
    data1[1].a = 5;
    if(runtime.ExecuteLine(line) < 0)
    {
        std::cout << "error: " << runtime.error.GetError().value_or("Unknown error");
    }



    line = "mcva a -ce 1";
    std::cout << "Checking ce 1 line" << std::endl;
    if(runtime.ExecuteLine(line) < 0)
    {
        std::cout << "error: " << runtime.error.GetError().value_or("Unknown error");
    }


    line = "mcva a -ces 1-3";
    std::cout << line << std::endl;
    if(runtime.ExecuteLine(line) < 0)
    {
        std::cout << "error: " << runtime.error.GetError().value_or("Unknown error");
    }


    std::cout << std::endl << std::endl << "Testing assigning values" << std::endl;

    line = "mcfa a 3";
    std::cout << line << std::endl;
    if(runtime.ExecuteLine(line) < 0)
    {
        std::cout << "error: " << runtime.error.GetError().value_or("Unknown error");
    }
    line = "mcva a";
    if(runtime.ExecuteLine(line) < 0)
    {
        std::cout << "error: " << runtime.error.GetError().value_or("Unknown error");
    }

    std::cout << "Assigning CE 2" << std::endl;
    line = "mcfa a 6 -ce 2";
    std::cout << line << std::endl;
    if(runtime.ExecuteLine(line) < 0)
    {
        std::cout << "error: " << runtime.error.GetError().value_or("Unknown error");
    }
    line = "mcva a";
    std::cout << line << std::endl;
    if(runtime.ExecuteLine(line) < 0)
    {
        std::cout << "error: " << runtime.error.GetError().value_or("Unknown error");
    }



    std::cout << std::endl << std::endl << "Testing file output" << std::endl;
    std::string script = "/home/terryn/sandbox/testData/testScript.g4sh";
    if(runtime.ExecuteScript(script) < 0)
    {
        std::cout << "error: " << runtime.error.GetError().value_or("Unknown error") << std::endl;
    }


    std::cout << std::endl << std::endl << "TESTING FS SCRIPT" << std::endl;

    script = "/home/terryn/sandbox/testData/fs_check.g4sh";
    if(runtime.ExecuteScript(script) < 0)
    {
        std::cout << "error: " << runtime.error.GetError().value_or("Unknown error") << std::endl;
    }


    script = "/home/terryn/sandbox/testData/aliasTest.g4sh";
    if(runtime.ExecuteScript(script) < 0)
    {
        std::cout << "error: " << runtime.error.GetError().value_or("Unknown error") << std::endl;
    }



    return 0;
}
