//
// Created by drin on 22/01/2025.
//

#ifndef RJMMO_ROOMTYPE_H
#define RJMMO_ROOMTYPE_H

#include "model.h"

class RoomType : public Model<RoomType> {
 public:
  using Model::Model;

  static const Table<RoomType>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  [[nodiscard]] const std::string& type() const { return type_; }
  [[nodiscard]] double coefficient() const { return coefficient_; }

  void type(std::string val) { type_ = std::move(val); }
  void coefficient(double val) { coefficient_ = val; }

 private:
  std::string type_;
  double coefficient_;
};

REGISTER_MODEL(RoomType);

#endif  //RJMMO_ROOMTYPE_H
