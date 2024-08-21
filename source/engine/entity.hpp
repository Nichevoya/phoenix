#pragma once

#include <queue>
#include <array>

namespace phoenix {

    namespace engine {
        
        using entity = std::uint32_t;
        using component_type = std::uint8_t;
        const entity max_entities = 5000;
        const component_type max_components = 32;
        using signature = std::bitset<max_components>;
        
        class entity_manager {
            public:
                explicit entity_manager(void)
                {
                    for (entity entity = 0; entity < max_entities; entity++) {
                        _available_entities.push(entity);
                    }
                }
                entity_manager(const entity_manager &) = delete;
                ~entity_manager() = default;
    
                entity create_entity(void)
                {
                    assert(_living_entity_count < max_entities && "Too many entities in existence.");
                    entity id = _available_entities.front();
                    _available_entities.pop();
                    _living_entity_count += 1;
                    return id;
                }
    
                void destroy_entity(const entity entity)
                {
                    assert(entity < max_entities && "entity out of range.");
                    _signatures.at(entity).reset();
                    _available_entities.push(entity);
                    _living_entity_count -= 1;
                }
    
                void set_signature(const entity entity, signature signature)
                {
                    assert(entity < max_entities && "entity out of range.");
                    _signatures[entity] = signature;
                }
        
                const signature get_signature(const entity entity) const
                {
                    assert(entity < max_entities && "entity out of range.");
                    return _signatures[entity];
                }
        
            protected:
            private:
                uint32_t _living_entity_count = {};
                std::queue<entity> _available_entities = {};
                std::array<signature, max_entities> _signatures = {};
        };
        
    } // engine
    
} // phoenix

