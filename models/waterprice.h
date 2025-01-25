//
// Created by drin on 25/01/2025.
//

#ifndef RJMMO_WATERPRICE_H
#define RJMMO_WATERPRICE_H

#include "model.h"

class WaterPrice : public Model<WaterPrice> {
 public:
  using Model::Model;

  static const Table<WaterPrice>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  [[nodiscard]] int year() const { return year_; }
  [[nodiscard]] double price() const { return price_; }

  void year(const int val) { year_ = val; }
  void price(const double val) { price_ = val; }

 private:
  int year_;
  double price_;
};

REGISTER_MODEL(WaterPrice);

#endif  //RJMMO_WATERPRICE_H
