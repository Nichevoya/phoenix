#pragma once

#include "std.hpp"

namespace phoenix {

    namespace experimental {

        using unique_void_ptr = std::unique_ptr<void, void (*)(void const *)>;

        template<typename T>
        auto unique_void(T *ptr) -> unique_void_ptr
        {
            return unique_void_ptr(ptr, [](void const *data) {
                delete static_cast<T const *>(data);
            });
        }

        template<typename T, typename... Args>
        auto make_unique_void(Args&&... args) -> unique_void_ptr
        {
            return unique_void(new T(std::forward<Args>(args)...));
        }
        
    } // experimental

} // phoenix
