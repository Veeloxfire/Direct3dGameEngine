#pragma once

//template <bool b>
//struct CONST_BOOL
//{
//    static constexpr bool value = b;
//    constexpr operator bool()
//    {
//        return value;
//    }
//};

template <typename ... T>
inline constexpr bool ALWAYS_FALSE = false;


template <typename ... Ty>
struct All;

template <typename First>
struct All<First>
{
    constexpr All() noexcept
        : first()
    {}
    constexpr All(First&& f) noexcept
        : first(std::forward<First>(f))
    {}
    constexpr All(const First& f) noexcept
        : first(f)
    {}
    First first;
};

template <typename First, typename ... Ty>
struct All<First, Ty...>
{
    constexpr All() noexcept
        : first(), rest()
    {}
    constexpr All(First&& f, Ty&& ... ty) noexcept
        : first(std::forward<First>(f)), rest(std::forward<Ty>(ty)...)
    {}
    constexpr All(const First& f, const Ty& ... ty) noexcept
        : first(f), rest(ty...)
    {}
    First first;
    All<Ty...> rest;
};

template <unsigned int idx, typename all>
struct ElementType;

template <unsigned int idx>
struct ElementType<idx, All<>>
{
    static_assert(ALWAYS_FALSE<idx>, "Index out of bounds");
};

template <typename ty, typename ... all>
struct ElementType<0, All<ty, all...>>
{
    using type = ty;
};

template <unsigned int idx, typename ty, typename ... rest>
struct ElementType<idx, All<ty, rest...>> : ElementType<idx - 1, All<rest...>>
{};

template <unsigned int idx, typename ty, typename All>
struct ElementIndexHelper;

template <unsigned int idx, typename ty, typename ... all>
struct ElementIndexHelper<idx, ty, All<ty, all...>>
{
    static constexpr unsigned int getIndex() { return idx; }
};

template <unsigned int idx, typename ty, typename first, typename ... rest>
struct ElementIndexHelper<idx, ty, All<first, rest...>>
    : ElementIndexHelper<idx + 1, ty, All<rest...>>
{};

template <typename ty, typename All>
struct ElementIndex
{
    static constexpr unsigned int getIndex()
    {
        return ElementIndexHelper<0, ty, All>::getIndex();
    }
};