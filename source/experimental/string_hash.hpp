#pragma once 

namespace phoenix {

    namespace experimental {

        /**
         * @brief Convert c string to its integral hash equivalent.
         * 
         * @param str Source str from which hashing takes place.
         * @param h Recursive calling index.
         * @return constexpr unsigned int. 
         */
        constexpr unsigned int hash(const char *str, int h = 0)
        {
            return !str[h] ? 5381 : (hash(str, h+1) * 33) ^ str[h];
        }
        
    } // experimental

} // phoenix
