#ifndef RJMMO_MODEL_H
#define RJMMO_MODEL_H

#include "db.h"

#include <concepts>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

template <typename T>
concept model_requirements = requires(T) {
  { T::table() } -> std::same_as<const Table<T>&>;
  { T::sql_req_sel_all } -> std::same_as<const std::string&>;
  { T::sql_req_sel_all_as_str } -> std::same_as<const std::string&>;
};

template <typename Derived>
class Model {
 public:
  explicit Model(const int id) : id_(id) {
    static_assert(model_requirements<Derived>);
  }
  Model() : Model(-1) {}
  virtual ~Model() = default;

  [[nodiscard]] int id() const { return id_; }
  void id(const int val) { id_ = val; }

  static std::optional<std::unordered_map<int, std::unique_ptr<Derived>>>
  get_all() {
    try {
      std::unordered_map<int, std::unique_ptr<Derived>> res;
      for (const soci::rowset rs = db::session.prepare
                                   << Derived::sql_req_sel_all;
           const soci::row& row : rs) {
        Derived model;
        soci_row_setter_helper(row, model);
        res[model.id()] = std::make_unique<Derived>(model);
      }
      return std::make_optional(std::move(res));
    } catch (const std::exception& e) {
      db::log(e.what(), true);
      return std::nullopt;
    }
  }

  static std::optional<std::unordered_map<int, std::string>> get_all_as_str() {
    try {
      db::log(Derived::sql_req_sel_all_as_str);
      std::unordered_map<int, std::string> res;
      for (const soci::rowset rs = db::session.prepare
                                   << Derived::sql_req_sel_all_as_str;
           const soci::row& row : rs) {
        res[row.get<int>("id")] = std::move(row.get<std::string>("as_str"));
      }
      return std::make_optional(std::move(res));
    } catch (const std::exception& e) {
      db::log(e.what(), true);
      return std::nullopt;
    }
  }

  bool insert() {
    try {
      long long last_id;
      db::session << "INSERT INTO "
                  << ModelRegistry::model(Derived::table().table_index)
                  << prep_column_headers() << " VALUES (" << prep_values()
                  << ");";
      db::session.get_last_insert_id(
          ModelRegistry::model(Derived::table().table_index), last_id);
      id(static_cast<int>(
          last_id));  //we'll never reach more than the upper limit of an int
      db::log("INSERT INTO " +
              std::string(ModelRegistry::model(Derived::table().table_index)));
    } catch (const soci::soci_error& e) {
      db::log(e.what(), true);
      return false;
    }
    return true;
  }

  [[nodiscard]] bool update() const {
    try {
      db::session << "UPDATE "
                  << ModelRegistry::model(Derived::table().table_index)
                  << " SET " << prep_values(true) << " WHERE id =" << id()
                  << ";";
    } catch (const soci::soci_error& e) {
      db::log(e.what(), true);
      return false;
    }
    return true;
  }

  [[nodiscard]] static bool remove(int id) {
    try {
      db::session << "DELETE FROM "
                  << ModelRegistry::model(Derived::table().table_index)
                  << " WHERE id = :id",
          soci::use(id);
    } catch (const soci::soci_error& e) {
      db::log(e.what(), true);
      return false;
    }
    return true;
  }

 private:
  int id_;
  [[nodiscard]] std::string_view prep_column_headers() const {
    static std::unordered_map<std::type_index, std::string> results;
    auto& result = results[Derived::table().table_index];
    if (result.empty()) {
      std::ostringstream os;
      os << "(";
      const auto& columns = Derived::table().columns;
      for (size_t i = 0; i < columns.size(); ++i) {
        os << columns[i].name();
        if (i < columns.size() - 1) {
          os << ",";
        }
      }
      os << ")";
      result = os.str();
    }
    return result;
  }

  [[nodiscard]] std::string prep_values(const bool update = false) const {
    const Columns<Derived>& columns = Derived::table().columns;
    std::ostringstream os;
    for (size_t i = 0; i < columns.size(); ++i) {
      if (update) {
        os << columns[i].name() << "=";
      }

      os << columns[i].getter(*static_cast<const Derived*>(this));

      if (i < columns.size() - 1) {
        os << ",";
      }
    }
    return os.str();
  }

  static void soci_row_setter_helper(const soci::row& row, Derived& model) {
    model.id(row.get<int>("id"));
    for (const Column<Derived>& col : Derived::table().columns) {
      if (row.get_indicator(col.name()) == soci::i_null) {
        col.setter(model, std::nullopt);
        continue;
      }
      switch (col.type()) {
        case ColumnType::INTEGER:
          col.setter(model, row.get<int>(col.name()));
          break;
        case ColumnType::REAL:
          col.setter(model, row.get<double>(col.name()));
          break;
        case ColumnType::DATE: {
          soci::indicator ind;
          std::ostringstream oss;
          std::tm date = row.get<std::tm>(col.name());
          oss << std::put_time(&date, "%Y-%m-%d");
          col.setter(model, oss.str());
          break;
        }
        case ColumnType::VARCHAR:
          col.setter(model, row.get<std::string>(col.name()));
          break;
        case ColumnType::FK:
          col.setter(model, std::make_pair(
                                row.get<int>(col.name()),
                                row.get<std::string>(col.name() + FK_SUFFIX)));
          break;
        default:
          throw std::invalid_argument("Unexpected column type");
      }
    }
  }
};

#endif  //RJMMO_MODEL_H
