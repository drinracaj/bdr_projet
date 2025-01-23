//
// Created by drin on 16/12/2024.
//

#include "modelregistry.h"

const std::string& ModelRegistry::model(const std::type_index& key) {
  return models_.at(key);
}
std::optional<std::unordered_map<int, std::string>>
ModelRegistry::get_all_as_str(const std::type_index& key) {
  return get_all_as_str_map_.at(key)();
}