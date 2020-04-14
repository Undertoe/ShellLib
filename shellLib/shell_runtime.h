#ifndef SHELLRUNTIME_H
#define SHELLRUNTIME_H

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <string_view>
#include <fstream>
#include <experimental/filesystem>

#include "variable_control.h"
#include "function_control.h"
#include "ShellCallbacks.h"

#include "string_helpers.h"
#include "error.h"

namespace Terryn {


namespace fs = std::experimental::filesystem;

class ShellRuntime
{    
private:



    enum ProcessType
    {
        ViewVar,
        ModifyVar,
        RunScript,
        ls,
        ChangeDirectory,
        Unknown,
    };

    const std::map<std::string, ProcessType> _processTypes
    {
        { "mcva", ViewVar },
        { "mcfa", ModifyVar },
        { "run", RunScript },
        { "ls", ls },
        { "cd", ChangeDirectory},
    };


    std::map<uint32_t, std::unique_ptr<VariableControlBase>> _vars;
    std::vector<std::unique_ptr<FunctionControlBase>> _funcs;
    ShellCallbacks * _callback;

    /// this will be initailized to the executable's working directory
    fs::path _localDir = fs::current_path();


    ProcessType parse_for_type_of_execution(std::string_view line);


    int view_var(std::string_view line);
    int modify_var(std::string_view line);

    int change_directory(std::string_view line);
    int check_directory();


public:

    Terryn::Error error;

    ShellRuntime(ShellCallbacks * callbackObject) :
        _callback(callbackObject)
    {

    }


    template<class ... Types>
    bool AddCEData(uint32_t ceNumber, Types* ... data)
    {
        for(auto & [ce, vars] : _vars)
        {
            if(ce == ceNumber)
            {
                error.SetError("CE already exists");
                return false;
            }
        }
        _vars[ceNumber] = std::make_unique<VariableControl<Types...>>(data...);
        return true;
    }

    template<class ... Types>
    void AddCEFunctions(Types* ... objects)
    {
        _funcs.push_back(std::make_unique<FunctionControl<Types...>>(objects...));
    }



    /// entry line for the runtime
    /// this will take a full string, then split it up.
    int ExecuteLine(std::string_view line);


    int ExecuteScript(std::string_view line);

    fs::path CurrentPath() const;
};


}

#endif // SHELLRUNTIME_H
