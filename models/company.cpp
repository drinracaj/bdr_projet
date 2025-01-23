//
// Created by drin on 22/01/2025.
//

#include "company.h"
const Table<Company>& Company::table() {
  static const Table<Company> table = {
      typeid(Company),
      {Column<Company>(
           [](const Company& c) -> std::string_view { return c.name(); },
           [](Company& c, FieldSetter val) -> void {
             c.name(std::move(std::get<std::string>(val)));
           },
           "name", "Name", ColumnType::VARCHAR, 35),
       Column<Company>(
           [](const Company& c) -> std::string_view { return c.email(); },
           [](Company& c, FieldSetter val) -> void {
             c.email(std::move(std::get<std::string>(val)));
           },
           "email", "Email", ColumnType::VARCHAR, 320),
       Column<Company>(
           [](const Company& c) -> std::string_view { return c.tel(); },
           [](Company& c, FieldSetter val) -> void {
             c.tel(std::move(std::get<std::string>(val)));
           },
           "tel", "Tel", ColumnType::VARCHAR, 11)}};

  return table;
}

const std::string Company::sql_req_sel_all =
    "SELECT * FROM " + ModelRegistry::model(typeid(Company));

const std::string Company::sql_req_sel_all_as_str =
    "SELECT id, name AS as_str FROM " + ModelRegistry::model(typeid(Company));