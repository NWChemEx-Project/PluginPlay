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

#include "pluginplay/module_manager.hpp"
#include "test_common.hpp"
#include <catch2/catch.hpp>

TEST_CASE("ModuleManager") {
    pluginplay::ModuleManager mm;

    SECTION("add_module(string)") {
        using mod_t = testing::NoPTModule; // Type of the Module we're adding

        mm.add_module<mod_t>("a mod");
        auto corr = testing::make_module<mod_t>("a mod");
        REQUIRE(mm.at("a mod") == *corr);
    }

    SECTION("add_module(string, shared_ptr") {
        using mod_t = testing::NoPTModule; // Type of the Module we're adding

        mm.add_module("a mod", std::make_shared<mod_t>());
        auto corr = testing::make_module<mod_t>("a mod");
        REQUIRE(mm.at("a mod") == *corr);
    }

    SECTION("iterator") {
        using mod_t = testing::NoPTModule; // Type of the Module we're adding

        mm.add_module("a key", std::make_shared<mod_t>());
        mm.add_module("b key", std::make_shared<testing::ReadyModule>());
        auto count = 0;
        for(auto& [key, mod] : mm) {
            count += 1;
            REQUIRE("key" == key.substr(key.size() - 3));
            REQUIRE_FALSE(mod->has_description());
        }
        REQUIRE(count == mm.size());
    }

    SECTION("keys") {
        using mod_t = testing::NoPTModule; // Type of the Module we're adding

        mm.add_module("a key", std::make_shared<mod_t>());
        mm.add_module("b key", std::make_shared<testing::ReadyModule>());
        auto keys = mm.keys();
        REQUIRE(keys.size() == mm.size());
        REQUIRE(keys[0] == "a key");
        REQUIRE(keys[1] == "b key");
    }
}
