#pragma once

#include "std.hpp"

namespace phoenix {

    class string {
        public:
            friend std::ostream &operator<<(std::ostream &os, const string &other) { return os << other.get(); }

            string() {}

            template <typename ...Args>
            string(Args &&...args)
            {
                (_stream << ... << args);
                set(_stream.str());
            }

            template <typename ...Args>
            string(const Args &&...args)
            {
                (_stream << ... << args);
                set(_stream.str());
            }
            
            ~string() = default;

            void set(const std::string &str) { _str = str; }
            
            std::string &get(void) { return _str; }
            const std::string &get(void) const { return _str; }

            const std::stringstream &stream(void) const { return _stream; }

            template <typename ...Args>
            void input(Args &&...args)
            {
                if (!get().empty()) clear();
                (_stream << ... << args);
                set(_stream.str());
            }

            template <typename ...Args>
            void append(Args &&...args) { get().append((std::stringstream() << ... << args).str()); }

            void clear(void)
            {
                _stream.clear();
                get().clear();
            }

            std::size_t size(void) const { return get().size(); }

            //TODO - String sanitizer

            const string contains(const std::string &occurence) const
            {
                std::smatch match;
                std::regex regex(occurence);
                while (std::regex_search(get(), match, regex)) return match.str();
                return "";
            }

            string contains(const std::string &occurence)
            {
                std::smatch match;
                std::regex regex(occurence);
                while (std::regex_search(get(), match, regex)) return match.str();
                return "";
            }

            const string endl(void)
            {
                std::stringstream stream;
                stream << std::endl;
                return get().append(stream.str());
            }

        protected:
        private:
            std::string _str = "";
            std::stringstream _stream;
    };

} // phoenix
