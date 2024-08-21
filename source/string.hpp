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

            std::size_t size(void) const & 
            {
                return _stream.str().size();
            }

            const string &sanitize(const std::string &illegal_characters, const std::size_t &size) & 
            {
                if (this->size() != size) 
                    return *this;

                std::string sanitized = _stream.str();

                for (const char &c : illegal_characters) {
                    sanitized.erase(
                        std::remove(sanitized.begin(), sanitized.end(), c), 
                        sanitized.end()
                    );
                }

                this->input(sanitized);
                return *this;
            }
            
            string contains(const std::string &occurence) & 
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

            const string &lowercase(void) & 
            {
                std::string lower = _stream.str();
                std::transform(lower.begin(), lower.end(), lower.begin(),
                [] (unsigned char c) { return std::tolower(c); });

                this->input(lower);
                return *this;
            }

            const string &uppercase(void) & 
            {
                std::string upper = _stream.str();
                std::transform(upper.begin(), upper.end(), upper.begin(),
                [] (unsigned char c) { return std::toupper(c); });

                this->input(upper);
                return *this;
            }

            const string &endl(void) & 
            {
                _stream << std::endl;
                return *this;
            }

        protected:
        private:
            std::stringstream _stream;
    };

} // phoenix
