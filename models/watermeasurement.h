//
// Created by drin on 21/01/2025.
//

#ifndef RJMMO_WATERMEASUREMENT_H
#define RJMMO_WATERMEASUREMENT_H

#include "model.h"

#include "watermeter.h"

class WaterMeasurement : public Model<WaterMeasurement> {
 public:
  using Model::Model;

  static const Table<WaterMeasurement>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  //getters
  [[nodiscard]] int year() const { return year_; }
  [[nodiscard]] double measure() const { return measure_; }
  [[nodiscard]] const ForeignKey& water_meter() const { return water_meter_; }

  //setters
  void year(int val) { year_ = val; }
  void measure(double val) { measure_ = val; }
  void water_meter(ForeignKey val) { water_meter_ = std::move(val); }

 private:
  int year_;
  double measure_;
  ForeignKey water_meter_;
};

REGISTER_MODEL(WaterMeasurement);

#endif  //RJMMO_WATERMEASUREMENT_H
