//
// Created by drin on 18/01/2025.
//

#ifndef RJMMO_TENANT_H
#define RJMMO_TENANT_H

#include "model.h"

class Tenant : public Model<Tenant> {
 public:
  using Model::Model;

  static const Table<Tenant>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  //getters
  [[nodiscard]] const std::string& firstname() const { return firstname_; };
  [[nodiscard]] const std::string& lastname() const { return lastname_; };
  [[nodiscard]] const std::string& email() const { return email_; };
  [[nodiscard]] const std::string& tel() const { return tel_; };

  //setters
  void firstname(std::string firstname) { firstname_ = std::move(firstname); }
  void lastname(std::string lastname) { lastname_ = std::move(lastname); }
  void email(std::string email) { email_ = std::move(email); }
  void tel(std::string tel) { tel_ = std::move(tel); }

 private:
  std::string firstname_;
  std::string lastname_;
  std::string email_;
  std::string tel_;
};

REGISTER_MODEL(Tenant);
#endif  //RJMMO_TENANT_H
