#pragma once

#include "std.hpp"

namespace phoenix {

    template <typename T = int32_t>
    class number {
        public:
            friend std::ostream& operator<<(std::ostream& os, const number<T>& number) { return os << std::fixed << std::setprecision(number.get_precision()) << number.get(); }

            number<T> operator+(const number<T>& other) const { return number<T>(get() + other.get()); }
            number<T> operator-(const number<T>& other) const { return number<T>(get() - other.get()); }
            number<T> operator*(const number<T>& other) const { return number<T>(get() * other.get()); }
            number<T> operator/(const number<T>& other) const { return other.get() != 0 ? number<T>(get() / other.get()) : number<T>(0); }
            
            number<T> operator+=(const number<T>& other) const { return number<T>(get() += other.get()); }
            number<T> operator-=(const number<T>& other) const { return number<T>(get() -= other.get()); }
            number<T> operator*=(const number<T>& other) const { return number<T>(get() *= other.get()); }
            number<T> operator/=(const number<T>& other) const { return other.get() != 0 ? number<T>(get() /= other.get()) : number<T>(0); }

            bool operator==(const number<T>& other) const { return get() == other.get(); }
            bool operator!=(const number<T>& other) const { return get() != other.get(); }


            number(): _value(0), _precision(2) {}
            number(const T &value, const int32_t &precision = 2) : _value(value), _precision(precision) {}

            void set(const T &value) { _value = value; }

            void set_precision(int32_t precision) { _precision = precision; }
            
            T get(void) const { return _value; }

            const int32_t &get_precision(void) const & { return _precision; }

            const std::string string(void) const
            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(get_precision()) << get();
                return stream.str();
            }

            void type_action(void) { type_specific_operation(get()); }

        private:            
            void type_specific_operation(int32_t)
            {
                std::cout << "Type is int32_t." << std::endl;
            }

            void type_specific_operation(float)
            {
                std::cout << "Type is float." << std::endl;
            }

            void type_specific_operation(double)
            {
                std::cout << "Type is double." << std::endl;
            }

            template <typename U>
            void type_specific_operation(U)
            {
                std::cout << "Type is unknown or not specialized." << std::endl;
            }

            T _value;
            int32_t _precision;

    };    
    
} // phoenix

