#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <optional>

namespace Terryn {

class Error
{
private:
    const std::string default_error = "no error";
    std::string _errorString = default_error;
    bool _inError = false;

    void reset();

public:
    Error() = default;

    bool InError() const;
    std::optional<std::string> GetError();

    void SetError(const std::string & error);
    void SetError(std::string && error);
    void AppendError(const std::string & error);
    void AppendError(std::string && error);
    void PropogateError(Terryn::Error & other);
};

}

#endif // ERROR_H
