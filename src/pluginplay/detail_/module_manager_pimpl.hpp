/*
 * Copyright 2022 NWChemEx-Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include "module_pimpl.hpp"
#include <memory>
#include <parallelzone/runtime/runtime_view.hpp>
#include <pluginplay/cache/module_manager_cache.hpp>
#include <pluginplay/module_base.hpp>
#include <pluginplay/module_manager.hpp>
#include <typeindex>

namespace pluginplay {
namespace detail_ {

/** @brief The class that implements the ModuleManager.
 *
 *  Users are expected to go through the API provided by the ModuleManager
 *  class. Inside the ModuleManager class calls are redirected to this class.
 *  When using this class calls should go through the member functions as much
 *  as possible so that error checking occurs. That said members are public so
 *  that it is easier to test the implementation.
 */
struct ModuleManagerPIMPL {
    ///@{
    /// Type of a pointer to a module's implemenation
    using module_base_ptr = typename ModuleManager::module_base_ptr;

    /// Type of a pointer to a read-only module implementation
    using const_module_base_ptr = typename ModuleManager::const_module_base_ptr;

    /// Type of a map from the module implementation's type to the
    /// implementation
    using base_map = std::map<std::type_index, const_module_base_ptr>;

    /// Type of a usable module
    using shared_module = std::shared_ptr<Module>;

    /// Type of a map holding usable modules
    using module_map = utilities::CaseInsensitiveMap<shared_module>;

    /// Type of a map holding the default module key for a given property type
    using default_map = std::map<std::type_index, type::key>;

    /// The type of the runtime
    using runtime_type = parallelzone::runtime::RuntimeView;

    /// A pointer to a runtime
    using runtime_ptr = std::shared_ptr<runtime_type>;

    /// Type of a map from key to Python implementation
    // TODO: remove when a more elegant solution is determined
    using py_base_map = std::map<type::key, const_module_base_ptr>;

    ///@}

    ModuleManagerPIMPL(runtime_ptr runtime) : m_runtime_(runtime) {}

    ModuleManagerPIMPL() : m_runtime_(std::make_shared<runtime_type>()) {}

    /// Makes a deep copy of this instance on the heap
    // auto clone() { return std::make_unique<ModuleManagerPIMPL>(*this); }

    /// Ensures we determine if we have a module consistently
    type::size count(const type::key& key) const noexcept {
        return m_modules.count(key);
    }

    /// Ensures we count the number of modules consistently
    type::size size() const noexcept { return m_modules.size(); }

    /** @brief Returns an iterator to the first element of the module map
     *
     * @return Iterator to the first element of the map
     */
    module_map::iterator begin() noexcept { return m_modules.begin(); };

    /** @brief Returns an iterator to the past-the-end element of the module map
     *
     * @return Iterator to the past-the-end element of the map
     */
    module_map::iterator end() noexcept { return m_modules.end(); };

    /** @brief Sets the default module to use for a given property type
     *
     * When a user requests a module that module initially has no submodules
     * set unless the user has bound some to that module. Instead, the
     * ModuleManager is now responsible to provide the module with submodules
     * using the defaults. This function allows one to specify what the
     * defaults are.
     *
     * @param type The type of the property type this default is for
     * @param key The module key for the module to use as the default
     */
    void set_default(const std::type_info& type, type::input_map inputs,
                     type::key key) {
        if(!count(key)) m_modules.at(key); // Throws a consistent error
        m_defaults[std::type_index(type)] = key;
        m_inputs[std::type_index(type)]   = std::move(inputs);
    }

    /** @brief This function actually adds a module to the list of available
     *         modules.
     *
     * @param key The key under which the module will be registered.
     * @param base The instance containing the algorithm
     */
    void add_module(type::key key, module_base_ptr base) {
        assert_unique_key_(key);
        auto uuid           = utility::generate_uuid();
        auto internal_cache = m_caches.get_or_make_user_cache(uuid);
        base->set_cache(internal_cache);
        base->set_runtime(m_runtime_);
        base->set_uuid(uuid);
        auto module_cache = m_caches.get_or_make_module_cache(key);
        std::unique_ptr<ModulePIMPL> pimpl;
        if(base->is_python()) {
            // This is a hacky patch to allow multiple python modules to be
            // added while avoiding the type_index collisions.
            // TODO: remove when a more elegant solution is determined
            m_py_bases[key] = base;
            pimpl =
              std::make_unique<ModulePIMPL>(m_py_bases[key], module_cache);
        } else {
            std::type_index type(base->type());
            if(!m_bases.count(type)) m_bases[type] = base;
            pimpl = std::make_unique<ModulePIMPL>(m_bases[type], module_cache);
        }
        auto ptr = std::make_shared<Module>(std::move(pimpl));
        ptr->set_name(key);
        m_modules.emplace(std::move(key), ptr);
    }

    /** @brief Unloads the specified module.
     *
     *  This function unloads the module with the specified key. After this
     *  operation the key is free to be used again. Calling this function does
     *  NOT clean any data out of the cache. This function is a no-op if @p key
     *  does not exist.
     *
     *  @param[in] key The key for the module which should be erased.
     *
     *  @throw None No throw guarantee.
     */
    void erase(const type::key& key) { m_modules.erase(key); }

    /** @brief Makes a deep copy of a module
     *
     * This function makes a deep copy of a module. The new module is unlocked
     * regardless of whether the old module was locked or not. The user can call
     * lock on the resulting module to make an exact copy
     * @param old_key The key for the module to copy
     * @param new_key The key under which the new module will live
     */
    void copy_module(const type::key& old_key, type::key new_key) {
        assert_unique_key_(new_key);
        Module mod = m_modules.at(old_key)->unlocked_copy();
        auto ptr   = std::make_shared<Module>(std::move(mod));
        ptr->set_name(new_key);
        m_modules.emplace(std::move(new_key), ptr);
    }

    /** @brief Returns a module, filling in all non-set submodules with defaults
     *         if a ready default exists.
     *
     * @param key The module you want
     * @return A shared_ptr to the requested module
     */
    shared_module at(const type::key& key) {
        if(!count(key)) {
            const std::string msg =
              "ModuleManager has no module with key: '" + key + "'";
            throw std::out_of_range(msg);
        }
        auto mod = m_modules.at(key);
        // Loop over submodules filling them in from the defaults
        for(auto& [k, v] : mod->submods()) {
            const auto& type = v.type();
            // Only change non-ready submodules
            if(!v.ready() && m_defaults.count(type)) {
                // Recursive to make sure that that module gets filled in
                auto default_mod = at(m_defaults.at(type));
                // Only change if the module is also ready
                if(default_mod->ready(m_inputs.at(type)))
                    mod->change_submod(k, default_mod);
            }
        }
        return mod;
    }

    ///@{
    /** @name Comparison operators
     *
     * @param rhs
     * @return
     */
    bool operator==(const ModuleManagerPIMPL& rhs) const {
        // Try to get out early
        if(m_bases.size() != rhs.m_bases.size()) return false;
        if(m_modules.size() != rhs.m_modules.size()) return false;
        if(m_defaults.size() != rhs.m_defaults.size()) return false;

        // TODO: Remove with the rest of the python hack
        if(m_py_bases.size() != rhs.m_py_bases.size()) return false;
        for(const auto& [k, v] : rhs.m_py_bases) {
            if(!m_py_bases.count(k)) return false;
            if(*m_py_bases.at(k) != *v) return false;
        }

        // Skip checking the values b/c implementations are compared by type
        for(const auto& [k, v] : rhs.m_bases) {
            if(!m_bases.count(k)) return false;
        }

        // Need to check the values b/c user may have switched options
        for(const auto& [k, v] : rhs.m_modules) {
            if(!m_modules.count(k)) return false;
            if(*m_modules.at(k) != *v) return false;
        }

        // Easy since not pointers
        if(m_defaults != rhs.m_defaults) return false;

        return true;
    }

    bool operator!=(const ModuleManagerPIMPL& rhs) const {
        return !((*this) == rhs);
    }

    void set_runtime(runtime_ptr runtime) noexcept { m_runtime_ = runtime; }

    runtime_type& get_runtime() const { return *m_runtime_; }

    ModuleManager::key_container_type keys() const {
        ModuleManager::key_container_type keys;
        keys.reserve(m_modules.size());
        for(const auto& [k, v] : m_modules) keys.push_back(k);
        return keys;
    }
    ///@}

    ///@{
    /** @name ModuleManager state
     *
     *  The members in this section are the state of the ModuleManager class.
     */
    // These are the Modules in the states provided to us by the developer
    base_map m_bases;

    // These are the Modules in the state set by the user
    module_map m_modules;

    // Part of the hacky patch to make multiple python modules work
    // TODO: remove when a more elegant solution is determined
    // These are the Python Modules in their developer state
    py_base_map m_py_bases;

    // These are the results of the modules running in the user's states
    cache::ModuleManagerCache m_caches;

    // A map of property types
    default_map m_defaults;

    // A map of inputs for property types
    std::map<std::type_index, type::input_map> m_inputs;

    // Pointer to this modules current runtime
    runtime_ptr m_runtime_;
    ///@}
private:
    /// Wraps the check for making sure @p key is not in use.
    void assert_unique_key_(const type::key& key) const {
        if(count(key)) throw std::invalid_argument("Key is in use");
    }
};

} // namespace detail_
} // namespace pluginplay
