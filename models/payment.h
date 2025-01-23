//
// Created by drin on 18/01/2025.
//

#ifndef RJMMO_PAYMENT_H
#define RJMMO_PAYMENT_H

#include "model.h"

class Payment : public Model<Payment> {
 public:
  using Model::Model;

  static const Table<Payment>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  [[nodiscard]] const std::string& date() const { return date_; }
  [[nodiscard]] int amount() const { return amount_; }
  [[nodiscard]] const ForeignKey& payment_type() const {
    return payment_type_;
  };
  [[nodiscard]] const ForeignKey& occupancy() const { return occupancy_; };

  void date(std::string val) { date_ = std::move(val); }
  void amount(int val) { amount_ = val; }
  void payment_type(ForeignKey val) { payment_type_ = std::move(val); }
  void occupancy(ForeignKey val) { occupancy_ = std::move(val); }

 private:
  std::string date_;
  int amount_;
  ForeignKey payment_type_;
  ForeignKey occupancy_;
};

REGISTER_MODEL(Payment);

#endif  //RJMMO_PAYMENT_H
