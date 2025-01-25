//
// Created by drin on 25/01/2025.
//

#include "waterprice.h"
const Table<WaterPrice>& WaterPrice::table() {
  static const Table<WaterPrice> table = {
      typeid(WaterPrice),
      {Column<WaterPrice>([](const WaterPrice& wp) -> int { return wp.year(); },
                          [](WaterPrice& wp, FieldSetter val) -> void {
                            wp.year(std::get<int>(val));
                          },
                          "year", "Year", ColumnType::INTEGER, 1900,
                          std::numeric_limits<int>::max()),
       Column<WaterPrice>(
           [](const WaterPrice& wp) -> double { return wp.price(); },
           [](WaterPrice& wp, FieldSetter val) -> void {
             wp.price(std::get<double>(val));
           },
           "price", "Price", ColumnType::REAL, .0, 1'000'000'000.0, 2)}};
  return table;
}

const std::string WaterPrice::sql_req_sel_all =
    "SELECT WaterPrice.* FROM WaterPrice";

const std::string WaterPrice::sql_req_sel_all_as_str;
