#include "refl.hh"

//#include "container_helpers.hh"

#include <type_traits>
#include <string_view>
#include <complex>
#include <string>
#include <vector>
#include <stdint.h>
#include <map>
#include <iostream>
#include <algorithm>

#include <iostream>


namespace Terryn {

enum class Type : uint8_t
{
    UNKNOWN,
    FLOAT,
    DOUBLE,
    INT,
    UINT,
    INT64,
    UINT64,
    COMPLEX_FLOAT,
    COMPLEX_DOUBLE,
    STRING,
    BOOL,
};


namespace STL_Impl
{
    template<typename T> struct AllowedContainer : std::false_type{};
    template<typename T, size_t S> struct AllowedContainer<std::array<T, S>> : std::true_type{} ;
}


namespace ArrayImpl
{
    template<typename T> struct NotAnArray : std::false_type{};
}

template<typename T>
struct AllowedSTL
{
    static constexpr bool const value = STL_Impl::AllowedContainer<T>::value;
};

template<typename MyType>
static constexpr bool CheckSTL()
{
    return AllowedSTL<MyType>::value;
}

template<typename MyType, typename T>
static constexpr bool DebugTest()
{
    using Decayed = std::remove_pointer_t<MyType>;

    if constexpr(CheckSTL<Decayed>())
    {
        return std::is_same_v<typename Decayed::value_type, T>;
    }
    return std::is_same_v<Decayed, T>;;
}

template<typename MyType>
static constexpr bool AllowedType()
{
    using FixedType = std::remove_pointer_t<MyType>;
    if constexpr(CheckSTL<FixedType>())
    {
        return AllowedType<typename FixedType::value_type>();
    }
    return (std::is_same_v<FixedType, float>                    ||
            std::is_same_v<FixedType, double>                   ||
            std::is_same_v<FixedType, int32_t>                  ||
            std::is_same_v<FixedType, uint32_t>                 ||
            std::is_same_v<FixedType, int64_t>                  ||
            std::is_same_v<FixedType, uint64_t>                 ||
            std::is_same_v<FixedType, bool>                     ||
            std::is_same_v<FixedType, std::string>

                        );
}



template<typename MyType>
static constexpr Type GetTypeEnum(MyType *ptr)
{
    using FixedType = std::remove_pointer_t<MyType>;

    if(std::is_same_v<FixedType, float>)
    {
        return Type::FLOAT;
    }
    if(std::is_same_v<FixedType, double>)
    {
        return Type::DOUBLE;
    }
    if(std::is_same_v<FixedType, int32_t>)
    {
        return Type::INT;
    }
    if(std::is_same_v<FixedType, uint32_t>)
    {
        return Type::UINT;
    }
    if(std::is_same_v<FixedType, int64_t>)
    {
        return Type::INT64;
    }
    if(std::is_same_v<FixedType, uint64_t>)
    {
        return Type::UINT64;
    }
    if(std::is_same_v<FixedType, bool>)
    {
        return Type::BOOL;
    }
    if(std::is_same_v<FixedType, std::string>)
    {
        return Type::STRING;
    }
    return Type::UNKNOWN;
}


template<typename MyType>
static constexpr Type GetTypeEnum()
{
    using FixedType = MyType;

    if(std::is_same_v<FixedType, float>)
    {
        return Type::FLOAT;
    }
    if(std::is_same_v<FixedType, double>)
    {
        return Type::DOUBLE;
    }
    if(std::is_same_v<FixedType, int32_t>)
    {
        return Type::INT;
    }
    if(std::is_same_v<FixedType, uint32_t>)
    {
        return Type::UINT;
    }
    if(std::is_same_v<FixedType, int64_t>)
    {
        return Type::INT64;
    }
    if(std::is_same_v<FixedType, uint64_t>)
    {
        return Type::UINT64;
    }
    if(std::is_same_v<FixedType, bool>)
    {
        return Type::BOOL;
    }
    if(std::is_same_v<FixedType, std::string>)
    {
        return Type::STRING;
    }
    return Type::UNKNOWN;
}

std::string TypeToString(Type type);

union Uint32Rotator
{
    uint64_t n;
    uint8_t a[4];

    void Swap()
    {
        std::cout << "Pre-Swap" << std::hex << n << std::endl;
        std::reverse(a, a + 4);
        std::cout << "Post-Swap" << std::hex << n<< std::endl;
    }

};

union Uint64Rotator
{
    uint64_t n;
    uint8_t a[8];

    void Swap()
    {
        std::cout << "Pre-Swap" << std::hex << n << std::endl;
        std::reverse(a, a + 8);
        std::cout << "Post-Swap" << std::hex << n << std::endl;
    }
};


namespace detail
{
    template<typename T> struct helper;

    template<typename T> struct helper<T*>
    {
        void operator()() const {std::cout << "Pointer" << std::endl;}
    };

    template<typename T, size_t N> struct helper<T[N]>
    {
        void operator()() const {std::cout << "Array" << std::endl;}
    };
}

/// interface to a series of pointers,
/// only outputs the correct type
class ReferencedType
{
private:

    using size_type = size_t;

    void* _ptr = nullptr;

    Type _type = Type::UNKNOWN;
    bool _firstInit = true;
    bool _set = false;
    /// 5 more bytes available
    size_type _arraySize = 0;

    void ptr_set(void* p);

    /// does not work with double and non-lowended types
    /// this formats the string to 0x#### format.
    template<typename LowType>
    std::string to_string(LowType val) const
    {
        std::stringstream ss;
        ss << "0x";
        ss << std::setfill('0') << std::setw(sizeof(LowType));
        ss << std::hex << val;
        return ss.str();
    }

    std::string getValueAtIndex(size_type index) const;
    void setValueAtIndex(size_type index, std::string stringVal);



    void Set_impl(float *v, std::true_type);
    void Set_impl(double *v, std::true_type);
    void Set_impl(uint32_t *v, std::true_type);
    void Set_impl(int32_t *v, std::true_type);
    void Set_impl(uint64_t *v, std::true_type);
    void Set_impl(int64_t *v, std::true_type);
    void Set_impl(bool *v, std::true_type);
    void Set_impl(std::complex<float> *v, std::true_type);
    void Set_impl(std::complex<double> *v, std::true_type);
    void Set_impl(std::string *v, std::true_type);


    template<typename BaseType, size_type size>
    void Set_impl(std::array<BaseType, size> *value, std::true_type)
    {
        static_assert (AllowedType<BaseType>(), "Arrays can only be made of basic types");
        static_assert (size <= std::numeric_limits<size_type>::max(), "Currently does not support values larger than current size_type");
        _arraySize = size;
        _type = GetTypeEnum<BaseType>();
        ptr_set((void*)value->data());

    }


    template<typename BaseType, size_type size>
    void Set_impl(BaseType(&value)[size], std::false_type)
    {
        std::cout << "Array Passed" << std::endl;
        static_assert (AllowedType<BaseType>(), "Arrays can only be made of basic types");
        static_assert (size <= std::numeric_limits<size_type>::max(), "Currently does not support values larger than current size_type");
        _arraySize = size;
        std::cout << "Size: " << size << std::endl;
        _type = GetTypeEnum(&value[0]);
        ptr_set((void*)&value[0]);
    }

public:

    ReferencedType() = default;

    ReferencedType(const ReferencedType &other) = delete;
    ReferencedType& operator=(const ReferencedType &other) = delete;
    ReferencedType(ReferencedType &&other) = delete;
    ReferencedType& operator=(ReferencedType &&other) = delete;

    ReferencedType(ReferencedType *another);


//    template<typename T>
//    void TMP(const T& t)
//    {
//        detail::helper<T>{}();
//    }

    template<class InputType>
    void TMP_impl(InputType *input, std::true_type)
    {
        std::cout << "Pointer Type" << std::endl;
//        std::cout << "Extent of type: " << std::extent_v<decltype (input)> << std::endl;
    }

    template<class InputType, std::size_t sz>
    void TMP_impl(InputType(&input)[sz], std::false_type)
    {
        std::cout << "Array Type" << std::endl;
        std::cout << "Extent of type: " << std::extent_v<decltype (input)> << std::endl;
        std::cout << "size: " << sz << std::endl;
    }


    template<class InputType>
    void TMP(InputType && forwarded)
    {
        TMP_impl(std::forward<InputType>(forwarded), std::is_pointer<std::remove_reference_t<InputType>>{});
    }



    template<typename InputType>
    void Set(InputType&& type)
    {
        Set_impl(std::forward<InputType>(type), std::is_pointer<std::remove_reference_t<InputType>>{});
    }


    void ExternalAssign(const std::string &val);

    std::string Get() const;
    std::string GetFromIndex(size_type index) const;
    std::string GetType() const;

    size_t Address() const;
    void* RawPtr() const;
    Type RawType() const;
};

}
