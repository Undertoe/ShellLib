#include "error.h"

void Terryn::Error::reset()
{
    _errorString = default_error;
    _inError = false;
}

bool Terryn::Error::InError() const
{
    return _inError;
}

std::optional<std::string> Terryn::Error::GetError()
{
    if(_inError)
    {
        std::string rror = _errorString;
        reset();
        return rror;
    }
    return std::nullopt;
}

void Terryn::Error::SetError(const std::string &error)
{
    if(error.size() > 0)
    {
        _inError = true;
        _errorString = error;
    }
}

void Terryn::Error::SetError(std::string && error)
{
    if(error.size() > 0)
    {
        _inError = true;
        _errorString = std::move(error);
    }
}

void Terryn::Error::AppendError(const std::string &error)
{
    if(error.size() > 0)
    {
        _inError = true;
        _errorString += "\n" +  error;
    }
}

void Terryn::Error::AppendError(std::string && error)
{
    if(error.size() > 0)
    {
        _inError = true;
        _errorString += "\n" + std::move(error);
    }
}

void Terryn::Error::PropogateError(Terryn::Error &other)
{
    auto check = other.GetError();
    if(check)
    {
        SetError(*check);
    }
}


