/*
This code shows the differences in string v/s string_view.

At a high level:
std::string      - owns memory
std::string_view - non-owning view (pointer + length)

Let's walk through step by step examples of using both.
*/

# include <iostream>
# include <string>

void func_str(const std::string& test) {
    std::cout << test << std::endl;
}

void func_str_view(std::string_view test) {
    std::cout << test << std::endl;
}

/*

Span: Size aware view without pointer decay (C++23).
void func_buf_span(std::span<const char> buf) {
    std::cout << buf.size() << std::endl;
}

Bad examples: Dangling pointer UB, returning a memory location being destroyed.
std::string_view bad() {
    std::string s = "hi this should print";
    return s;
}

// String view as member.
struct User {
    std::string_view name; // ❌ dangling risk
};

*/
int main() {
    std::string s = "helloworld";
    char v[20] = "kesarben"; // Includes null termination.

    // No Copy
    func_str(s);

    // rvalue - allocation happens before passing to const&.
    func_str("tester");

    // alloc + strlen
    func_str(v);

    // alloc + copy
    func_str(s.substr((0, 5)));

    // All of the string view usages don't result in copy; improving performance.
    /*
    Refresh memory: String View struct
    struct string_view {
        const char* ptr;
        size_t size;
    };

    string_view(s.data(), s.size());
    string_view(s.data()) <- length will be done by strlen.
    */
    func_str_view(s);

    // No heap allocation happens while supplying rvalues.
    func_str_view("tester");

    // substring doesn't result in allocation.
    func_str_view(s.substr((0, 5)));

    // Due to pointer decay, advisable to pass strlen to prevent UB.
    func_str_view({v, strlen(v)});

    //If string not null-terminated, UB. See example below
    /*
    char u[3] = {'t', 'u', 'v'};
    func_str_view(u);
    */

    //func_buf_span(v);

    /*
    // These lines will have (UB). 
    std::string_view u = bad();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << u << std::endl;
    */
    return 0;
}