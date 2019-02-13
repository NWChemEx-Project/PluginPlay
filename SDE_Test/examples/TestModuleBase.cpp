#include "examples/TestModuleBase.hpp"
#include <SDE/detail_/ModulePIMPL.hpp>
#include <catch2/catch.hpp>

using namespace SDE::detail_;

TEST_CASE("Rectangle Class : Inputs") {
    Rectangle r;
    auto inputs = r.inputs();
    REQUIRE(inputs.at("Dimension 1").description() == "The height of the"
                                                      " rectangle");
    REQUIRE(inputs.at("Dimension 2").description() == "The width of the "
                                                      "rectangle");
    REQUIRE(inputs.at("Name").description() == "The name of the "
                                               "rectangle");
}
TEST_CASE("Rectangle Class : Results") {
    Rectangle r;
    auto results = r.results();
    REQUIRE(results.at("Area").description() == "The area of the shape");
    REQUIRE(results.at("Picture").description() == "An ASCII picture of "
                                                   "the rectangle");
}

TEST_CASE("Rectangle Class : Submods") {
    Rectangle r;
    REQUIRE(r.submods().size() == 0);
}

TEST_CASE("Rectangle Class : run") {
    Rectangle r;
    auto in = r.inputs();
    in.at("Dimension 1").change(1.23);
    in.at("Dimension 2").change(4.56);
    in.at("Name").change("Test");
    auto out = r.run(in, r.submods());
    REQUIRE(out.at("Area").value<double>() == Approx(5.6088));
    std::stringstream ss;
    ss << "Test :" << std::endl;
    ss << "**********" << std::endl;
    ss << "*        *" << std::endl;
    ss << "*        *" << std::endl;
    ss << "*        *" << std::endl;
    ss << "**********";
    REQUIRE(ss.str() == out.at("Picture").value<std::string>());
}

TEST_CASE("Prism Class : Inputs") {
    Prism r;
    auto inputs = r.inputs();
    REQUIRE(inputs.at("Dimensions").description() ==
            "1st 2 elements are for the base, last is height");
}

TEST_CASE("Prism Class : Results") {
    Prism r;
    auto results = r.results();
    REQUIRE(results.at("Base area").description() == "The area of the base");
    REQUIRE(results.at("Volume").description() == "The volume of the prism");
}

TEST_CASE("Prism Class : Submods") {
    Prism r;
    auto submods = r.submods();
    REQUIRE(submods.at("area").description() ==
            "Submodule used to compute base's area");
}

TEST_CASE("Prism Class : run") {
    Prism r;
    auto in = r.inputs();
    std::vector<double> dims{1.23, 4.56, 7.89};
    in.at("Dimensions").change(dims);
    auto submods = r.submods();

    // Normally the next few lines are done by the ModuleManager
    ModulePIMPL mod_pimpl(std::make_shared<Rectangle>());
    SDE::Module mod(std::make_unique<ModulePIMPL>(std::move(mod_pimpl)));
    submods.at("area").change(std::make_shared<SDE::Module>(mod));

    auto out           = r.run(in, submods);
    auto[area, volume] = PrismVolume::unwrap_results(out);
    REQUIRE(area == Approx(5.6088));
    REQUIRE(volume == Approx(44.253432));
}
