//
// Created by drin on 01/01/2025.
//

#include "occupancy.h"

#include "flat.h"

const Table<Occupancy>& Occupancy::table() {
  static const Table<Occupancy> table = {
      typeid(Occupancy),
      {Column<Occupancy>(
           [](const Occupancy& o) -> std::string_view { return o.start(); },
           [](Occupancy& o, FieldSetter val) -> void {
             o.start(std::move(std::get<std::string>(val)));
           },
           "startdate", "Start date", ColumnType::DATE),
       Column<Occupancy>([](const Occupancy& o) -> double { return o.rent(); },
                         [](Occupancy& o, FieldSetter val) -> void {
                           o.rent(std::get<double>(val));
                         },
                         "rent", "Rent", ColumnType::REAL, .0,
                         1'000'000'000.00, 2),
       Column<Occupancy>([](const Occupancy& o) -> double { return o.fees(); },
                         [](Occupancy& o, FieldSetter val) -> void {
                           o.fees(std::get<double>(val));
                         },
                         "fees", "Fees", ColumnType::REAL, .0,
                         1'000'000'000.00, 2),
       Column<Occupancy>([](const Occupancy& o) -> int { return o.ddm(); },
                         [](Occupancy& o, FieldSetter val) -> void {
                           o.ddm(std::get<int>(val));
                         },
                         "ddm", "Due day of month", ColumnType::INTEGER, 0, 31),
       Column<Occupancy>(
           [](const Occupancy& o) -> ForeignKeyRef {
             return std::cref(o.flat());
           },
           [](Occupancy& o, FieldSetter val) -> void {
             o.flat(std::move(std::get<ForeignKey>(val)));
           },
           "idflat", "Flat", ColumnType::FK, OnDelete::CASCADE, typeid(Flat)),
       Column<Occupancy>(
           [](const Occupancy& o) -> StrViewOpt { return o.end(); },
           [](Occupancy& o, FieldSetter val) -> void {
             if (std::holds_alternative<std::nullopt_t>(val)) {
               o.end(std::nullopt);
             } else {
               o.end(std::move(std::get<std::string>(val)));
             }
           },
           "enddate", "End date", ColumnType::DATE, true)}};
  return table;
}

const std::string Occupancy::sql_req_sel_all =
    "SELECT Occupancy.*, CONCAT(Building.street, ' ', Building.no, ' - ', "
    "Flat.no) AS idFlat" +
    FK_SUFFIX +
    " FROM Occupancy "
    "INNER JOIN Flat ON Occupancy.idFlat = Flat.id "
    "INNER JOIN Building ON Flat.idBuilding = Building.id";

const std::string Occupancy::sql_req_sel_all_as_str =
    "SELECT Occupancy.id, CONCAT(Building.street, ' ', Building.no, ' - ', "
    "Flat.no, ' - ', Occupancy.id) AS as_str "
    "FROM Occupancy "
    "INNER JOIN Flat ON Occupancy.idFlat = Flat.id "
    "INNER JOIN Building ON Flat.idBuilding = Building.id";
