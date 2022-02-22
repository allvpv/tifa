#pragma once
#include "impl/type_traits.hpp"

namespace tifa {
    template<class T, class U>
    concept is_same_ct = impl::is_same_ct<T, U>;

    /* Removes `const` and `volatile` qualifiers from the type */
    template<class T>
    using dequalified_t = impl::dequalified_t<T>;
}
