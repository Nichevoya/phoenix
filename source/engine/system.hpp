#pragma once

#include "entity.hpp"

namespace phoenix {
    
    namespace engine {
        
        class system_loader {
            public:
                std::set<entity> get_entity_set(void) { return _entities; }
                
            protected:
            private:
                std::set<entity> _entities;
        };
        
        class system_manager {
            public:
                /**
                 * @brief Create a pointer to the system and return it so it can be used externally
                 * 
                 * @tparam T 
                 * @return std::shared_ptr<T> 
                 */
                template <typename T>
                std::shared_ptr<T> register_system(void)
                {
                    const char *type_name = typeid(T).name();
                    assert(_systems.find(type_name) == _systems.end() && "Registering system more than once.");
                    auto system = std::make_shared<T>();
                    _systems.insert({type_name, system});
                    return system;
                }
            
                template <typename T>
                void set_signature(signature signature)
                {
                    const char *type_name = typeid(T).name();
                    assert(_systems.find(type_name) != _systems.end() && "System used before registered.");
                    // Set the signature for this system
                    _signatures.insert({type_name, signature});
                }
            
                void entity_destroyed(entity entity)
                {
                    // Erase a destroyed entity from all system lists
                    // _Entities is a set so no check needed
                    for (const auto &pair : _systems) {
                        const std::shared_ptr<system_loader> &system = pair.second;
                        system->get_entity_set().erase(entity);
                    }
                }
            
                /**
                 * @brief Notify each system that an entity's signature changed, if the entity signature matches system signature, it insert  it into set, else if entity signature does not match system signature, it erase it from set
                 * 
                 * @param entity 
                 * @param entity_signature 
                 */
                void entity_signature_changed(entity entity, signature entity_signature)
                {
                    for (const auto &pair : _systems) {
                        const char *const &type = pair.first;
                        const std::shared_ptr<system_loader> &system = pair.second;
                        const signature &system_signature = _signatures[type];
                        if ((entity_signature &system_signature) == system_signature) {
                            system->get_entity_set().insert(entity);
                        }
                        else {
                            system->get_entity_set().erase(entity);
                        }
                    }
                }
            
            private:
                // Map from system type string pointer to a signature
                std::unordered_map<const char *, signature> _signatures = {};
                // Map from system type string pointer to a system pointer
                std::unordered_map<const char *, std::shared_ptr<system_loader>> _systems = {};
        };
    
    } // engine

} // phoenix

