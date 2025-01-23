//
// Created by drin on 22/01/2025.
//

#include "heatmonthlycoef.h"
const Table<HeatMonthlyCoefficient>& HeatMonthlyCoefficient::table() {
  static const Table<HeatMonthlyCoefficient> table = {
      typeid(HeatMonthlyCoefficient),
      {Column<HeatMonthlyCoefficient>(
           [](const HeatMonthlyCoefficient& hmc) -> int { return hmc.month(); },
           [](HeatMonthlyCoefficient& hmc, FieldSetter val) -> void {
             hmc.month(std::get<int>(val));
           },
           "month", "Month", ColumnType::INTEGER, 1, 12),
       Column<HeatMonthlyCoefficient>(
           [](const HeatMonthlyCoefficient& hmc) -> double {
             return hmc.coefficient();
           },
           [](HeatMonthlyCoefficient& hmc, FieldSetter val) -> void {
             hmc.coefficient(std::get<double>(val));
           },
           "coefficient", "Coefficient", ColumnType::REAL, .0, 100.0, 2)}};
  return table;
}

const std::string HeatMonthlyCoefficient::sql_req_sel_all =
    "SELECT HeatMonthlyCoefficient.* FROM HeatMonthlyCoefficient";

const std::string HeatMonthlyCoefficient::sql_req_sel_all_as_str;
