//
// Created by drin on 21/01/2025.
//

#ifndef RJMMO_WATERMETERFLAT_H
#define RJMMO_WATERMETERFLAT_H

#include "model.h"

#include "flat.h"
#include "watermeter.h"

class WaterMeterFlat : public Model<WaterMeterFlat> {
 public:
  using Model::Model;

  static const Table<WaterMeterFlat>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  [[nodiscard]] const ForeignKey& flat() const { return flat_; };
  [[nodiscard]] const ForeignKey& water_meter() const { return watermeter_; };

  void flat(ForeignKey val) { flat_ = std::move(val); }
  void water_meter(ForeignKey val) { watermeter_ = std::move(val); }

 private:
  ForeignKey flat_;
  ForeignKey watermeter_;
};

REGISTER_MODEL(WaterMeterFlat);

#endif  //RJMMO_WATERMETERFLAT_H
