#pragma once

#include "number.hpp"

namespace phoenix {

    template <typename T>
    class random : public number<T> {
        public:    
            random() = delete;
            ~random() = delete;
    
        protected:
        private:
    };

    template <>
    class random<int32_t> : public number<int32_t> {
        public:
            friend std::ostream &operator<<(std::ostream &os, const random<int32_t> &value) { return os << value.value(); }
            
            random() : _rng(_seed()), _dist(-RAND_MAX, RAND_MAX) {  value(_dist(_rng)); }
            random(const int32_t &from, const int32_t &to) : _rng(_seed()), _dist(from, to) { value(_dist(_rng)); }
            ~random() = default;
    
        protected:
        private:
            std::random_device _seed;
            std::mt19937 _rng;
            std::uniform_int_distribution<int32_t> _dist;
    };

    template <>
    class random<float> : public number<float> {
        public:
            friend std::ostream &operator<<(std::ostream &os, const random<float> &value) { return os << value.value(); }
            
            random() : number<float>(0.0f), _rng(_seed()), _dist(-10.0f, 10.0f) {  value(_dist(_rng)); }
            random(const float &from, const float &to) : number<float>(0.0f), _rng(_seed()), _dist(from, to) { value(_dist(_rng)); }
            ~random() = default;
    
        protected:
        private:
            std::random_device _seed;
            std::mt19937 _rng;
            std::uniform_real_distribution<float> _dist;
    };

    template <>
    class random<double> : public number<double> {
        public:
            friend std::ostream &operator<<(std::ostream &os, const random<double> &value) { return os << value.value(); }
            
            random() : number<double>(0.0f), _rng(_seed()), _dist(-10.0f, 10.0f) {  value(_dist(_rng)); }
            random(const double &from, const double &to) : number<double>(0.0f), _rng(_seed()), _dist(from, to) { value(_dist(_rng)); }
            ~random() = default;
    
        protected:
        private:
            std::random_device _seed;
            std::mt19937 _rng;
            std::uniform_real_distribution<double> _dist;
    };

} // phoenix
