#pragma once 

#include <memory>

namespace phoenix {

    namespace experimental {

        using unique_void_ptr = std::unique_ptr<void, void (*)(const void *)>;

        template<typename T>
        T &get_unique_void(const unique_void_ptr &ptr) 
        {
            return *static_cast<T *>(ptr.get());
        }

        template<typename T>
        unique_void_ptr unique_void(T *ptr) 
        {
            return unique_void_ptr(ptr, [] (const void *data) {
                delete static_cast<const T *>(data);
            });
        }

        template<typename T, typename... Args>
        unique_void_ptr make_unique_void(Args&&... args) 
        {
            return unique_void(new T(std::forward<Args>(args)...));
        }
        
    } // experimental

} // phoenix
