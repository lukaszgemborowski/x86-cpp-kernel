#pragma once
#include <cstdint>
#include <type_traits>

template<typename T, std::size_t Addr>
struct compile_time_address
{
    using type = T;

    constexpr T* pointer() noexcept
    {
        return reinterpret_cast<T *>(Addr);
    }
};

template<typename T>
struct run_time_address
{
    using type = T;

    run_time_address(T* address) noexcept
        : address_ (address)
    {
    }

    T* pointer() const noexcept
    {
        return address_;
    }

    T* address_;
};

template<typename Addr, std::size_t N, typename T = typename Addr::type>
struct memory_view
{
    constexpr memory_view(Addr address)
        : base (address)
    {}

    constexpr T& at(std::size_t index)
    {
        return base.pointer()[index];
    }

    constexpr const T& at(std::size_t index) const
    {
        return base.pointer()[index];
    }

    constexpr T& operator[](std::size_t index)
    {
        return at(index);
    }

    constexpr const T& operator[](std::size_t index) const
    {
        return at(index);
    }

    template<std::size_t I>
    T& get()
    {
        static_assert(I < N, "index out of bounds");
        return at(I);
    }

    template<std::size_t I>
    const T& get() const
    {
        static_assert(I < N, "index out of bounds");
        return at(I);
    }

    constexpr auto begin() noexcept
    {
        return base.pointer();
    }

    constexpr auto end() noexcept
    {
        return base.pointer() + N;
    }

private:
    Addr base;
};

template<typename T, std::enable_if_t<std::is_pointer_v<T>, int> = 0>
auto address(T &&ptr)
{
    return run_time_address<std::remove_pointer_t<T>>(ptr);
}

template<typename T, std::size_t N>
auto address(T (&arr) [N])
{
    return run_time_address<T>(&arr[0]);
}

template<typename T, std::enable_if_t<std::is_reference_v<T>, int> = 0>
auto address_of(T &&obj)
{
    return run_time_address<std::remove_reference_t<T>>(&obj);
}

template<typename T, std::size_t Addr>
constexpr auto static_address()
{
    return compile_time_address<T, Addr>{};
}

template<std::size_t N, typename Addr>
constexpr auto make_memory_view(Addr address)
{
    return memory_view<Addr, N>(address);
}

template<std::size_t N, typename T>
auto make_memory_view(T (&arr)[N])
{
    auto addr = address(arr);
    return memory_view<decltype(addr), N>(addr);
}