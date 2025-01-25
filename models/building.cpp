#include "building.h"
#include "modelregistry.h"

const Table<Building>& Building::table() {
  static const Table<Building> table = {
      typeid(Building),
      {Column<Building>(
           [](const Building& b) -> std::string_view { return b.street(); },
           [](Building& b, FieldSetter val) -> void {
             b.street(std::move(std::get<std::string>(val)));
           },
           "street", "Street", ColumnType::VARCHAR, 50),
       Column<Building>(
           [](const Building& b) -> std::string_view { return b.no(); },
           [](Building& b, FieldSetter val) -> void {
             b.no(std::move(std::get<std::string>(val)));
           },
           "no", "No", ColumnType::VARCHAR, 10),
       Column<Building>([](const Building& b) -> int { return b.npa(); },
                        [](Building& b, FieldSetter val) -> void {
                          b.npa(std::get<int>(val));
                        },
                        "npa", "NPA", ColumnType::INTEGER, 1000, 9999),
       Column<Building>(
           [](const Building& b) -> std::string_view { return b.city(); },
           [](Building& b, FieldSetter val) -> void {
             b.city(std::move(std::get<std::string>(val)));
           },
           "city", "City", ColumnType::VARCHAR, 50)}};
  return table;
}

const std::string Building::sql_req_sel_all =
    "SELECT * FROM Building;";

const std::string Building::sql_req_sel_all_as_str =
    "SELECT id, CONCAT(street, ' ', no) AS as_str FROM Building;";