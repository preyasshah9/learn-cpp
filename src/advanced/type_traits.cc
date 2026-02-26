/* This example walks through the usage of type traits. Type
traits are persuasive in the template metaprogramming. We will explore 
some examples on how they will be useful.
*/

#include <type_traits>
#include <iostream>

// Example 1: Reference collapsing with std::remove_reference.
template <typename T>
void inspect(T&& value) {
    std::cout << "T is reference? " << std::is_reference<T>::value << std::endl;
    std::cout << "T is const?" << std::is_const<T>::value << std::endl;
    using Plain = typename std::remove_const<typename std::remove_reference<T>::type>::type;
    std::cout << "T: " << typeid(T).name() << std::endl;
    std::cout << "Plain Type: " << typeid(Plain).name() << std::endl;
}

// Example 2: Use std::remove_const type trait
// In templated code, you want to create a mutable copy of variable
// and modify it. However, you might be passed a `const` type, and you need
// an ability to deduce a non-const type from it. That's where std::remove_const
// will be useful.

template <typename T>
void remove_const(T input) {
    using MutableT = typename std::remove_const<T>::type;
    MutableT output = input;
    std::cout << "Output is: " << output << std::endl;
    // modify output
}

// Example 3: For universal references, use std::decay.
// Using SFINAE to ensure we are only enabling for integral type.
template <typename T, typename U = std::enable_if_t<std::is_integral_v<T>>>
void modify_integral(T&& input) {
    using MutableT = typename std::decay<T>::type;
    // Other option is:
    // using MutableT = std::remove_const<std::remove_reference<T>::type>::type;
    MutableT output = std::forward<T>(input);
    ++output;
}

template <typename, typename = void>
struct has_pre_increment: std::false_type {};

template <typename T>
struct has_pre_increment <T, std::void_t<decltype(++std::declval<T&>())>>
    : std::true_type {};

template <typename T>
inline constexpr bool has_pre_increment_v = has_pre_increment<T>::value;

// For universal references, use std::decay.
// Using SFINAE to keep this generic for any types that support pre_increment.
template <typename T, typename U = std::enable_if_t<has_pre_increment_v<T>>>
void modify(T&& input) {
    std::cout << "Input: " << input << std::endl;
    using MutableT = typename std::decay<T>::type;
    MutableT output = std::forward<T>(input);
    ++output;
    std::cout << "Output " << output << std::endl;
}

// Main
int main() {
    const int x = 12;
    const int& y = x;
    
    inspect(x);
    inspect(12);
    inspect(y);

    remove_const(x);
    remove_const(y); // This works;

    modify_integral(12);
    // Fails to compile: modify_integral(3.14);

    modify(10);
    modify(3.14);
}
