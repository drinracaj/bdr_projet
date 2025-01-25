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
          "type", "Type", ColumnType::VARCHAR, 50)}};
  return table;
}

const std::string FeeType::sql_req_sel_all = "SELECT * FROM FeeType";

const std::string FeeType::sql_req_sel_all_as_str =
    "SELECT id, type AS as_str FROM FeeType";
