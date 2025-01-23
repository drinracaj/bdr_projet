//
// Created by drin on 21/01/2025.
//

#ifndef RJMMO_WATERMETER_H
#define RJMMO_WATERMETER_H

#include "model.h"

class WaterMeter : public Model<WaterMeter> {
 public:
  using Model::Model;

  static const Table<WaterMeter>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  [[nodiscard]] const std::string& no() const { return no_; };
  void no(std::string no) { no_ = std::move(no); }

 private:
  std::string no_;
};

REGISTER_MODEL(WaterMeter);
#endif  //RJMMO_WATERMETER_H
