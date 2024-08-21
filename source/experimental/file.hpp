#pragma once

#include <fstream>
#include "string.hpp"

namespace phoenix {

    namespace experimental {

        class file {
            public:
                file() = default;
                file(const std::string &file) : _file(file) { create(file); }
                file(const string &file) : _file(file.value()) { create(file); }
                ~file() { _file.close(); }

                void create(const char *file)
                {
                    if (_file.is_open()) return;
                    _file.open(file, std::fstream::out);
                }

                void create(const std::string &file)
                {
                    if (_file.is_open()) return;
                    _file.open(file, std::fstream::out);
                }

                void create(const string &file)
                {
                    if (_file.is_open()) return;
                    _file.open(file.value(), std::fstream::out);
                }

                void write(const char *data)
                {
                    _file << data;
                }

                void write(const std::string &data)
                {
                    _file << data;
                }

                void write(const string &data)
                {
                    _file << data.value();
                }
        
            protected:
            private:
                std::fstream _file;
        };
        
    } // experimental

} // phoenix
