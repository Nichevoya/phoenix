#pragma once

#include "library.hpp"

#include <algorithm>

namespace phoenix {

    namespace load {

        template <typename Signature>
        class plugin {
            public:
                using wrapper = std::pair<std::unique_ptr<const phoenix::load::library>, std::shared_ptr<void>>;

                struct information {
                    std::string name;
                    Signature signature;
                    std::string entry_point = "entry_point";
                    bool loaded = false;
                };

                plugin() = default;
                plugin(const plugin &) = delete;
                plugin(const plugin &&) = delete;
                explicit plugin(const std::string &path, const std::vector<information> &information) : _directory(path), _libraries(information) {}

                /* --------------------------------- setters -------------------------------- */

                void directory(const std::string &path) { _directory = path; }
                void missing(const std::vector<Signature> &missing) { _missing = missing; }
                void libraries(const std::vector<information> &information) { _libraries = information; }

                /* --------------------------------- getters -------------------------------- */

                const std::string &directory(void) { return _directory; }
                std::vector<Signature> &missing(void) { return _missing; }
                std::vector<information> &libraries(void) { return _libraries; }

                std::unordered_map<Signature, wrapper> &all(void) { return _plugins; }

                template <typename T>
                std::pair<const phoenix::load::library *, std::shared_ptr<T>> at(const Signature &signature) 
                {
                    try {
                    
                        std::unique_ptr<const phoenix::load::library> &lib = _plugins.at(signature).first;
                        std::shared_ptr<T> ptr = std::static_pointer_cast<T>(_plugins.at(signature).second);
                        return {lib.get(), ptr};
                    
                    } catch(const std::exception &error) {
                    
                        std::cerr << "Unable to access plugin: " << error.what() << std::endl;
                    
                    } return {nullptr, nullptr};
                }

                /* ---------------------------------- load ---------------------------------- */

                template <typename ...Args>
                void load(const Args &...) {}

                template <typename First, typename ...Rest>
                void load(void)
                {
                    load<First, Rest ...>(libraries().begin());
                    check();
                }

                template <typename First, typename ...Rest>
                void load(std::vector<plugin<Signature>::information>::iterator it) 
                {
                    try {
                        auto &[name, signature, entry_point, loaded] = libraries().at(it - libraries().begin());

                        if (path(name).empty() || loaded) return load<Rest ...>(it += 1);
                        
                        std::unique_ptr<phoenix::load::library> library = std::make_unique<phoenix::load::library>(path(name), entry_point);
                        wrapper instance(std::move(library), library->load_shared<First>());
                        
                        _plugins.emplace(signature, std::move(instance));
                        loaded = true;
                    
                        load<Rest ...>(it += 1);
                    
                    } catch(const std::exception &error) {}

                }

                template <typename First, typename ...Rest>
                void load(std::vector<plugin<Signature>::information>::iterator it, const information &plugin) 
                {
                    try {
                        auto [name, signature, entry_point, loaded] = plugin;
                        
                        if (path(name).empty()) return load<Rest ...>(it += 1);

                        std::unique_ptr<phoenix::load::library> library = std::make_unique<phoenix::load::library>(path(name), entry_point);
                        wrapper instance(std::move(library), library->load_shared<First>());
                        
                        _plugins.emplace(signature, std::move(instance));
                        loaded = true;

                        information &entry = libraries().at(it);

                        entry.name = name;
                        entry.signature = signature;
                        entry.entry_point = entry_point;
                        entry.loaded = loaded;

                        load<Rest ...>(it);
                    
                    } catch(const std::exception &error) {}
                }

                /* ------------------------------ load missing ------------------------------ */

                template <typename ...Args>
                void load_missing(const Args &...) {}

                template <typename First, typename ...Rest>
                void load_missing(void) 
                {
                    check_reload_missing();
                    load_missing<First, Rest ...>(this->missing().begin());
                    check_missing();
                }

                template <typename First, typename ...Rest>
                void load_missing(std::vector<Signature>::iterator it) //FIXME - Uge memory allocation
                {
                    try {
                        uint32_t index = it - missing().begin();
                        const Signature &missing = this->missing().at(index);
                        
                        for (auto &[name, signature, entry_point, loaded] : libraries()) {
                            if (missing == signature) {
                                load_missing<First, Rest ...>(this->missing().begin(), {name, signature, entry_point, loaded});
                                update_missing(missing);
                            }
                        }

                        load_missing<Rest ...>(it += 1);

                    } catch(const std::exception &error) {}
                }

                template <typename First, typename ...Rest>
                void load_missing(std::vector<Signature>::iterator it, const information &plugin) 
                {
                    try {
                        auto [name, signature, entry_point, loaded] = plugin;
                        
                        if (path(name).empty()) return load_missing<Rest ...>(it += 1);

                        std::unique_ptr<phoenix::load::library> library = std::make_unique<phoenix::load::library>(path(name), entry_point);
                        wrapper instance(std::move(library), library->load_shared<First>());
                        
                        _plugins.emplace(signature, std::move(instance));
                        loaded = true;

                        for (information &entry : libraries()) {
                            if (entry.signature == signature) {
                                entry.name = name;
                                entry.signature = signature;
                                entry.entry_point = entry_point;
                                entry.loaded = loaded;
                            }
                        }

                        load_missing<Rest ...>(it);
                    
                    } catch(const std::exception &error) {}
                }

                /* ---------------------------------- reset --------------------------------- */

                template <typename T>
                void reset(const Signature &signature)
                {
                    if (all().size() == 0) return;
                    const std::string name = at<T>(signature).first->name();
                    const std::string entry_point = at<T>(signature).first->entry_point();
                    erase(signature); load<T>(libraries().begin(), {name, signature, entry_point});
                }

                template <typename ...Args>
                void reset(void) 
                {
                    if (all().size() == 0) return;
                    clear();
                    load<Args ...>();
                }

                /* ---------------------------------- check --------------------------------- */

                void check_reload_missing(void) 
                {
                    std::cout << std::endl << "Reload missing: " << '\'' << this->missing().size() << '/' << libraries().size() << '\'' << std::endl;
                    for (const Signature &missing : this->missing()) check(missing);
                }

                void check_missing(void) 
                {
                    if (missing().size() == 0) return;
                    
                    all().size() > 0 && std::cout << std::endl;
                    std::cout << "Missing: " << '\'' << missing().size() << '/' << libraries().size() << '\'' << std::endl;
                    for (const Signature &missing : missing()) {
                        check(missing);
                    }
                }

                void check(const Signature &signature) 
                {
                    for (const information &entry : libraries()) {
                        if (entry.signature == signature) 
                            std::cout << "\t[" << entry.loaded << "] - " << entry.name << std::endl;
                    }

                }

                void check(void) 
                {
                    all().size() > 0 && std::cout << "Loaded: " << '\'' << all().size() << '/' << libraries().size() << '\'' << std::endl;
                    for (const auto &[name, signature, entry_point, loaded] : libraries()) {
                        if (!loaded && std::find(missing().begin(), missing().end(), signature) == missing().end()) 
                            missing().push_back(signature);
                        else if (loaded && std::find(missing().begin(), missing().end(), signature) == missing().end())
                            std::cout << "\t[" << loaded << "] - " << name << std::endl;
                    }

                    return check_missing();
                }

                /* ---------------------------------- logic --------------------------------- */

                void erase(const Signature &signature) 
                {
                    all().erase(signature);
                }

                void clear(void) 
                {
                    all().clear();
                }

                const std::string path(const std::string &name) 
                {
                    for (const auto &file : std::filesystem::recursive_directory_iterator(directory())) {
                        if (file.exists() && file.path().filename().c_str() == name) return file.path().c_str();
                    } return "";
                }

                void update_missing(const Signature &missing) 
                {
                    for (auto found = this->missing().begin(); found < this->missing().end(); found++) {
                        for (information &entry : libraries()) {
                            if (*found == missing && *found == entry.signature && entry.loaded) {
                                this->missing().erase(found);
                            }
                        }
                    }
                }

            protected:
            private:
                std::string _directory;
                std::vector<Signature> _missing;
                std::vector<information> _libraries;
                std::unordered_map<Signature, wrapper> _plugins;
        };

    } // load

} // phoenix
