#pragma once
#include <cstdint>
#include <type_traits>

template<typename T, std::size_t Addr>
struct compile_time_address
{
    using type = T;

    constexpr T* pointer()
    {
        return reinterpret_cast<T *>(Addr);
    }
};

template<typename T>
struct run_time_address
{
    using type = T;

    run_time_address(T* address)
        : address_ (address)
    {
    }

    T* pointer() const
    {
        return address_;
    }

    T* address_;
};

template<typename Addr, std::size_t N, typename T = typename Addr::type>
struct memory_view
{
    memory_view(Addr address)
        : base (address)
    {}

    T& at(std::size_t index)
    {
        return base.pointer()[index];
    }

private:
    Addr base;
};

template<typename T, std::enable_if_t<std::is_pointer_v<T>, int> = 0>
auto address(T &&ptr)
{
    return run_time_address<std::remove_pointer_t<T>>(ptr);
}

template<typename T, std::size_t Addr>
constexpr auto static_address()
{
    return compile_time_address<T, Addr>{};
}

template<std::size_t N, typename Addr>
auto make_memory_view(Addr address)
{
    return memory_view<Addr, N>(address);
}