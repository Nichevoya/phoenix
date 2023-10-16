#pragma once

namespace phoenix {

    namespace experimental {

        class file {
            public:
                file() {}
                file(const std::string &file) : _file(file) { create(file); }
                file(const string &file) : _file(file.get()) { create(file); }
                ~file() { _file.close(); }

                void create(const char *file)
                {
                    if (_file.is_open()) return;
                    _file.open(string(file).get(), std::fstream::out);
                }

                void create(const std::string &file)
                {
                    if (_file.is_open()) return;
                    _file.open(file, std::fstream::out);
                }

                void create(const string &file)
                {
                    if (_file.is_open()) return;
                    _file.open(file.get(), std::fstream::out);
                }

                void write(const char *data)
                {
                    _file << string(data);
                }

                void write(const std::string &data)
                {
                    _file << data;
                }

                void write(const string &data)
                {
                    _file << data.get();
                }
        
            protected:
            private:
                std::fstream _file;
        };
        
    } // experimental

} // phoenix
