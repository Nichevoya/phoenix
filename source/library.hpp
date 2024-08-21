#pragma once 

#include <memory>
#include <iostream>

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
                    T load(std::string const &entry_point) const & { return (T)dlsym(_handler, entry_point.c_str()); }

                    int close() const & { return dlclose(_handler); }

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
                    library(const std::string &file, const std::string &entry_point = "entry_point") : _file(file), _entry_point(entry_point) {}
                    ~library()
                    {
                        if (_handler.get() != nullptr) _handler.close();
                    }

                    template <typename T>
                    std::unique_ptr<T> load_unique(void) &
                    {
                        using unique = typename smart_type_allocator<T>::unique;
                        if (!_handler.open(_file)) throw std::runtime_error(_handler.error());
                        return _handler.load<unique>(_entry_point)();
                    }

                    template <typename T>
                    std::unique_ptr<T> load_unique(std::string &file, const std::string &entry_point = "entry_point") &
                    {
                        using unique = typename smart_type_allocator<T>::unique;
                        if (!_handler.open(file)) throw std::runtime_error(_handler.error());
                        _file = file;
                        return _handler.load<unique>(_entry_point)();
                    }

                    template <typename T>
                    std::shared_ptr<T> load_shared(void) &
                    {
                        using shared = typename smart_type_allocator<T>::shared;
                        if (!_handler.open(_file)) throw std::runtime_error(_handler.error());
                        return _handler.load<shared>(_entry_point)();
                    }

                    template <typename T>
                    std::shared_ptr<T> load_shared(std::string &file, const std::string &entry_point = "entry_point") &
                    {
                        using shared = typename smart_type_allocator<T>::shared;
                        if (!_handler.open(file)) throw std::runtime_error(_handler.error());
                        _file = file;
                        return _handler.load<shared>(_entry_point)();
                    }

                    void free(void) const & { _handler.close(); }

                    const std::string &path(void) const & { return _file; }

                    const std::string &entry_point(void) const & { return _entry_point; }

                    const std::string extension(void) const & { return _file.substr(_file.find_last_of('.') + 1); }

                    const std::string name(void) const & { return _file.substr(_file.find_last_of('/') + 1, _file.size()); }

                protected:                
                private:
                    handler _handler;
                    std::string _file, _entry_point;
            };

        } // load

    } // phoenix

#elif WIN32

    namespace phoenix {

        namespace load {

            class handler {
                public:
                    handler() = default;
                    handler(const handler &) = delete;
                    handler(const handler &&) = delete;
                    ~handler() = default;

                    void *get(void) const & { return _handler; }

                    void *const &open(const std::string &file) & { return (_handler = LoadLibrary(file.c_str())); }

                    template <typename T>
                    T load(const std::string &entry_point) const & { return (T)GetProcAddress((HMODULE)_handler, entry_point.c_str()); }

                    int close(void) const & { return FreeLibrary((HMODULE)_handler); }

                    const std::string error(void) const & 
                    {
                        DWORD error_message = GetLastError();

                        LPSTR buffer = nullptr;
                        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                                    nullptr, error_message, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                                    reinterpret_cast<LPSTR>(&buffer), 0, nullptr);
                        
                        std::string message(buffer, size);

                        LocalFree(buffer);

                        return message;
                    }

                protected:
                private:
                    void *_handler = nullptr;
            };

            class library {
                public:
                    library() = default;
                    library(const library &) = delete;
                    library(const library &&) = delete;
                    library(const std::string &file, const std::string &entry_point = "entry_point") : _file(file), _entry_point(entry_point) {}
                    ~library()
                    {
                        if (_handler.get() != nullptr) _handler.close();
                    }

                    template <typename T>
                    T load(void) & 
                    {
                        using ptr = std::add_pointer<T>::type;
                        using function_ptr = std::add_pointer<ptr ()>::type;
                        if (!_handler.open(_file)) throw std::runtime_error(_handler.error());
                        const T &instance = *(*_handler.load<function_ptr>(_entry_point))();
                        return instance;
                    }

                    template <typename T>
                    std::unique_ptr<T> load_unique(void) &
                    {
                        using ptr = std::add_pointer<T>::type;
                        using function_ptr = std::add_pointer<ptr ()>::type;
                        if (!_handler.open(_file)) throw std::runtime_error(_handler.error());
                        const auto &instance = *(*_handler.load<function_ptr>(_entry_point))();
                        return std::make_unique<T>(instance);
                    }

                    template <typename T>
                    std::shared_ptr<T> load_shared(void) &
                    {
                        using ptr = std::add_pointer<T>::type;
                        using function_ptr = std::add_pointer<ptr ()>::type;
                        if (!_handler.open(_file)) throw std::runtime_error(_handler.error());
                        const auto &instance = *(*_handler.load<function_ptr>(_entry_point))();
                        return std::make_shared<T>(instance);
                    }

                    void free(void) const & { _handler.close(); }

                    const std::string &path(void) const & { return _file; }

                    const std::string &entry_point(void) const & { return _entry_point; }

                    const std::string extension(void) const & { return _file.substr(_file.find_last_of('.') + 1); }

                    const std::string name(void) const & { return _file.substr(_file.find_last_of('\\') + 1, _file.size()); }

                protected:                
                private:
                    handler _handler;
                    std::string _file, _entry_point;
            };

        } // load
        
    } // phoenix
    

#endif
