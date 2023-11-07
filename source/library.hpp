#pragma once

#include "std.hpp"

#ifdef UNIX

    namespace phoenix {

        namespace load {

            template <class T>
            class smart_type_allocator {
                public:
                    typedef std::unique_ptr<T> (*unique)();
                    typedef std::shared_ptr<T> (*shared)();
            };

            class handler {
                public:
                    handler() = default;
                    handler(const handler &) = delete;
                    handler(const handler &&) = delete;
                    ~handler() = default;

                    void *get(void) const { return _handler; }

                    void *const &open(const std::string &file) & { return (_handler = dlopen(file.c_str(), RTLD_LAZY)); }

                    template <typename T>
                    T load(std::string const &entry_point) const { return (T)dlsym(_handler, entry_point.c_str()); }

                    int close() const { return dlclose(_handler); }

                    const char *error() const & { return dlerror(); }

                protected:
                private:
                    void *_handler = nullptr;
            };

            class library {
                public:
                    library() = default;
                    library(const library &) = delete;
                    library(const library &&) = delete;
                    library(const std::string &file, const std::string &entry_point = "entry_point") : _file(file), _path(file.substr(file.find_last_of('/') + 1, file.size())), _entry_point(entry_point) {}
                    ~library()
                    {
                        if (_handler.get() != nullptr) _handler.close();
                    }

                    template <typename T>
                    std::unique_ptr<T> load_unique(void)
                    {
                        using unique = typename smart_type_allocator<T>::unique;
                        if (!_handler.open(_file)) throw std::runtime_error(_handler.error());
                        std::cout << "Handled library: " << name() << std::endl;
                        return _handler.load<unique>(_entry_point)();
                    }

                    template <typename T>
                    std::unique_ptr<T> load_unique(const std::string &file, const std::string &entry_point = "entry_point")
                    {
                        using unique = typename smart_type_allocator<T>::unique;
                        if (!_handler.open(file)) throw std::runtime_error(_handler.error());
                        _file = file;
                        _path = _file.substr(_file.find_last_of('/') + 1, _file.size());
                        std::cout << "Handled library: " << name() << std::endl;
                        return _handler.load<unique>(_entry_point)();
                    }

                    template <typename T>
                    std::shared_ptr<T> load_shared(void)
                    {
                        using shared = typename smart_type_allocator<T>::shared;
                        if (!_handler.open(_file)) throw std::runtime_error(_handler.error());
                        std::cout << "Handled library: " << name() << std::endl;
                        return _handler.load<shared>(_entry_point)();
                    }

                    template <typename T>
                    std::shared_ptr<T> load_shared(const std::string &file, const std::string &entry_point = "entry_point")
                    {
                        using shared = typename smart_type_allocator<T>::shared;
                        if (!_handler.open(file)) throw std::runtime_error(_handler.error());
                        _file = file;
                        _path = _file.substr(_file.find_last_of('/') + 1, _file.size());
                        std::cout << "Handled library: " << name() << std::endl;
                        return _handler.load<shared>(_entry_point)();
                    }

                    void free(void) const { _handler.close(); }

                    const std::string &path(void) const & { return _path; }

                    const std::string &name(void) const & { return _file; }

                private:
                    handler _handler;
                    std::string _file, _path, _entry_point;
            };

        } // Load

    } // phoenix

#elif WIN32

#endif
