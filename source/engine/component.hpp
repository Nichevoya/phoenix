#pragma once

#include "entity.hpp"

namespace phoenix {
    
    namespace engine {
        
        class container {
            public:
                virtual ~container() = default;
            
                virtual void entity_destroyed(entity entity) = 0;
            
            protected:
            private:
        };
        
        template <typename T>
        class component_array : public container {
            public:
                void insert_data(entity entity, T component)
                {
                    assert(_entity_to_index_map.find(entity) == _entity_to_index_map.end() && "Component added to same entity more than once.");
                    size_t new_index = _size;
                    _entity_to_index_map[entity] = new_index;
                    _index_to_entity_map[new_index] = entity;
                    _component_array[new_index] = component;
                    _size++;
                }
    
                void remove_component(engine::entity entity)
                {
                    assert(_entity_to_index_map.find(entity) != _entity_to_index_map.end() && "Removing non-existent component.");
                    size_t index_of_removed_entity = _entity_to_index_map[entity];
                    size_t index_of_last_element = _size - 1;
                    _component_array[index_of_removed_entity] = _component_array[index_of_last_element];
                    engine::entity entity_of_last_element = _index_to_entity_map[index_of_last_element];
                    _entity_to_index_map[entity_of_last_element] = index_of_removed_entity;
                    _index_to_entity_map[index_of_removed_entity] = entity_of_last_element;
                    _entity_to_index_map.erase(entity);
                    _index_to_entity_map.erase(index_of_last_element);
                    --_size;
                }
                
                T &get_data(entity entity)
                {
                    assert(_entity_to_index_map.find(entity) != _entity_to_index_map.end() && "Retrieving non-existent component.");
                    return _component_array[_entity_to_index_map[entity]];
                }
                
                void entity_destroyed(entity entity) override
                {
                    if (_entity_to_index_map.find(entity) != _entity_to_index_map.end())
                        remove_component(entity);
                }
    
            protected:
            private:
                std::size_t _size;
                std::array<T, max_entities> _component_array;
                std::unordered_map<entity, std::size_t> _entity_to_index_map;
                std::unordered_map<std::size_t, entity> _index_to_entity_map;
        };
        
        class component_manager {
            public:
                template <typename T>
                void register_component(void)
                {
                    const char *type_name = typeid(T).name();
                    assert(_component_type_map.find(type_name) == _component_type_map.end() && "Registering component type more than once.");
                    // Add this component type to the component type map
                    _component_type_map.insert({type_name, _next_component_type});
                    // Create a component_array pointer and add it to the component arrays map
                    _component_arrays.insert({type_name, std::make_shared<component_array<T>>()});
                    // Increment the value so that the next component registered will be different
                    ++_next_component_type;
                }
    
                template <typename T>
                component_type get_component_type(void)
                {
                    const char *type_name = typeid(T).name();
                    assert(_component_type_map.find(type_name) != _component_type_map.end() && "Component not registered before use.");
                    // Return this component's type - used for creating signatures
                    return _component_type_map[type_name];
                }
    
                template <typename T>
                void add_component(entity entity, T component)
                {
                    // Add a component to the array for an entity
                    get_component_array<T>()->insert_data(entity, component);
                }
    
                template <typename T>
                void remove_component(entity entity)
                {
                    // Remove a component from the array for an entity
                    get_component_array<T>()->remove_data(entity);
                }
                
                template <typename T>
                T &get_component(entity entity)
                {
                    return get_component_array<T>()->get_data(entity);
                }
                
                void entity_destroyed(entity entity)
                {
                    for (auto const &pair : _component_arrays) {
                        auto const &component = pair.second;
                        component->entity_destroyed(entity);
                    }
                }
            
            private:
                /**
                 * @brief Convenience function to get the statically casted pointer to the component_array of type T.
                 * 
                 * @tparam T 
                 * @return std::shared_ptr<component_array<T>> 
                 */
                template <typename T>
                std::shared_ptr<component_array<T>> get_component_array(void)
                {
                    const char *type_name = typeid(T).name();
                    assert(_component_type_map.find(type_name) != _component_type_map.end() && "Component not registered before use.");
                    return std::static_pointer_cast<component_array<T>>(_component_arrays[type_name]);
                }
                
                // Map from type string pointer to a component type
                std::unordered_map<const char *, component_type> _component_type_map = {};
                // Map from type string pointer to a component array
                std::unordered_map<const char *, std::shared_ptr<container>> _component_arrays = {};
                // The component type to be assigned to the next registered component - starting at 0
                component_type _next_component_type = {};
        };
    
    } // engine

} // phoenix

