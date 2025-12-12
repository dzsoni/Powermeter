#ifndef FUNCTIONHOLDER_H
#define FUNCTIONHOLDER_H

#include "IFunctionHolder.h"
#include <WString.h>
#include <string>
#include <functional>
#include <Arduino.h>
#include <type_traits>
#include "enum_CommandRepo.h"
#include "StringConversions.h"

//#define DEBUG_FUNCTIONHOLDER //comment out or not 
#ifdef  DEBUG_FUNCTIONHOLDER
#define _FUNCTHOLDER_PP(a) Serial.print(a);
#define _FUNCTHOLDER_PL(a) Serial.println(a);
#else
#define _FUNCTHOLDER_PP(a)
#define _FUNCTHOLDER_PL(a)
#endif

template <typename T>
class FunctionHolder : public IFunctionHolder
{
private:
    std::shared_ptr<T> Tptr;

public:
    FunctionHolder(std::shared_ptr<T> shrptr);
    ~FunctionHolder() {};
    String run(std::vector<String> params) override;

    template <typename Z>
    struct function_traits;

    template <typename R, typename... Args>
    struct function_traits<std::function<R(Args...)> *>
    {
        static const size_t argnum = sizeof...(Args);
        using return_type = R;
        using args_tuple_type = std::tuple<Args...>;
        template <std::size_t N>
        using arg_type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };
    template <typename R, typename... Args>
    struct function_traits<std::function<R(Args...)>>
    {
        static const size_t argnum = sizeof...(Args);
        using return_type = R;
        using args_tuple_type = std::tuple<Args...>;
        template <std::size_t N>
        using arg_type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };
    template <typename R, typename... Args>
    struct function_traits<std::function<R(Args...)>&>
    {
    static const size_t argnum = sizeof...(Args);
    using return_type = R;
    using args_tuple_type = std::tuple<Args...>;
    template <std::size_t N>
    using arg_type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };
    template <typename R, typename... Args>
    struct function_traits<const std::function<R(Args...)>&>
    {
    static const size_t argnum = sizeof...(Args);
    using return_type = R;
    using args_tuple_type = std::tuple<Args...>;
    template <std::size_t N>
    using arg_type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };
    // --- tuple_from_strings: convert vector<string> to tuple<Args...> ---
    template <typename Tuple, std::size_t... I>
    Tuple tuple_from_strings(const std::vector<String> &v, std::index_sequence<I...>)
    {
        return Tuple{from_string<typename std::tuple_element<I, Tuple>::type>(v[I])...};
    }
    template <typename Func>
    String apply_functions(Func func, const std::vector<String> &args)
    {
        String response = "";
        _FUNCTHOLDER_PL(String(__FUNCTION__) + " expected parameter num:" +
                        String(function_traits<decltype(func)>::argnum) + " real:" +
                        String(args.size()));
        using traits = function_traits<Func>;
        using tuple_type = typename traits::args_tuple_type;
        using return_type = typename traits::return_type;
        constexpr std::size_t N = std::tuple_size<tuple_type>::value;
        if (args.size() != N)
        {
            Serial.println(String(__FUNCTION__) + ":Argument count mismatch");
            return response;
        }
        auto tuple_args = tuple_from_strings<tuple_type>(args, std::make_index_sequence<N>{});
        if constexpr (std::is_same_v<return_type, void>)
        {
            std::apply(func, tuple_args);
            return "";
        }
        else
        {
            return to_string_result(std::apply(func, tuple_args));
        }
    }
    
};



template <typename T>
FunctionHolder<T>::FunctionHolder(std::shared_ptr<T> shrptr)
{
    if (shrptr)
        Tptr = shrptr;
    else
        _FUNCTHOLDER_PL("FunctionHolder::FunctionHolder: shrptr is null");
}

template <typename T>
String FunctionHolder<T>::run(std::vector<String> params)
{
    // std::shared_ptr<T> fpt = std::static_pointer_cast<T>(IFunctionHolder::voidptr);
    if (Tptr)
        return apply_functions(*Tptr, params);
    else
        _FUNCTHOLDER_PL("FunctionHolder::run: Tptr is null");
    return String();
}

#endif /* FUNCTIONHOLDER_H */
