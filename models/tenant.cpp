//
// Created by drin on 18/01/2025.
//

#include "tenant.h"

const Table<Tenant>& Tenant::table() {
  static const Table<Tenant> table = {
      typeid(Tenant),
      {Column<Tenant>(
           [](const Tenant& t) -> std::string_view { return t.firstname(); },
           [](Tenant& t, FieldSetter val) -> void {
             t.firstname(std::move(std::get<std::string>(val)));
           },
           "firstname", "First name", ColumnType::VARCHAR, 50),
       Column<Tenant>(
           [](const Tenant& t) -> std::string_view { return t.lastname(); },
           [](Tenant& t, FieldSetter val) -> void {
             t.lastname(std::move(std::get<std::string>(val)));
           },
           "lastname", "Last name", ColumnType::VARCHAR, 70),
       Column<Tenant>(
           [](const Tenant& t) -> std::string_view { return t.email(); },
           [](Tenant& t, FieldSetter val) -> void {
             t.email(std::move(std::get<std::string>(val)));
           },
           "email", "Email", ColumnType::VARCHAR, 320),
       Column<Tenant>(
           [](const Tenant& t) -> std::string_view { return t.tel(); },
           [](Tenant& t, FieldSetter val) -> void {
             t.tel(std::move(std::get<std::string>(val)));
           },
           "tel", "Tel", ColumnType::VARCHAR, 11)}};

  return table;
}

const std::string Tenant::sql_req_sel_all =
    "SELECT * FROM Tenant";

const std::string Tenant::sql_req_sel_all_as_str =
    "SELECT id, CONCAT(firstname, ' ', lastname) AS as_str FROM Tenant";