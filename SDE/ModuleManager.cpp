#include "SDE/ModuleManager.hpp"
#include "SDE/detail_/ModuleManagerPIMPL.hpp"
#include "SDE/detail_/ModulePIMPL.hpp"
namespace SDE {

using module_base_ptr = typename ModuleManager::module_base_ptr;
using cache_type      = typename detail_::ModulePIMPL::cache_type;
template<typename T>
using CIM = Utilities::CaseInsensitiveMap<T>;

ModuleManager::ModuleManager() :
  pimpl_(std::make_unique<detail_::ModuleManagerPIMPL>()) {}
ModuleManager::ModuleManager(const ModuleManager& rhs) :
  pimpl_(rhs.pimpl_->clone()) {}
ModuleManager::ModuleManager(ModuleManager&& rhs) noexcept = default;
ModuleManager& ModuleManager::operator=(ModuleManager&& rhs) noexcept = default;
ModuleManager::~ModuleManager() noexcept                              = default;

type::size ModuleManager::count(type::key key) const noexcept {
    return pimpl_->count(key);
}

void ModuleManager::add_module(type::key key, module_base_ptr base) {
    pimpl_->add_module(std::move(key), base);
}

void ModuleManager::copy_module(const type::key& old_key, type::key new_key) {
    pimpl_->copy_module(old_key, std::move(new_key));
}

const Module& ModuleManager::at(const type::key& module_key) const {
    return *pimpl_->m_modules.at(module_key);
}

void ModuleManager::set_default_(const std::type_info& type, type::key key) {
    pimpl_->set_default(type, std::move(key));
}
} // namespace SDE
