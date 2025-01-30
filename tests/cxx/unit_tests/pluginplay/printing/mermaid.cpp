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

DECLARE_MODULE(Allnmers);
inline MODULE_CTOR(Allnmers) {
    satisfies_property_type<testing::NullPT>();
    add_submodule<testing::NullPT>("Monomer Maker");
}
inline MODULE_RUN(Allnmers) { return results(); }

DECLARE_MODULE(AtomicCapping);
inline MODULE_CTOR(AtomicCapping) {
    satisfies_property_type<testing::NullPT>();
}
inline MODULE_RUN(AtomicCapping) { return results(); }

DECLARE_MODULE(BondBasedFragmenter);
inline MODULE_CTOR(BondBasedFragmenter) {
    satisfies_property_type<testing::NullPT>();
}
inline MODULE_RUN(BondBasedFragmenter) { return results(); }

DECLARE_MODULE(BrokenBonds);
inline MODULE_CTOR(BrokenBonds) { satisfies_property_type<testing::NullPT>(); }
inline MODULE_RUN(BrokenBonds) { return results(); }

DECLARE_MODULE(ClusterPartition);
inline MODULE_CTOR(ClusterPartition) {
    satisfies_property_type<testing::NullPT>();
}
inline MODULE_RUN(ClusterPartition) { return results(); }

DECLARE_MODULE(CovalentRadius);
inline MODULE_CTOR(CovalentRadius) {
    satisfies_property_type<testing::NullPT>();
}
inline MODULE_RUN(CovalentRadius) { return results(); }

DECLARE_MODULE(DCLCCapping);
inline MODULE_CTOR(DCLCCapping) {
    satisfies_property_type<testing::NullPT>();
    add_submodule<testing::NullPT>("Connectivity");
}
inline MODULE_RUN(DCLCCapping) { return results(); }

DECLARE_MODULE(EnergyMethod);
inline MODULE_CTOR(EnergyMethod) { satisfies_property_type<testing::NullPT>(); }
inline MODULE_RUN(EnergyMethod) { return results(); }

DECLARE_MODULE(FragmentBasedMethod);
inline MODULE_CTOR(FragmentBasedMethod) {
    satisfies_property_type<testing::NullPT>();
    add_submodule<testing::NullPT>("Energy method");
    add_submodule<testing::NullPT>("Subsystem former");
    add_submodule<testing::NullPT>("Weighter");
}
inline MODULE_RUN(FragmentBasedMethod) { return results(); }

DECLARE_MODULE(FragmentDriver);
inline MODULE_CTOR(FragmentDriver) {
    satisfies_property_type<testing::NullPT>();
    add_submodule<testing::NullPT>("Atomic connectivity");
    add_submodule<testing::NullPT>("Cap broken bonds");
    add_submodule<testing::NullPT>("Find broken bonds");
    add_submodule<testing::NullPT>("Fragment builder");
    add_submodule<testing::NullPT>("Intersection finder");
    add_submodule<testing::NullPT>("Molecular graph");
    add_submodule<testing::NullPT>("N-mer builder");
}
inline MODULE_RUN(FragmentDriver) { return results(); }

DECLARE_MODULE(FragmentedChemicalSystemDriver);
inline MODULE_CTOR(FragmentedChemicalSystemDriver) {
    satisfies_property_type<testing::NullPT>();
    add_submodule<testing::NullPT>("Fragmenter");
}
inline MODULE_RUN(FragmentedChemicalSystemDriver) { return results(); }

DECLARE_MODULE(GMBEWeights);
inline MODULE_CTOR(GMBEWeights) { satisfies_property_type<testing::NullPT>(); }
inline MODULE_RUN(GMBEWeights) { return results(); }

DECLARE_MODULE(HeavyAtomPartition);
inline MODULE_CTOR(HeavyAtomPartition) {
    satisfies_property_type<testing::NullPT>();
    add_submodule<testing::NullPT>("Connectivity");
}
inline MODULE_RUN(HeavyAtomPartition) { return results(); }

DECLARE_MODULE(Intersections);
inline MODULE_CTOR(Intersections) {
    satisfies_property_type<testing::NullPT>();
}
inline MODULE_RUN(Intersections) { return results(); }

DECLARE_MODULE(NuclearGraph);
inline MODULE_CTOR(NuclearGraph) {
    satisfies_property_type<testing::NullPT>();
    add_submodule<testing::NullPT>("Connectivity");
    add_submodule<testing::NullPT>("Nodes");
}
inline MODULE_RUN(NuclearGraph) { return results(); }

DECLARE_MODULE(WeightedDistance);
inline MODULE_CTOR(WeightedDistance) {
    satisfies_property_type<testing::NullPT>();
}
inline MODULE_RUN(WeightedDistance) { return results(); }

TEST_CASE("hello_world") {
    pluginplay::ModuleManager mm;
    mm.add_module<Allnmers>("All nmers");
    mm.add_module<AtomicCapping>("Atomic Capping");
    mm.add_module<BondBasedFragmenter>("Bond-Based Fragmenter");
    mm.add_module<BrokenBonds>("Broken Bonds");
    mm.add_module<ClusterPartition>("Cluster Partition");
    mm.add_module<CovalentRadius>("Covalent Radius");
    mm.add_module<DCLCCapping>("DCLC Capping");
    mm.add_module<EnergyMethod>("Energy Method");
    mm.add_module<FragmentBasedMethod>("Fragment Based Method");
    mm.add_module<FragmentDriver>("Fragment Driver");
    mm.add_module<FragmentedChemicalSystemDriver>(
      "FragmentedChemicalSystem Driver");
    mm.add_module<GMBEWeights>("GMBE Weights");
    mm.add_module<HeavyAtomPartition>("Heavy Atom Partition");
    mm.add_module<Intersections>("Intersections");
    mm.add_module<NuclearGraph>("Nuclear Graph");
    mm.add_module<WeightedDistance>("Weighted Distance");
    mm.change_submod("DCLC Capping", "Connectivity", "Covalent Radius");
    mm.change_submod("Fragment Based Method", "Energy method", "Energy Method");
    mm.change_submod("Fragment Based Method", "Subsystem former",
                     "FragmentedChemicalSystem Driver");
    mm.change_submod("Fragment Based Method", "Weighter", "GMBE Weights");
    mm.change_submod("Fragment Driver", "Atomic connectivity",
                     "Covalent Radius");
    mm.change_submod("Fragment Driver", "Cap broken bonds",
                     "Weighted Distance");
    mm.change_submod("Fragment Driver", "Find broken bonds", "Broken Bonds");
    mm.change_submod("Fragment Driver", "Fragment builder",
                     "Bond-Based Fragmenter");
    mm.change_submod("Fragment Driver", "Intersection finder", "Intersections");
    mm.change_submod("Fragment Driver", "Molecular graph", "Nuclear Graph");
    mm.change_submod("Fragment Driver", "N-mer builder", "All nmers");
    mm.change_submod("FragmentedChemicalSystem Driver", "Fragmenter",
                     "Fragment Driver");
    mm.change_submod("Heavy Atom Partition", "Connectivity", "Covalent Radius");
    mm.change_submod("Nuclear Graph", "Connectivity", "Covalent Radius");
    mm.change_submod("Nuclear Graph", "Nodes", "Heavy Atom Partition");

    SECTION("huh") {
        auto hello = create_mermaid_graph(mm);
        std::cout << "Bro the test ran but idk if the internals ran"
                  << std::endl;
        REQUIRE(hello == "Hello World!");
    }
}
