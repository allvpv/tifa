#include <stdio.h>

template<typename T>
concept dereferencable = requires(T v) {
    *v;
};

int main() {
    static_assert(dereferencable<int*>);
    static_assert(!dereferencable<int>);
}
