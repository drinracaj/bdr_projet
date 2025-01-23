//
// Created by drin on 22/01/2025.
//

#ifndef RJMMO_COMPANY_H
#define RJMMO_COMPANY_H

#include "model.h"

class Company : public Model<Company> {
 public:
  using Model::Model;

  static const Table<Company>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  [[nodiscard]] const std::string& name() const { return name_; };
  [[nodiscard]] const std::string& email() const { return email_; };
  [[nodiscard]] const std::string& tel() const { return tel_; };

  void name(std::string val) { name_ = std::move(val); }
  void email(std::string val) { email_ = std::move(val); }
  void tel(std::string val) { tel_ = std::move(val); }

 private:
  std::string name_;
  std::string email_;
  std::string tel_;
};

REGISTER_MODEL(Company);

#endif  //RJMMO_COMPANY_H
