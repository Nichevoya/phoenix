#pragma once

#include "string.hpp"

namespace phoenix {

    enum class colors {
        none,
        red,
        green,
        yellow,
        magenta,
        blue,
        cyan,
        white,
        black
    };

    class log {
        public:
            friend std::ostream &operator<<(std::ostream &os, const log &other) { return os << other.get(); }

            log(const string &message = "", const colors &color = colors::none) : _color(color), _message(message), _color_code(get_color_code())
            {
                _stream << "\033[" << _color_code << "m" << _message << "\033[0m";
            }

            ~log() = default;

            const string get(void) const & { return _stream.str(); }

        protected:
        private:
            const char *get_color_code(void) const &
            {
                switch (_color) {
                    case colors::black: return "30";
                    case colors::red: return "31";
                    case colors::green: return "32";
                    case colors::yellow: return "33";
                    case colors::blue: return "34";
                    case colors::magenta: return "35";
                    case colors::cyan: return "36";
                    case colors::white: return "37";
                    default: return "37";
                }
            }

            std::stringstream _stream;
            const colors &_color = colors::none;
            const std::string _message = "", _color_code = "";
    };

};
