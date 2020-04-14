#include "reflected_value.hh"

std::string Terryn::TypeToString(Type type)
{
    switch(type)
    {
    case Type::FLOAT:
        return "float";
    case Type::DOUBLE:
        return "double";
    case Type::INT:
        return "int32_t";
    case Type::UINT:
        return "uint32_t";
    case Type::INT64:
        return "int64_t";
    case Type::UINT64:
        return "uint64_t";
    case Type::COMPLEX_FLOAT:
        return "std::complex<float>";
    case Type::COMPLEX_DOUBLE:
        return "std::complex<double>";
    case Type::STRING:
        return "std::string";
    case Type::BOOL:
        return "bool";
    default:
        return "UNKNOWN";
    }
}

Terryn::ReferencedType::ReferencedType(ReferencedType *other) :
    _ptr(other->RawPtr()),
    _type(other->RawType()),
    _firstInit(false),
    _set(true)
{ }

void Terryn::ReferencedType::ptr_set(void* p)
{
    _ptr = p;
    _set = true;
}

std::string Terryn::ReferencedType::getValueAtIndex(size_type index) const
{
    switch(_type)
    {
    case Type::FLOAT:
    {
        float* ptr = (float*)_ptr;
        return std::to_string(ptr[index]);
    }
    case Type::DOUBLE:
    {
        double* ptr = (double*)_ptr;
        return std::to_string(ptr[index]);
    }
    case Type::INT:
    {
        int32_t* ptr = (int32_t*)_ptr;
        return std::to_string(ptr[index]);
    }
    case Type::UINT:
        /// local to_string()
    {
        uint32_t* ptr = (uint32_t*)_ptr;
        return to_string(ptr[index]);
    }
    case Type::INT64:
    {
        int64_t* ptr = (int64_t*)_ptr;
        return std::to_string(ptr[index]);
    }
    case Type::UINT64:
        /// local to_string()
    {
        uint64_t* ptr = (uint64_t*)_ptr;
        return to_string(ptr[index]);
    }

    case Type::STRING:
    {
        std::string* ptr = (std::string*)_ptr;
        return ptr[index];
    }
    case Type::BOOL:
    {
        bool* ptr = (bool*)_ptr;
        if(ptr[index])
        {
            return "true";
        }
        return "false";
    }
    default:
        return "";
    }
}

void Terryn::ReferencedType::setValueAtIndex(size_type index, std::string stringVal)
{
    switch(_type)
    {
    case Type::FLOAT:
    {
        float * ptr = (float*)_ptr;
        ptr[index] = std::stof(stringVal);
        break;
    }
    case Type::DOUBLE:
    {
        double * ptr = (double*)_ptr;
        ptr[index] = std::stod(stringVal);
        break;
    }
    case Type::INT:
    {
        int32_t * ptr = (int32_t*)_ptr;
        ptr[index] = std::stoi(stringVal);
        break;
    }
    case Type::UINT:
    {
        if(stringVal[1] == 'x')
        {
            uint32_t * ptr = (uint32_t*)_ptr;
            ptr[index] = std::stoul(stringVal, nullptr, 16);
        }
        else
        {
            uint32_t * ptr = (uint32_t*)_ptr;
            ptr[index] = std::stoul(stringVal);
        }
        break;
    }
    case Type::INT64:
    {
        int64_t * ptr = (int64_t*)_ptr;
        ptr[index] = std::stol(stringVal);
        break;
    }
    case Type::UINT64:
    {
        if(stringVal[1] == 'x')
        {
            uint64_t * ptr = (uint64_t*)_ptr;
            ptr[index] = std::stoul(stringVal, nullptr, 16);
        }
        else
        {
            uint64_t * ptr = (uint64_t*)_ptr;
            ptr[index] = std::stoul(stringVal);
        }
        break;
    }

    case Type::STRING:
    {
        std::string * ptr = (std::string*)_ptr;
        ptr[index] = stringVal;
        break;
    }
    case Type::BOOL:
        if(stringVal.front() == 't')
        {
            bool * ptr = (bool*)_ptr;
            ptr[index] = true;
        }
        else
        {
            bool * ptr = (bool*)_ptr;
            ptr[index] = false;
        }
        break;
    default:
        return;
    }
}


void Terryn::ReferencedType::Set_impl(float *v, std::true_type)
{
    _type = Type::FLOAT;
    ptr_set(v);
}


void Terryn::ReferencedType::Set_impl(double *v, std::true_type)
{
    _type = Type::DOUBLE;
    ptr_set(v);
}


void Terryn::ReferencedType::Set_impl(uint32_t *v, std::true_type)
{
    _type = Type::UINT;
    ptr_set(v);
}


void Terryn::ReferencedType::Set_impl(int32_t *v, std::true_type)
{
    _type = Type::INT;
    ptr_set(v);
}

void Terryn::ReferencedType::Set_impl(uint64_t *v, std::true_type)
{
    _type = Type::UINT64;
    ptr_set(v);
}


void Terryn::ReferencedType::Set_impl(int64_t *v, std::true_type)
{
    _type = Type::INT64;
    ptr_set(v);
}

void Terryn::ReferencedType::Set_impl(std::complex<float> *v, std::true_type)
{
    _type = Type::COMPLEX_FLOAT;
    ptr_set(v);
}


void Terryn::ReferencedType::Set_impl(std::complex<double> *v, std::true_type)
{
    _type = Type::COMPLEX_DOUBLE;
    ptr_set(v);
}

void Terryn::ReferencedType::Set_impl(std::string *v, std::true_type)
{
    _type = Type::STRING;
    ptr_set(v);
}

void Terryn::ReferencedType::Set_impl(bool *v, std::true_type)
{
    _type = Type::BOOL;
    ptr_set(v);
}

void Terryn::ReferencedType::ExternalAssign(const std::string &val)
{
    if(!_set)
    {
        return;
    }

    /// handle external assign for arrays!!!
    if(_arraySize > 0)
    {
        /// expected format:
        /// [index]=value
        if(val.size() == 0)
        {
            return;
        }
        if(val.front() != '[')
        {
            return;
        }

        /// look for index of next ]
        auto pos = val.find(']');
        if(pos == std::string::npos)
        {
            std::cout << "no closing bracket" << std::endl
                      << "String: " << val << std::endl;
            return;
        }
        /// assert value after ] is =
        if(val.at(pos + 1) != '=')
        {
            std::cout << "no equal sign" << std::endl
                      << "String: " << val << std::endl;
            return;
        }


        /// acquire everything between as a size_type value.
        /// This is our index we are assigning.
        std::string indexStr = val.substr(1, pos);
        size_type index = static_cast<size_type>(std::stoul(indexStr));

        /// no spaces.
        /// acquire after the =
        std::string valueStr = val.substr(pos+2, val.size());

        setValueAtIndex(index, valueStr);


        return;
    }

    switch(_type)
    {
    case Type::FLOAT:
    {
        *(float*)_ptr = std::stof(val);
        break;
    }
    case Type::DOUBLE:
    {
        *(double*)_ptr = std::stod(val);
        break;
    }
    case Type::INT:
    {
        *(int32_t*)_ptr = std::stoi(val);
        break;
    }
    case Type::UINT:
    {
        if(val[1] == 'x')
        {
            *(uint32_t*)_ptr = std::stoi(val, nullptr, 16);
        }
        else
        {
            *(uint32_t*)_ptr = std::stoi(val);
        }
        break;
    }
    case Type::INT64:
    {
        *(int64_t*)_ptr = std::stol(val);
        break;
    }
    case Type::UINT64:
    {
        if(val[1] == 'x')
        {
            *(uint64_t*)_ptr = std::stoul(val, nullptr, 16);
        }
        else
        {
            *(uint64_t*)_ptr = std::stoul(val);
        }
        break;
    }
    case Type::COMPLEX_FLOAT:
    {
        auto find = val.find_first_of(" ", 0, val.length());
        if(find == std::string::npos)
        {
            break;
        }

        auto str1 = val.substr(0, find);
        auto str2 = val.substr(find+1, val.size() - 1);

        std::complex<float> *tmp = (std::complex<float>*)_ptr;

        tmp->real(std::stof(str1));
        tmp->imag(std::stof(str2));
        break;
    }
    case Type::COMPLEX_DOUBLE:
    {
        // NYI
        //        cf->real(0.0);
        //        cf->imag(0.0);
        break;
    }
    case Type::STRING:
        *(std::string*)_ptr = val;
        break;
    case Type::BOOL:
        if(val.front() == 't')
        {
            *(bool*)_ptr = true;
        }
        else
        {
            *(bool*)_ptr = false;
        }
        break;
    default:
        return;
    }
}

std::string Terryn::ReferencedType::Get() const
{
    if(!_set)
    {
        return "";
    }

    if(_arraySize != 0)
    {
        std::string retval = "[";
        for(size_type i = 0; i < _arraySize; i ++)
        {
            retval += getValueAtIndex(i);
            if(i+1 < _arraySize)
            {
                retval += ", ";
            }
        }
        retval += "]";
        return retval;
    }

    switch(_type)
    {
    case Type::FLOAT:
        return std::to_string(*(float*)_ptr);
    case Type::DOUBLE:
        return std::to_string(*(double*)_ptr);
    case Type::INT:
        return std::to_string(*(int32_t*)_ptr);
    case Type::UINT:
        /// local to_string()
        return to_string(*(uint32_t*)_ptr);
    case Type::INT64:
        return std::to_string(*(int64_t*)_ptr);
    case Type::UINT64:
        /// local to_string()
        return to_string(*(uint64_t*)_ptr);
    case Type::COMPLEX_FLOAT:
    {
        std::complex<float> *tmp = (std::complex<float>*)_ptr;
        return std::to_string(tmp->real()) + " " + std::to_string(tmp->imag());
    }
    case Type::COMPLEX_DOUBLE:
//        return std::to_string(cd->real()) + ", " + std::to_string(cd->imag());
        return "(0.0, 0.0)";
//        return "";
    case Type::STRING:
        return *(std::string*)_ptr;
    case Type::BOOL:
        if(*(bool*)(_ptr))
        {
            return "true";
        }
        return "false";
    default:
        return "";
    }
}

std::string Terryn::ReferencedType::GetFromIndex(size_type index) const
{
    return getValueAtIndex(index);
}

std::string Terryn::ReferencedType::GetType() const
{
    return TypeToString(_type);
}

size_t Terryn::ReferencedType::Address() const
{
    return (size_t) _ptr;
}


void* Terryn::ReferencedType::RawPtr() const
{
    return _ptr;
}

Terryn::Type Terryn::ReferencedType::RawType() const
{
    return _type;
}
