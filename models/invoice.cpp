//
// Created by drin on 22/01/2025.
//

#include "invoice.h"
const Table<Invoice>& Invoice::table() {
  static const Table<Invoice> table = {
      typeid(Invoice),
      {Column<Invoice>([](const Invoice& i) -> double { return i.amount(); },
                       [](Invoice& i, FieldSetter val) -> void {
                         i.amount(std::get<double>(val));
                       },
                       "amount", "Amount", ColumnType::REAL, .0,
                       1'000'000'000.0, 2),
       Column<Invoice>(
           [](const Invoice& i) -> std::string_view { return i.date(); },
           [](Invoice& i, FieldSetter val) -> void {
             i.date(std::move(std::get<std::string>(val)));
           },
           "date", "Date", ColumnType::DATE),
       Column<Invoice>([](const Invoice& i)
                           -> FieldGetter { return std::cref(i.building()); },
                       [](Invoice& i, FieldSetter val) -> void {
                         i.building(std::move(std::get<ForeignKey>(val)));
                       },
                       "building_id", "Building", ColumnType::FK,
                       OnDelete::CASCADE, typeid(Building)),
       Column<Invoice>([](const Invoice& i)
                           -> FieldGetter { return std::cref(i.fee_type()); },
                       [](Invoice& i, FieldSetter val) -> void {
                         i.fee_type(std::move(std::get<ForeignKey>(val)));
                       },
                       "fee_type_id", "Fee Type", ColumnType::FK,
                       OnDelete::CASCADE, typeid(FeeType)),
       Column<Invoice>([](const Invoice& i)
                           -> FieldGetter { return std::cref(i.company()); },
                       [](Invoice& i, FieldSetter val) -> void {
                         i.company(std::move(std::get<ForeignKey>(val)));
                       },
                       "company_id", "Company", ColumnType::FK,
                       OnDelete::CASCADE, typeid(Company))}};
  return table;
}

const std::string Invoice::sql_req_sel_all =
    "SELECT Invoice.*, CONCAT(Building.street, ' ', Building.no) AS "
    "building_id" +
    FK_SUFFIX + ", FeeType.type AS fee_type_id" + FK_SUFFIX +
    ", Company.name AS company_id" + FK_SUFFIX +
    " FROM Invoice INNER JOIN Building ON Invoice.building_id = Building.id "
    "INNER JOIN FeeType ON Invoice.fee_type_id = FeeType.id "
    "INNER JOIN Company ON Invoice.company_id = Company.id";

const std::string Invoice::sql_req_sel_all_as_str;