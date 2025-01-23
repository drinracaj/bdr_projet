//
// Created by drin on 28/12/2024.
//

#ifndef RJMMO_FLAT_H
#define RJMMO_FLAT_H

#include "model.h"

class Flat : public Model<Flat> {
 public:
  using Model::Model;

  static const Table<Flat>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  //getters
  [[nodiscard]] const std::string& no() const { return no_; };
  [[nodiscard]] const ForeignKey& building() const { return building_; };

  //setters
  void no(std::string no) { no_ = std::move(no); }
  void building(ForeignKey val) { building_ = std::move(val); }

 private:
  std::string no_;
  ForeignKey building_;
};

REGISTER_MODEL(Flat);

#endif  //RJMMO_FLAT_H
