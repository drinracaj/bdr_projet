//
// Created by drin on 22/01/2025.
//

#ifndef RJMMO_HEATMONTHLYCOEF_H
#define RJMMO_HEATMONTHLYCOEF_H

#include "model.h"

class HeatMonthlyCoefficient : public Model<HeatMonthlyCoefficient> {
 public:
  using Model::Model;

  static const Table<HeatMonthlyCoefficient>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  [[nodiscard]] int month() const { return month_; }
  [[nodiscard]] double coefficient() const { return coefficient_; }

  void month(int val) { month_ = val; }
  void coefficient(double val) { coefficient_ = val; }

 private:
  int month_;
  double coefficient_;
};

REGISTER_MODEL(HeatMonthlyCoefficient);

#endif  //RJMMO_HEATMONTHLYCOEF_H
