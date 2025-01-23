//
// Created by drin on 28/12/2024.
//

#include "flat.h"

#include "building.h"

const Table<Flat>& Flat::table() {
  static const Table<Flat> table = {
      typeid(Flat),
      {Column<Flat>([](const Flat& f) -> std::string_view { return f.no(); },
                    [](Flat& f, FieldSetter val) -> void {
                      f.no(std::move(std::get<std::string>(val)));
                    },
                    "no", "No", ColumnType::VARCHAR, 10),
       Column<Flat>(
           [](const Flat& f) -> FieldGetter { return std::cref(f.building()); },
           [](Flat& f, FieldSetter val) -> void {
             f.building(std::move(std::get<ForeignKey>(val)));
           },
           "building_id", "Building", ColumnType::FK, OnDelete::CASCADE,
           typeid(Building))}};
  return table;
}

const std::string Flat::sql_req_sel_all =
    "SELECT Flat.*, CONCAT(Building.street, ' ', Building.no) AS building_id" +
    FK_SUFFIX +
    " FROM Flat INNER JOIN Building ON Flat.building_id = Building.id";

const std::string Flat::sql_req_sel_all_as_str =
    "SELECT Flat.id, CONCAT(Building.street, ' ', Building.no, ' - ', Flat.no) "
    "AS as_str FROM Flat INNER JOIN Building ON Flat.building_id = Building.id";