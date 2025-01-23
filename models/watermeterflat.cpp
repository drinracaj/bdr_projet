//
// Created by drin on 21/01/2025.
//

#include "watermeterflat.h"

const Table<WaterMeterFlat>& WaterMeterFlat::table() {
  static const Table<WaterMeterFlat> table = {
      typeid(WaterMeterFlat),
      {Column<WaterMeterFlat>(
           [](const WaterMeterFlat& wmf) -> FieldGetter {
             return std::cref(wmf.flat());
           },
           [](WaterMeterFlat& wmf, FieldSetter val) -> void {
             wmf.flat(std::move(std::get<ForeignKey>(val)));
           },
           "flat_id", "Flat", ColumnType::FK, OnDelete::CASCADE, typeid(Flat)),
       Column<WaterMeterFlat>(
           [](const WaterMeterFlat& wmf) -> FieldGetter {
             return std::cref(wmf.water_meter());
           },
           [](WaterMeterFlat& wmf, FieldSetter val) -> void {
             wmf.water_meter(std::move(std::get<ForeignKey>(val)));
           },
           "watermeter_id", "Water Meter", ColumnType::FK, OnDelete::CASCADE,
           typeid(WaterMeter))}};
  return table;
}

const std::string WaterMeterFlat::sql_req_sel_all =
    "SELECT WaterMeterFlat.*, CONCAT(Building.street, ' ', Building.no, ' - ', "
    "Flat.no) AS "
    "flat_id" +
    FK_SUFFIX + ", WaterMeter.no AS watermeter_id" + FK_SUFFIX +
    " FROM WaterMeterFlat "
    "INNER JOIN Flat ON Flat.id = WaterMeterFlat.flat_id "
    "INNER JOIN Building ON Flat.building_id = Building.id "
    "INNER JOIN WaterMeter ON WaterMeter.id = WaterMeterFlat.watermeter_id";

const std::string WaterMeterFlat::sql_req_sel_all_as_str;