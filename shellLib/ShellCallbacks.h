#ifndef SHELLCALLBACKS_H
#define SHELLCALLBACKS_H

#include <string>

class ShellCallbacks
{
public:
    ShellCallbacks() = default;

    virtual void EchoCallback(const std::string & echo) = 0;

    virtual void ScritpLinePlayback(const std::string & output) = 0;

    virtual void OutputCallback(const std::string & output) = 0;
    virtual void VarAssignCallback(bool success) = 0;

    virtual void LSCallBack(const std::string & output) = 0;
    virtual void CDCallBack(const std::string & output, bool successful) = 0;
};

#endif // SHELLCALLBACKS_H
