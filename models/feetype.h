//
// Created by drin on 22/01/2025.
//

#ifndef RJMMO_FEETYPE_H
#define RJMMO_FEETYPE_H

#include "model.h"

class FeeType : public Model<FeeType> {
 public:
  using Model::Model;

  static const Table<FeeType>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  //getters
  [[nodiscard]] const std::string& type() const { return type_; };

  //setters
  void type(std::string val) { type_ = std::move(val); }

 private:
  std::string type_;
};

REGISTER_MODEL(FeeType);

#endif  //RJMMO_FEETYPE_H
