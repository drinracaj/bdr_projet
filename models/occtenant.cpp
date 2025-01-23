//
// Created by drin on 18/01/2025.
//

#include "occtenant.h"

#include "occupancy.h"
#include "tenant.h"

const Table<OccTenant>& OccTenant::table() {
  static const Table<OccTenant> table = {
      typeid(OccTenant),
      {Column<OccTenant>(
           [](const OccTenant& ot) -> FieldGetter {
             return std::cref(ot.occupancy());
           },
           [](OccTenant& ot, FieldSetter val) -> void {
             ot.occupancy(std::move(std::get<ForeignKey>(val)));
           },
           "occupancy_id", "Occupancy", ColumnType::FK, OnDelete::CASCADE,
           typeid(Occupancy)),
       Column<OccTenant>([](const OccTenant& ot)
                             -> FieldGetter { return std::cref(ot.tenant()); },
                         [](OccTenant& ot, FieldSetter val) -> void {
                           ot.tenant(std::move(std::get<ForeignKey>(val)));
                         },
                         "tenant_id", "Tenant", ColumnType::FK,
                         OnDelete::CASCADE, typeid(Tenant))}};
  return table;
}

const std::string OccTenant::sql_req_sel_all =
    "SELECT OccTenant.*, "
    "CONCAT(Building.street, ' ', Building.no, ' - ', Flat.no, ' - ', "
    "Occupancy.id) AS occupancy_id" +
    FK_SUFFIX +
    ", "
    "CONCAT(Tenant.firstname, ' ', Tenant.lastname) AS tenant_id" +
    FK_SUFFIX +
    " FROM OccTenant "
    "INNER JOIN Tenant ON Tenant.id = OccTenant.tenant_id "
    "INNER JOIN Occupancy ON Occupancy.id = OccTenant.occupancy_id "
    "INNER JOIN Flat ON Occupancy.flat_id = Flat.id "
    "INNER JOIN Building ON Flat.building_id = Building.id";

//empty because will never be used in another table
const std::string OccTenant::sql_req_sel_all_as_str;