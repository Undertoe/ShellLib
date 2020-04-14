#include "shell_runtime.h"

int Terryn::ShellRuntime::ExecuteLine(std::string_view line)
{
    ProcessType type = parse_for_type_of_execution(line);
    switch(type)
    {
    case ViewVar:
        return view_var(line);
        break;
    case ModifyVar:
        return modify_var(line);
        break;

    case RunScript:
        return ExecuteScript(line);
        break;

    case ls:
        return check_directory();
        break;

    case ChangeDirectory:
        return change_directory(line);
        break;

    case Unknown:
        error.SetError("Unknown type grabbed, exiting");
        return -1;
    default:
        error.SetError("Unknown type grabbed, exiting");
        return -1;
    }


    error.SetError("Process execution failed");
    return -1;
}


Terryn::ShellRuntime::ProcessType Terryn::ShellRuntime::parse_for_type_of_execution(std::string_view line)
{
    auto split = StringHelpers::split(line, ' ');
    if(split.size() == 0)
    {
        for(auto & [key, value] : _processTypes)
        {
            if(line == key)
            {
                return value;
            }
        }
    }

    std::string keyToSearch = split[0];
    for(auto & [key, value] : _processTypes)
    {
        if(keyToSearch == key)
        {
            return value;
        }
    }
    return Unknown;
}

/// valid formats
/// mcva varname
/// mcva varname -ce #
/// mcva varname -ces #-#
///
/// CE Range is inclusive: [N, M] rather than [N, M)
int Terryn::ShellRuntime::view_var(std::string_view line)
{
    auto split = StringHelpers::split(line, ' ');

    if(split.size() < 2)
    {
        error.SetError("No variable found to search");
        return -1;
    }



    if(split.size() != 2 && split.size() != 4)
    {
        error.SetError("invalid formed line: " + std::string(line));
        return -1;
    }

    std::string varName(split[1]);

    std::vector<uint32_t> CERange;

    if(split.size() == 4)
    {
        if(split[2] == "-ce")
        {
            CERange.push_back(stoi(split[3]));
        }
        else if(split[2] == "-ces")
        {
            auto range = split[3];
            auto nums = StringHelpers::split(range, '-');
            if(nums.size() != 2)
            {
                error.SetError("Invalid CE Range caught: " + std::string(line));
                return -1;
            }

            int first = stoi(nums[0]);
            int last = stoi(nums[1]);
            for(int i = first; i <= last; i ++)
            {
                CERange.push_back(i);
            }
        }
        else
        {
            error.SetError("Invalid formed line: " + std::string(line));
            return -1;
        }
    }

    if(CERange.size() == 0)
    {
        for(auto & [ce, varBank] : _vars)
        {
            auto mVal = varBank->Get(varName);
            if(mVal)
            {
                _callback->OutputCallback(*mVal);
            }
        }
    }
    else
    {
        for(const auto & ce : CERange)
        {
            if(_vars.find(ce) != _vars.end())
            {
                auto mVal = _vars[ce]->Get(varName);
                if(mVal)
                {
                    _callback->OutputCallback(*mVal);
                }
            }
        }
    }
    return 0;
}

/// valid formats
/// mcfa varname val
/// mcfa varname val -ce #
/// mcfa varname val -ces #-#
///
/// CE Range is inclusive: [N, M] rather than [N, M)
int Terryn::ShellRuntime::modify_var(std::string_view line)
{
    auto split = StringHelpers::split(line, ' ');

    if(split.size() < 3)
    {
        error.SetError("No variable found to search: " + std::string(line));
        return -1;
    }

    std::string varName(split[1]);

    std::vector<uint32_t> CERange;

    /// CE specific
    if(split.size() == 5)
    {
        if(split[3] == "-ce")
        {
            CERange.push_back(stoi(split[4]));
            std::cout << CERange[0] << std::endl;
        }
        else if(split[3] == "-ces")
        {
            auto range = split[4];
            auto nums = StringHelpers::split(range, '-');
            if(nums.size() != 2)
            {
                error.SetError("Invalid CE Range caught: " + std::string(line));
                return -1;
            }

            int first = stoi(nums[0]);
            int last = stoi(nums[1]);
            for(int i = first; i <= last; i ++)
            {
                CERange.push_back(i);
            }
        }
        else
        {
            error.SetError("Invalid formed line: " + std::string(line));
            return -1;
        }
    }

    std::string value(split[2]);

    if(CERange.size() == 0)
    {
        for(auto & [ce, var] : _vars)
        {
            _callback->VarAssignCallback(var->AssignValue(varName, value));
        }
    }
    else
    {
//        std::cout << "Finding specific CE" << std::endl;
        for(const auto & ce : CERange)
        {
            std::cout << ce << std::endl;
            if(_vars.find(ce) != _vars.end())
            {
                _callback->VarAssignCallback(_vars[ce]->AssignValue(varName, value));
            }
        }
    }

    return 0;
}

int Terryn::ShellRuntime::ExecuteScript(std::string_view line)
{
    std::ifstream inputFile;
    inputFile.open(std::string(line));
    if(!inputFile.is_open())
    {
        error.SetError("File failed to open: " + std::string(line));
        return -1;
    }


    std::string file((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    auto lines = StringHelpers::split(file, '\n');
    for(const auto & line : lines)
    {
        if(line.size() != 0)
        {
            _callback->ScritpLinePlayback(line);
            if(this->ExecuteLine(line) < 0)
            {
                return -1;
            }
        }
    }

    return 0;
}

int Terryn::ShellRuntime::change_directory(std::string_view line)
{
    auto split = StringHelpers::split(line, ' ');
    if(split.size() != 2)
    {
        error.SetError("Invalid line: " + std::string(line));
        return -1;
    }
    try
    {
        _localDir /= split[1];
    }
    catch(std::exception & e)
    {
        error.SetError(std::string(e.what()) + " caught with line: " + std::string(line));
        return -1;
    }

    return 0;
}


int Terryn::ShellRuntime::check_directory()
{
    try
    {
        for(auto & item : fs::directory_iterator(_localDir))
        {
            _callback->LSCallBack(item.path().filename());
        }
    }
    catch(std::exception & e)
    {
        error.SetError(e.what());
        return -1;
    }

    return 0;
}


std::experimental::filesystem::path Terryn::ShellRuntime::CurrentPath() const
{
    return _localDir;
}
