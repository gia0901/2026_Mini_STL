#pragma once

namespace gstd {
    // 1. remove_reference: T&, T&& -> T
    template<typename T>
    struct remove_reference<T&> {
        using type = T; // strip reference for lvalue reference
    }

    template<typename T>
    struct remove_reference<T&&> {
        using type = T; // strip reference for rvalue reference
    }

    // C++11 style alias for convenience: 
    template<typename T>
    using remove_reference_t = typename remove_reference<T>::type;


    // 2. move
    template<typename T>
    remove_reference_t<T>&& move(T&& arg) noexcept {
        using ReturnType = remove_reference_t<T>&&;  // strip T's reference and add &&
        return static_cast<ReturnType>(arg);
    }


    // 3. forward: handled by collapsing rules
    template<typename T>
    T&& forward(remove_reference_t<T>& arg) noexcept {
        return static_cast<T&&>(arg);
    }

    template<typename T>
    T&& forward(remove_reference_t<T>&& arg) noexcept {
        return static_cast<T&&>(arg);
    }
}