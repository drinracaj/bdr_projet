//
// Created by drin on 1/11/25.
//

#ifndef COLUMNTYPE_H
#define COLUMNTYPE_H

#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <variant>

enum class ColumnType { INTEGER = 0, REAL, VARCHAR, FK, DATE };
enum class OnDelete { NA = 0, NO_ACTION, RESTRICT, CASCADE };

using ForeignKey = std::pair<int, std::string>;
using ForeignKeyRef = std::reference_wrapper<const ForeignKey>;
using IntOpt = std::optional<int>;
using DoubleOpt = std::optional<double>;
using StrOpt = std::optional<std::string>;
using StrViewOpt = std::optional<std::string_view>;
using FieldGetter = std::variant<int, double, std::string_view, ForeignKeyRef,
                                 IntOpt, DoubleOpt, StrViewOpt>;
using FieldSetter =
    std::variant<int, double, std::string, ForeignKey, std::nullopt_t>;

constexpr std::string_view column_type_str(const ColumnType col_type) {
  switch (col_type) {
    case ColumnType::INTEGER:
      return "INTEGER";
    case ColumnType::REAL:
      return "REAL";
    case ColumnType::VARCHAR:
      return "VARCHAR";
    case ColumnType::FK:
      return "FOREIGN KEY";
    case ColumnType::DATE:
      return "DATE";
    default:
      throw std::invalid_argument("Unexpected column type");
  }
}
constexpr std::string_view od_str(const OnDelete od) {
  switch (od) {
    case OnDelete::NA:
      return "NA";
    case OnDelete::CASCADE:
      return "CASCADE";
    case OnDelete::NO_ACTION:
      return "NO ACTION";
    case OnDelete::RESTRICT:
      return "RESTRICT";
    default:
      throw std::invalid_argument("Unexpected ON DELETE clause");
  }
}

template <typename T>
void output_optional(std::ostream& os, const std::optional<T>& opt) {
  if (opt.has_value()) {
    if (std::is_same_v<T, std::string_view>) {
      os << "'" << opt.value() << "'";
      return;
    }
    os << opt.value();
  } else {
    os << "NULL";
  }
}

std::ostream& operator<<(std::ostream& os, const FieldGetter& variant);

#endif  //COLUMNTYPE_H
