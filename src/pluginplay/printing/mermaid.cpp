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
#include <sstream>

void print_submods(const std::string module_key,
                   const pluginplay::ModuleManager& mm, std::stringstream& ss,
                   int level = 0) {
    const auto& mm_module = mm.at(module_key); // Results in a PluginPlay Module
    const auto& submods = mm_module.submods(); // Results in list of Submodules
    char letter         = 65 + level;

    // Key is the ID/Tag, Value is the reference to the Module
    for(const auto& [key, value] : submods) {
        ss << "\t" << letter + level << level + 1 << "[" << key
           << "]\n"; // Second indent: ----
        if(value.has_module() == false) {
            ss << "\t" << letter + level << level + 2
               << "[Submod Name: No Submodule associated with Key]\n";
            continue;
        } else {
            ss << "\t" << letter + level << level + 2 << "[" << value.get_name()
               << "]\n"; // Second indent: ----
            print_submods(value.get_name(), mm, ss, level + 1);
        }
    }
}

std::string create_mermaid_graph(const pluginplay::ModuleManager& mm) {
    auto n_modules = mm.size();
    std::stringstream ss;
    for(decltype(n_modules) i = 0; i < n_modules; i++) {
        ss << "flowchart LR\n";
        auto mod    = mm.keys()[i];
        char letter = 65;
        ss << "\t" << letter << "[" << mod << "]\n";
        print_submods(mod, mm, ss);
    }
    std::cout << ss.str();
    return "Hello World!";
}
