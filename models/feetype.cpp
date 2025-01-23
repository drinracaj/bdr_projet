//
// Created by drin on 22/01/2025.
//

#include "feetype.h"
const Table<FeeType>& FeeType::table() {
  static const Table<FeeType> table = {
      typeid(FeeType),
      {Column<FeeType>(
          [](const FeeType& f) -> std::string_view { return f.type(); },
          [](FeeType& f, FieldSetter val) -> void {
            f.type(std::move(std::get<std::string>(val)));
          },
          "type", "Type", ColumnType::VARCHAR, 30)}};
  return table;
}

const std::string FeeType::sql_req_sel_all = "SELECT FeeType.* FROM FeeType";

const std::string FeeType::sql_req_sel_all_as_str =
    "SELECT FeeType.id, FeeType.type AS as_str FROM FeeType";
