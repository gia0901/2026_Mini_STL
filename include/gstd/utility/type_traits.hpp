#pragma once

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
    // In modern C++ (C++14 onwards), utilities like move, forward, and swap are typically constexpr so they can be evaluated at compile time.
    // Noexcept được sử dụng để báo cho compiler biết rằng hàm này không ném ra exception.
    //
    template<typename T>
    constexpr remove_reference_t<T>&& move(T&& arg) noexcept {
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
    constexpr T&& forward(remove_reference_t<T>& arg) noexcept {
        // Nếu T = int&
        // return int& && -> collapse thành int&
        
        // Nếu T = int
        // return int&&
        return static_cast<T&&>(arg);
    }

    // Case 2: arg là rvalue. 
    // Ví dụ: gstd::forward<std::string>("Hello")           -> T = std::string
    //        gstd::forward<std::string>(gstd::move(str))   -> T = std::string

    template<typename T>
    constexpr T&& forward(remove_reference_t<T>&& arg) noexcept {
        return static_cast<T&&>(arg);
    }

    // =========================================================
    // 4. swap
    // =========================================================
    template<typename T>
    constexpr void swap(T& a, T& b) noexcept {
        T temp = gstd::move(a);
        a = gstd::move(b);
        b = gstd::move(temp);
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
        pair(U1&& a, U2&& b) : first(gstd::forward<U1>(a)), second(gstd::forward<U2>(b)) {}

        // copy constructor
        pair(const pair& other) : first(other.first), second(other.second) {}

        // move constructor
        pair(pair&& other) : first(gstd::move(other.first)), second(gstd::move(other.second)) {}

        // copy assignment
        pair& operator=(const pair& other) {
            if (this != &other) {
                first = other.first;
                second = other.second;
            }
            return *this;
        }

        // move assignment
        pair& operator=(pair&& other) {
            if (this != &other) {
                first = gstd::move(other.first);
                second = gstd::move(other.second);
            }
            return *this;
        }

        bool operator==(const pair& other) const {
            return (first == other.first) && (second == other.second);
        }

        bool operator>(const pair& other) const {
            return (first > other.first) || (first == other.first && second > other.second); 
        }

        bool operator<(const pair& other) const {
            return (first < other.first) || (first == other.first && second < other.second);
        }

        bool operator>=(const pair& other) const {
            return (*this == other) || (*this > other);
        }

        bool operator<=(const pair& other) const {
            return (*this == other) || (*this < other);
        }

        // swap (compiler sẽ tự động tìm ra kiểu dữ liệu T)
        void swap(pair& other) {
            gstd::swap(first, other.first);
            gstd::swap(second, other.second);
        }
    };

    // =========================================================
    //  Helper: make_pair
    // =========================================================
    template<typename T1, typename T2>
    pair<remove_reference_t<T1>, remove_reference_t<T2>> make_pair(T1&& a, T2&& b) {
        return pair<remove_reference_t<T1>, remove_reference_t<T2>>(
            gstd::forward<T1>(a), gstd::forward<T2>(b)
        );
    }

    // =========================================================
    // 6. iterator_traits
    // =========================================================
    
}