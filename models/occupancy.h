//
// Created by drin on 01/01/2025.
//

#ifndef RJMMO_OCCUPANCY_H
#define RJMMO_OCCUPANCY_H

#include "model.h"

class Occupancy : public Model<Occupancy> {
 public:
  using Model::Model;

  static const Table<Occupancy>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  //getters
  [[nodiscard]] const std::string& start() const { return start_; };
  [[nodiscard]] int rent() const { return rent_; };
  [[nodiscard]] int fees() const { return fees_; };
  [[nodiscard]] int ddm() const { return due_day_month_; };
  [[nodiscard]] const ForeignKey& flat() const { return flat_; };
  [[nodiscard]] StrViewOpt end() const { return end_; };

  //setters
  void start(std::string val) { start_ = std::move(val); }
  void rent(int val) { rent_ = val; }
  void fees(int val) { fees_ = val; }
  void ddm(int val) { due_day_month_ = val; }
  void flat(ForeignKey val) { flat_ = std::move(val); }
  void end(StrOpt val) { end_ = std::move(val); }

 private:
  std::string start_;
  int rent_;
  int fees_;
  int due_day_month_;
  ForeignKey flat_;
  StrOpt end_;
};

REGISTER_MODEL(Occupancy);

#endif  //RJMMO_OCCUPANCY_H
