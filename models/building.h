#ifndef RJMMO_BUILDING_H
#define RJMMO_BUILDING_H

#include "model.h"

class Building : public Model<Building> {
 public:
  using Model::Model;

  static const Table<Building>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  //getters
  [[nodiscard]] const std::string& street() const { return street_; };
  [[nodiscard]] const std::string& no() const { return no_; };
  [[nodiscard]] int npa() const { return npa_; };
  [[nodiscard]] const std::string& city() const { return city_; };

  //setters
  void street(std::string street) { street_ = std::move(street); }
  void no(std::string no) { no_ = std::move(no); }
  void npa(int val) { npa_ = val; }
  void city(std::string val) { city_ = std::move(val); }

 private:
  std::string street_;
  std::string no_;
  int npa_;
  std::string city_;
};

REGISTER_MODEL(Building);

#endif  //RJMMO_BUILDING_H
