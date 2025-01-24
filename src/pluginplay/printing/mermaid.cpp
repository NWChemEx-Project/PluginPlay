/*
 * Copyright 2024 NWChemEx-Project
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

#include "module/module_class.hpp"
#include "module_manager/module_manager_class.hpp"
#include <pluginplay/pluginplay.hpp>

void print_submods(const std::string module,
                   const pluginplay::ModuleManager& mm, int level = 0) {
    auto mm_module = mm.at(module);
    auto submods   = mm_module.submods();

    std::cout << "STARTING FOR LOOP ON SUBMODS" << std::endl;
    for(const auto& [key, value] : submods) {
        std::cout << "Submod ID: " << key << std::endl;
        if(value.has_module() == false) {
            std::cout << "No Submodule associated with Key yet" << std::endl;
            continue;
        }
        std::cout << "Submod Name: " << value.get_name() << std::endl;

        std::string submod_name = value.get_name();

        print_submods(submod_name, mm, level + 1);
    }
}

std::string hello_world(const pluginplay::ModuleManager& mm) {
    auto n_modules = mm.size();
    for(decltype(n_modules) i = 0; i < n_modules; i++) {
        auto mod = mm.keys()[i];
        std::cout << "RUNNING PRINT_SUBMODS in hello_world" << std::endl;
        print_submods(mod, mm);
    }
    return "Hello World!";
}