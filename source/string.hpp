#pragma once

#include "std.hpp"

namespace phoenix {

    class string {
        public:
            friend std::ostream &operator<<(std::ostream &os, const string &other) { return os << other.get(); }

            operator std::string() const { return this->get(); }
            operator char *() const 
            {
                char *str = std::strcpy(str, this->get().c_str());
                return str;
            }

            string() = default;

            template <typename ...Args>
            string(Args &&...args) { (_stream << ... << args); }

            template <typename ...Args>
            string(const Args &&...args) { (_stream << ... << args); }
            
            ~string() = default;

            std::string get(void) { return stream().str(); }
            const std::string get(void) const { return stream().str(); }

            std::stringstream &stream(void) { return _stream; }
            const std::stringstream &stream(void) const { return _stream; }

            template <typename ...Args>
            void input(Args &&...args)
            {
                if (!get().empty()) clear();
                (_stream << ... << args);
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
                const std::string &str = get();
                while (std::regex_search(str, match, regex)) return match.str();
                return "";
            }

            string contains(const std::string &occurence)
            {
                std::smatch match;
                std::regex regex(occurence);
                const std::string &str = get();
                while (std::regex_search(str, match, regex)) return match.str();
                return "";
            }

            const string endl(void)
            {
                this->stream() << std::endl;
                return get();
            }

        protected:
        private:
            std::stringstream _stream;
    };

} // phoenix
