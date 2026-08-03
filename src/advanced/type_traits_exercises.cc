#include <iostream>
#include <string>
#include <type_traits>
#include <utility>


template <typename T>
constexpr std::string_view type_name()
{
#if defined(__clang__)
    std::string_view p = __PRETTY_FUNCTION__;
    auto start = p.find("T = ") + 4;
    auto end = p.find(']', start);
    return p.substr(start, end - start);

#elif defined(__GNUC__)
    std::string_view p = __PRETTY_FUNCTION__;
    auto start = p.find("with T = ") + 9;
    auto end = p.find(';', start);
    return p.substr(start, end - start);

#elif defined(_MSC_VER)
    std::string_view p = __FUNCSIG__;
    auto start = p.find("type_name<") + 10;
    auto end = p.find(">(void)", start);
    return p.substr(start, end - start);

#else
    return "unknown";
#endif
}

template <typename T>
void inspect(T&& value, const char* name)
{
    using NoRef = std::remove_reference_t<T>;
    using Decay = std::decay_t<T>;

    std::cout << name << "\n";
    std::cout << "  T: " << type_name<T>()
              << " | remove_ref: " << type_name<NoRef>()
              << " | decay: " << type_name<Decay>()
              << "\n\n";

    (void)value;
}

void foo(int) {}

int main()
{
    int x = 10;
    const int cx = 20;
    std::string s = "hello";
    int arr[5];


    // Expected:
    // x -> T=int&, remove_ref=int, decay=int
    inspect(x, "1) int lvalue");

    // Expected:
    // cx -> T=const int&, remove_ref=const int, decay=int
    inspect(cx, "2) const int lvalue");

    // Expected:
    // 42 -> T=int, remove_ref=int, decay=int
    inspect(42, "3) int rvalue");

    // Expected:
    // move(s) -> T=string, remove_ref=string, decay=string
    inspect(std::move(s), "4) moved string");

    // Expected:
    // arr -> T=int(&)[5], remove_ref=int[5], decay=int*
    inspect(arr, "5) array");

    // Expected:
    // foo -> T=void(&)(int), remove_ref=void(int), decay=void(*)(int)
    inspect(foo, "6) function");

    return 0;
}