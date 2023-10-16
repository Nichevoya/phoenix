#pragma once

#include "std.hpp"

namespace phoenix {
    
    class input {
        public:                
            friend std::ostream &operator<<(std::ostream &os, const input &input) { return os << input.inputMessage(); }
            operator std::string() const  { return get(); }

            input(const input &) = delete;
            input(const input &&) = delete;
            input(const std::string &message = "") : _message(message)
            {
                if (!_message.empty()) outputMessage();
                getLine();
                inputMessage();
            }
            ~input() = default;

            std::string &get(void) { return _input; }
            const std::string &get(void) const { return _input; }

            void outputMessage(void) const { std::cout << _message << std::endl; }
            const std::string &inputMessage(void) const { return get(); }

            void getLine(void) { std::getline(std::cin, get()); }
        
        protected:
        private:
            std::string _input;
            const std::string &_message = "";
    };

} // phoenix
