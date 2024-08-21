#pragma once

namespace phoenix {

    template <typename T = int32_t>
    class number {
        public:
            friend std::ostream& operator<<(std::ostream& os, const number<T>& number) { return os << std::fixed << std::setprecision(number.precision()) << number.value(); }

            number<T> operator+(const number<T> &other) const { return number<T>(value() + other.value()); }
            number<T> operator-(const number<T> &other) const { return number<T>(value() - other.value()); }
            number<T> operator*(const number<T> &other) const { return number<T>(value() * other.value()); }
            number<T> operator/(const number<T> &other) const { return other.value() != 0 ? number<T>(value() / other.value()) : number<T>(0); }
            
            number<T> operator+=(const number<T> &other) const { return number<T>(value() += other.value()); }
            number<T> operator-=(const number<T> &other) const { return number<T>(value() -= other.value()); }
            number<T> operator*=(const number<T> &other) const { return number<T>(value() *= other.value()); }
            number<T> operator/=(const number<T> &other) const { return other.value() != 0 ? number<T>(value() /= other.value()) : number<T>(0); }

            bool operator==(const number<T> &other) const { return value() == other.value(); }
            bool operator!=(const number<T> &other) const { return value() != other.value(); }


            number(): _value(0), _precision(2) {}
            number(const T &value, const int32_t &precision = 2) : _value(value), _precision(precision) {}

            T &value(void) { return _value; }
            const T &value(void) const { return _value; }
            void value(const T &value) { _value = value; }            

            void precision(int32_t precision) { _precision = precision; }
            const short &precision(void) const & { return _precision; }

            const std::string string(void) const
            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(precision()) << value();
                return stream.str();
            }

            void type_action(void) { type_specific_operation(value()); }

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
            short _precision;

    };    
    
} // phoenix

