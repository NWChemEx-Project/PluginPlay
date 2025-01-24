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

#include "../catch.hpp"
#include "../test_common.hpp"
#include "module/macros.hpp"
#include "unit_testing_pts.hpp"
#include <pluginplay/module_manager/module_manager.hpp>
#include <pluginplay/printing/mermaid.hpp>

#define DECLARE_TEST_MODULE(number)                        \
    DECLARE_MODULE(SubmodLvl##number);                     \
    inline MODULE_CTOR(SubmodLvl##number) {                \
        satisfies_property_type<testing::NullPT>();        \
        add_submodule<testing::NullPT>("Submod " #number); \
    }                                                      \
    inline MODULE_RUN(SubmodLvl##number) { return results(); }
// Module Name: SubmodLvl#
// Submodule Name: Submod #

DECLARE_TEST_MODULE(3)
DECLARE_TEST_MODULE(2)
DECLARE_TEST_MODULE(1)

#undef DECLARE_TEST_MODULE

DECLARE_MODULE(TooManySubmods);
inline MODULE_CTOR(TooManySubmods) {
    satisfies_property_type<testing::NullPT>();
    add_submodule<testing::NullPT>("Submods");
}
inline MODULE_RUN(TooManySubmods) { return results(); }
// Make it to where the modules branch

TEST_CASE("hello_world") {
    pluginplay::ModuleManager mm;
    // Load in Modules
    mm.add_module<SubmodLvl3>("SubmodLvl3");
    mm.add_module<SubmodLvl2>("SubmodLvl2");
    mm.add_module<SubmodLvl1>("SubmodLvl1");
    mm.add_module<TooManySubmods>("TooManySubmods");
    mm.change_submod("TooManySubmods", "Submods", "SubmodLvl1");
    mm.change_submod("SubmodLvl1", "Submod 1", "SubmodLvl2");
    mm.change_submod("SubmodLvl2", "Submod 2", "SubmodLvl3");

    SECTION("huh") {
        auto hello = hello_world(mm);
        std::cout << "Bro the test ran but idk if the internals ran"
                  << std::endl;
        REQUIRE(hello == "Hello World!");
    }
}
