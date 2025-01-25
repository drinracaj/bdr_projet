//
// Created by drin on 21/01/2025.
//

#include "watermeter.h"
const Table<WaterMeter>& WaterMeter::table() {
  static const Table<WaterMeter> table = {
      typeid(WaterMeter),
      {Column<WaterMeter>(
          [](const WaterMeter& wm) -> std::string_view { return wm.no(); },
          [](WaterMeter& wm, FieldSetter val) -> void {
            wm.no(std::move(std::get<std::string>(val)));
          },
          "no", "No", ColumnType::VARCHAR, 10)}};
  return table;
}

const std::string WaterMeter::sql_req_sel_all =
    "SELECT * FROM WaterMeter";

const std::string WaterMeter::sql_req_sel_all_as_str =
    "SELECT id, no AS as_str FROM WaterMeter";