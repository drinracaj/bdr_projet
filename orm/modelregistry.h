//
// Created by drin on 16/12/2024.
//

#ifndef RJMMO_MODELREGISTRY_H
#define RJMMO_MODELREGISTRY_H

#include <functional>
#include <optional>
#include <string>
#include <typeindex>
#include <unordered_map>

using RegisteredModels = std::unordered_map<std::type_index, std::string>;
using GetAllAsStrFunc =
    std::function<std::optional<std::unordered_map<int, std::string>>()>;

class ModelRegistry {
 public:
  ModelRegistry() = delete;
  ~ModelRegistry() = delete;

  template <typename T>
  static void register_model(std::string name) {
    models_.emplace(typeid(T), std::move(name));
    get_all_as_str_map_.emplace(typeid(T), &T::get_all_as_str);
  }

  static const std::string& model(const std::type_index& key);
  static std::optional<std::unordered_map<int, std::string>> get_all_as_str(
      const std::type_index& key);

 private:
  static inline RegisteredModels models_;
  static inline std::unordered_map<std::type_index, GetAllAsStrFunc>
      get_all_as_str_map_;
};

#define REGISTER_MODEL(Class)                     \
  static const bool modelRegistry##Class = [] {   \
    ModelRegistry::register_model<Class>(#Class); \
    return true;                                  \
  }()

#endif  //RJMMO_MODELREGISTRY_H
