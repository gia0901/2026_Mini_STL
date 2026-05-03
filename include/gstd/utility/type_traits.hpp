#pragma once
#include <iostream>

namespace gstd {
    // =========================================================
    // 1. remove_reference
    // =========================================================
    //
    // Mục đích:
    // --------
    // Loại bỏ reference (& hoặc &&) khỏi một kiểu dữ liệu.
    //
    // Ví dụ:
    // int&      -> int
    // int&&     -> int
    // int       -> int (không đổi)
    //
    // Đây là một "type trait" - công cụ làm việc với TYPE ở compile-time.
    //

    // Primary template
    template<typename T>
    struct remove_reference {
        using type = T;
    };
    
    // Specialization for lvalue reference (T&)
    template<typename T>
    struct remove_reference<T&> {
        using type = T;
    };

    // Specialization for rvalue reference (T&&)
    template<typename T>
    struct remove_reference<T&&> {
        using type = T;
    };

    template<typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    // =========================================================
    // 2. move
    // =========================================================
    //
    // Mục đích:
    // --------
    // Ép một object thành rvalue → cho phép "move semantics"
    //
    // IMPORTANT:
    // move KHÔNG di chuyển dữ liệu!
    // Nó chỉ "cast" sang rvalue reference.
    //
    // Việc move thật sự xảy ra khi:
    // - copy constructor / assignment operator có overload move
    //
    // Cơ chế:
    // -------
    // T&& trong template → gọi là "universal reference"
    // remove_reference_t<T> giúp bỏ & nếu có
    //
    // Sau đó ép về kiểu: T&& (rvalue reference)
    //
    template<typename T>
    remove_reference_t<T>&& move(T&& arg) noexcept {
        // Ví dụ:
        // Nếu T = int&  → remove_reference_t<T> = int
        // ReturnType = int&&
        using ReturnType = remove_reference_t<T>&&;

        // static_cast để ép kiểu về rvalue reference
        return static_cast<ReturnType>(arg);
    }

    // =========================================================
    // 3. forward
    // =========================================================
    //
    // Mục đích:
    // --------
    // "Perfect forwarding"
    // → Giữ nguyên giá trị category (lvalue / rvalue)
    //
    // Khác với move:
    // move: luôn ép sang rvalue
    // forward: giữ nguyên bản chất ban đầu
    //
    // Khi nào dùng?
    // --------------
    // Trong template function wrapper:
    //
    // template<typename T>
    // void wrapper(T&& arg) {
    //     func(forward<T>(arg)); // giữ nguyên lvalue/rvalue
    // }
    //
    // Nếu không dùng forward:
    // → tất cả biến trong function đều là lvalue → mất rvalue
    //

    // Case 1: arg là lvalue
    template<typename T>
    T&& forward(remove_reference_t<T>& arg) noexcept {
        // Nếu T = int&
        // return int& && -> collapse thành int&
        
        // Nếu T = int
        // return int&&
        std::cout << "forward(lvalue)" << std::endl;
        return static_cast<T&&>(arg);
    }

    // Case 2: arg là rvalue. 
    // Ví dụ: gstd::forward<std::string>("Hello")           -> T = std::string
    //        gstd::forward<std::string>(gstd::move(str))   -> T = std::string

    template<typename T>
    T&& forward(remove_reference_t<T>&& arg) noexcept {
        std::cout << "forward(rvalue)" << std::endl;
        return static_cast<T&&>(arg);
    }

    // =========================================================
    // 4. swap
    // =========================================================
    template<typename T>
    void swap(T& a, T& b) {
        T temp = move(a);
        a = move(b);
        b = move(temp);
    }

    // =========================================================
    // 5. pair
    // =========================================================
    template<typename T1, typename T2>
    struct pair {
        T1 first;
        T2 second;

        // default constructor
        pair() = default;

        // parameterized constructor
        pair(const T1& a, const T2& b) : first(a), second(b) {}

        // template constructor for perfect forwarding
        // - dùng forward để chỉ cast nếu a, b là rvalue, giữ nguyên tính chất nếu a,b là lvalue (tránh bị move ngoài ý muốn)
        // - ví dụ: pair<std::string, std::string> p("str_a" , "str_b");
        template<typename U1, typename U2>
        pair(U1&& a, U2&& b) : first(forward<U1>(a)), second(forward<U2>(b)) {}

        // defaults
        pair(const pair&) = default;
        pair(pair&&) = default;
        pair& operator=(const pair&) = default;
        pair& operator=(pair&&) = default;
    };

    // =========================================================
    // 6. iterator_traits
    // =========================================================

}