//
// Created by drin on 18/01/2025.
//

#ifndef RJMMO_PAYMENTTYPE_H
#define RJMMO_PAYMENTTYPE_H

#include "model.h"

class PaymentType : public Model<PaymentType> {
 public:
  using Model::Model;

  static const Table<PaymentType>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  [[nodiscard]] const std::string& type() const { return type_; };

  void type(std::string val) { type_ = std::move(val); };

 private:
  std::string type_;
};

REGISTER_MODEL(PaymentType);

#endif  //RJMMO_PAYMENTTYPE_H
