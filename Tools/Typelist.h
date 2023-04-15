#pragma once

#include <type_traits>

template<typename... Args>
struct typelist {};

template<typename List>
struct is_empty
{
    static constexpr bool value = false;
};

template<>
struct is_empty<typelist<>>
{
    static constexpr bool value = true;
};

template<typename List>
static constexpr bool is_empty_v = is_empty<List>::value;


//front
template<typename List>
struct front;

template<typename Head, typename... Tail>
struct front<typelist<Head, Tail...>>
{
    using type = Head;
};

template<typename List>
using front_t = typename front<List>::type;


//popfront
template<typename List>
struct pop_front;

template<typename Head, typename... Tail>
struct pop_front<typelist<Head, Tail...>>
{
    using type = typelist<Tail...>;
};

template<typename List>
using pop_front_t = typename pop_front<List>::type;





template<bool Condition, typename TrueType, typename FalseType>
struct if_then_else;

template<typename TrueType, typename FalseType>
struct if_then_else<true, TrueType, FalseType> {
    using type = TrueType;
};

template<typename TrueType, typename FalseType>
struct if_then_else<false, TrueType, FalseType> {
    using type = FalseType;
};

template<bool Condition, typename TrueType, typename FalseType>
using if_then_else_t = typename if_then_else<Condition, TrueType, FalseType>::type;




template<typename List, typename Type, bool IsEmpty = is_empty_v<List>>
struct contains;

template<typename List, typename Type>
struct contains<List, Type, true> : std::integral_constant<bool, false>
{
};

template<typename List, typename Type>
struct contains<List, Type, false> : if_then_else_t<
    std::is_same_v<front_t<List>, Type>,
    std::integral_constant<bool, true>,
    contains<pop_front_t<List>, Type>
> {
};

template<typename List, typename Type>
static constexpr bool contains_v = contains<List, Type>::value;


template<typename List, typename Type, bool IsEmpty = is_empty_v<List>>
struct contains_base_of;

template<typename List, typename Type>
struct contains_base_of<List, Type, true> : std::integral_constant<bool, false>
{
};
template<typename List, typename Type>
struct contains_base_of<List, Type, false> : if_then_else_t<
    std::is_base_of_v<front_t<List>, Type>,
    std::integral_constant<bool, true>,
    contains_base_of<pop_front_t<List>, Type>
> {
};

template<typename List, typename Type>
static constexpr bool contains_base_of_v = contains_base_of<List, Type>::value;