//
// Created by drin on 18/01/2025.
//

#ifndef RJMMO_OCCTENANT_H
#define RJMMO_OCCTENANT_H

#include "model.h"

class Occupancy_Tenant : public Model<Occupancy_Tenant> {
 public:
  using Model::Model;

  static const Table<Occupancy_Tenant>& table();

  static const std::string sql_req_sel_all;
  static const std::string sql_req_sel_all_as_str;

  [[nodiscard]] const ForeignKey& occupancy() const { return occupancy_; };
  [[nodiscard]] const ForeignKey& tenant() const { return tenant_; };

  void occupancy(ForeignKey val) { occupancy_ = std::move(val); }
  void tenant(ForeignKey val) { tenant_ = std::move(val); }

 private:
  ForeignKey occupancy_;
  ForeignKey tenant_;
};

REGISTER_MODEL(Occupancy_Tenant);

#endif  //RJMMO_OCCTENANT_H
