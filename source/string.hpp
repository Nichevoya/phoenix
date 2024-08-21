#pragma once

#include <regex>
#include <iostream>

namespace phoenix {

    class string {
        public:
            friend std::ostream &operator<<(std::ostream &os, const string &other) { return os << other.value(); }
            phoenix::string &operator=(const phoenix::string &other) 
            {
                this->input(other.value());
                return *this;
            }

            operator std::string() const & { return this->value(); }

            string() = default;

            template <typename ...Args>
            string(Args &&...args) 
            {
                (_stream << ... << args);
            }

            template <typename ...Args>
            string(const Args &&...args) 
            {
                (_stream << ... << args);
            }
            
            ~string() = default;

            const std::stringstream &stream(void) const & { return _stream; }

            const std::string value(void) const & { return stream().str(); }

            template <typename ...Args>
            void append(Args &&...args)
            {
                (_stream << ... << args);
            }

            template <typename ...Args>
            void append(const Args &&...args)
            {
                (_stream << ... << args);
            }

            template <typename ...Args>
            void input(Args &&...args)
            {
                this->clear();
                (_stream << ... << args);
            }

            template <typename ...Args>
            void input(const Args &&...args)
            {
                this->clear();
                (_stream << ... << args);
            }

            void clear(void) & 
            {
                std::stringstream stream;
                _stream.swap(stream);
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

            string lowercase(void) 
            {
                std::string lower = get();
                std::transform(lower.begin(), lower.end(), lower.begin(),
                [] (unsigned char c) { return std::tolower(c); });

                return lower;
            }

            string uppercase(void) 
            {
                std::string lower = get();
                std::transform(lower.begin(), lower.end(), lower.begin(),
                [] (unsigned char c) { return std::toupper(c); });

                return lower;
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
