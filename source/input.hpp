#pragma once

#include "std.hpp"

namespace phoenix {
    
    class input {
        public:                
            friend std::ostream &operator<<(std::ostream &os, const input &input) { return os << input.get(); }
            operator std::string() const  { return _input; }

            input(const input &) = delete;
            input(const input &&) = delete;
            input(const std::string &message = "", const std::string &prompt = "") : _message(message), _prompt(prompt) 
            {
                if (!_message.empty()) 
                    output_message();
                
                if (!prompt.empty())
                    this->prompt();
                
                getline();
            }

            ~input() = default;

            const std::string &get(void) const { return _input; }
            const std::string &get(const std::string &message = "", const std::string &prompt = "") 
            {
                _input = "";
                _message = message;
                _prompt = prompt;

                if (!_message.empty()) 
                    output_message();
                
                if (!prompt.empty())
                    this->prompt();
                
                getline();

                return _input;
            }

            void prompt(void) const { std::cout << _prompt; }
            void output_message(void) const { std::cout << _prompt << _message << std::endl; }

            void getline(void) { std::getline(std::cin, _input); }
        
        protected:
        private:
            std::string _input = "", _message = "", _prompt = "";
    };

} // phoenix
