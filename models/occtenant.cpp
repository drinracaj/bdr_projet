//
// Created by drin on 18/01/2025.
//

#include "occtenant.h"

#include "occupancy.h"
#include "tenant.h"

const Table<Occupancy_Tenant>& Occupancy_Tenant::table() {
  static const Table<Occupancy_Tenant> table = {
      typeid(Occupancy_Tenant),
      {Column<Occupancy_Tenant>(
           [](const Occupancy_Tenant& ot) -> FieldGetter {
             return std::cref(ot.occupancy());
           },
           [](Occupancy_Tenant& ot, FieldSetter val) -> void {
             ot.occupancy(std::move(std::get<ForeignKey>(val)));
           },
           "idoccupancy", "Occupancy", ColumnType::FK, OnDelete::CASCADE,
           typeid(Occupancy)),
       Column<Occupancy_Tenant>([](const Occupancy_Tenant& ot)
                             -> FieldGetter { return std::cref(ot.tenant()); },
                         [](Occupancy_Tenant& ot, FieldSetter val) -> void {
                           ot.tenant(std::move(std::get<ForeignKey>(val)));
                         },
                         "idtenant", "Tenant", ColumnType::FK,
                         OnDelete::CASCADE, typeid(Tenant))}};
  return table;
}

const std::string Occupancy_Tenant::sql_req_sel_all =
    "SELECT Occupancy_Tenant.*, "
    "CONCAT(Building.street, ' ', Building.no, ' - ', Flat.no, ' | ', "
    "Occupancy.id) AS idOccupancy" +
    FK_SUFFIX +
    ", "
    "CONCAT(Tenant.firstname, ' ', Tenant.lastname) AS idTenant" +
    FK_SUFFIX +
    " FROM Occupancy_Tenant "
    "INNER JOIN Tenant ON Tenant.id = Occupancy_Tenant.idTenant "
    "INNER JOIN Occupancy ON Occupancy.id = Occupancy_Tenant.idOccupancy "
    "INNER JOIN Flat ON Occupancy.idFlat = Flat.id "
    "INNER JOIN Building ON Flat.idBuilding = Building.id";

//empty because will never be used in another table
const std::string Occupancy_Tenant::sql_req_sel_all_as_str;