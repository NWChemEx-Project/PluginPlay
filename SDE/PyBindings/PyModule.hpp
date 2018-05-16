#pragma once
#include "SDE/Module.hpp"
#include <pybind11/pybind11.h>

namespace SDE {
namespace detail_ {

template<typename Derived, typename tuple_type>
struct PyModuleType;

template<typename Derived, typename... Args>
struct PyModuleType<Derived, std::tuple<Args...>> : Derived {
    using return_type = typename PropertyBase<Derived>::return_type;

    return_type run(Args... args) {
        PYBIND11_OVERLOAD_PURE(return_type, Derived, run, args...);
    }
};

template<typename ModuleAPI>
struct PyPropertyHelper {
    using base_type     = PropertyBase<ModuleAPI>;
    using return_type   = typename base_type::return_type;
    using shared_return = typename base_type::shared_return;
    using args_type     = typename base_type::args_type;

    static return_type call(base_type& prop, pybind11::args args) {
        constexpr auto nargs = std::tuple_size<args_type>::value;
        return *call_(prop, args, std::make_index_sequence<nargs>());
    }

    template<size_t... I>
    static shared_return call_(base_type& prop, pybind11::args args,
                               std::index_sequence<I...>) {
        return prop(
          args[I].cast<typename std::tuple_element<I, args_type>::type>()...);
    }
};

} // namespace detail_

template<typename ModuleAPI>
void register_module(pybind11::module& m, std::string name) {
    using args_type  = typename PropertyBase<ModuleAPI>::args_type;
    using trampoline = detail_::PyModuleType<ModuleAPI, args_type>;
    pybind11::class_<ModuleAPI, std::shared_ptr<ModuleAPI>, trampoline>(
      m, name.c_str())
      .def(pybind11::init<>())
      .def("run", &ModuleAPI::run);
}

template<typename ModuleAPI>
void register_property(pybind11::module& m, std::string name) {
    using prop_type   = PropertyBase<ModuleAPI>;
    using pyprop_type = detail_::PyPropertyHelper<ModuleAPI>;
    pybind11::class_<prop_type>(m, name.c_str())
      .def("__call__", &pyprop_type::call);
}

} // namespace SDE

#define DEFINE_PYTHON_PROPERTY(m, ModuleAPI, PropertyName) \
    SDE::register_module<ModuleAPI>(m, #ModuleAPI);        \
    SDE::register_property<ModuleAPI>(m, #PropertyName)
