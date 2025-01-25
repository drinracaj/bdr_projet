//
// Created by drin on 21/01/2025.
//
#include "watermeasurement.h"
const Table<WaterMeasurement>& WaterMeasurement::table() {
  static const Table<WaterMeasurement> table = {
      typeid(WaterMeasurement),
      {Column<WaterMeasurement>(
           [](const WaterMeasurement& wm) -> int { return wm.year(); },
           [](WaterMeasurement& wm, FieldSetter val) -> void {
             wm.year(std::get<int>(val));
           },
           "year", "Year", ColumnType::INTEGER, 1900,
           std::numeric_limits<int>::max()),
       Column<WaterMeasurement>(
           [](const WaterMeasurement& wm) -> double { return wm.measure(); },
           [](WaterMeasurement& wm, FieldSetter val) -> void {
             wm.measure(std::get<double>(val));
           },
           "measure", "Measure", ColumnType::REAL, .0, 1'000'000'000.0, 2),
       Column<WaterMeasurement>(
           [](const WaterMeasurement& wm) -> FieldGetter {
             return std::cref(wm.water_meter());
           },
           [](WaterMeasurement& wm, FieldSetter val) -> void {
             wm.water_meter(std::move(std::get<ForeignKey>(val)));
           },
           "idwatermeter", "Water Meter", ColumnType::FK, OnDelete::CASCADE,
           typeid(WaterMeter))}};
  return table;
}

const std::string WaterMeasurement::sql_req_sel_all =
    "SELECT WaterMeasurement.*, WaterMeter.no AS "
    "idWaterMeter" +
    FK_SUFFIX +
    " FROM WaterMeasurement "
    "INNER JOIN WaterMeter ON WaterMeter.id = WaterMeasurement.idWaterMeter";

const std::string WaterMeasurement::sql_req_sel_all_as_str;
