//
// Created by drin on 21/01/2025.
//

#include "watermeterflat.h"

const Table<Flat_WaterMeter>& Flat_WaterMeter::table() {
  static const Table<Flat_WaterMeter> table = {
      typeid(Flat_WaterMeter),
      {Column<Flat_WaterMeter>(
           [](const Flat_WaterMeter& wmf) -> FieldGetter {
             return std::cref(wmf.flat());
           },
           [](Flat_WaterMeter& wmf, FieldSetter val) -> void {
             wmf.flat(std::move(std::get<ForeignKey>(val)));
           },
           "idflat", "Flat", ColumnType::FK, OnDelete::CASCADE, typeid(Flat)),
       Column<Flat_WaterMeter>(
           [](const Flat_WaterMeter& wmf) -> FieldGetter {
             return std::cref(wmf.water_meter());
           },
           [](Flat_WaterMeter& wmf, FieldSetter val) -> void {
             wmf.water_meter(std::move(std::get<ForeignKey>(val)));
           },
           "idwatermeter", "Water Meter", ColumnType::FK, OnDelete::CASCADE,
           typeid(WaterMeter))}};
  return table;
}

const std::string Flat_WaterMeter::sql_req_sel_all =
    "SELECT Flat_WaterMeter.*, CONCAT(Building.street, ' ', Building.no, ' - ', "
    "Flat.no) AS "
    "idFlat" +
    FK_SUFFIX + ", WaterMeter.no AS idWaterMeter" + FK_SUFFIX +
    " FROM Flat_WaterMeter "
    "INNER JOIN Flat ON Flat.id = Flat_WaterMeter.idFlat "
    "INNER JOIN WaterMeter ON WaterMeter.id = Flat_WaterMeter.idWaterMeter "
    "INNER JOIN Building ON Flat.idBuilding = Building.id";

const std::string Flat_WaterMeter::sql_req_sel_all_as_str;