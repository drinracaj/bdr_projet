//
// Created by drin on 22/01/2025.
//

#ifndef RJMMO_INVOICE_H
#define RJMMO_INVOICE_H

#include "building.h"
#include "company.h"
#include "feetype.h"
#include "model.h"

class Invoice : public Model<Invoice> {
 public:
  using Model::Model;

  static const Table<Invoice>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  [[nodiscard]] double amount() const { return amount_; }
  [[nodiscard]] const std::string& date() const { return date_; };
  [[nodiscard]] const ForeignKey& building() const { return building_; }
  [[nodiscard]] const ForeignKey& fee_type() const { return fee_type_; }
  [[nodiscard]] const ForeignKey& company() const { return company_; }

  void amount(double val) { amount_ = val; }
  void date(std::string val) { date_ = std::move(val); }
  void building(ForeignKey val) { building_ = std::move(val); }
  void fee_type(ForeignKey val) { fee_type_ = std::move(val); }
  void company(ForeignKey val) { company_ = std::move(val); }

 private:
  double amount_;
  std::string date_;
  ForeignKey building_;
  ForeignKey fee_type_;
  ForeignKey company_;
};

REGISTER_MODEL(Invoice);

#endif  //RJMMO_INVOICE_H
